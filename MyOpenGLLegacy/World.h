#pragma once
#include <GL/gl.h>
#include <memory>

#include "FPSCamera.h"
#include "Input.h"
#include "Building.h"

class World
{
public:
    void Init(int w, int h, const Building& building); // pass by const ref
    void Resize(int w, int h);
    void Update(float dt, const Input& input);
    void Render() const;

private:
    FPSCamera cam;

    std::unique_ptr<Building> b;  // ✅ no default constructor needed

    void Apply3D() const;
    void DrawGround(float half, float y) const;
    void DrawGrid(float half, float step, float y) const;
    void DrawSkySphere(float radius, float yawOffsetDeg) const;

    int width = 1280;
    int height = 720;

    GLuint groundTex = 0;
    GLuint skyTex = 0;
    GLuint cubeTex = 0;
};
