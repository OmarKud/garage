#pragma once
#include "Point.h"
#include "Cube.h"
#include <cmath>
#include "Input.h"

class Switch
{
public:
    Switch(Point pos, float radius = 25.0f)
        : pos(pos), radius(radius),
        body(pos, 6.0, 1.0, 3.0) // شكل زر بسيط: مكعب صغير
    {
    }

    bool IsNear(const Point& p) const
    {
        float dx = (float)(p.x - pos.x);
        float dy = (float)(p.y - pos.y);
        float dz = (float)(p.z - pos.z);
        return (dx * dx + dy * dy + dz * dz) <= (radius * radius);
    }

    bool Update(const class Input& input, const Point& playerPos)
    {
        if (IsNear(playerPos) && (input.Pressed('e') || input.Pressed('E')))
        {
            state = !state;
            return true;
        }
        return false;
    }

    bool State() const { return state; }

    void Draw(GLuint tex)
    {
        body.drawWithTexture(tex, 1, 1); 
    }

private:
    Point pos;
    float radius = 25.0f;
    bool state = false;
    Cube body;
};
