#pragma once
#include <GL/glut.h>
#include "Point.h"
#include "Cube.h"

struct CollisionWorld; // ✅ forward declaration

class FrontWall {
public:
    FrontWall(Point center, double height, double length, double width);
    void draw();
    static void drawGlassCube(Point center, float height, float length, float width);
    void Init();
    void SetTexture(GLuint t) { texture = t; }
    void BuildColliders(CollisionWorld& cw) const;

private:
    Point center;
    double height, length, width;

    Cube wall1, wall2, wall3, wall4, wall5, wall6, wall7, wall8;
    Cube winLeft;
    Cube winRight;
    GLuint texture = 0;
};
