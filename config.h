#pragma once
#include <windows.h>

/*
 * ============================================================================
 * CS2 INTERNAL - KONFIGURASYON YAPISI v2.0
 * Kaynak: Menu string analizi [0x0BBF9C - 0x0BC4A8]
 * ============================================================================
 */

struct Config
{
    // ========================================================================
    // AIMBOT AYARLARI
    // Kanitlar: "Aimbot" [0x0BBF9C], "FOV Dairesi" [0x0BBFB0],
    //           "Smooth" [0x0BBFBC], "Triggerbot" [0x0BBFD8]
    // ========================================================================
    struct {
        bool bEnabled = false;
        bool bVisibleCheck = true;      // Sadece gorunur hedeflere
        bool bTeamCheck = true;         // Takim arkadaslarini atla
        bool bDrawFOV = false;          // "FOV Dairesi" [0x0BBFB0]
        bool bDrawTarget = false;       // Hedef isaretleme
        bool bAutoShoot = false;        // Otomatik ates
        bool bTriggerbot = false;       // "Triggerbot" [0x0BBFD8]
        float fFOV = 5.0f;              // FOV yaricapi (derece)
        float fSmooth = 3.0f;           // "Smooth" [0x0BBFBC] - yumusatma
        float fRcsPitch = 1.0f;       // RCS Pitch etkisi
        float fRcsYaw = 1.0f;         // RCS Yaw etkisi
        int iHitbox = 0;                // 0=Head, 1=Neck, 2=Chest, 3=Stomach
        int iPriority = 0;            // 0=Distance, 1=FOV, 2=Health
    } aimbot;

    // ========================================================================
    // ESP AYARLARI
    // Kanitlar: "ESP KUTU" [0x0BBFE8], "Bomba ESP" [0x0BC088],
    //           "ESP PREVIEW" [0x0BC4A8], "Armor" [0x0BC010]
    // ========================================================================
    struct {
        bool bEnabled = true;
        bool bBox = true;               // "ESP KUTU" [0x0BBFE8] (Varsayılan Açık)
        bool bBoxOutline = true;        // Kutu dis cizgisi
        bool bCornerBox = false;        // Koseli kutu
        bool bHealthBar = true;         // Can cubugu
        bool bArmorBar = false;         // "Armor" [0x0BC010]
        bool bName = true;              // Isim gostergesi
        bool bWeapon = true;            // Silah ismi
        bool bDistance = false;         // Mesafe
        bool bSkeleton = true;          // Iskelet ESP (Varsayılan Açık)
        bool bHeadDot = true;           // Kafa noktasi (Varsayılan Açık)
        bool bSnaplines = true;         // Cizgi (Varsayılan Açık)
        bool bBombESP = true;           // "Bomba ESP" [0x0BC088]
        bool bGrenadeESP = false;       // El bombasi ESP
        bool bChickens = false;         // Tavuk ESP (meme)
        bool bPreview = false;          // "ESP PREVIEW" [0x0BC4A8]
        float fMaxDistance = 5000.0f;   // Maksimum mesafe
    } esp;

    // ========================================================================
    // GLOW / CHAMS AYARLARI
    // Kanitlar: "GLOW CHAMS" [0x0BC0E0], "Glow Mod" [0x0BC0F0],
    //           "Glow Yogunluk" [0x0BC100], "Olu Glow Kapat" [0x0BC130]
    // ========================================================================
    struct {
        bool bEnabled = false;          // "GLOW CHAMS" [0x0BC0E0]
        bool bChams = false;            // Chams aktif
        bool bChamsXQZ = false;         // Duvar arkasini goster
        bool bChamsFlat = false;        // Duz renk chams
        bool bChamsWireframe = false;   // Tel kafes chams
        int iGlowMode = 0;              // "Glow Mod" [0x0BC0F0]
        float fGlowIntensity = 1.0f;    // "Glow Yogunluk" [0x0BC100]
        bool bDisableOnDead = true;     // "Olu Glow Kapat" [0x0BC130]
        bool bTeamGlow = false;         // Takim arkadaslari glow
        float fColorT[4] = {1,0,0,1};   // T takimi rengi
        float fColorCT[4] = {0,0,1,1};  // CT takimi rengi
    } glow;

