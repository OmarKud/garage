#pragma once
#include "Point.h"
#include "Cube.h"
#include <GL/gl.h>

class LuxuryRoom {
public:
    LuxuryRoom(Point floorCenter, double height, double length, double width);

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
};
