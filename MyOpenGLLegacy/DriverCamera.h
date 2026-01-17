#pragma once
#include <Windows.h>
#include "Car.h"
#include <GL/glu.h>

class DriverCamera
{
public:
    // Constructor to bind the camera to a specific car instance
    DriverCamera(Car* carPtr) : car(carPtr) {}

    void ApplyView()
    {
        if (!car) return;

        // Applies the camera transformation based on the driver's perspective
        Point eye = car->GetDriverEyePos();
        Point target = car->GetDriverTargetPos();

        gluLookAt(eye.x, eye.y, eye.z,
            target.x, target.y, target.z,
            0.0f, 1.0f, 0.0f); 
    }

private:
    Car* car;
};