    // ========================================================================
    // HAREKET AYARLARI (MISC)
    // Kanitlar: "Bunnyhop" [0x0BC1F0], "NoRecoil" [0x0BC240],
    //           "Radar Hack" [0x0BC230]
    // ========================================================================
    struct {
        bool bBunnyhop = false;         // "Bunnyhop" [0x0BC1F0]
        bool bAutoStrafe = false;       // Otomatik strafe
        bool bEdgeJump = false;         // Kenar ziplama
        bool bNoRecoil = false;         // "NoRecoil" [0x0BC240]
        bool bNoVisualRecoil = false;   // Gorsel geri tepme kaldirma
        bool bRadarHack = false;        // "Radar Hack" [0x0BC230]
        bool bThirdPerson = false;      // Ucuncu sahis
        bool bFastStop = false;         // Hizli durma
        bool bAutoAccept = false;       // Otomatik kabul
        bool bRevealRanks = false;      // Rank gosterme
        bool bSpectatorList = false;    // Izleyici listesi
        float fThirdPersonDistance = 100.0f;
    } misc;

    // ========================================================================
    // FOV AYARLARI
    // Kanitlar: "FOV Aktif" [0x0BC430], "FOV Degeri" [0x0BC440],
    //           "Viewmodel FOV" [0x0BC450]
    // ========================================================================
    struct {
        bool bEnabled = false;          // "FOV Aktif" [0x0BC430]
        float fValue = 90.0f;           // "FOV Degeri" [0x0BC440]
        float fViewmodel = 68.0f;       // "Viewmodel FOV" [0x0BC450]
        bool bOverrideZoom = false;     // Zoom FOV'u degistir
        float fZoomFOV = 30.0f;         // Zoom FOV degeri
    } fov;

    // ========================================================================
    // VISUAL AYARLAR
    // Kanitlar: "Ozel Crosshair" [0x0BC498], "Steam Bilgi" [0x0BC068]
    // ========================================================================
    struct {
        bool bCustomCrosshair = false;  // "Ozel Crosshair" [0x0BC498]
        bool bRecoilCrosshair = false;  // Geri tepme crosshair'i
        bool bSpreadCrosshair = false;  // Dagilim crosshair'i
        bool bNoScopeCrosshair = false; // Durbun crosshair'i
        bool bSteamInfo = false;        // "Steam Bilgi" [0x0BC068]
        bool bNoSmoke = false;          // Dumani kaldir
        bool bNoFlash = false;          // Flashbang etkisini kaldir
        bool bNoScope = false;          // Durbun kenarlarini kaldir
        bool bNightMode = false;        // Gece modu
        float fBrightness = 1.0f;       // Parlaklik
        bool bWireframeSmoke = false;   // Tel kafes duman
    } visual;

    // ========================================================================
    // SKIN CHANGER
    // Kanitlar: "=== KnifeHook v97b Init ===" [0x0BC750],
    //           20 bicak modeli [0x0B9A70-0x0B9F40]
    // ========================================================================
    struct {
        bool bEnabled = false;
        bool bKnifeChanger = false;     // Bicak degistirici
        int iKnifeModel = 0;            // Secili bicak (0-19)
        bool bGloveChanger = false;     // Eldiven degistirici
        int iGloveModel = 0;            // Secili eldiven
        bool bSkinChanger = false;      // Skin degistirici
        int iSkinID = 0;                // Secili skin
        float fWear = 0.01f;            // Asinma degeri
        int iSeed = 0;                  // Skin seed
        bool bStatTrak = false;         // StatTrak aktif
        int iStatTrakKills = 0;         // StatTrak kill sayisi
        char szNameTag[32] = {};       // Isim etiketi
    } skinchanger;

    // ========================================================================
    // MENU DURUMU
    // ========================================================================
    bool bMenuOpen = false;
    int iMenuKey = VK_INSERT;      // Menu tus (varsayilan INSERT)
    int iPanicKey = VK_END;        // Panik tus (varsayilan END)
};

inline Config g_Config;
