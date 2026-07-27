#pragma once
#include "sdk/sdk.h"

namespace Aimbot
{
    Vector CalcAngle(const Vector& src, const Vector& dst);
    float GetFOV(const Vector& viewAngle, const Vector& aimAngle);
    Vector SmoothAngle(const Vector& current, const Vector& target, float factor);
    bool IsVisible(uintptr_t entity);
    Vector GetBonePosition(uintptr_t entity, int boneId);
    void Run();
    void RenderFOV();
}
