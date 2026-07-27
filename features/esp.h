#pragma once
#include "sdk/sdk.h"

namespace ESP
{
    bool WorldToScreen(const Vector& worldPos, Vector2D& screen);
    void DrawBox(Vector2D top, Vector2D bottom, Color color, bool outline);
    void DrawHealthBar(Vector2D top, Vector2D bottom, int health, int maxHealth);
    void DrawName(Vector2D top, const char* name);
    void DrawWeapon(Vector2D bottom, const char* weapon);
    void DrawSkeleton(uintptr_t entity, Color color);
    void DrawSnaplines(Vector2D screenPos, Color color);
    void DrawBombInfo(uintptr_t bomb);
    void Render();
}
