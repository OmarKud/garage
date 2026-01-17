#include <Windows.h> 
#include "MainCar.h"
#include "Texture.h" 
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>

// ????? ??????? ????????
#ifndef DEG2RAD
#define DEG2RAD(x) ((x) * 3.14159265f / 180.0f)
#endif

MainCar::MainCar() {
    wheelShape = new Cylinder(Point(0, 0, 0), wheelRadius, wheelRadius, 4.25f, 40, 4);
    InitializeParts();
}

MainCar::~MainCar() {
    if (wheelShape) delete wheelShape;
}

void MainCar::InitializeParts() {
    bodyParts.clear();

    float chassisHeight = height * 0.4f;
    float cabinHeight = height * 0.50f;
    float glassThickness = 0.05f;
    float pPosH = chassisHeight + (cabinHeight / 2);

    // ??? ?????? ??? ?? ????????
    bodyParts.push_back({ Cube(Point(0, 0, 0), cabinHeight * 0.5f, glassThickness, width * 0.8f), Point(0, pPosH + 0.2f, length * 0.27f), -28.0f, 0, 0, glassTex, true, 0.3f });
    bodyParts.push_back({ Cube(Point(0, 0, 0), cabinHeight * 0.5f, glassThickness, width * 0.8f), Point(0, pPosH + 0.2f, -length * 0.27f), 25.0f, 0, 0, glassTex, true, 0.3f });

    // ??????? (??? ??)
    float seatWidth = width * 0.28f;
    float seatX[2] = { -width * 0.22f, width * 0.22f };
    for (int i = 0; i < 2; i++) {
        interiorParts.push_back({ Cube(Point(0, 0, 0), 1.0f, 3.5f, seatWidth), Point(seatX[i], chassisHeight + 0.5f, -1.0f), 0, 0, 0, seatTex, false, 1.0f });
        interiorParts.push_back({ Cube(Point(0, 0, 0), 5.0f, 0.6f, seatWidth), Point(seatX[i], chassisHeight + 2.8f, -2.8f), -15.0f, 0, 0, seatTex, false, 1.0f });
    }
}
void MainCar::DrawSeats() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, seatTex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    float seatWidth = width * 0.28f;
    float seatX[2] = { -width * 0.22f, width * 0.22f };
    float chassisHeight = height * 0.4f;

    for (int i = 0; i < 2; i++) {
        // --- ??????? (??????) ---
        glPushMatrix();
        glTranslatef(seatX[i], chassisHeight + 0.5f, -1.0f);
        Cube base(Point(0, 0, 0), 1.0f, 3.5f, seatWidth);
        base.drawWithTexture(seatTex, 1, 1);
        glPopMatrix();

        // --- ?????? (?????) ---
        glPushMatrix();
        glTranslatef(seatX[i], chassisHeight + 2.8f, -2.8f);
        glRotatef(-15.0f, 1, 0, 0); 
        Cube back(Point(0, 0, 0), 5.0f, 0.6f, seatWidth);
        back.drawWithTexture(seatTex, 1, 1);
        glPopMatrix();
    }
}
void MainCar::DrawChassis() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, carBodyTex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glPushMatrix();
    float chassisHeight = height * 0.2f;
    glTranslatef(0, chassisHeight / 2, 0);
    Cube chassis(Point(0, 0, 0), chassisHeight, length, width * 0.95f);
    chassis.drawWithTexture(carBodyTex, 1, 1);
    glPopMatrix();
}

void MainCar::DrawRoof() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, carBodyTex);

    glPushMatrix();
    float chassisHeight = height * 0.4f;
    float cabinHeight = height * 0.50f;
    float roofLength = length * 0.58f;

    glTranslatef(0, chassisHeight + cabinHeight + 0.25f, 0);
    Cube roof(Point(0, 0, 0), 0.5f, roofLength, width * 0.88f);
    roof.drawWithTexture(carBodyTex, 1, 1);
    glPopMatrix();
}

