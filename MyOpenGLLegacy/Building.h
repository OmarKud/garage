#pragma once
#include <Point.h>
#include <GL/glut.h>
#include <Cube.h>
#include "AABB.h"
#include <LuxuryRoom.h>
#include "FrontWall.h"
#include "Cylinder.h"
#include <FamilyRoom.h>
#include <TaxiRoom.h>
#include "Collision.h"





class Building {
public:
    Building(Point center, double height, double length, double width);
    void Init();
    void draw();
    void Update(float dt);
    void ToggleDoor();
    void SetDoorOpen(bool open);
    Point DoorTriggerPoint() const;
    void Update(const Input& input) { luxuryRoom.Update(input); }
    void BuildColliders(CollisionWorld& cw) const;

private:
    Point center;
    double height, length, width;

    Cube leftWall, rightWall, behindWall, roof, floor, innerWall1, innerWall2;
    FrontWall frontWall;
	LuxuryRoom luxuryRoom;
    Cylinder carStageUpper , carStageLower;
    FamilyRoom familyRoom;
    TaxiRoom   taxiRoom;
    GLuint floorTexture = 0;
    GLuint wallTexture = 0;
    GLuint roofTexture = 0;
    GLuint upperStageTexture = 0, lowerStageTexture = 0;
	GLuint texture = 0;

};
