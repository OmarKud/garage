#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <GL/gl.h>

struct SpotLight
{
    bool enabled = true;

    float pos[4] = { 0, 0, 0, 1 };
    float dir[3] = { 0, -1, 0 };

    float diffuse[4] = { 1.00f, 0.90f, 0.55f, 1.0f };  // warm yellow
    float specular[4] = { 1.00f, 0.95f, 0.70f, 1.0f };
    float ambient[4] = { 0.02f, 0.02f, 0.01f, 1.0f };


    float cutoff = 22.0f;     //  
    float exponent = 25.0f;   // 
    float constantAtt = 1.0f;
    float linearAtt = 0.03f;
    float quadAtt = 0.002f;

    void Apply(unsigned int lightId) const
    {
        if (!enabled)
        {
            glDisable(lightId);
            return;
        }

        glEnable(lightId);
        glLightfv(lightId, GL_POSITION, pos);
        glLightfv(lightId, GL_SPOT_DIRECTION, dir);

        glLightfv(lightId, GL_AMBIENT, ambient);
        glLightfv(lightId, GL_DIFFUSE, diffuse);
        glLightfv(lightId, GL_SPECULAR, specular);

        glLightf(lightId, GL_SPOT_CUTOFF, cutoff);
        glLightf(lightId, GL_SPOT_EXPONENT, exponent);

        glLightf(lightId, GL_CONSTANT_ATTENUATION, constantAtt);
        glLightf(lightId, GL_LINEAR_ATTENUATION, linearAtt);
        glLightf(lightId, GL_QUADRATIC_ATTENUATION, quadAtt);
    }
};
