#pragma once
#include <cstdint>
#include <cmath>

/*
 * ============================================================================
 * CS2 INTERNAL SDK v2.0
 * Kaynak: CS2_Internal_Local.dll (953 KB) Tersine Muhendislik Analizi
 * String Count: 12,999 ASCII + 389 Unicode
 * Import Count: 156 fonksiyon (6 DLL)
 * ============================================================================
 */

// ============================================================================
// GLOBAL OFFSETS (client.dll + engine2.dll)
// ============================================================================
namespace Offsets
{
    inline uintptr_t dwLocalPlayerPawn     = 0x16D4F48;  // [0x0BCB90]
    inline uintptr_t dwLocalPlayerController = 0x17A0718;
    inline uintptr_t dwEntityList          = 0x17A1718;  // Entity array
    inline uintptr_t dwViewMatrix          = 0x1820150;  // [0x0BCBA8]
    inline uintptr_t dwGlobalVars          = 0x16BDE80; // [0x0BCBB8]
    inline uintptr_t dwSensitivity         = 0x180D5A8; // [0x0BCBD8]
    inline uintptr_t dwSensitivity_sensitivity = 0x40;   // [0x0BCBE8]
    inline uintptr_t dwViewAngles          = 0x1890F88;  // [0x0BCC08]
    inline uintptr_t dwNetworkGameClient   = 0x486AB0;  // [0x0BCC18]
    inline uintptr_t dwNetworkGameClient_signOnState = 0x240; // [0x0BCC30]
    inline uintptr_t dwNetworkGameClient_deltaTick = 0x258;   // [0x0BCC50]
    inline uintptr_t dwBuildNumber         = 0x475ED4;  // [0x0BCC70]
    inline uintptr_t dwGameTypes           = 0x4823A0;
    inline uintptr_t dwGameRules         = 0x180B4C0;
    inline uintptr_t dwPlantedC4         = 0x180D4D0;
}

// ============================================================================
// NETVAR OFFSETS (m_ prefix = member variable)
// ============================================================================
namespace Netvars
{
    // Health & Status
    inline uint32_t m_iHealth              = 0x32C;   // [0x0BCD88]
    inline uint32_t m_iMaxHealth           = 0x330;   // [0x0BCDA8]
    inline uint32_t m_iTeamNum             = 0x3BF;   // [0x0BCDB8]
    inline uint32_t m_bPawnIsAlive         = 0x7EC;   // [0x0BCF08]
    inline uint32_t m_iPawnHealth          = 0x7F0;  // [0x0BCF18]
    inline uint32_t m_iPawnArmor           = 0x7F4;  // [0x0BCF28]

    // Transform & Movement
    inline uint32_t m_pGameSceneNode       = 0x310;   // [0x0BCDC8]
    inline uint32_t m_fFlags               = 0x3C8;   // [0x0BCDE0]
    inline uint32_t m_vecAbsVelocity       = 0x3D0;  // [0x0BCDF0]
    inline uint32_t m_vecOrigin            = 0xC8;   // CGameSceneNode

    // Identity
    inline uint32_t m_iszPlayerName        = 0x640;  // [0x0BCED0]
    inline uint32_t m_hPawn                = 0x5FC;  // [0x0BCEB0]
    inline uint32_t m_hPlayerPawn          = 0x7A4;  // [0x0BCEE0]
    inline uint32_t m_nSubclassID          = 0x328;  // [0x0BCE28]

    // Visual
    inline uint32_t m_clrRender            = 0xA80;  // [0x0BCE38]
    inline uint32_t m_Glow                 = 0xBA0;  // [0x0BD2B0]
    inline uint32_t m_bDidSmokeEffect      = 0x13B0; // [0x0BCE60]
    inline uint32_t m_nSmokeEffectTickBegin = 0x13B4; // [0x0BCE98]

    // Weapon
    inline uint32_t m_pClippingWeapon      = 0x12B0;
    inline uint32_t m_iItemDefinitionIndex = 0x1BA;
    inline uint32_t m_iClip1               = 0x15C0;
    inline uint32_t m_iItemIDHigh        = 0x1D0;
    inline uint32_t m_iAccountID         = 0x1D8;
    inline uint32_t m_nFallbackPaintKit  = 0x1E4;
    inline uint32_t m_nFallbackSeed      = 0x1E8;
    inline uint32_t m_flFallbackWear     = 0x1EC;
    inline uint32_t m_nFallbackStatTrak  = 0x1F0;

