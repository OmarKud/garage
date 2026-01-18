#pragma once
#include "Point.h"
#include "Cube.h"
class TaxiRoom {
public:
    TaxiRoom(Point floorCenter, double height, double length, double width);

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
    // ✅ لوحة التاكسي
    Cube backSign;
    GLuint signTex = 0;
    // 
    GLuint wallTex = 0;
    GLuint floorTex = 0;
    GLuint ceilingTex = 0;
    GLuint roofTex = 0;
    GLuint taxiTex = 0;  // <--- أضف هذا السطر هنا لتخزين تيكستشر السيارة
};