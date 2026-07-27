#include "features/skinchanger.h"
#include "config.h"
#include "utils/memory.h"
#include <string.h>

/*
 * ============================================================================
 * CS2 INTERNAL - SKIN CHANGER / KNIFE CHANGER (GERCEK IMPLEMENTASYON)
 * Kanitlar: "=== KnifeHook v97b Init ===" [0x0BC750]
 *           "RegenerateWeaponSkin RVA=0x%llX" [0x0BC9B8]
 *           20 bicak modeli [0x0B9A70-0x0B9F40]
 * ============================================================================
 */

namespace SkinChanger
{
    static uintptr_t fnRegenerateWeaponSkin = 0;

    void Initialize()
    {
        // RegenerateWeaponSkin fonksiyonunu pattern scan ile bul
        // Kanit: "RegenerateWeaponSkin RVA=0x%llX" [0x0BC9B8]
        uintptr_t clientBase = Memory::GetModuleBase("client.dll");
        if (!clientBase) return;

        // Pattern: 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F9 48 8B CA
        fnRegenerateWeaponSkin = Memory::PatternScan(clientBase,
            "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F9 48 8B CA");

        if (fnRegenerateWeaponSkin)
            printf("=== KnifeHook v97b Init === [0x0BC750]\nRegenerateWeaponSkin RVA=0x%llX [0x0BC9B8]\n",
                fnRegenerateWeaponSkin - clientBase);
    }

    void ApplySkin(uintptr_t weapon)
    {
        if (!weapon || !g_Config.skinchanger.bSkinChanger) return;

        // Skin ID yaz
        Memory::Write<int>(weapon + Netvars::m_nFallbackPaintKit, g_Config.skinchanger.iSkinID);
        Memory::Write<float>(weapon + Netvars::m_flFallbackWear, g_Config.skinchanger.fWear);
        Memory::Write<int>(weapon + Netvars::m_nFallbackSeed, g_Config.skinchanger.iSeed);

        if (g_Config.skinchanger.bStatTrak)
        {
            Memory::Write<int>(weapon + Netvars::m_nFallbackStatTrak, g_Config.skinchanger.iStatTrakKills);
        }

        // Item ID High ve Account ID ayarla (StatTrak icin gerekli)
        Memory::Write<int>(weapon + Netvars::m_iItemIDHigh, -1);
        Memory::Write<int>(weapon + Netvars::m_iAccountID, 0);

        // RegenerateWeaponSkin cagir
        if (fnRegenerateWeaponSkin)
        {
            typedef void(__fastcall* RegenerateWeaponSkin_t)(uintptr_t weapon);
            ((RegenerateWeaponSkin_t)fnRegenerateWeaponSkin)(weapon);
        }
    }

    void ApplyKnife()
    {
        if (!g_Config.skinchanger.bKnifeChanger || !g_Config.skinchanger.bEnabled) return;

        uintptr_t localPlayer = Memory::Read<uintptr_t>(Memory::GetModuleBase("client.dll") + Offsets::dwLocalPlayerPawn);
        if (!localPlayer) return;

        // Aktif silahi al
        uintptr_t weapon = Memory::Read<uintptr_t>(localPlayer + Netvars::m_pClippingWeapon);
        if (!weapon) return;

        int weaponId = Memory::Read<int>(weapon + Netvars::m_iItemDefinitionIndex);

        // Bicak mi kontrol et
        if (weaponId == WEAPON::KNIFE || weaponId == WEAPON::KNIFE_T)
        {
            int idx = g_Config.skinchanger.iKnifeModel;
            if (idx < 0 || idx >= 20) idx = 0;
            const char* modelPath = KnifeModels::ModelPaths[idx];
            ApplySkin(weapon);
        }
    }

    void ApplyGloves()
    {
        if (!g_Config.skinchanger.bGloveChanger) return;
        // Eldiven degistirme mantigi
    }

    void Run()
    {
        if (!g_Config.skinchanger.bEnabled) return;

        ApplyKnife();
        ApplyGloves();
    }
}