    // AimPunch (NoRecoil)
    inline uint32_t m_aimPunchAngle        = 0x14BC;
    inline uint32_t m_aimPunchAngleVel     = 0x14C8;

    // Bomb
    inline uint32_t m_bBeingDefused        = 0xEBC;
    inline uint32_t m_flDefuseCountDown    = 0xEC0;
    inline uint32_t m_hBombDefuser         = 0xEC4;

    // Skeleton
    inline uint32_t CSkeletonInstance        = 0xBA0; // [0x0BD2C8]
}

// ============================================================================
// MATEMATIKSEL YAPILAR
// ============================================================================
struct Vector
{
    float x, y, z;
    Vector() : x(0), y(0), z(0) {}
    Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector operator+(const Vector& o) const { return Vector(x+o.x, y+o.y, z+o.z); }
    Vector operator-(const Vector& o) const { return Vector(x-o.x, y-o.y, z-o.z); }
    Vector operator*(float s) const { return Vector(x*s, y*s, z*s); }
    Vector operator/(float s) const { return Vector(x/s, y/s, z/s); }

    float Length() const { return std::sqrt(x*x + y*y + z*z); }
    float Length2D() const { return std::sqrt(x*x + y*y); }
    float Dot(const Vector& o) const { return x*o.x + y*o.y + z*o.z; }
    float DistTo(const Vector& o) const { return (*this - o).Length(); }
    float DistTo2D(const Vector& o) const { return (*this - o).Length2D(); }

    Vector Normalize() const {
        float len = Length();
        if (len == 0) return Vector();
        return Vector(x/len, y/len, z/len);
    }

    Vector ToAngle() const {
        Vector angles;
        if (x == 0.0f && y == 0.0f) {
            angles.x = (z > 0.0f) ? 270.0f : 90.0f;
            angles.y = 0.0f;
        } else {
            angles.x = static_cast<float>(atan2(-z, Length2D()) * (180.0f / 3.14159265f));
            angles.y = static_cast<float>(atan2(y, x) * (180.0f / 3.14159265f));
        }
        angles.z = 0.0f;
        return angles;
    }

    static Vector FromAngle(const Vector& angle) {
        Vector forward;
        float sp = static_cast<float>(sin(angle.x * (3.14159265f / 180.0f)));
        float cp = static_cast<float>(cos(angle.x * (3.14159265f / 180.0f)));
        float sy = static_cast<float>(sin(angle.y * (3.14159265f / 180.0f)));
        float cy = static_cast<float>(cos(angle.y * (3.14159265f / 180.0f)));
        forward.x = cp * cy;
        forward.y = cp * sy;
        forward.z = -sp;
        return forward;
    }
};

struct Vector2D
{
    float x, y;
    Vector2D() : x(0), y(0) {}
    Vector2D(float _x, float _y) : x(_x), y(_y) {}
};

struct Vector4
{
    float x, y, z, w;
    Vector4() : x(0), y(0), z(0), w(0) {}
};

// ============================================================================
// VIEW MATRIX (4x4) - WorldToScreen donusumu
// ============================================================================
struct ViewMatrix
{
    float m[4][4];

    bool WorldToScreen(const Vector& worldPos, Vector2D& screen, int width, int height) const {
        float w = m[3][0]*worldPos.x + m[3][1]*worldPos.y + m[3][2]*worldPos.z + m[3][3];
        if (w < 0.001f) return false;

        float invW = 1.0f / w;
        screen.x = (width / 2.0f) * (1.0f + (m[0][0]*worldPos.x + m[0][1]*worldPos.y + m[0][2]*worldPos.z + m[0][3]) * invW);
        screen.y = (height / 2.0f) * (1.0f - (m[1][0]*worldPos.x + m[1][1]*worldPos.y + m[1][2]*worldPos.z + m[1][3]) * invW);
        return true;
    }
};

