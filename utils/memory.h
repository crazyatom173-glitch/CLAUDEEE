#pragma once
#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include <initializer_list>
#include <vector>
#include <cstdio>

#pragma comment(lib, "psapi.lib")

/*
 * ============================================================================
 * CS2 INTERNAL - BELLEK OKUMA/YAZMA YARDIMCI FONKSIYONLARI
 * ============================================================================
 */

namespace Memory
{
    inline uintptr_t GetModuleBase(const char* moduleName)
    {
        return (uintptr_t)GetModuleHandleA(moduleName);
    }

    inline uintptr_t GetModuleBaseW(const wchar_t* moduleName)
    {
        return (uintptr_t)GetModuleHandleW(moduleName);
    }

    inline MODULEINFO GetModuleInfo(HMODULE hModule)
    {
        MODULEINFO modInfo = {};
        if (hModule)
        {
            GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo));
        }
        return modInfo;
    }

    template <typename T>
    inline T Read(uintptr_t address)
    {
        if (!address) return T{};
        __try {
            return *(T*)address;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return T{};
        }
    }

    template <typename T>
    inline T ReadUnsafe(uintptr_t address)
    {
        return *(T*)address;
    }

    template <typename T>
    inline bool Write(uintptr_t address, T value)
    {
        if (!address) return false;

        DWORD oldProtect;
        if (!VirtualProtect((void*)address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect))
            return false;

        __try {
            *(T*)address = value;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            VirtualProtect((void*)address, sizeof(T), oldProtect, &oldProtect);
            return false;
        }

        VirtualProtect((void*)address, sizeof(T), oldProtect, &oldProtect);
        return true;
    }

    inline uintptr_t ReadChain(uintptr_t base, std::initializer_list<uintptr_t> offsets)
    {
        uintptr_t addr = base;
        for (auto offset : offsets)
        {
            addr = Read<uintptr_t>(addr + offset);
            if (!addr) return 0;
        }
        return addr;
    }

    inline uintptr_t PatternScan(uintptr_t moduleBase, const char* signature)
    {
        if (!moduleBase || !signature) return 0;

        MODULEINFO modInfo = GetModuleInfo((HMODULE)moduleBase);
        if (!modInfo.SizeOfImage) return 0;

        auto patternBytes = std::vector<int>();
        auto start = const_cast<char*>(signature);
        auto end = start + strlen(signature);

        for (auto current = start; current < end; ++current)
        {
            if (*current == ' ' || *current == '\t') continue;
            if (*current == '?')
            {
                patternBytes.push_back(-1);
                if (*(current + 1) == '?') ++current;
            }
            else
            {
                patternBytes.push_back(strtoul(current, &current, 16));
            }
        }

        auto scanBytes = reinterpret_cast<uint8_t*>(moduleBase);
        auto patternLength = patternBytes.size();
        auto dataLength = modInfo.SizeOfImage - patternLength;

        for (size_t i = 0; i <= dataLength; ++i)
        {
            bool found = true;
            for (size_t j = 0; j < patternLength; ++j)
            {
                if (patternBytes[j] != -1 && scanBytes[i + j] != patternBytes[j])
                {
                    found = false;
                    break;
                }
            }
            if (found) return moduleBase + i;
        }
        return 0;
    }

    inline uintptr_t GetRelativeAddress(uintptr_t instructionAddr, int offset, int instructionSize)
    {
        if (!instructionAddr) return 0;
        __try {
            return instructionAddr + instructionSize + *(int32_t*)(instructionAddr + offset);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return 0;
        }
    }

    inline bool IsValidMemory(uintptr_t address)
    {
        MEMORY_BASIC_INFORMATION mbi;
        if (!VirtualQuery((void*)address, &mbi, sizeof(mbi))) return false;
        return (mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_NOACCESS));
    }
}