void MainCar::DrawHoodAndTrunk() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, carBodyTex);

    float chassisHeight = height * 0.4f;
    float partH = height * 0.2f;

    //  (Hood)
    glPushMatrix();
    glTranslatef(0, chassisHeight + partH / 2, length * 0.35f);
    Cube hood(Point(0, 0, 0), partH, length * 0.25f, width * 0.9f);
    hood.drawWithTexture(carBodyTex, 1, 1);
    glPopMatrix();

    //  (Trunk)
    glPushMatrix();
    glTranslatef(0, chassisHeight + partH / 2, -length * 0.35f);
    Cube trunk(Point(0, 0, 0), partH, length * 0.25f, width * 0.9f);
    trunk.drawWithTexture(carBodyTex, 1, 1);
    glPopMatrix();
}
void MainCar::DrawDoors() {
    float doorLen = length * 0.35f;
    float doorHeight = height * 0.4f;
    float windowHeight = height * 0.35f;

    float sideOffset = width * 0.48f;

    for (int i = 0; i < 2; i++) {
        float side = (i == 0) ? 1.0f : -1.0f;

        glPushMatrix();

        glTranslatef(side * sideOffset, height * 0.2f, doorLen / 2);

        float currentAngle = (side > 0) ? doorAngle : -doorAngle;
        glRotatef(currentAngle, 0, -1, 0);

        glTranslatef(0, 0, -doorLen / 2);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, carBodyTex);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        Cube doorBody(Point(0, 0, 0), doorHeight, doorLen, 0.4f);
        doorBody.drawWithTexture(carBodyTex, 1, 1);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glTranslatef(0, doorHeight / 2 + windowHeight / 2, 0);
        glColor4f(0.7f, 0.8f, 1.0f, 0.4f); 
        Cube window(Point(0, 0, 0), windowHeight, doorLen, 0.2f);
        window.draw();
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        glPopMatrix();
    }
}
void MainCar::LoadAssets() {
    carBodyTex = LoadTexture2D("assets/textures/blue_metal_plate_diff_1k.jpg");
    glassTex = LoadTexture2D("assets/textures/glass.jpg");
    wheelTex = LoadTexture2D("assets/textures/rubber_tiles_diff_1k.jpg");
    dashboardTex = LoadTexture2D("assets/textures/dashboard.jpg");
    seatTex = LoadTexture2D("assets/textures/brown_leather_albedo_1k.jpg");
}

void MainCar::Init(float x, float z) {
    position = Point(x, 1.6f, z);
    speed = 0;
    angle = 0;
}

void MainCar::Update(float dt, const Input& input) {
    bool moveFwd = input.IsDown(VK_UP);
    bool moveBwd = input.IsDown(VK_DOWN);

    float acceleration = 15.0f * dt;
    float maxSpeed = 20.0f;
    float friction = 0.95f;

    if (moveFwd) speed = (std::min)(speed + acceleration, maxSpeed);
    else if (moveBwd) speed = (std::max)(speed - acceleration, -maxSpeed * 0.4f);
    else speed *= friction;

    float rad = DEG2RAD(angle);
    position.x += sinf(rad) * speed * dt;
    position.z += cosf(rad) * speed * dt;

    float turnSpeed = 80.0f * dt;
    if (abs(speed) > 0.1f) {
        if (input.IsDown(VK_LEFT)) angle += turnSpeed;
        if (input.IsDown(VK_RIGHT)) angle -= turnSpeed;
    }

    wheelRotation += speed * 50.0f * dt;

    if (input.Pressed('E')) isDoorOpen = !isDoorOpen;
    float targetDoorAngle = isDoorOpen ? 65.0f : 0.0f;
    doorAngle += (targetDoorAngle - doorAngle) * 5.0f * dt;
    if (input.Pressed('C')) {
        ToggleDriverCamera();
    }
}
Point MainCar::GetLocalDriverEyePosition() const {
    float localX = -width * 0.22f;      
    float localY = (height * 0.4f) + 2.5f; 
    float localZ = 0.5f;               

    return Point(localX, localY, localZ);
}
void MainCar::UpdateDriverCameraPosition() {
    if (!cameraPositionInitialized) {
        driverEyeLocal = GetLocalDriverEyePosition();
        driverTargetLocal = Point(0.0f, -0.5f, 20.0f); 
        cameraPositionInitialized = true;
    }
}

