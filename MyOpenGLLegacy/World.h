#pragma once
#include <Windows.h> // ??? ?? ???? ????? ??????
#include <GL/gl.h>
#include <memory>

#include "FPSCamera.h"
#include "Input.h"
#include "Building.h"
#include "MainCar.h"       // <--- ??? ???
#include "DriverCamera.h"
#include <Model.h>
#include "Collision.h"


class World
{
public:
    void Init(int w, int h, const Building& building); // pass by const ref
    void Resize(int w, int h);
    void Update(float dt, const Input& input);
    void Render() const;
    void ToggleCameraMode() { isDriverCamera = !isDriverCamera; }
private:
    FPSCamera cam;
    std::unique_ptr<MainCar> myCar;
    std::unique_ptr<DriverCamera> driverCam;
    std::unique_ptr<Building> b;  // ? no default constructor needed
    bool isDriverCamera = false;
    void Apply3D() const;
    void DrawGround(float half, float y) const;
    void DrawGrid(float half, float step, float y) const;
    void DrawSkySphere(float radius, float yawOffsetDeg) const;
    CollisionWorld collisions;

    int width = 1280;
    int height = 720;
    
    Model car;

    GLuint groundTex = 0;
    GLuint skyTex = 0;
    GLuint cubeTex = 0;
};
