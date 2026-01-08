#pragma once
#include "Math3D.h"
#include "Input.h"

class FPSCamera
{
public:
    void Update(float dt, const Input& input);
    void ApplyView() const;

    Vec3 pos{ 0.0f, 1.8f, 5.0f };
    float yawDeg = -90.0f;
    float pitchDeg = 0.0f;

    float moveSpeed = 6.0f;
    float mouseSens = 0.12f;

    // ✅ toggle with F
    bool flyMode = false;
    float walkEyeY = 1.8f;

private:
    Vec3 Forward() const;        // yaw + pitch
    Vec3 ForwardFlat() const;    // yaw only (XZ)
    Vec3 RightFlat(const Vec3& fFlat) const;
};
