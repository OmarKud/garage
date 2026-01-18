#pragma once
#include <vector>
#include <string>
#include <cmath>

#include "Point.h"
#include "Cube.h"
#include "Input.h"
#include "Switch.h"
#include "SpotLight.h"

class LuxuryRoom
{
public:
    LuxuryRoom(Point floorCenter, double height, double length, double width);

    void Init();
    void Update(const Input& input);          // إذا بدك toggle مباشر بـ L
    void draw();

private:
    // --- helpers ---
    static void DrawFakeSpotOnFloor(float lx, float ly, float lz,
        float floorY, float cutoffDeg, float intensity);

    static void DrawFakeSpotOnBackWall(float lx, float ly, float lz,
        float wallZ, float cutoffDeg, float intensity);

    static void DrawFakeSpotOnLeftWall(float lx, float ly, float lz,
        float wallX, float cutoffDeg, float intensity);

    static void DrawFakeSpotOnRightWall(float lx, float ly, float lz,
        float wallX, float cutoffDeg, float intensity);

private:
    Point floorCenter;
    double height, length, width;
    double t = 2.0;

    bool spotsOn = false;

    std::vector<SpotLight> spots;

    Cube floorPiece;
    Cube ceilingPiece;
    Switch lightSwitch;

    Cube leftWall;
    Cube rightWall;
    Cube backWall;

    unsigned int wallTex = 0;
    unsigned int floorTex = 0;
    unsigned int roofTex = 0;
    unsigned int ceilingTex = 0;
};
