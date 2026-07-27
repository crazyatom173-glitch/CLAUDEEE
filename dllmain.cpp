#include <windows.h>
#include "hooks.h"
#include "sdk/interfaces.h"
#include "sdk/sdk.h"
#include "config.h"
#include "features.h"
#include "utils/auto_updater.h"
#include <stdio.h>

static HMODULE g_hModule = nullptr;

DWORD WINAPI MainThread(LPVOID lpParam)
{
    AllocConsole();
    FILE* pFile = nullptr;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    freopen_s(&pFile, "CONIN$", "r", stdin);

    printf("[+] Bay_Hosaf_education DLL yuklendi!\n");

    HMODULE hClient = nullptr;
    while (!(hClient = GetModuleHandleA("client.dll")))
    {
        Sleep(100);
    }
    printf("[+] client.dll bulundu!\n");

    HMODULE hEngine = nullptr;
    while (!(hEngine = GetModuleHandleA("engine2.dll")))
    {
        Sleep(100);
    }
    printf("[+] engine2.dll bulundu!\n");

    printf("[*] AutoUpdater calistiriliyor...\n");
    AutoUpdater::UpdateOffsets();

    if (!Hooks::Initialize())
    {
        printf("[-] DX11 Hook basarisiz!\n");
        if (pFile) fclose(pFile);
        FreeConsole();
        FreeLibraryAndExitThread(g_hModule, 1);
        return 1;
    }
    printf("[+] DX11 Hook Basarili!\n");

    while (!(GetAsyncKeyState(g_Config.iPanicKey) & 0x8000))
    {
        Sleep(10);
    }

    printf("[+] DLL unloaded.\n");
    Hooks::Cleanup();

    if (pFile) fclose(pFile);
    FreeConsole();
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