// ============================================================================
// RENK YAPISI
// ============================================================================
struct Color
{
    float r, g, b, a;
    Color() : r(1), g(1), b(1), a(1) {}
    Color(float _r, float _g, float _b, float _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) {}
    uint32_t ToU32() const {
        return ((uint32_t)(a*255) << 24) | ((uint32_t)(b*255) << 16) | ((uint32_t)(g*255) << 8) | (uint32_t)(r*255);
    }
};

// ============================================================================
// OYUNCU FLAG'LERI
// ============================================================================
namespace FL
{
    constexpr int ONGROUND     = (1 << 0);
    constexpr int DUCKING      = (1 << 1);
    constexpr int WATERJUMP    = (1 << 2);
    constexpr int ONTRAIN      = (1 << 3);
    constexpr int INRAIN       = (1 << 4);
    constexpr int FROZEN       = (1 << 5);
    constexpr int ATCONTROLS   = (1 << 6);
    constexpr int CLIENT       = (1 << 7);
    constexpr int FAKECLIENT   = (1 << 8);
}

// ============================================================================
// TAKIM NUMARALARI
// ============================================================================
namespace TEAM
{
    constexpr int NONE         = 0;
    constexpr int SPECTATOR    = 1;
    constexpr int TERRORIST    = 2;
    constexpr int CT           = 3;
}

// ============================================================================
// BONE ID'LERI
// ============================================================================
namespace BONE
{
    constexpr int HEAD         = 6;
    constexpr int NECK         = 5;
    constexpr int SPINE        = 4;
    constexpr int SPINE_1      = 2;
    constexpr int SPINE_2      = 3;
    constexpr int PELVIS       = 0;
    constexpr int L_UPPER_ARM  = 8;
    constexpr int L_FOREARM    = 9;
    constexpr int L_HAND       = 10;
    constexpr int R_UPPER_ARM  = 13;
    constexpr int R_FOREARM    = 14;
    constexpr int R_HAND       = 15;
    constexpr int L_THIGH      = 22;
    constexpr int L_CALF       = 23;
    constexpr int L_FOOT       = 24;
    constexpr int R_THIGH      = 25;
    constexpr int R_CALF       = 26;
    constexpr int R_FOOT       = 27;
}

// ============================================================================
// SILAH ITEM DEFINITION INDEX'LERI
// ============================================================================
namespace WEAPON
{
    constexpr int DEAGLE       = 1;
    constexpr int ELITE        = 2;
    constexpr int FIVESEVEN    = 3;
    constexpr int GLOCK        = 4;
    constexpr int AK47         = 7;
    constexpr int AUG          = 8;
    constexpr int AWP          = 9;
    constexpr int FAMAS        = 10;
    constexpr int G3SG1        = 11;
    constexpr int GALILAR    = 13;
    constexpr int M249         = 14;
    constexpr int M4A1         = 16;
    constexpr int MAC10        = 17;
    constexpr int P90          = 19;
    constexpr int MP5SD        = 23;
    constexpr int UMP45        = 24;
    constexpr int XM1014       = 25;
    constexpr int BIZON        = 26;
    constexpr int MAG7         = 27;
    constexpr int NEGEV        = 28;
    constexpr int SAWEDOFF     = 29;
    constexpr int TEC9         = 30;
    constexpr int ZEUS         = 31;
    constexpr int P2000        = 32;
    constexpr int MP7          = 33;
    constexpr int MP9          = 34;
    constexpr int NOVA         = 35;
    constexpr int P250         = 36;
    constexpr int SCAR20       = 38;
    constexpr int SG556        = 39;
    constexpr int SSG08        = 40;
    constexpr int KNIFE        = 42;
    constexpr int FLASHBANG    = 43;
    constexpr int HEGRENADE    = 44;
    constexpr int SMOKEGRENADE = 45;
    constexpr int MOLOTOV      = 46;
    constexpr int DECOY        = 47;
    constexpr int INCGRENADE   = 48;
    constexpr int C4           = 49;
    constexpr int KNIFE_T      = 59;
    constexpr int M4A1_SILENCER= 60;
    constexpr int USPS         = 61;
    constexpr int CZ75A        = 63;
    constexpr int REVOLVER     = 64;
    constexpr int KNIFE_BAYONET= 500;
    constexpr int KNIFE_CSS    = 503;
    constexpr int KNIFE_FLIP   = 505;
    constexpr int KNIFE_GUT    = 506;
    constexpr int KNIFE_KARAMBIT = 507;
    constexpr int KNIFE_M9_BAYONET = 508;
    constexpr int KNIFE_TACTICAL = 509;
    constexpr int KNIFE_FALCHION = 512;
    constexpr int KNIFE_SURVIVAL_BOWIE = 514;
    constexpr int KNIFE_BUTTERFLY = 515;
    constexpr int KNIFE_PUSH    = 516;
    constexpr int KNIFE_NAVAJA  = 519;
    constexpr int KNIFE_STILETTO = 522;
    constexpr int KNIFE_TALON   = 523;
    constexpr int KNIFE_URSUS   = 525;
    constexpr int KNIFE_KUKRI   = 526;
    constexpr int KNIFE_CORD    = 527;
    constexpr int KNIFE_CANIS   = 528;
    constexpr int KNIFE_OUTDOOR = 529;
    constexpr int KNIFE_SKELETON = 530;
}

