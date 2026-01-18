#include "TaxiRoom.h"

void TaxiRoom::Init()
{
    wallTex = LoadTexture2D("assets/textures/clean.jpg");
    floorTex = LoadTexture2D("assets/textures/Taxi.jpg");
    roofTex = LoadTexture2D("assets/textures/polystyrene.jpg");
    signTex = LoadTexture2D("assets/textures/taxis.jpg");

    //[
    ceilingTex = wallTex;

    if (wallTex == 0)  wallTex = 0;
    if (floorTex == 0) floorTex = wallTex;
    if (ceilingTex == 0) ceilingTex = wallTex;
}

TaxiRoom::TaxiRoom(Point floorCenter, double height, double length, double width)
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
    // p
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
            floorCenter.y + (height / 2.0),
            floorCenter.z - (length / 2.0 + t / 2.0) + 165

        ),
        height * 1.1,
        t,
        width
    ), backSign(
        Point(

            floorCenter.x+40,
            floorCenter.y + (height / 2.0) - 17.0
            ,
            floorCenter.z - (length / 2.0 + t / 2.0) + 80.0
        ),
        height * 0.23,     // Y
        width * 0.23,      // Z  ← صار امتداد على الحيط
        0.1                // X  ← سماكة رفيعة
    )



{
}

void TaxiRoom::draw()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    floorPiece.drawWithTexture(floorTex, 3, 3);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(0.65f, 0.65f, 0.65f);
    ceilingPiece.drawWithTexture(roofTex, 3, 3);

    glColor3f(1.0f, 1.0f, 1.0f);
    backWall.drawWithTexture(wallTex, 1, 1);
    leftWall.drawWithTexture(wallTex, 1, 1);
   // rightWall.drawWithTexture(wallTex, 1, 1);


    glColor3f(1, 1, 1);

    // نطبّق التيكستشر فقط على الوجه الأمامي
    backSign.drawWithTextureOnOneFace(

        signTex,
        "left",
        1,
        1
    );


} 