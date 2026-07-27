#include "features/glow.h"
#include "config.h"
#include "utils/memory.h"

/*
 * ============================================================================
 * CS2 INTERNAL - GLOW / CHAMS (OPTIMIZE EDILMIS & GUVENLI IMPLEMENTASYON)
 * ============================================================================
 */

namespace Glow
{
    void SetGlowProperties(uintptr_t entity, float intensity, int mode)
    {
        if (!entity) return;

        Memory::Write<float>(entity + Netvars::m_Glow + 0x00, intensity);
        Memory::Write<float>(entity + Netvars::m_Glow + 0x04, intensity);
        Memory::Write<float>(entity + Netvars::m_Glow + 0x08, intensity);
        Memory::Write<float>(entity + Netvars::m_Glow + 0x0C, 1.0f);
        Memory::Write<int>(entity + Netvars::m_Glow + 0x30, mode);
    }

    void Run()
    {
        if (!g_Config.glow.bEnabled) return;

        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        if (!clientBase) return;

        uintptr_t localPlayer = Memory::Read<uintptr_t>(clientBase + Offsets::dwLocalPlayerPawn);
        if (!localPlayer) return;

        int localTeam = Memory::Read<int>(localPlayer + Netvars::m_iTeamNum);

        uintptr_t entityList = Memory::Read<uintptr_t>(clientBase + Offsets::dwEntityList);
        if (!entityList) return;

        for (int i = 0; i < 64; i++)
        {
            uintptr_t listEntry = Memory::Read<uintptr_t>(entityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);
            if (!listEntry) continue;

            uintptr_t entity = Memory::Read<uintptr_t>(listEntry + 0x78 * (i & 0x1FF));
            if (!entity || entity == localPlayer) continue;

            int team = Memory::Read<int>(entity + Netvars::m_iTeamNum);
            if (team == TEAM::NONE || team == TEAM::SPECTATOR) continue;

            bool alive = Memory::Read<bool>(entity + Netvars::m_bPawnIsAlive);
            if (!alive && g_Config.glow.bDisableOnDead) continue;

            if (!g_Config.glow.bTeamGlow && team == localTeam) continue;

            float* color = (team == TEAM::TERRORIST) ? g_Config.glow.fColorT : g_Config.glow.fColorCT;

            Memory::Write<float>(entity + Netvars::m_Glow + 0x00, color[0] * g_Config.glow.fGlowIntensity);
            Memory::Write<float>(entity + Netvars::m_Glow + 0x04, color[1] * g_Config.glow.fGlowIntensity);
            Memory::Write<float>(entity + Netvars::m_Glow + 0x08, color[2] * g_Config.glow.fGlowIntensity);
            Memory::Write<float>(entity + Netvars::m_Glow + 0x0C, color[3]);
            Memory::Write<int>(entity + Netvars::m_Glow + 0x30, g_Config.glow.iGlowMode);
        }
    }

    void ApplyChams()
    {
    }
}
