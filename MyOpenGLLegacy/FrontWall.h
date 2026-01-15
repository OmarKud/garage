#pragma once
#include <GL/glut.h>
#include "Point.h"
#include "Cube.h"

class FrontWall {
public:
    FrontWall(Point center, double height, double length, double width);
    void draw();
    void Init();              // load texture
    void SetTexture(GLuint t) { texture = t; } // optional if you want Building texture
    void Update(float dt);
    void ToggleDoor();                 // فتح/إغلاق
    void SetDoorOpen(bool open);       // تحكم مباشر
    Point GetTriggerPoint() const { return Point(center.x, center.y - height * 0.5, center.z); }


private:
    Point center;
    double height, length, width;

    Cube wall1, wall2, wall3, wall4, wall5, wall6, wall7, wall8;
    Cube leftGlass, rightGlass;
    Cube leftEntryGlassDoor, rightEntryGlassDoor;
    GLuint texture = 0;

    float doorT = 0.0f;        // 0 closed, 1 open
    float doorTarget = 0.0f;   // الهدف
    float doorDuration = 0.8f; // زمن الفتح بالثواني (عدّلها براحتك)
    float doorMaxSlide = 0.0f; // كم ينزلق كل باب على X
};