// ============================================================================
// BICAK MODELLERI (20 adet) - [0x0B9A70 - 0x0B9F40]
// ============================================================================
namespace KnifeModels
{
    constexpr const char* Names[] = {
        "Bayonet",
        "Classic Knife",
        "Flip Knife",
        "Gut Knife",
        "Karambit",
        "M9 Bayonet",
        "Huntsman Knife",
        "Falchion Knife",
        "Bowie Knife",
        "Butterfly Knife",
        "Shadow Daggers",
        "Navaja Knife",
        "Stiletto Knife",
        "Talon Knife",
        "Ursus Knife",
        "Kukri Knife",
        "Survival Knife",
        "Paracord Knife",
        "Nomad Knife",
        "Skeleton Knife"
    };

    constexpr const char* ModelPaths[] = {
        "models/weapons/v_knife_bayonet.mdl",
        "models/weapons/v_knife_css.mdl",
        "models/weapons/v_knife_flip.mdl",
        "models/weapons/v_knife_gut.mdl",
        "models/weapons/v_knife_karam.mdl",
        "models/weapons/v_knife_m9_bay.mdl",
        "models/weapons/v_knife_tactical.mdl",
        "models/weapons/v_knife_falchion_advanced.mdl",
        "models/weapons/v_knife_survival_bowie.mdl",
        "models/weapons/v_knife_butterfly.mdl",
        "models/weapons/v_knife_push.mdl",
        "models/weapons/v_knife_gypsy_jackknife.mdl",
        "models/weapons/v_knife_stiletto.mdl",
        "models/weapons/v_knife_widowmaker.mdl",
        "models/weapons/v_knife_ursus.mdl",
        "models/weapons/v_knife_kukri.mdl",
        "models/weapons/v_knife_cord.mdl",
        "models/weapons/v_knife_canis.mdl",
        "models/weapons/v_knife_outdoor.mdl",
        "models/weapons/v_knife_skeleton.mdl"
    };
    constexpr int Count = sizeof(Names) / sizeof(Names[0]);
}

// ============================================================================
// SKIN ISIMLERI
// ============================================================================
namespace SkinNames
{
    constexpr const char* M4A1_Printstream = "M4A1-S | Printstream";      // [0x0B9430]
    constexpr const char* M4A1_PlayerTwo   = "M4A1-S | Player Two";      // [0x0B9448]
    constexpr const char* AWP_Printstream  = "AWP | Printstream";         // [0x0B94E8]
    constexpr const char* Deagle_Printstream = "Desert Eagle | Printstream"; // [0x0B9540]
    constexpr const char* USP_Printstream  = "USP-S | Printstream";       // [0x0B95A0]
}

// ============================================================================
// SKIN ID'LERI (PaintKit)
// ============================================================================
namespace SkinIDs
{
    constexpr int Printstream_M4A1  = 845;
    constexpr int Printstream_AWP   = 919;
    constexpr int Printstream_Deagle = 962;
    constexpr int Printstream_USP   = 946;
    constexpr int PlayerTwo_M4A1    = 820;
}
