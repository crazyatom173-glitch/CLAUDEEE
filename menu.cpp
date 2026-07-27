#include "menu.h"
#include "config.h"
#include "features.h"
#include <imgui.h>

/*
 * ============================================================================
 * CS2 INTERNAL - ImGui MODERN DARK CYBER MENU SISTEMI
 * ============================================================================
 */

namespace Menu
{
    void ApplyTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        style.WindowRounding = 8.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 5.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 4.0f;
        style.GrabRounding = 4.0f;
        style.TabRounding = 6.0f;

        style.WindowPadding = ImVec2(14.0f, 14.0f);
        style.FramePadding = ImVec2(10.0f, 6.0f);
        style.ItemSpacing = ImVec2(10.0f, 8.0f);
        style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;

        colors[ImGuiCol_Text]                  = ImVec4(0.92f, 0.94f, 0.98f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.55f, 0.65f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.09f, 0.10f, 0.14f, 0.94f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.12f, 0.14f, 0.19f, 0.60f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.11f, 0.12f, 0.17f, 0.96f);
        colors[ImGuiCol_Border]                = ImVec4(0.22f, 0.26f, 0.36f, 0.50f);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.15f, 0.18f, 0.25f, 0.60f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.22f, 0.26f, 0.36f, 0.80f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.28f, 0.33f, 0.46f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.09f, 0.13f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.13f, 0.16f, 0.24f, 1.00f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.38f, 0.55f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.38f, 0.55f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.48f, 0.65f, 1.00f, 1.00f);
        colors[ImGuiCol_Button]                = ImVec4(0.18f, 0.22f, 0.32f, 0.70f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.35f, 0.52f, 1.00f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.38f, 0.45f, 0.68f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.25f, 0.38f, 0.65f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.28f, 0.35f, 0.52f, 0.80f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.38f, 0.45f, 0.68f, 1.00f);
        colors[ImGuiCol_Tab]                   = ImVec4(0.12f, 0.14f, 0.20f, 0.80f);
        colors[ImGuiCol_TabHovered]            = ImVec4(0.26f, 0.32f, 0.48f, 1.00f);
        colors[ImGuiCol_TabActive]             = ImVec4(0.22f, 0.28f, 0.42f, 1.00f);
    }

    void Render()
    {
        ImGui::SetNextWindowSize(ImVec2(560, 460), ImGuiCond_FirstUseEver);
        ImGui::Begin("Bay_Hosaf_education | CS2 Internal v2.0", &g_Config.bMenuOpen, ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginTabBar("##MainTabs"))
        {
            if (ImGui::BeginTabItem("Aimbot"))   { RenderAimbotTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("ESP"))      { RenderESPTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("Glow"))     { RenderGlowTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("Misc"))     { RenderMiscTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("FOV"))      { RenderFOVTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("Visuals"))   { RenderVisualTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("Skins"))    { RenderSkinTab(); ImGui::EndTabItem(); }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    void RenderAimbotTab()
    {
        ImGui::Checkbox("Aimbot", &g_Config.aimbot.bEnabled);
        ImGui::Checkbox("Visible Check", &g_Config.aimbot.bVisibleCheck);
        ImGui::Checkbox("Team Check", &g_Config.aimbot.bTeamCheck);
        ImGui::Checkbox("FOV Dairesi", &g_Config.aimbot.bDrawFOV);
        ImGui::Checkbox("Draw Target", &g_Config.aimbot.bDrawTarget);
        ImGui::Checkbox("Auto Shoot", &g_Config.aimbot.bAutoShoot);
        ImGui::Checkbox("Triggerbot", &g_Config.aimbot.bTriggerbot);
        ImGui::SliderFloat("Smooth", &g_Config.aimbot.fSmooth, 1.0f, 20.0f);
        ImGui::SliderFloat("FOV", &g_Config.aimbot.fFOV, 1.0f, 30.0f);
        ImGui::SliderFloat("RCS Pitch", &g_Config.aimbot.fRcsPitch, 0.0f, 2.0f);
        ImGui::SliderFloat("RCS Yaw", &g_Config.aimbot.fRcsYaw, 0.0f, 2.0f);
        const char* hitboxes[] = { "Head", "Neck", "Chest", "Stomach" };
        ImGui::Combo("Hitbox", &g_Config.aimbot.iHitbox, hitboxes, IM_ARRAYSIZE(hitboxes));
    }

    void RenderESPTab()
    {
        ImGui::Checkbox("Enabled", &g_Config.esp.bEnabled);
        ImGui::Checkbox("ESP KUTU", &g_Config.esp.bBox);
        ImGui::Checkbox("Corner Box", &g_Config.esp.bCornerBox);
        ImGui::Checkbox("Health Bar", &g_Config.esp.bHealthBar);
        ImGui::Checkbox("Armor", &g_Config.esp.bArmorBar);
        ImGui::Checkbox("Name", &g_Config.esp.bName);
        ImGui::Checkbox("Weapon", &g_Config.esp.bWeapon);
        ImGui::Checkbox("Distance", &g_Config.esp.bDistance);
        ImGui::Checkbox("Iskelet (Skeleton)", &g_Config.esp.bSkeleton);
        ImGui::Checkbox("Kafa Noktasi (Head Dot)", &g_Config.esp.bHeadDot);
        ImGui::Checkbox("Snaplines", &g_Config.esp.bSnaplines);
        ImGui::Checkbox("Bomba ESP", &g_Config.esp.bBombESP);
        ImGui::Checkbox("Grenade ESP", &g_Config.esp.bGrenadeESP);
        ImGui::Checkbox("ESP PREVIEW", &g_Config.esp.bPreview);
        ImGui::SliderFloat("Max Distance", &g_Config.esp.fMaxDistance, 100.0f, 10000.0f);
    }

    void RenderGlowTab()
    {
        ImGui::Checkbox("GLOW CHAMS", &g_Config.glow.bEnabled);
        ImGui::Checkbox("Chams", &g_Config.glow.bChams);
        ImGui::Checkbox("Chams XQZ", &g_Config.glow.bChamsXQZ);
        ImGui::Checkbox("Chams Flat", &g_Config.glow.bChamsFlat);
        ImGui::Checkbox("Chams Wireframe", &g_Config.glow.bChamsWireframe);
        const char* glowModes[] = { "Default", "Full Body", "Wireframe" };
        ImGui::Combo("Glow Mod", &g_Config.glow.iGlowMode, glowModes, IM_ARRAYSIZE(glowModes));
        ImGui::SliderFloat("Glow Yogunluk", &g_Config.glow.fGlowIntensity, 0.1f, 5.0f);
        ImGui::Checkbox("Olu Glow Kapat", &g_Config.glow.bDisableOnDead);
        ImGui::Checkbox("Team Glow", &g_Config.glow.bTeamGlow);
        ImGui::ColorEdit4("T Color", g_Config.glow.fColorT);
        ImGui::ColorEdit4("CT Color", g_Config.glow.fColorCT);
    }

    void RenderMiscTab()
    {
        ImGui::Checkbox("Bunnyhop", &g_Config.misc.bBunnyhop);
        ImGui::Checkbox("Auto Strafe", &g_Config.misc.bAutoStrafe);
        ImGui::Checkbox("Edge Jump", &g_Config.misc.bEdgeJump);
        ImGui::Checkbox("Radar Hack", &g_Config.misc.bRadarHack);
        ImGui::Checkbox("NoRecoil", &g_Config.misc.bNoRecoil);
        ImGui::Checkbox("No Visual Recoil", &g_Config.misc.bNoVisualRecoil);
        ImGui::Checkbox("Third Person", &g_Config.misc.bThirdPerson);
        ImGui::Checkbox("Fast Stop", &g_Config.misc.bFastStop);
        ImGui::Checkbox("Auto Accept", &g_Config.misc.bAutoAccept);
        ImGui::Checkbox("Reveal Ranks", &g_Config.misc.bRevealRanks);
        ImGui::Checkbox("Spectator List", &g_Config.misc.bSpectatorList);
        ImGui::SliderFloat("TP Distance", &g_Config.misc.fThirdPersonDistance, 50.0f, 200.0f);
    }

    void RenderFOVTab()
    {
        ImGui::Checkbox("FOV Aktif", &g_Config.fov.bEnabled);
        ImGui::SliderFloat("FOV Degeri", &g_Config.fov.fValue, 60.0f, 150.0f);
        ImGui::SliderFloat("Viewmodel FOV", &g_Config.fov.fViewmodel, 50.0f, 120.0f);
        ImGui::Checkbox("Override Zoom", &g_Config.fov.bOverrideZoom);
        ImGui::SliderFloat("Zoom FOV", &g_Config.fov.fZoomFOV, 10.0f, 60.0f);
    }

    void RenderVisualTab()
    {
        ImGui::Checkbox("Ozel Crosshair", &g_Config.visual.bCustomCrosshair);
        ImGui::Checkbox("Recoil Crosshair", &g_Config.visual.bRecoilCrosshair);
        ImGui::Checkbox("Spread Crosshair", &g_Config.visual.bSpreadCrosshair);
        ImGui::Checkbox("NoScope Crosshair", &g_Config.visual.bNoScopeCrosshair);
        ImGui::Checkbox("Steam Bilgi", &g_Config.visual.bSteamInfo);
        ImGui::Checkbox("No Smoke", &g_Config.visual.bNoSmoke);
        ImGui::Checkbox("No Flash", &g_Config.visual.bNoFlash);
        ImGui::Checkbox("No Scope", &g_Config.visual.bNoScope);
        ImGui::Checkbox("Night Mode", &g_Config.visual.bNightMode);
        ImGui::SliderFloat("Brightness", &g_Config.visual.fBrightness, 0.1f, 2.0f);
    }

    void RenderSkinTab()
    {
        ImGui::Checkbox("Skin Changer", &g_Config.skinchanger.bEnabled);
        ImGui::Checkbox("Knife Changer", &g_Config.skinchanger.bKnifeChanger);
        ImGui::Combo("Bicak Modeli", &g_Config.skinchanger.iKnifeModel,
            "Karambit\0Bayonet\0M9 Bayonet\0Butterfly\0Flip\0Gut\0\0");
        ImGui::Checkbox("Glove Changer", &g_Config.skinchanger.bGloveChanger);
        ImGui::Checkbox("Skin Override", &g_Config.skinchanger.bSkinChanger);
        ImGui::SliderInt("Skin ID", &g_Config.skinchanger.iSkinID, 0, 1000);
        ImGui::SliderFloat("Wear", &g_Config.skinchanger.fWear, 0.0f, 1.0f);
        ImGui::SliderInt("Seed", &g_Config.skinchanger.iSeed, 0, 1000);
        ImGui::Checkbox("StatTrak", &g_Config.skinchanger.bStatTrak);
        ImGui::SliderInt("ST Kills", &g_Config.skinchanger.iStatTrakKills, 0, 999999);
        ImGui::InputText("Name Tag", g_Config.skinchanger.szNameTag, sizeof(g_Config.skinchanger.szNameTag));
    }
}
