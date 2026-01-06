#pragma once
#include <vector>
#include <string>
#include "AABB.h"

struct Collider {
    AABB box;
    std::string tag;
    bool isTrigger = false;
};

class CollisionWorld {
public:
    void Clear();
    void Add(const AABB& box, const std::string& tag, bool isTrigger = false);
    bool CollidesPoint(const Vec3& p, const std::string& ignoreTag = "") const;

private:
    std::vector<Collider> colliders;
};
