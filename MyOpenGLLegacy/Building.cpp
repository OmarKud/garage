#include "Building.h"
#include <GL/gl.h>
#include "Cube.h"
#include <Cylinder.h>

void Building::Init()
{
    roofTexture = LoadTexture2D("assets/textures/dirt.jpg");
    floorTexture = LoadTexture2D("assets/textures/siramik.jpg");
    wallTexture = LoadTexture2D("assets/textures/gray2.jpg");
    luxuryRoom.Init();
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

      frontWall(
          Point(center.x, center.y, center.z + (length / 2) - 2.4),
          height,
          5,
          width - 0.1)
{
    Init();
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
    frontWall.draw();
}
