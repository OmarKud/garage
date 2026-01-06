#pragma once
#include "Math3D.h"

class CollisionWorld; // forward declaration

class Room
{
public:
    virtual ~Room() = default;

    virtual void RegisterColliders(CollisionWorld& world) const = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() const = 0;
    virtual void TryInteract(const Vec3& playerPos) = 0;
};
