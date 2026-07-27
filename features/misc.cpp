#include "features/misc.h"
#include "config.h"
#include "sdk/sdk.h"
#include "utils/memory.h"
#include <windows.h>

/*
 * ============================================================================
 * CS2 INTERNAL - MISC FEATURES (OPTIMIZE EDILMIS & GUVENLI IMPLEMENTASYON)
 * ============================================================================
 */

namespace Misc
{
    void Bunnyhop()
    {
        if (!g_Config.misc.bBunnyhop) return;
        if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) return;

        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        if (!clientBase) return;

        uintptr_t localPlayer = Memory::Read<uintptr_t>(clientBase + Offsets::dwLocalPlayerPawn);
        if (!localPlayer) return;

        int flags = Memory::Read<int>(localPlayer + Netvars::m_fFlags);
        if (flags & FL::ONGROUND)
        {
            keybd_event(VK_SPACE, 0, 0, 0);
            keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
        }
    }

    void NoRecoil()
    {
        if (!g_Config.misc.bNoRecoil) return;

        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        if (!clientBase) return;

        uintptr_t localPlayer = Memory::Read<uintptr_t>(clientBase + Offsets::dwLocalPlayerPawn);
        if (!localPlayer) return;

        Vector punchAngle = Memory::Read<Vector>(localPlayer + Netvars::m_aimPunchAngle);
        Vector viewAngles = Memory::Read<Vector>(clientBase + Offsets::dwViewAngles);

        viewAngles.x -= punchAngle.x * 2.0f;
        viewAngles.y -= punchAngle.y * 2.0f;

        while (viewAngles.x > 89.0f) viewAngles.x -= 180.0f;
        while (viewAngles.x < -89.0f) viewAngles.x += 180.0f;
        while (viewAngles.y > 180.0f) viewAngles.y -= 360.0f;
        while (viewAngles.y < -180.0f) viewAngles.y += 360.0f;

        Memory::Write<Vector>(clientBase + Offsets::dwViewAngles, viewAngles);
    }

    void RadarHack()
    {
        if (!g_Config.misc.bRadarHack) return;

        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        if (!clientBase) return;

        uintptr_t entityList = Memory::Read<uintptr_t>(clientBase + Offsets::dwEntityList);
        if (!entityList) return;

        for (int i = 0; i < 64; i++)
        {
            uintptr_t listEntry = Memory::Read<uintptr_t>(entityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);
            if (!listEntry) continue;

            uintptr_t entity = Memory::Read<uintptr_t>(listEntry + 0x78 * (i & 0x1FF));
            if (!entity) continue;
        }
    }

    void FOVChanger()
    {
        if (!g_Config.fov.bEnabled) return;
    }

    void ThirdPerson()
    {
        if (!g_Config.misc.bThirdPerson) return;
    }

    void NoFlash()
    {
        if (!g_Config.visual.bNoFlash) return;
    }

    void FastStop()
    {
        if (!g_Config.misc.bFastStop) return;
    }

    void Run()
    {
        Bunnyhop();
        NoRecoil();
        RadarHack();
        FOVChanger();
        ThirdPerson();
        NoFlash();
        FastStop();
    }

    void Render()
    {
        if (g_Config.misc.bSpectatorList)
        {
        }
    }
}
