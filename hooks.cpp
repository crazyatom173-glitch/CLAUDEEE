#include "hooks.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#include "config.h"
#include "menu.h"
#include "features.h"
#include "sdk/sdk.h"
#include "utils/memory.h"
#include <MinHook.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <stdio.h>

/*
 * ============================================================================
 * CS2 INTERNAL - DX11 HOOK SISTEMI (GUVENLI & KARARLI IMPLEMENTASYON)
 * ============================================================================
 */

HRESULT __stdcall Hooks::hkPresent(IDXGISwapChain* pSwapChainRef, UINT SyncInterval, UINT Flags)
{
    if (bShuttingDown || !pSwapChainRef)
        return oPresent(pSwapChainRef, SyncInterval, Flags);

    if (!bInitialized)
    {
        if (SUCCEEDED(pSwapChainRef->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)) && pDevice)
        {
            pDevice->GetImmediateContext(&pContext);

            DXGI_SWAP_CHAIN_DESC desc;
            pSwapChainRef->GetDesc(&desc);
            hWnd = desc.OutputWindow;

            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.IniFilename = nullptr;

            io.Fonts->AddFontDefault();

            ImGui_ImplWin32_Init(hWnd);
            ImGui_ImplDX11_Init(pDevice, pContext);

            Menu::ApplyTheme();

            oWndProc = (WndProc_t)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);

            bInitialized = true;
            printf("[DX11] ImGui init OK\n");
        }
    }

    if (bInitialized && pContext && !bShuttingDown)
    {
        if (!pRenderTargetView)
        {
            ID3D11Texture2D* pBackBuffer = nullptr;
            if (SUCCEEDED(pSwapChainRef->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)) && pBackBuffer)
            {
                pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
                pBackBuffer->Release();
            }
        }

        if (pRenderTargetView)
        {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            static bool bLastMenuKey = false;
            bool bCurrentMenuKey = ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState('K') & 0x8000)) ||
                                   ((GetAsyncKeyState(VK_MULTIPLY) & 0x8000)) ||
                                   ((GetAsyncKeyState(g_Config.iMenuKey) & 0x8000));
            if (bCurrentMenuKey && !bLastMenuKey)
                g_Config.bMenuOpen = !g_Config.bMenuOpen;
            bLastMenuKey = bCurrentMenuKey;

            __try {
                Aimbot::Run();
                Glow::Run();
                Misc::Run();
                SkinChanger::Run();
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {}

            if (g_Config.bMenuOpen)
                Menu::Render();

            ESP::Render();
            Misc::Render();

            ImGui::SetNextWindowPos(ImVec2(15, 15), ImGuiCond_Always);
            ImGui::Begin("Watermark", nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav |
                ImGuiWindowFlags_NoMove);

            uintptr_t engineBase = Memory::GetModuleBase("engine2.dll");
            int buildNum = engineBase ? Memory::Read<int>(engineBase + Offsets::dwBuildNumber) : 0;

            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.4f, 1.0f), "[● ONLINE]");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.85f, 0.90f, 0.98f, 1.0f), "Bay_Hosaf_education | CS2 Internal | %.0f FPS | Build: %d",
                ImGui::GetIO().Framerate,
                buildNum);
            ImGui::End();

            ImGui::Render();

            pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    }

    return oPresent(pSwapChainRef, SyncInterval, Flags);
}

void __stdcall Hooks::hkDrawIndexed(ID3D11DeviceContext* pCtx, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
    oDrawIndexed(pCtx, IndexCount, StartIndexLocation, BaseVertexLocation);
}

LRESULT CALLBACK Hooks::hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (bInitialized && !bShuttingDown && ImGui::GetCurrentContext())
    {
        if (g_Config.bMenuOpen)
        {
            ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

            if (msg != WM_SETFOCUS && msg != WM_KILLFOCUS && msg != WM_ACTIVATE && msg != WM_NCACTIVATE && msg != WM_SYSCOMMAND)
            {
                ImGuiIO& io = ImGui::GetIO();
                if (io.WantCaptureMouse || io.WantCaptureKeyboard)
                {
                    return true;
                }
            }
        }
    }

    if (oWndProc)
        return CallWindowProcW(oWndProc, hWnd, msg, wParam, lParam);

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool Hooks::Initialize()
{
    printf("[DX11] Hook() starting with MinHook...\n");

    if (MH_Initialize() != MH_OK)
    {
        printf("[DX11] MH_Initialize failed\n");
        return false;
    }
    printf("[DX11] MinHook initialized\n");

    HWND hTempWnd = CreateWindowA("BUTTON", "DX11Temp", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, nullptr, nullptr);
    if (!hTempWnd)
        hTempWnd = GetForegroundWindow();

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Width = 2;
    sd.BufferDesc.Height = 2;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hTempWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ID3D11Device* pTempDevice = nullptr;
    ID3D11DeviceContext* pTempContext = nullptr;
    IDXGISwapChain* pTempSwapChain = nullptr;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 2, D3D11_SDK_VERSION,
        &sd, &pTempSwapChain, &pTempDevice, &featureLevel, &pTempContext);

    if (FAILED(hr))
    {
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0,
            featureLevels, 2, D3D11_SDK_VERSION,
            &sd, &pTempSwapChain, &pTempDevice, &featureLevel, &pTempContext);
    }

    if (FAILED(hr) || !pTempSwapChain)
    {
        printf("[DX11] D3D11CreateDeviceAndSwapChain failed: 0x%08X\n", (unsigned int)hr);
        if (hTempWnd && IsWindow(hTempWnd)) DestroyWindow(hTempWnd);
        return false;
    }

    void** pVTable = *(void***)pTempSwapChain;
    void* pPresent = pVTable[8];

    if (MH_CreateHook(pPresent, hkPresent, (void**)&oPresent) != MH_OK ||
        MH_EnableHook(pPresent) != MH_OK)
    {
        printf("[DX11] MH_EnableHook failed\n");
        pTempSwapChain->Release();
        pTempDevice->Release();
        pTempContext->Release();
        if (hTempWnd && IsWindow(hTempWnd)) DestroyWindow(hTempWnd);
        return false;
    }

    void** pContextVTable = *(void***)pTempContext;
    void* pDrawIndexed = pContextVTable[12];

    if (MH_CreateHook(pDrawIndexed, hkDrawIndexed, (void**)&oDrawIndexed) == MH_OK)
    {
        MH_EnableHook(pDrawIndexed);
    }

    pTempSwapChain->Release();
    pTempDevice->Release();
    pTempContext->Release();
    if (hTempWnd && IsWindow(hTempWnd)) DestroyWindow(hTempWnd);

    printf("[+] DX11 Hook Basarili!\n");
    return true;
}

void Hooks::Cleanup()
{
    if (bShuttingDown) return;
    bShuttingDown = true;

    if (oWndProc && hWnd)
    {
        SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
        oWndProc = nullptr;
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    if (bInitialized)
    {
        bInitialized = false;
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        if (ImGui::GetCurrentContext())
        {
            ImGui::DestroyContext();
        }
    }

    if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }
    if (pContext) { pContext->Release(); pContext = nullptr; }
    if (pDevice) { pDevice->Release(); pDevice = nullptr; }

    printf("[+] Hooklar temizlendi.\n");
}
