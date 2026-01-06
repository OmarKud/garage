#pragma once
#include "Room.h"
#include "CollisionWorld.h"
#include <Door.h>

class RoomTest : public Room
{
public:
    void RegisterColliders(CollisionWorld& world) const override;
    void Update(float dt) override;
    void Render() const override;
    void TryInteract(const Vec3& playerPos) override;
private:
    Door door;
};
