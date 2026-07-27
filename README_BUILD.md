# CS2 Internal v2.0 - Derleme Talimatlari

## Gereksinimler

- **Visual Studio 2022** (v143 toolset) - Community Edition ucretsiz
- **Windows SDK 10.0** (Visual Studio kurulumu sirasinda secin)
- **ImGui** - https://github.com/ocornut/imgui
- **MinHook** - https://github.com/TsudaKageyu/minhook

## Adim 1: Third-Party Kutuphaneleri Indir

### ImGui
1. https://github.com/ocornut/imgui adresine git
2. `Code > Download ZIP` ile indir
3. Zip'i ac, `imgui/` klasorundeki tum dosyalari su yola kopyala:
   ```
   third_party/imgui/
   ```
4. Gerekli dosyalar:
   - `imgui.cpp`, `imgui_draw.cpp`, `imgui_tables.cpp`, `imgui_widgets.cpp`
   - `imgui.h`, `imgui_internal.h`, `imconfig.h`
   - `imgui_impl_dx11.cpp`, `imgui_impl_dx11.h`
   - `imgui_impl_win32.cpp`, `imgui_impl_win32.h`

### MinHook
1. https://github.com/TsudaKageyu/minhook adresine git
2. `Code > Download ZIP` ile indir
3. Zip'i ac, `MinHook/` klasorundeki tum dosyalari su yola kopyala:
   ```
   third_party/minhook/
   ```
4. Gerekli dosyalar:
   - `include/MinHook.h`
   - `src/buffer.c`, `src/hook.c`, `src/trampoline.c`
   - `src/hde/hde64.c`, `src/hde/hde64.h`, `src/hde/pstdint.h`, `src/hde/table64.h`

## Adim 2: Solution'u Ac

1. `CS2_Internal.sln` dosyasina cift tikla
2. Visual Studio 2022 acilacak

## Adim 3: Build Et

1. Ust menuden `Build > Build Solution` (Ctrl+Shift+B)
2. Veya sag tikla `Build`
3. Cikti dosyalari:
   - `Build/Release/CS2_Internal_Local.dll`
   - `Build/Release/CS2_Injector.exe`

## Adim 4: Calistir

1. CS2'yi ac
2. `CS2_Injector.exe`'yi **Yonetici olarak calistir**
3. Injector otomatik olarak CS2'yi bulup DLL'i inject edecek
4. Oyun icinde **INSERT** ile menuyu ac
5. **END** ile kapat (DLL unload olur)

## Proje Yapisi

```
CS2_Internal/
|
|-- CS2_Internal.sln          # Solution dosyasi
|-- CS2_Internal.vcxproj       # DLL projesi
|-- CS2_Injector.vcxproj       # EXE projesi
|
|-- dllmain.cpp                # DLL giris noktasi
|-- hooks.cpp / hooks.h        # DX11 hook sistemi
|-- menu.cpp / menu.h           # ImGui menu
|-- config.h                   # Ayarlar yapisi
|-- features.h                 # Feature modul header'lari
|-- injector.cpp               # Manual Map Injector
|
|-- sdk/
|   |-- sdk.h                  # Offset/Netvar/Vector tanimlari
|   |-- interfaces.h/.cpp      # Source2 interface sistemi
|
|-- utils/
|   |-- memory.h               # Bellek okuma/yazma
|   |-- auto_updater.h/.cpp    # Offset guncelleyici
|
|-- features/
|   |-- aimbot.h/.cpp          # Aimbot + Triggerbot
|   |-- esp.h/.cpp             # ESP kutu + bomba
|   |-- glow.h/.cpp            # Glow / Chams
|   |-- misc.h/.cpp            # Bunnyhop, NoRecoil, Radar, FOV
|   |-- skinchanger.h/.cpp     # Skin/Bicak degistirici
|
|-- third_party/
|   |-- imgui/                 # ImGui kutuphanesi
|   |-- minhook/               # MinHook kutuphanesi
```

## Onemli Notlar

- **Offset'ler** CS2 her guncellemede degisir. `AutoUpdater` internetten guncel offset'leri indirir.
- Eger internet baglantisi yoksa, `sdk/sdk.h` icindeki varsayilan degerler kullanilir.
- **VAC Ban riski** vardir. Bu kod egitim amaclidir.
- Derlenmis DLL boyutu ~500-900 KB arasinda olabilir (Release modu, optimizasyonlarla).

## Hata Cozumleri

| Hata | Cozum |
|------|-------|
| `Cannot open include file: 'imgui.h'` | ImGui dosyalarini `third_party/imgui/` altina kopyala |
| `Cannot open include file: 'MinHook.h'` | MinHook dosyalarini `third_party/minhook/` altina kopyala |
| `LNK2019: unresolved external symbol` | Proje ayarlarinda Additional Include Directories kontrol et |
| `OpenProcess Hata` | Injector'u Yonetici olarak calistir |

## Ozellestirme

Artik kaynak kodlar tamamen senin kontrolunde! Istedigin degisikligi yapabilirsin:
- Menu renkleri, fontu, boyutu
- Yeni feature ekleme
- Aimbot algoritmasini degistirme
- ESP cizim stili
- Yeni skin/bicak modelleri
- Anti-cheat bypass teknikleri

Her degisiklikten sonra `Build > Rebuild Solution` yap ve yeni DLL'i test et.
