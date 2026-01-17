#pragma once
#include <Windows.h>
#include "Point.h"
#include "Input.h"
#include <math.h>

#ifndef DEG2RAD
#define DEG2RAD(x) ((x) * 3.14159265f / 180.0f)
#endif

class Car
{
public:
    Car() : position(0, 0, 0), speed(0), angle(0), wheelRotation(0) {}
    virtual ~Car() {}

    virtual void Update(float dt, const Input& input) = 0;
    virtual void Draw() = 0;

    virtual Point GetDriverEyePos() const = 0;
    virtual Point GetDriverTargetPos() const = 0;

    Point position;      
    float speed;         
    float angle;         
    float wheelRotation; 
};