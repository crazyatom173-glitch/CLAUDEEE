#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <windows.h>
#include "third_party/imgui/imgui_impl_win32.h"

/*
 * ============================================================================
 * CS2 INTERNAL - DX11 HOOK SISTEMI (HEADER)
 * Kanitlar: D3D11CreateDeviceAndSwapChain [d3d11.dll]
 *           D3DCompile [D3DCOMPILER_47.dll]
 *           SetWindowLongPtrA, CallWindowProcA [USER32.dll]
 *           MinHook stringleri [0x0BBD40 - 0x0BBE60]
 * ============================================================================
 */

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall* DrawIndexed_t)(ID3D11DeviceContext* pCtx, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall* DrawIndexedInstanced_t)(ID3D11DeviceContext* pCtx, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall* ClearRenderTargetView_t)(ID3D11DeviceContext* pCtx, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);
typedef LRESULT(CALLBACK* WndProc_t)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
    // Orijinal fonksiyon pointer'lari
    inline Present_t                oPresent = nullptr;
    inline DrawIndexed_t            oDrawIndexed = nullptr;
    inline DrawIndexedInstanced_t   oDrawIndexedInstanced = nullptr;
    inline ClearRenderTargetView_t  oClearRenderTargetView = nullptr;
    inline WndProc_t                oWndProc = nullptr;

    // DX11 nesneleri
    inline ID3D11Device*            pDevice = nullptr;
    inline ID3D11DeviceContext*     pContext = nullptr;
    inline ID3D11RenderTargetView*  pRenderTargetView = nullptr;
    inline IDXGISwapChain*          pSwapChain = nullptr;
    inline HWND                     hWnd = nullptr;

    // Durum
    inline bool                     bInitialized = false;
    inline bool                     bShuttingDown = false;

    // Fonksiyonlar
    bool Initialize();
    void Cleanup();
    void WaitForInitialization();

    // Hook fonksiyonlari
    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    void __stdcall hkDrawIndexed(ID3D11DeviceContext* pCtx, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
    void __stdcall hkDrawIndexedInstanced(ID3D11DeviceContext* pCtx, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
    void __stdcall hkClearRenderTargetView(ID3D11DeviceContext* pCtx, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);
    LRESULT CALLBACK hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
