#pragma once
#include "Math3D.h" // نفس Vec3 اللي بتستخدمه بالكوليجن

struct AABB
{
    Vec3 min;
    Vec3 max;

    bool Contains(const Vec3& p) const
    {
        return (p.x >= min.x && p.x <= max.x) &&
            (p.y >= min.y && p.y <= max.y) &&
            (p.z >= min.z && p.z <= max.z);
    }

};
