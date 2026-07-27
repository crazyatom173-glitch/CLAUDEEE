#pragma once
#include <windows.h>
#include <cstdint>
#include "sdk.h"

/*
 * ============================================================================
 * CS2 INTERNAL - SOURCE 2 INTERFACE SISTEMI (HEADER)
 * Kanitlar: "CreateInterface" [0x0B9250], "Source2Client002" [0x0BC788]
 * ============================================================================
 */

typedef void* (*CreateInterfaceFn)(const char* name, int* returnCode);

// Source 2 Client Interface
class IBaseClientDLL
{
public:
    virtual int              Connect(void* appSystemFactory, void* pGlobals) = 0;
    virtual int              Disconnect() = 0;
    virtual int              Init(void* appSystemFactory, void* pGlobals) = 0;
    virtual void             PostInit() = 0;
    virtual void             Shutdown() = 0;
    virtual void             LevelInitPreEntity(const char* pMapName) = 0;
    virtual void             LevelInitPostEntity() = 0;
    virtual void             LevelShutdown() = 0;
    virtual void*            GetAllClasses() = 0;
};

// Client Mode
class IClientMode
{
public:
    virtual ~IClientMode() {}
    virtual int             DrawViewport() = 0;
    virtual int             GetViewportWidth() = 0;
    virtual int             GetViewportHeight() = 0;
};

// Engine Client
class IVEngineClient
{
public:
    virtual void            GetScreenSize(int& width, int& height) = 0;
    virtual bool            GetPlayerInfo(int ent_num, void* pinfo) = 0;
    virtual int             GetPlayerForUserID(int userID) = 0;
    virtual bool            Con_IsVisible() = 0;
    virtual int             GetLocalPlayer() = 0;
    virtual void            GetViewAngles(Vector& va) = 0;
    virtual void            SetViewAngles(Vector& va) = 0;
    virtual int             GetMaxClients() = 0;
    virtual bool            IsInGame() = 0;
    virtual bool            IsConnected() = 0;
    virtual bool            IsDrawingLoadingImage() = 0;
    virtual void            ExecuteClientCmd(const char* szCmdString) = 0;
};

// Entity System
class CGameEntitySystem
{
public:
    virtual void            Release() = 0;
    virtual void*           GetBaseEntity(int index) = 0;
    virtual void*           GetBaseEntityFromHandle(void* handle) = 0;
    virtual int             GetNumEntities() = 0;
    virtual int             GetHighestEntityIndex() = 0;
};

namespace Interfaces
{
    inline void*            pClient = nullptr;        // Source2Client002 [0x0BC788]
    inline IVEngineClient*    pEngine = nullptr;
    inline IClientMode*       pClientMode = nullptr;
    inline CGameEntitySystem* pEntitySystem = nullptr;
    inline HMODULE            hClientDll = nullptr;    // [0x0BC630]
    inline HMODULE            hEngineDll = nullptr;    // [0x0BC620]

    bool Initialize();
    void* GetInterface(const char* moduleName, const char* interfaceName);
}
