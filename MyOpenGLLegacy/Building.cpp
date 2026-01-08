#include "Building.h"
#include <GL/gl.h>  
#include "Cube.h"

void Building::Init()
{

    texture = LoadTexture2D("assets/textures/stone.jpg");

}
Building::Building(Point center, double height, double length, double width)
    : center(center), height(height), length(length), width(width),

    leftWall(Point(center.x - (width / 2), center.y-(height/2), center.z), height, length, 0.1),

    rightWall(Point(center.x + (width / 2), center.y - (height / 2), center.z), height, length, 0.1),

    behindWall(Point(center.x, center.y - (height / 2), center.z - (length / 2)), height, 0.1, width), // see note below

    roof(Point(center.x, center.y + (height / 2), center.z), 0.1, length, width),

    floor(Point(center.x, center.y - (height / 2), center.z), 0.1, length, width)
{
}

//Building::Building(Point center, double height, double length, double width) 
//{ 
//    this->center = center; 
//    this->height = height; 
//    this->length = length; 
//    this->width = width; 
//    this->leftWall = Cube(Point(center.x - (length / 2), center.y, center.z), height, width, 0.1); 
//    this->rightWall = Cube(Point(center.x + (length / 2), center.y, center.z), height, width, 0.1); 
//    this->behindWall = Cube(Point(center.x, center.y, (center.z - width / 2)), height, 0.1, length); 
//    this->roof = Cube(Point(center.x, (center.y + (height / 2)), center.z), 0.1, length, width); 
//    this->floor = Cube(Point(center.x, (center.y - (height / 2)), center.z), 0.1, length, width); 
//}


void Building::draw()
{
    leftWall.drawWithTexture(texture,1,1);
    rightWall.drawWithTexture(texture, 1, 1);
    behindWall.drawWithTexture(texture, 1, 1);
    roof.drawWithTexture(texture, 1, 1);
    floor.drawWithTexture(texture, 1, 1);
}

