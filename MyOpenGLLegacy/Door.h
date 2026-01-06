#pragma once
#include "Math3D.h"
#include "AABB.h"

class Door {
public:
    Door() = default;

    void SetPose(const Vec3& hingePos, float width, float height);
    void Update(float dt);
    void Render() const;

    // تفاعل: E قريب من الباب
    void TryToggle(const Vec3& playerPos);

    // كوليدر (مغلق فقط)
    bool IsOpen() const { return open; }
    AABB ColliderBox() const;

private:
    Vec3 hinge{ 0,0,0 };
    float w = 1.0f;
    float h = 2.2f;

    bool open = false;
    float angle = 0.0f;       // current
    float targetAngle = 0.0f; // 0 أو 90
};
