#pragma once
#include <GL/glut.h>
#include "Point.h"
#include "Cube.h"

class FrontWall {
public:
    FrontWall(Point center, double height, double length, double width);
    void draw();
    static void drawGlassCube(Point center, float height, float length, float width);
    void Init();              // load texture
    void SetTexture(GLuint t) { texture = t; } // optional if you want Building texture

private:
    Point center;
    double height, length, width;

    Cube wall1, wall2, wall3, wall4, wall5, wall6, wall7, wall8;
    GLuint texture = 0;
};
