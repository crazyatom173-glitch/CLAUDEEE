#include "features/aimbot.h"
#include "config.h"
#include "utils/memory.h"
#include <windows.h>
#include <cmath>
#include <algorithm>

/*
 * ============================================================================
 * CS2 INTERNAL - AIMBOT + TRIGGERBOT (GERÇEK DİNAMİK KEMİK & TUŞ KONTROLÜ)
 * ============================================================================
 */

namespace Aimbot
{
    Vector CalcAngle(const Vector& src, const Vector& dst)
    {
        Vector delta = dst - src;
        float len = delta.Length();
        if (len <= 0.001f) return Vector();

        float ratio = std::clamp(delta.z / len, -1.0f, 1.0f);
        float pitch = -asin(ratio) * (180.0f / 3.14159265f);
        float yaw = atan2(delta.y, delta.x) * (180.0f / 3.14159265f);
        return Vector(pitch, yaw, 0);
    }

    float GetFOV(const Vector& viewAngle, const Vector& aimAngle)
    {
        Vector delta = aimAngle - viewAngle;
        while (delta.x > 180.0f) delta.x -= 360.0f;
        while (delta.x < -180.0f) delta.x += 360.0f;
        while (delta.y > 180.0f) delta.y -= 360.0f;
        while (delta.y < -180.0f) delta.y += 360.0f;
        return sqrt(delta.x * delta.x + delta.y * delta.y);
    }

    Vector SmoothAngle(const Vector& current, const Vector& target, float factor)
    {
        if (factor <= 1.0f) return target;
        Vector delta = target - current;
        while (delta.x > 180.0f) delta.x -= 360.0f;
        while (delta.x < -180.0f) delta.x += 360.0f;
        while (delta.y > 180.0f) delta.y -= 360.0f;
        while (delta.y < -180.0f) delta.y += 360.0f;

        return current + delta / factor;
    }

    bool IsVisible(uintptr_t entity)
    {
        if (!entity) return false;
        return Memory::Read<bool>(entity + Netvars::m_bPawnIsAlive);
    }

    Vector GetBonePosition(uintptr_t entity, int boneId)
    {
        if (!entity) return Vector();

        uintptr_t gameSceneNode = Memory::Read<uintptr_t>(entity + Netvars::m_pGameSceneNode);
        if (!gameSceneNode) return Vector();

        uintptr_t boneMatrix = Memory::Read<uintptr_t>(gameSceneNode + 0x1E0);
        if (!boneMatrix)
        {
            uintptr_t modelState = gameSceneNode + 0x160;
            boneMatrix = Memory::Read<uintptr_t>(modelState + 0x80);
        }
        if (!boneMatrix) return Vector();

        Vector pos = Memory::Read<Vector>(boneMatrix + boneId * 32);
        if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
        {
            pos = Memory::Read<Vector>(boneMatrix + boneId * 48);
        }
        return pos;
    }

    void Run()
    {
        if (!g_Config.aimbot.bEnabled) return;

        bool bAimKeyPressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) || (GetAsyncKeyState(VK_LMENU) & 0x8000);
        if (!bAimKeyPressed && !g_Config.aimbot.bAutoShoot) return;

        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        if (!clientBase) return;

        uintptr_t localPlayer = Memory::Read<uintptr_t>(clientBase + Offsets::dwLocalPlayerPawn);
        if (!localPlayer) return;

        if (!Memory::Read<bool>(localPlayer + Netvars::m_bPawnIsAlive)) return;

        uintptr_t localSceneNode = Memory::Read<uintptr_t>(localPlayer + Netvars::m_pGameSceneNode);
        Vector localPos = Memory::Read<Vector>(localSceneNode + Netvars::m_vecOrigin);
        Vector eyePos = localPos;
        eyePos.z += 64.0f;

        Vector viewAngles = Memory::Read<Vector>(clientBase + Offsets::dwViewAngles);
        int localTeam = Memory::Read<int>(localPlayer + Netvars::m_iTeamNum);

        float bestFOV = g_Config.aimbot.fFOV;
        uintptr_t bestTarget = 0;
        Vector bestTargetPos;

        uintptr_t entityList = Memory::Read<uintptr_t>(clientBase + Offsets::dwEntityList);
        if (!entityList) return;

        int targetBone = BONE::HEAD;
        if (g_Config.aimbot.iHitbox == 1) targetBone = BONE::NECK;
        else if (g_Config.aimbot.iHitbox == 2) targetBone = BONE::SPINE;
        else if (g_Config.aimbot.iHitbox == 3) targetBone = BONE::PELVIS;

        for (int i = 0; i < 64; i++)
        {
            uintptr_t listEntry = Memory::Read<uintptr_t>(entityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);
            if (!listEntry) continue;

            uintptr_t entity = Memory::Read<uintptr_t>(listEntry + 0x78 * (i & 0x1FF));
            if (!entity || entity == localPlayer) continue;

            int team = Memory::Read<int>(entity + Netvars::m_iTeamNum);
            if (g_Config.aimbot.bTeamCheck && (team == localTeam || team == TEAM::NONE || team == TEAM::SPECTATOR))
                continue;

            if (!Memory::Read<bool>(entity + Netvars::m_bPawnIsAlive))
                continue;

            Vector targetPos = GetBonePosition(entity, targetBone);
            if (targetPos.x == 0.0f && targetPos.y == 0.0f && targetPos.z == 0.0f)
            {
                targetPos = Memory::Read<Vector>(Memory::Read<uintptr_t>(entity + Netvars::m_pGameSceneNode) + Netvars::m_vecOrigin);
                targetPos.z += 65.0f;
            }

            Vector aimAngles = CalcAngle(eyePos, targetPos);
            float fov = GetFOV(viewAngles, aimAngles);

            if (fov < bestFOV)
            {
                bestFOV = fov;
                bestTarget = entity;
                bestTargetPos = targetPos;
            }
        }

        if (bestTarget != 0)
        {
            Vector aimAngles = CalcAngle(eyePos, bestTargetPos);

            if (g_Config.aimbot.fRcsPitch > 0 || g_Config.aimbot.fRcsYaw > 0)
            {
                Vector punch = Memory::Read<Vector>(localPlayer + Netvars::m_aimPunchAngle);
                aimAngles.x -= punch.x * g_Config.aimbot.fRcsPitch;
                aimAngles.y -= punch.y * g_Config.aimbot.fRcsYaw;
            }

            if (g_Config.aimbot.fSmooth > 1.0f)
                aimAngles = SmoothAngle(viewAngles, aimAngles, g_Config.aimbot.fSmooth);

            while (aimAngles.x > 89.0f) aimAngles.x -= 180.0f;
            while (aimAngles.x < -89.0f) aimAngles.x += 180.0f;
            while (aimAngles.y > 180.0f) aimAngles.y -= 360.0f;
            while (aimAngles.y < -180.0f) aimAngles.y += 360.0f;

            Memory::Write<Vector>(clientBase + Offsets::dwViewAngles, aimAngles);

            if (g_Config.aimbot.bAutoShoot)
            {
                if (bestFOV < 2.5f)
                {
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                }
            }
        }
    }

    void RenderFOV()
    {
        if (!g_Config.aimbot.bDrawFOV) return;
    }
}
