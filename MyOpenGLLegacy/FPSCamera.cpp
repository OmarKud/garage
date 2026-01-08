#include "GLWin.h"
#include "FPSCamera.h"
#include <cmath>

static float deg2radf(float d) { return d * 0.01745329251994329577f; }

Vec3 FPSCamera::Forward() const
{
    float yawR = deg2radf(yawDeg);
    float pitR = deg2radf(pitchDeg);

    Vec3 f;
    f.x = std::cos(yawR) * std::cos(pitR);
    f.y = std::sin(pitR);
    f.z = std::sin(yawR) * std::cos(pitR);

    return normalize(f);
}

Vec3 FPSCamera::ForwardFlat() const
{
    float yawR = deg2radf(yawDeg);

    Vec3 f;
    f.x = std::cos(yawR);
    f.y = 0.0f;
    f.z = std::sin(yawR);

    return normalize(f);
}

Vec3 FPSCamera::RightFlat(const Vec3& fFlat) const
{
    // right on ground plane
    Vec3 r{ -fFlat.z, 0.0f, fFlat.x };
    return normalize(r);
}

void FPSCamera::Update(float dt, const Input& input)
{
    // toggle mode once per press
    if (input.Pressed('F')) {              // 'F' = 0x46 :contentReference[oaicite:1]{index=1}
        flyMode = !flyMode;
        if (!flyMode) pos.y = walkEyeY;    // snap back to walking height
    }

    // mouse look
    yawDeg += input.MouseDX() * mouseSens;
    pitchDeg -= input.MouseDY() * mouseSens;
    pitchDeg = clampf(pitchDeg, -89.0f, 89.0f);

    Vec3 fFly = Forward();        // includes pitch
    Vec3 fWalk = ForwardFlat();    // yaw only
    Vec3 f = flyMode ? fFly : fWalk;

    // keep strafing flat (feels stable even in fly)
    Vec3 r = RightFlat(fWalk);

    Vec3 v{ 0,0,0 };

    if (input.IsDown('W')) v = v + f;
    if (input.IsDown('S')) v = v - f;
    if (input.IsDown('D')) v = v + r;
    if (input.IsDown('A')) v = v - r;

    if (flyMode)
    {
        // vertical in fly mode
        if (input.IsDown(VK_SPACE))   v.y += 1.0f;
        if (input.IsDown(VK_CONTROL)) v.y -= 1.0f;
    }
    else
    {
        // walk mode: no vertical movement
        v.y = 0.0f;
    }

    if (length(v) > 0.0001f)
    {
        Vec3 dir = normalize(v);
        pos = pos + dir * (moveSpeed * dt);
    }

    // lock height only in walk mode
    if (!flyMode)
        pos.y = walkEyeY;
}

void FPSCamera::ApplyView() const
{
    Vec3 f = Forward();
    gluLookAt(pos.x, pos.y, pos.z,
        pos.x + f.x, pos.y + f.y, pos.z + f.z,
        0.0, 1.0, 0.0);
}
