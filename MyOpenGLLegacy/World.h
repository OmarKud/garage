#pragma once
#include <GL/gl.h>
#include "FPSCamera.h"
#include "Input.h"

class World
{
public:
    void Init(int w, int h);
    void Resize(int w, int h);
    void Update(float dt, const Input& input);
    void Render() const;

private:
    FPSCamera cam;

    void Apply3D() const;
    void DrawGround(float half, float y) const;
    void DrawGrid(float half, float step, float y) const;

    // ✅ جديد
    void DrawSkySphere(float radius, float yawOffsetDeg) const;

    int width = 1280;
    int height = 720;

    GLuint groundTex = 0;
    GLuint skyTex = 0;

    float skyYaw = 0.0f; // إذا بدك تدوّر السماء
};
