#pragma once
#include <Windows.h>
#include "Car.h"
#include "Cube.h"
#include "Cylinder.h"
#include <vector>

class MainCar : public Car
{
public:
    MainCar();
    ~MainCar();

    void Init(float x, float z);
    void LoadAssets();
    void Update(float dt, const Input& input) override;
    void Draw() override;

    Point GetDriverEyePos() const override;
    Point GetDriverTargetPos() const override;


    void ToggleDriverCamera() {
        isDriverView = !isDriverView;
        if (isDriverView) {
            UpdateDriverCameraPosition();
        }
    }    bool IsDriverView() const { return isDriverView; }
    Point GetLocalDriverEyePosition() const;

private:
    float width = 30.0f;
    float length = 50.0f;
    float height = 15.5f;
    float wheelRadius = 5.4f;

    float doorAngle = 0.0f;
    bool isDoorOpen = false;
    bool isDriverView = false;
    Point driverEyeLocal;  
    Point driverTargetLocal;
    Point TransformLocalToWorld(const Point& localPoint) const;
    bool cameraPositionInitialized = false; 

    struct CarPart {
        Cube shape;
        Point offset;
        float rotationX, rotationY, rotationZ;
        GLuint texture;
        bool isTransparent;
        float alpha;
    };

    std::vector<CarPart> bodyParts;
    std::vector<CarPart> interiorParts;
    Cylinder* wheelShape;

    GLuint carBodyTex = 0;
    GLuint glassTex = 0;
    GLuint wheelTex = 0;
    GLuint interiorTex = 0;
    GLuint dashboardTex = 0;
    GLuint seatTex = 0;

    void InitializeParts();
    void DrawExterior();
    void DrawInterior();
    void DrawWheels();
    void DrawDoors();
    void DrawWindows();
    void DrawDashboard();
    void DrawSteeringWheel();
    void DrawChassis();
    void DrawRoof();
    void DrawHoodAndTrunk();
    void DrawSeats();
    void UpdateDriverCameraPosition();


};