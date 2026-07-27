#pragma once
#include <cstdint>

/*
 * ============================================================================
 * CS2 INTERNAL - OTOMATIK OFFSET GUNCELLEYICI (HEADER)
 * Kanitlar: WININET import'lari, "[AutoUpdater] ERROR:" [0x0BCD30]
 * ============================================================================
 */

namespace AutoUpdater
{
    bool UpdateOffsets();
    bool UpdateNetvars();
    uintptr_t ParseOffset(const char* json, const char* name);
}
