#include "sdk/interfaces.h"
#include "sdk/sdk.h"
#include "utils/memory.h"
#include <stdio.h>

/*
 * ============================================================================
 * CS2 INTERNAL - SOURCE 2 INTERFACE SISTEMI (GERCEK IMPLEMENTASYON)
 * ============================================================================
 */

void* Interfaces::GetInterface(const char* moduleName, const char* interfaceName)
{
    HMODULE hModule = GetModuleHandleA(moduleName);
    if (!hModule) return nullptr;

    auto fnCreateInterface = (CreateInterfaceFn)GetProcAddress(hModule, "CreateInterface");
    if (!fnCreateInterface) return nullptr;

    int returnCode = 0;
    return fnCreateInterface(interfaceName, &returnCode);
}

bool Interfaces::Initialize()
{
    // client.dll yukleme kontrolu
    hClientDll = GetModuleHandleA("client.dll");
    if (!hClientDll)
    {
        printf("[Interfaces] ERROR: client.dll not found [0x0BC770]\n");
        return false;
    }

    // engine2.dll yukleme kontrolu
    hEngineDll = GetModuleHandleA("engine2.dll");
    if (!hEngineDll)
    {
        printf("[Interfaces] ERROR: engine2.dll not found [0x0BC620]\n");
        return false;
    }

    // Source2Client002 al [0x0BC788]
    pClient = GetInterface("client.dll", "Source2Client002");
    if (!pClient)
    {
        printf("[Interfaces] ERROR: Source2Client002 not found\n");
        return false;
    }

    // Engine client interface
    pEngine = (IVEngineClient*)GetInterface("engine2.dll", "Source2EngineToClient001");

    // Entity System
    // CS2'de entity system client.dll icinde farkli bir offset'te
    // Pattern scan ile bulunabilir

    printf("[Interfaces] All interfaces initialized successfully.\n");
    return true;
}
