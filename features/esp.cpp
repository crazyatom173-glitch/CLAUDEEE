#include "features/esp.h"
#include "config.h"
#include "hooks.h"
#include "utils/memory.h"
#include <imgui.h>
#include <string>
#include <vector>
#include <utility>

/*
 * ============================================================================
 * CS2 INTERNAL - ESP (GUNCEL CS2 KEMIK VE 3D KUTU SİSTEMİ)
 * ============================================================================
 */

namespace ESP
{
    static inline bool WorldToScreenFast(const Vector& worldPos, Vector2D& screen, uintptr_t clientBase, float width, float height)
    {
        if (!clientBase || width <= 0 || height <= 0) return false;
        ViewMatrix matrix = Memory::Read<ViewMatrix>(clientBase + Offsets::dwViewMatrix);
        return matrix.WorldToScreen(worldPos, screen, (int)width, (int)height);
    }

    bool WorldToScreen(const Vector& worldPos, Vector2D& screen)
    {
        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        ImGuiIO& io = ImGui::GetIO();
        return WorldToScreenFast(worldPos, screen, clientBase, io.DisplaySize.x, io.DisplaySize.y);
    }

    static uintptr_t GetBoneMatrix(uintptr_t entity)
    {
        if (!entity) return 0;
        uintptr_t gameSceneNode = Memory::Read<uintptr_t>(entity + Netvars::m_pGameSceneNode);
        if (!gameSceneNode) return 0;

        uintptr_t boneMatrix = Memory::Read<uintptr_t>(gameSceneNode + 0x1E0);
        if (!boneMatrix)
        {
            uintptr_t modelState = gameSceneNode + 0x160;
            boneMatrix = Memory::Read<uintptr_t>(modelState + 0x80);
        }
        return boneMatrix;
    }

    static Vector GetBonePos(uintptr_t boneMatrix, int boneId)
    {
        if (!boneMatrix) return Vector();
        Vector pos = Memory::Read<Vector>(boneMatrix + boneId * 32);
        if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
        {
            pos = Memory::Read<Vector>(boneMatrix + boneId * 48);
        }
        return pos;
    }

    void DrawBox(Vector2D top, Vector2D bottom, Color color, bool outline)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float boxHeight = bottom.y - top.y;
        float boxWidth = boxHeight * 0.40f;

        if (outline)
        {
            drawList->AddRect(
                ImVec2(top.x - boxWidth - 1, top.y - 1),
                ImVec2(top.x + boxWidth + 1, bottom.y + 1),
                IM_COL32(0, 0, 0, 255), 0.0f, 0, 1.0f);
        }

