#include <vector>
#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <string.h>
#include "utils/auto_updater.h"
#include "sdk/sdk.h"

#pragma comment(lib, "wininet.lib")

/*
 * ============================================================================
 * CS2 INTERNAL - OTOMATIK OFFSET GUNCELLEYICI (EVRENSEL PARSER)
 * ============================================================================
 */

static uintptr_t ParseOffsetValue(const char* json, const char* name)
{
    if (!json || !name) return 0;
    const char* pos = strstr(json, name);
    if (!pos) return 0;

    pos += strlen(name);
    while (*pos && (*pos == ' ' || *pos == '\t' || *pos == '"' || *pos == ':')) pos++;

    if (strncmp(pos, "offset", 6) == 0 || strncmp(pos, "value", 5) == 0)
    {
        const char* sub = strchr(pos, ':');
        if (sub) pos = sub + 1;
        while (*pos && (*pos == ' ' || *pos == '\t')) pos++;
    }

    if (*pos == '0' && (*(pos + 1) == 'x' || *(pos + 1) == 'X'))
    {
        return (uintptr_t)strtoull(pos + 2, nullptr, 16);
    }
    return (uintptr_t)strtoull(pos, nullptr, 10);
}

bool AutoUpdater::UpdateOffsets()
{
    HINTERNET hInternet = InternetOpenA(
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);

    if (!hInternet)
    {
        printf("[AutoUpdater] ERROR: Could not open internet connection.\n");
        return false;
    }

    HINTERNET hUrl = InternetOpenUrlA(
        hInternet,
        "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/client_dll.json",
        nullptr, 0,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);

    if (!hUrl)
    {
        printf("[AutoUpdater] ERROR: Could not download client_dll.json.\n");
        InternetCloseHandle(hInternet);
        return false;
    }

    std::vector<char> buffer(262144, 0);
    DWORD totalRead = 0;
    DWORD bytesRead = 0;

    while (InternetReadFile(hUrl, buffer.data() + totalRead, (DWORD)buffer.size() - totalRead - 1, &bytesRead) && bytesRead > 0)
    {
        totalRead += bytesRead;
    }
    buffer[totalRead] = '\0';

    uintptr_t parsed = 0;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwLocalPlayerPawn")) != 0) Offsets::dwLocalPlayerPawn = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwLocalPlayerController")) != 0) Offsets::dwLocalPlayerController = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwEntityList")) != 0) Offsets::dwEntityList = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwViewMatrix")) != 0) Offsets::dwViewMatrix = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwGlobalVars")) != 0) Offsets::dwGlobalVars = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwSensitivity")) != 0) Offsets::dwSensitivity = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwViewAngles")) != 0) Offsets::dwViewAngles = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwNetworkGameClient")) != 0) Offsets::dwNetworkGameClient = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwBuildNumber")) != 0) Offsets::dwBuildNumber = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwGameRules")) != 0) Offsets::dwGameRules = parsed;
    if ((parsed = ParseOffsetValue(buffer.data(), "dwPlantedC4")) != 0) Offsets::dwPlantedC4 = parsed;

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    printf("[AutoUpdater] Offsets updated successfully from remote.\n");
    return true;
}

bool AutoUpdater::UpdateNetvars()
{
    return true;
}

uintptr_t AutoUpdater::ParseOffset(const char* json, const char* name)
{
    return ParseOffsetValue(json, name);
}
