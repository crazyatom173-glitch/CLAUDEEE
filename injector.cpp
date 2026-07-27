#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>

#pragma runtime_checks("", off)

typedef NTSTATUS(NTAPI* NtCreateThreadEx_t)(
    PHANDLE hThread,
    ACCESS_MASK DesiredAccess,
    PVOID ObjectAttributes,
    HANDLE ProcessHandle,
    LPTHREAD_START_ROUTINE lpStartAddress,
    PVOID lpParameter,
    ULONG Flags,
    SIZE_T StackZeroBits,
    SIZE_T SizeOfStackCommit,
    SIZE_T SizeOfStackReserve,
    PVOID lpBytesBuffer);

typedef HMODULE(WINAPI* LoadLibraryA_t)(LPCSTR lpLibFileName);
typedef FARPROC(WINAPI* GetProcAddress_t)(HMODULE hModule, LPCSTR lpProcName);
typedef BOOLEAN(NTAPI* RtlAddFunctionTable_t)(PRUNTIME_FUNCTION FunctionTable, DWORD EntryCount, DWORD64 BaseAddress);

struct ShellcodeData
{
    BYTE* pTargetBase;
    LoadLibraryA_t fnLoadLibraryA;
    GetProcAddress_t fnGetProcAddress;
    RtlAddFunctionTable_t fnRtlAddFunctionTable;
};

static DWORD __stdcall Shellcode(ShellcodeData* pData)
{
    if (!pData || !pData->pTargetBase)
        return 0;

    BYTE* pBase = pData->pTargetBase;
    PIMAGE_DOS_HEADER pDos = reinterpret_cast<PIMAGE_DOS_HEADER>(pBase);
    PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(pBase + pDos->e_lfanew);

    ULONG_PTR delta = reinterpret_cast<ULONG_PTR>(pBase) - pNtHeaders->OptionalHeader.ImageBase;
    if (delta != 0)
    {
        auto& relocDir = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
        if (relocDir.Size > 0)
        {
            PIMAGE_BASE_RELOCATION pReloc = reinterpret_cast<PIMAGE_BASE_RELOCATION>(pBase + relocDir.VirtualAddress);
            while (pReloc->VirtualAddress != 0)
            {
                WORD* pRelative = reinterpret_cast<WORD*>(pReloc + 1);
                DWORD count = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
                for (DWORD i = 0; i < count; ++i)
                {
                    if ((pRelative[i] >> 12) == IMAGE_REL_BASED_DIR64)
                    {
                        ULONG_PTR* pPatch = reinterpret_cast<ULONG_PTR*>(pBase + pReloc->VirtualAddress + (pRelative[i] & 0xFFF));
                        *pPatch += delta;
                    }
                    else if ((pRelative[i] >> 12) == IMAGE_REL_BASED_HIGHLOW)
                    {
                        DWORD* pPatch = reinterpret_cast<DWORD*>(pBase + pReloc->VirtualAddress + (pRelative[i] & 0xFFF));
                        *pPatch += static_cast<DWORD>(delta);
                    }
                }
                pReloc = reinterpret_cast<PIMAGE_BASE_RELOCATION>(reinterpret_cast<BYTE*>(pReloc) + pReloc->SizeOfBlock);
            }
        }
    }

    auto& importDir = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDir.Size > 0)
    {
        PIMAGE_IMPORT_DESCRIPTOR pImport = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(pBase + importDir.VirtualAddress);
        while (pImport->Name != 0)
        {
            char* modName = reinterpret_cast<char*>(pBase + pImport->Name);
            HMODULE hMod = pData->fnLoadLibraryA(modName);
            if (hMod)
            {
                PIMAGE_THUNK_DATA pThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(pBase + pImport->FirstThunk);
                PIMAGE_THUNK_DATA pOriginalThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(pBase + (pImport->OriginalFirstThunk ? pImport->OriginalFirstThunk : pImport->FirstThunk));
                while (pOriginalThunk->u1.AddressOfData != 0)
                {
                    if (IMAGE_SNAP_BY_ORDINAL(pOriginalThunk->u1.Ordinal))
                    {
                        LPCSTR ordinal = reinterpret_cast<LPCSTR>(IMAGE_ORDINAL(pOriginalThunk->u1.Ordinal));
                        pThunk->u1.Function = reinterpret_cast<ULONG_PTR>(pData->fnGetProcAddress(hMod, ordinal));
                    }
                    else
                    {
                        PIMAGE_IMPORT_BY_NAME pIBN = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(pBase + pOriginalThunk->u1.AddressOfData);
                        pThunk->u1.Function = reinterpret_cast<ULONG_PTR>(pData->fnGetProcAddress(hMod, pIBN->Name));
                    }
                    ++pThunk;
                    ++pOriginalThunk;
                }
            }
            ++pImport;
        }
    }

    auto& exceptionDir = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
    if (exceptionDir.Size > 0 && pData->fnRtlAddFunctionTable)
    {
        PRUNTIME_FUNCTION pFunctionTable = reinterpret_cast<PRUNTIME_FUNCTION>(pBase + exceptionDir.VirtualAddress);
        DWORD entryCount = exceptionDir.Size / sizeof(RUNTIME_FUNCTION);
        pData->fnRtlAddFunctionTable(pFunctionTable, entryCount, reinterpret_cast<DWORD64>(pBase));
    }

    auto& tlsDir = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
    if (tlsDir.Size > 0)
    {
        PIMAGE_TLS_DIRECTORY pTLS = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(pBase + tlsDir.VirtualAddress);
        PIMAGE_TLS_CALLBACK* pCallback = reinterpret_cast<PIMAGE_TLS_CALLBACK*>(pTLS->AddressOfCallBacks);
        while (pCallback && *pCallback)
        {
            (*pCallback)(pBase, DLL_PROCESS_ATTACH, nullptr);
            ++pCallback;
        }
    }

    using DllMain_t = BOOL(WINAPI*)(HINSTANCE, DWORD, LPVOID);
    DllMain_t pDllMain = reinterpret_cast<DllMain_t>(pBase + pNtHeaders->OptionalHeader.AddressOfEntryPoint);
    return pDllMain(reinterpret_cast<HINSTANCE>(pBase), DLL_PROCESS_ATTACH, nullptr);
}

