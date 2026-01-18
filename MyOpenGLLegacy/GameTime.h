#pragma once
#include <GL/glut.h>
#include <algorithm>

class Time
{
public:
    void Tick()
    {
        int now = glutGet(GLUT_ELAPSED_TIME);
        if (lastMs == 0) lastMs = now;
        dt = (now - lastMs) / 1000.0f;
        lastMs = now;
        dt = std::min(dt, 0.05f);
    }

    float Delta() const { return dt; }

private:
    int lastMs = 0;
    float dt = 0.0f;
};
