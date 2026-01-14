#pragma once
#include <Point.h>
#include <GL/glut.h>
#include <Cube.h>
#include "AABB.h"
#include <LuxuryRoom.h>
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
	LuxuryRoom luxuryRoom;

    GLuint floorTexture = 0;
    GLuint wallTexture = 0;
    GLuint roofTexture = 0;
	GLuint texture = 0;

};
