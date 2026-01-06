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

Vec3 FPSCamera::Right(const Vec3& f) const
{
    // يمين على مستوى الأرض
    Vec3 r{ -f.z, 0.0f, f.x };
    return normalize(r);
}

void FPSCamera::Update(float dt, const Input& input)
{
    // mouse look
    yawDeg += input.MouseDX() * mouseSens;
    pitchDeg -= input.MouseDY() * mouseSens;
    pitchDeg = clampf(pitchDeg, -89.0f, 89.0f);

    Vec3 f = Forward();
    Vec3 r = Right(f);

    Vec3 v{ 0,0,0 };

    if (input.IsDown('W')) v = v + f;
    if (input.IsDown('S')) v = v - f;
    if (input.IsDown('D')) v = v + r;
    if (input.IsDown('A')) v = v - r;

    v.y = 0.0f; // مشي

    if (length(v) > 0.0001f)
    {
        Vec3 dir = normalize(v);
        pos = pos + dir * (moveSpeed * dt);
    }

    // ثبّت ارتفاع العين
    pos.y = 1.8f;
}

void FPSCamera::ApplyView() const
{
    Vec3 f = Forward();
    gluLookAt(pos.x, pos.y, pos.z,
        pos.x + f.x, pos.y + f.y, pos.z + f.z,
        0.0, 1.0, 0.0);
}
