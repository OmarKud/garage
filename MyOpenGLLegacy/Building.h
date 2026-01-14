#pragma once
#include <GL/glut.h>
#include "Point.h"
#include "Cube.h"
#include "FrontWall.h"

class Building {
public:
    Building(Point center, double height, double length, double width);
    void Init();
    void draw();

private:
    Point center;
    double height, length, width;

    Cube leftWall, rightWall, behindWall, roof, floor, innerWall1, innerWall2;
    FrontWall frontWall;

    GLuint floorTexture = 0;
    GLuint wallTexture = 0;
    GLuint roofTexture = 0;
};
