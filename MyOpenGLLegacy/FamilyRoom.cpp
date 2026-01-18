#include "FamilyRoom.h"

void FamilyRoom::Init()
{
    wallTex = LoadTexture2D("assets/textures/patterned.jpg");
    floorTex = LoadTexture2D("assets/textures/wood_floor.jpg");
    roofTex = LoadTexture2D("assets/textures/beige_wall.jpg");
    // ✅ تيكستشر المنصة (اختاري يلي بدك)
    stageUpperTex = LoadTexture2D("assets/textures/siramik.jpg");
    stageLowerTex = LoadTexture2D("assets/textures/gray2.jpg");
    ceilingTex = wallTex;

    if (wallTex == 0)  wallTex = 0;
    if (floorTex == 0) floorTex = wallTex;
    if (ceilingTex == 0) ceilingTex = wallTex;
}

FamilyRoom::FamilyRoom(Point floorCenter, double height, double length, double width)
    : floorCenter(floorCenter), height(height), length(length), width(width),

    // أرض
    floorPiece(
        Point(floorCenter.x, floorCenter.y + 0.1, floorCenter.z),
        0.2,
        length,
        width
    ),

    ceilingPiece(
        Point(floorCenter.x, floorCenter.y + height - 0.1, floorCenter.z),
        0.2,
        length,
        width
    ),

    backWall(
        Point(
            floorCenter.x,
            floorCenter.y + (height / 2.0) - 40,
            floorCenter.z - (length / 2.0 + t / 2.0)

        ),
        height * 1.1,
        t,
        width
    ),

    leftWall(
        Point(
            floorCenter.x - width / 2.0 + (t / 2.0) + 135,
            floorCenter.y + (height / 2.0) - 40,
            floorCenter.z
        ),
        height * 1.1,
        length,
        t
    ),


    rightWall(
        Point(
            floorCenter.x,
            floorCenter.y + (height / 2.0) - 40,
            floorCenter.z - (length / 2.0 + t / 2.0) + 165

        ),
        height * 1.1,
        t,
        width
    ),
    stageLower(
        Point(floorCenter.x, floorCenter.y + 6.0, floorCenter.z),
        width * 0.19f, width * 0.19f, height * 0.04f, 32, 32
    ),
    stageUpper(
        Point(floorCenter.x, floorCenter.y + 8.0, floorCenter.z),
        width * 0.1f, width * 0.14f, height * 0.02f, 32, 32
    )

{
}

void FamilyRoom::draw()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    floorPiece.drawWithTexture(floorTex, 5, 5);
    // ✅ المنصة
    glColor3f(1, 1, 1);
    stageLower.drawWithTexture(stageLowerTex);
    stageUpper.drawWithTexture(stageUpperTex);
    // السقف (خليه أفتح شوي من الحيطان)
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1.25f, 1.25f, 1.25f);   // إذا ما فرق خليها 1.0
    ceilingPiece.drawWithTexture(roofTex, 3, 3);

    // ✅ الحيطان: فتّحيهم (قيم أكبر من 1)
    glColor3f(1.15f, 1.15f, 1.15f);   // جربي 1.1 أو 1.2 حسب ما بدك
    backWall.drawWithTexture(wallTex, 1, 1);
    leftWall.drawWithTexture(wallTex, 1, 1);
    rightWall.drawWithTexture(wallTex, 1, 1);

    // رجّعي اللون
    glColor3f(1.0f, 1.0f, 1.0f);
}
