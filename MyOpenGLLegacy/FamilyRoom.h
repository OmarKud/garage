#pragma once
#pragma once
#include "Point.h"
#include "Cube.h"
#include "Cylinder.h"   // ✅ جديد

class FamilyRoom {
public:
    FamilyRoom(Point floorCenter, double height, double length, double width);

    void Init();
    void draw();

private:
    Point floorCenter;
    double height, length, width;
    double t = 2.0; // 

    Cube floorPiece;
    Cube ceilingPiece;

    Cube leftWall;
    Cube rightWall;
    Cube backWall;   // 

    //
    GLuint wallTex = 0;
    GLuint floorTex = 0;
    GLuint ceilingTex = 0;
    GLuint roofTex = 0;
    Cylinder stageUpper;
    Cylinder stageLower;
    GLuint stageUpperTex = 0;
    GLuint stageLowerTex = 0;
};