static DWORD GetProcessIdByName(const char* processName)
{
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32))
        {
            do
            {
                if (_stricmp(pe32.szExeFile, processName) == 0)
                {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return pid;
}

static bool ManualMap(HANDLE hProcess, const char* dllPath)
{
    std::ifstream file(dllPath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        printf("[-] DLL Okunamadi: %s\n", dllPath);
        return false;
    }

    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<BYTE> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();

    PIMAGE_DOS_HEADER pDos = reinterpret_cast<PIMAGE_DOS_HEADER>(buffer.data());
    if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
    {
        printf("[-] Gecersiz DOS basligi.\n");
        return false;
    }

    PIMAGE_NT_HEADERS pNt = reinterpret_cast<PIMAGE_NT_HEADERS>(buffer.data() + pDos->e_lfanew);
    if (pNt->Signature != IMAGE_NT_SIGNATURE)
    {
        printf("[-] Gecersiz NT basligi.\n");
        return false;
    }

    BYTE* pTargetBase = reinterpret_cast<BYTE*>(VirtualAllocEx(
        hProcess,
        nullptr,
        pNt->OptionalHeader.SizeOfImage,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE));

    if (!pTargetBase)
    {
        printf("[-] Bellek tahsis edilemedi: %lu\n", GetLastError());
        return false;
    }

    WriteProcessMemory(hProcess, pTargetBase, buffer.data(), pNt->OptionalHeader.SizeOfHeaders, nullptr);

    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
    for (WORD i = 0; i < pNt->FileHeader.NumberOfSections; ++i, ++pSection)
    {
        if (pSection->SizeOfRawData == 0) continue;
        WriteProcessMemory(
            hProcess,
            pTargetBase + pSection->VirtualAddress,
            buffer.data() + pSection->PointerToRawData,
            pSection->SizeOfRawData,
            nullptr);
    }

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");

    ShellcodeData shellData = {};
    shellData.pTargetBase = pTargetBase;
    shellData.fnLoadLibraryA = (LoadLibraryA_t)GetProcAddress(hKernel32, "LoadLibraryA");
    shellData.fnGetProcAddress = (GetProcAddress_t)GetProcAddress(hKernel32, "GetProcAddress");
    shellData.fnRtlAddFunctionTable = (RtlAddFunctionTable_t)GetProcAddress(hNtdll, "RtlAddFunctionTable");

    BYTE* pRemoteShellData = reinterpret_cast<BYTE*>(VirtualAllocEx(
        hProcess, nullptr, sizeof(ShellcodeData), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    WriteProcessMemory(hProcess, pRemoteShellData, &shellData, sizeof(ShellcodeData), nullptr);

    BYTE* pShellcode = reinterpret_cast<BYTE*>(VirtualAllocEx(
        hProcess, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    WriteProcessMemory(hProcess, pShellcode, (void*)Shellcode, 0x1000, nullptr);

    NtCreateThreadEx_t pNtCreateThreadEx = (NtCreateThreadEx_t)GetProcAddress(hNtdll, "NtCreateThreadEx");

    HANDLE hThread = nullptr;
    if (pNtCreateThreadEx)
    {
        pNtCreateThreadEx(&hThread, GENERIC_ALL, nullptr, hProcess, (LPTHREAD_START_ROUTINE)pShellcode, pRemoteShellData, 0, 0, 0, 0, nullptr);
    }

    if (!hThread)
    {
        hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pShellcode, pRemoteShellData, 0, nullptr);
    }

    if (!hThread)
    {
        printf("[-] Thread olusturulamadi: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, pTargetBase, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, pRemoteShellData, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    VirtualFreeEx(hProcess, pRemoteShellData, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);

    return true;
}

int main()
{
    printf("========================================\n");
    printf(" Bay_Hosaf_education INJECTOR\n");
    printf("========================================\n\n");

    const char* targetDll = "CS2_Internal_Local.dll";

    if (GetFileAttributesA(targetDll) == INVALID_FILE_ATTRIBUTES)
    {
        printf("[-] DLL Bulunamadi: %s\n", targetDll);
        system("pause");
        return 1;
    }
    printf("[+] DLL Bulundu: CS2_Internal_Local.dll\n");

    printf("[*] cs2.exe bekleniyor...\n");
    DWORD pid = 0;
    while (!(pid = GetProcessIdByName("cs2.exe")))
    {
        Sleep(1000);
    }
    printf("[+] cs2.exe bulundu! PID: %lu\n\n", pid);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        printf("[-] OpenProcess Hata: %lu. YONETICI OLARAK CALISTIR!\n", GetLastError());
        system("pause");
        return 1;
    }

    printf("[*] Manual Mapping basladi...\n");
    if (ManualMap(hProcess, targetDll))
    {
        printf("\n=====================================\n");
        printf(" [+] INJECT BASARILI!\n");
        printf("=====================================\n");
        printf(" END / * / SHIFT+K = Menuyu Ac\n");
    }
    else
    {
        printf("[-] Manual Map basarisiz oldu.\n");
    }

    CloseHandle(hProcess);
    printf("\nKapatmak icin ENTER'a bas...");
    getchar();
    return 0;
}