        drawList->AddRect(
            ImVec2(top.x - boxWidth, top.y),
            ImVec2(top.x + boxWidth, bottom.y),
            color.ToU32(), 0.0f, 0, 1.8f);
    }

    void DrawCornerBox(Vector2D top, Vector2D bottom, Color color)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float h = bottom.y - top.y;
        float w = h * 0.40f;
        float x = top.x - w;
        float y = top.y;

        float lineW = w / 3.0f;
        float lineH = h / 3.0f;

        drawList->AddLine(ImVec2(x, y), ImVec2(x + lineW, y), color.ToU32(), 1.8f);
        drawList->AddLine(ImVec2(x, y), ImVec2(x, y + lineH), color.ToU32(), 1.8f);

        drawList->AddLine(ImVec2(x + w * 2, y), ImVec2(x + w * 2 - lineW, y), color.ToU32(), 1.8f);
        drawList->AddLine(ImVec2(x + w * 2, y), ImVec2(x + w * 2, y + lineH), color.ToU32(), 1.8f);

        drawList->AddLine(ImVec2(x, y + h), ImVec2(x + lineW, y + h), color.ToU32(), 1.8f);
        drawList->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - lineH), color.ToU32(), 1.8f);

        drawList->AddLine(ImVec2(x + w * 2, y + h), ImVec2(x + w * 2 - lineW, y + h), color.ToU32(), 1.8f);
        drawList->AddLine(ImVec2(x + w * 2, y + h), ImVec2(x + w * 2, y + h - lineH), color.ToU32(), 1.8f);
    }

    void DrawHealthBar(Vector2D top, Vector2D bottom, int health, int maxHealth)
    {
        if (maxHealth <= 0) maxHealth = 100;
        if (health < 0) health = 0;
        if (health > maxHealth) health = maxHealth;

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float boxHeight = bottom.y - top.y;
        float barHeight = boxHeight * (health / (float)maxHealth);
        float barWidth = 3.5f;
        float boxWidth = boxHeight * 0.40f;

        drawList->AddRectFilled(
            ImVec2(top.x - boxWidth - barWidth - 3, top.y),
            ImVec2(top.x - boxWidth - 3, bottom.y),
            IM_COL32(0, 0, 0, 180));

        Color healthColor = health > 70 ? Color(0, 1, 0, 1) :
                              health > 30 ? Color(1, 1, 0, 1) : Color(1, 0, 0, 1);

        drawList->AddRectFilled(
            ImVec2(top.x - boxWidth - barWidth - 3, bottom.y - barHeight),
            ImVec2(top.x - boxWidth - 3, bottom.y),
            healthColor.ToU32());
    }

    void DrawName(Vector2D top, const char* name)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        ImVec2 textSize = ImGui::CalcTextSize(name);
        drawList->AddText(
            ImVec2(top.x - textSize.x / 2.0f, top.y - 15.0f),
            IM_COL32(255, 255, 255, 255),
            name);
    }

    void DrawWeapon(Vector2D bottom, const char* weapon)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        ImVec2 textSize = ImGui::CalcTextSize(weapon);
        drawList->AddText(
            ImVec2(bottom.x - textSize.x / 2.0f, bottom.y + 2.0f),
            IM_COL32(255, 255, 255, 255),
            weapon);
    }

    void DrawSkeleton(uintptr_t entity, Color color, uintptr_t clientBase, float width, float height)
    {
        uintptr_t boneMatrix = GetBoneMatrix(entity);
        if (!boneMatrix) return;

        static const std::pair<int, int> bonePairs[] = {
            { BONE::HEAD, BONE::NECK },
            { BONE::NECK, BONE::SPINE },
            { BONE::SPINE, BONE::PELVIS },
            { BONE::NECK, BONE::L_UPPER_ARM },
            { BONE::L_UPPER_ARM, BONE::L_FOREARM },
            { BONE::L_FOREARM, BONE::L_HAND },
            { BONE::NECK, BONE::R_UPPER_ARM },
            { BONE::R_UPPER_ARM, BONE::R_FOREARM },
            { BONE::R_FOREARM, BONE::R_HAND },
            { BONE::PELVIS, BONE::L_THIGH },
            { BONE::L_THIGH, BONE::L_CALF },
            { BONE::L_CALF, BONE::L_FOOT },
            { BONE::PELVIS, BONE::R_THIGH },
            { BONE::R_THIGH, BONE::R_CALF },
            { BONE::R_CALF, BONE::R_FOOT }
        };

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        uint32_t col = color.ToU32();

        for (const auto& pair : bonePairs)
        {
            Vector b1World = GetBonePos(boneMatrix, pair.first);
            Vector b2World = GetBonePos(boneMatrix, pair.second);

            Vector2D b1Screen, b2Screen;
            if (WorldToScreenFast(b1World, b1Screen, clientBase, width, height) &&
                WorldToScreenFast(b2World, b2Screen, clientBase, width, height))
            {
                drawList->AddLine(ImVec2(b1Screen.x, b1Screen.y), ImVec2(b2Screen.x, b2Screen.y), col, 1.8f);
            }
        }
    }

    void DrawHeadDot(uintptr_t entity, Color color, uintptr_t clientBase, float width, float height)
    {
        uintptr_t boneMatrix = GetBoneMatrix(entity);
        if (!boneMatrix) return;

        Vector headWorld = GetBonePos(boneMatrix, BONE::HEAD);
        Vector2D headScreen;
        if (WorldToScreenFast(headWorld, headScreen, clientBase, width, height))
        {
            ImDrawList* drawList = ImGui::GetBackgroundDrawList();
            drawList->AddCircleFilled(ImVec2(headScreen.x, headScreen.y), 4.0f, color.ToU32());
            drawList->AddCircle(ImVec2(headScreen.x, headScreen.y), 4.0f, IM_COL32(0, 0, 0, 255));
        }
    }

    void DrawSnaplines(Vector2D screenPos, Color color, float screenHeight)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(
            ImVec2(screenPos.x, screenPos.y),
            ImVec2(screenPos.x, screenHeight),
            color.ToU32(), 1.2f);
    }

    void DrawBombInfo(uintptr_t bomb, uintptr_t clientBase, float width, float height)
    {
        if (!bomb) return;

        Vector bombPos = Memory::Read<Vector>(Memory::Read<uintptr_t>(bomb + Netvars::m_pGameSceneNode) + Netvars::m_vecOrigin);
        Vector2D screenPos;
        if (!WorldToScreenFast(bombPos, screenPos, clientBase, width, height)) return;

        bool beingDefused = Memory::Read<bool>(bomb + Netvars::m_bBeingDefused);
        float defuseTime = Memory::Read<float>(bomb + Netvars::m_flDefuseCountDown);

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        char buffer[128];
        if (beingDefused)
            sprintf_s(buffer, "[BOMB] Imha Ediliyor: %.1fs", defuseTime);
        else
            sprintf_s(buffer, "[BOMB] Kuruldu!");

        drawList->AddText(
            ImVec2(screenPos.x, screenPos.y),
            IM_COL32(255, 50, 50, 255),
            buffer);
    }

    void Render()
    {
        if (!g_Config.esp.bEnabled) return;

        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        if (!clientBase) return;

        uintptr_t localPlayer = Memory::Read<uintptr_t>(clientBase + Offsets::dwLocalPlayerPawn);
        if (!localPlayer) return;

        int localTeam = Memory::Read<int>(localPlayer + Netvars::m_iTeamNum);
        Vector localPos = Memory::Read<Vector>(Memory::Read<uintptr_t>(localPlayer + Netvars::m_pGameSceneNode) + Netvars::m_vecOrigin);

        uintptr_t entityList = Memory::Read<uintptr_t>(clientBase + Offsets::dwEntityList);
        if (!entityList) return;

        ImGuiIO& io = ImGui::GetIO();
        float screenWidth = io.DisplaySize.x;
        float screenHeight = io.DisplaySize.y;

        for (int i = 0; i < 64; i++)
        {
            uintptr_t listEntry = Memory::Read<uintptr_t>(entityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);
            if (!listEntry) continue;

            uintptr_t entity = Memory::Read<uintptr_t>(listEntry + 0x78 * (i & 0x1FF));
            if (!entity || entity == localPlayer) continue;

            int team = Memory::Read<int>(entity + Netvars::m_iTeamNum);
            if (team == TEAM::NONE || team == TEAM::SPECTATOR) continue;

            bool alive = Memory::Read<bool>(entity + Netvars::m_bPawnIsAlive);
            if (!alive) continue;

            Vector pos = Memory::Read<Vector>(Memory::Read<uintptr_t>(entity + Netvars::m_pGameSceneNode) + Netvars::m_vecOrigin);
            float distance = pos.DistTo(localPos);

            if (distance > g_Config.esp.fMaxDistance) continue;

            uintptr_t boneMatrix = GetBoneMatrix(entity);
            Vector headPos = GetBonePos(boneMatrix, BONE::HEAD);
            if (headPos.x == 0.0f && headPos.y == 0.0f)
            {
                headPos = pos;
                headPos.z += 72.0f;
            }
            else
            {
                headPos.z += 8.0f;
            }

            Vector2D screenPos, headScreenPos;
            if (!WorldToScreenFast(pos, screenPos, clientBase, screenWidth, screenHeight) ||
                !WorldToScreenFast(headPos, headScreenPos, clientBase, screenWidth, screenHeight))
                continue;

            Color color = (team == localTeam) ? Color(0, 1, 0, 1) :
                          (team == TEAM::TERRORIST) ? Color(1, 0, 0, 1) : Color(0, 0, 1, 1);

            if (g_Config.esp.bBox)
                DrawBox(headScreenPos, screenPos, color, true);

            if (g_Config.esp.bCornerBox)
                DrawCornerBox(headScreenPos, screenPos, color);

            if (g_Config.esp.bHealthBar)
            {
                int health = Memory::Read<int>(entity + Netvars::m_iPawnHealth);
                int maxHealth = Memory::Read<int>(entity + Netvars::m_iMaxHealth);
                DrawHealthBar(headScreenPos, screenPos, health, maxHealth);
            }

            if (g_Config.esp.bName)
            {
                DrawName(headScreenPos, "Enemy");
            }

            if (g_Config.esp.bSnaplines)
                DrawSnaplines(screenPos, color, screenHeight);

            if (g_Config.esp.bSkeleton)
                DrawSkeleton(entity, color, clientBase, screenWidth, screenHeight);

            if (g_Config.esp.bHeadDot)
                DrawHeadDot(entity, color, clientBase, screenWidth, screenHeight);
        }

        if (g_Config.esp.bBombESP)
        {
            uintptr_t plantedC4 = Memory::Read<uintptr_t>(clientBase + Offsets::dwPlantedC4);
            if (plantedC4)
                DrawBombInfo(plantedC4, clientBase, screenWidth, screenHeight);
        }
    }
}
