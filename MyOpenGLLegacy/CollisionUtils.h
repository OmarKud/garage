#pragma once
#include "Collision.h"
#include "Cube.h"

static inline AABB AABBFromCube(const Cube& c)
{
    const Point& p = c.GetCenter();
    double h = c.GetHeight();
    double l = c.GetLength();
    double w = c.GetWidth();

    AABB b;
    b.min = Point(p.x - w * 0.5, p.y, p.z - l * 0.5);
    b.max = Point(p.x + w * 0.5, p.y + h, p.z + l * 0.5);
    return b;
}
