#include "Building.h"
#include <GL/gl.h>
#include "Cube.h"
#include <Cylinder.h>
#include <CollisionUtils.h>

void Building::Init()
{
    roofTexture = LoadTexture2D("assets/textures/dirt.jpg");
    floorTexture = LoadTexture2D("assets/textures/siramik.jpg");
    wallTexture = LoadTexture2D("assets/textures/gray2.jpg");
    luxuryRoom.Init();
    familyRoom.Init();
    taxiRoom.Init();
    upperStageTexture = LoadTexture2D("assets/textures/siramik.jpg");
    lowerStageTexture = LoadTexture2D("assets/textures/gray2.jpg");
}

Building::Building(Point center, double height, double length, double width)
    : center(center), height(height), length(length), width(width),

      leftWall(Point(center.x - (width / 2) + 2.5, center.y - (height / 2), center.z), height, length, 5),

      rightWall(Point(center.x + (width / 2) - 2.5, center.y - (height / 2), center.z), height, length, 5),

      behindWall(Point(center.x, center.y - (height / 2), center.z - (length / 2) + 2.4), height, 5, width - 1),

      roof(Point(center.x, center.y + (height / 2), center.z), 5, length, width),

      floor(Point(center.x, center.y - (height / 2), center.z), 0.1, length, width),

      innerWall1(Point(center.x - (width / 3) - width * 0.02, center.y - height / 2, center.z),
                 height, 4, width * 0.28),

      innerWall2(Point(center.x + (width / 3) + width * 0.02, center.y - height / 2, center.z),
                 height, 4, width * 0.28),
      luxuryRoom(
          Point(
              center.x - width / 2 + (width / 4) - 50,
              center.y - (height / 2),
              center.z - 85),
          height - 1,
          length * 0.47,
          width * 0.275),
    carStageUpper(Point(
        center.x,
        center.y - (height / 2) + height * 0.05+ height*0.001 ,
        center.z),
        width * 0.11,
        width * 0.11,
        height * 0.02,
        32,
        32),
    carStageLower(Point(
        center.x,
        center.y - (height / 2) + height * 0.05,
        center.z),
        width * 0.1,
        width * 0.1,
        height * 0.05,
        32,
        32),
      frontWall(
          Point(center.x, center.y, center.z + (length / 2) - 2.4),
          height,
          5,
          width - 0.1)
    , familyRoom(
        Point(
            center.x - width / 2 + (width)-75
            ,
            center.y - (height / 2),
            center.z - 85
        ),
        height - 1,
        length * 0.47,
        width * 0.275
    ), taxiRoom(
        Point(
            center.x - width / 2 + (width)-75
            ,
            center.y - (height / 2),
            center.z + 87
        ),
        height - 1,
        length * 0.47,
        width * 0.275
    )
{
    Init();
}
void Building::BuildColliders(CollisionWorld& cw) const
{
    // جدران المبنى الأساسية
    cw.AddAABB(AABBFromCube(leftWall));
    cw.AddAABB(AABBFromCube(rightWall));
    cw.AddAABB(AABBFromCube(behindWall));

    cw.AddAABB(AABBFromCube(innerWall1));
    cw.AddAABB(AABBFromCube(innerWall2));

    // FrontWall (قطع متعددة مع فراغ باب)
    frontWall.BuildColliderss(cw);

    // LuxuryRoom إذا بدك كمان
   // luxuryRoom.BuildColliders(cw);
}

void Building::draw()
{
    leftWall.drawWithTexture(wallTexture, 1, 1);
    rightWall.drawWithTexture(wallTexture, 1, 1);
    behindWall.drawWithTexture(wallTexture, 1, 1);

    roof.drawWithTexture(roofTexture, 1, 1);

    floor.drawWithTexture(floorTexture, 50, 10);

    innerWall1.drawWithTexture(wallTexture, 1, 1);
    innerWall2.drawWithTexture(wallTexture, 1, 1);

    luxuryRoom.draw();
    taxiRoom.draw();
    familyRoom.draw();
    carStageUpper.drawWithTexture(upperStageTexture);
    carStageLower.drawWithTexture(lowerStageTexture);


    frontWall.draw();
}

void Building::Update(float dt) { frontWall.Update(dt); }
void Building::ToggleDoor() { frontWall.ToggleDoor(); }
void Building::SetDoorOpen(bool open) { frontWall.SetDoorOpen(open); }
Point Building::DoorTriggerPoint() const { return frontWall.GetTriggerPoint(); }