Point MainCar::GetDriverEyePos() const {
    float rad = DEG2RAD(angle);
    return Point(
        position.x - sinf(rad) * 15.0f,
        position.y + 7.0f,
        position.z - cosf(rad) * 15.0f
    );
    if (!isDriverView) {
        return Point(position.x - sinf(rad) * 15.0f, position.y + 7.0f, position.z - cosf(rad) * 15.0f);
    }

    float localEyeX = -width * 0.22f;   
    float localEyeY = (height * 0.4f) + 3.0f; 
    float localEyeZ = -0.8f;
    Point localEye(localEyeX, localEyeY, localEyeZ);

    if (!cameraPositionInitialized) {
        const_cast<MainCar*>(this)->UpdateDriverCameraPosition();
    }
    float worldX = position.x + (driverEyeLocal.x * cosf(rad) - driverEyeLocal.z * sinf(rad));
    float worldY = position.y + driverEyeLocal.y;
    float worldZ = position.z + (driverEyeLocal.x * sinf(rad) + driverEyeLocal.z * cosf(rad));
    return TransformLocalToWorld(localEye);
}
Point MainCar::GetDriverTargetPos() const {

    if (!isDriverView)
    {
        return position;
    }

    Point eye = GetDriverEyePos();

    float rad = DEG2RAD(angle);


    if (!cameraPositionInitialized) {
        const_cast<MainCar*>(this)->UpdateDriverCameraPosition();
    }

    float lookDistance = 50.0f;
    float targetX = eye.x + sinf(rad) * lookDistance;
    float targetY = eye.y - 2.0f;  
    float targetZ = eye.z + cosf(rad) * lookDistance;

    float worldTargetX = position.x + (driverTargetLocal.x * cosf(rad) - driverTargetLocal.z * sinf(rad));
    float worldTargetY = position.y + driverTargetLocal.y;
    float worldTargetZ = position.z + (driverTargetLocal.x * sinf(rad) + driverTargetLocal.z * cosf(rad));
    return Point(targetX, targetY, targetZ);
}
Point MainCar::TransformLocalToWorld(const Point& localPoint) const {
    float rad = DEG2RAD(angle);

    float x = localPoint.x * cosf(rad) - localPoint.z * sinf(rad);
    float z = localPoint.x * sinf(rad) + localPoint.z * cosf(rad);
    float y = localPoint.y;

    return Point(position.x + x, position.y + y, position.z + z);
}
void MainCar::Draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(angle, 0, 1, 0);

    DrawChassis();
    DrawRoof();
    DrawHoodAndTrunk();

    DrawWheels();
    DrawSeats();
    DrawDoors();

    DrawWindows();

    if (isDriverView) {
        DrawDashboard();
        DrawSteeringWheel();

        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        Point localEye = GetLocalDriverEyePosition();
        glVertex3f(localEye.x, localEye.y, localEye.z);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

void MainCar::DrawExterior() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    for (auto& part : bodyParts) {
        if (part.isTransparent) continue; 

        glPushMatrix();
        glTranslatef(part.offset.x, part.offset.y, part.offset.z);
        glRotatef(part.rotationX, 1, 0, 0);

        if (part.texture != 0) {
            glBindTexture(GL_TEXTURE_2D, part.texture);
            glColor3f(1.0f, 1.0f, 1.0f);
            part.shape.drawWithTexture(part.texture, 1, 1);
        }
        else {
            glColor3f(0.5f, 0.5f, 0.5f);
            part.shape.draw();
        }
        glPopMatrix();
    }
}

void MainCar::DrawWindows() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); 

    for (auto& part : bodyParts) {
        if (!part.isTransparent) continue;
        glPushMatrix();
        glTranslatef(part.offset.x, part.offset.y, part.offset.z);
        glColor4f(0.7f, 0.8f, 1.0f, 0.3f); 
        part.shape.draw();
        glPopMatrix();
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void MainCar::DrawWheels() {
    float xOffset = width * 0.43f; 
    float zFrontOffset = length * 0.32f;
    float zBackOffset = -length * 0.32f;
    float xOffsetLeft = width * 0.60f;
    float wheelWidth = 4.0f; 

    Point wheelPos[4] = {
        Point(xOffset, 0, zFrontOffset),   
        Point(-xOffsetLeft, 0, zFrontOffset),  
        Point(xOffset, 0, zBackOffset),    
        Point(-xOffsetLeft, 0, zBackOffset)  
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();

        glTranslatef(wheelPos[i].x, wheelRadius, wheelPos[i].z);
        glRotatef(wheelRotation, 1, 0, 0);
        glRotatef(90, 0, 0, 1);
        glTranslatef(0, 0, -wheelWidth / 2.0f);

        if (wheelTex) wheelShape->drawWithTexture(wheelTex);
        else wheelShape->draw();

        glPopMatrix();
    }
}

void MainCar::DrawInterior() {
    glEnable(GL_TEXTURE_2D);
    for (auto& part : interiorParts) {
        glPushMatrix();
        glTranslatef(part.offset.x, part.offset.y, part.offset.z);
        glRotatef(part.rotationX, 1, 0, 0); 

        if (part.texture != 0) {
            glBindTexture(GL_TEXTURE_2D, part.texture);
            glColor3f(1.0f, 1.0f, 1.0f);
            part.shape.drawWithTexture(part.texture, 1, 1);
        }
        else {
            glColor3f(0.4f, 0.2f, 0.1f); 
            part.shape.draw();
        }
        glPopMatrix();
    }
}

void MainCar::DrawDashboard() {
    glPushMatrix();
    glTranslatef(0, height * 0.45f, -length * 0.32f);
    Cube dashboard(Point(0, 0, 0), 0.35f, 0.15f, 0.9f);
    dashboard.draw(); 
    glPopMatrix();
}

void MainCar::DrawSteeringWheel() {
    glPushMatrix();
    glTranslatef(-width * 0.25f, height * 0.5f, -length * 0.25f);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.02f, 0.12f, 10, 20);
    glPopMatrix();
}