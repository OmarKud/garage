#pragma once
#include <vector>
#include <cmath>

#include "Point.h"
#include "Math3D.h"   // Vec3
#include "AABB.h"     // ✅ لازم حتى يعرف AABB

// clamp helper (واحدة فقط)
static inline float CollClamp(float v, float a, float b)
{
    return (v < a) ? a : (v > b ? b : v);
}

// تحويلات بسيطة بين Vec3 و Point
static inline Point ToPoint(const Vec3& v)
{
    return Point((double)v.x, (double)v.y, (double)v.z);
}
static inline Vec3 ToVec3(const Point& p)
{
    Vec3 v;
    v.x = (float)p.x;
    v.y = (float)p.y;
    v.z = (float)p.z;
    return v;
}

// دائرة على XZ ضد صندوق AABB (مع اعتبار الارتفاع اختيارياً)
static inline void ResolveCircleAABB_XZ(
    Point& pos,
    float radius,
    float playerBottomY,
    float playerTopY,
    const AABB& b)
{
    // إذا اللاعب عمودياً خارج نطاق الصندوق، تجاهله (تصادم جدران فقط)
    if (playerTopY < (float)b.min.y || playerBottomY >(float)b.max.y)
        return;

    float cx = (float)pos.x;
    float cz = (float)pos.z;

    float nearestX = CollClamp(cx, (float)b.min.x, (float)b.max.x);
    float nearestZ = CollClamp(cz, (float)b.min.z, (float)b.max.z);

    float dx = cx - nearestX;
    float dz = cz - nearestZ;

    float dist2 = dx * dx + dz * dz;
    float r2 = radius * radius;

    if (dist2 >= r2)
        return; // ما في تصادم

    // إذا مركز الدائرة داخل الصندوق (dx=dz=0) بدنا ندفّعو لأقرب وجه
    if (dist2 < 1e-8f)
    {
        float leftPen = cx - (float)b.min.x;
        float rightPen = (float)b.max.x - cx;
        float backPen = cz - (float)b.min.z;
        float frontPen = (float)b.max.z - cz;

        float minPen = leftPen;
        int axis = 0; // 0=x-, 1=x+, 2=z-, 3=z+

        if (rightPen < minPen) { minPen = rightPen; axis = 1; }
        if (backPen < minPen) { minPen = backPen;  axis = 2; }
        if (frontPen < minPen) { minPen = frontPen; axis = 3; }

        float push = radius + 0.001f;
        if (axis == 0) pos.x = b.min.x - push;
        if (axis == 1) pos.x = b.max.x + push;
        if (axis == 2) pos.z = b.min.z - push;
        if (axis == 3) pos.z = b.max.z + push;
        return;
    }

    float dist = std::sqrt(dist2);
    float overlap = radius - dist;

    // اتجاه الدفع للخارج
    float nx = dx / dist;
    float nz = dz / dist;

    pos.x += nx * overlap;
    pos.z += nz * overlap;
}

struct CollisionWorld
{
    std::vector<AABB> boxes;

    void Clear() { boxes.clear(); }
    void AddAABB(const AABB& b) { boxes.push_back(b); }

    // ✅ النسخة الأصلية (تستعمل pos.y كـ "قدم اللاعب")
    void ResolvePlayer(Point& pos, float radius, float playerHeight) const
    {
        float bottom = (float)pos.y;
        float top = (float)pos.y + playerHeight;

        for (const auto& b : boxes)
            ResolveCircleAABB_XZ(pos, radius, bottom, top, b);

        // تكرار لتحسين الزوايا
        for (const auto& b : boxes)
            ResolveCircleAABB_XZ(pos, radius, bottom, top, b);
    }

    // ✅ لو Vec3 يمثل "قدم اللاعب" (مش عين الكاميرا)
    void ResolvePlayer(Vec3& pos, float radius, float playerHeight) const
    {
        Point p = ToPoint(pos);
        ResolvePlayer(p, radius, playerHeight);
        pos = ToVec3(p);
    }

    // ✅ نسخة صحيحة للكاميرا: pos = عين، وليس قدم
    void ResolvePlayerCamera(Vec3& camPos, float radius, float eyeY, float playerHeight) const
    {
        // حوّل لموضع قدم اللاعب
        Point foot((double)camPos.x, (double)(camPos.y - eyeY), (double)camPos.z);

        ResolvePlayer(foot, radius, playerHeight);

        // رجّع XZ فقط، وخلي Y ثابت (الكاميرا ارتفاع عين)
        camPos.x = (float)foot.x;
        camPos.z = (float)foot.z;
    }
};
