#include "FrontWall.h"
#include <GL/gl.h>
#include "Cube.h"

// لازم يكون عندك الدالة معرفة في مكان ما

void FrontWall::Init()
{
        texture = LoadTexture2D("assets/textures/gray2.jpg"); 
}

static float clamp01(float x) { return (x < 0.f) ? 0.f : (x > 1.f ? 1.f : x); }


static float easeInOutSine(float t)
{
    // 0..1 -> 0..1 بحركة ناعمة (تسارع ثم تباطؤ) :contentReference[oaicite:4]{index=4}
    const float pi = 3.1415926f;
    return 0.5f - 0.5f * cosf(pi * clamp01(t));
}

void FrontWall::SetDoorOpen(bool open)
{
    doorTarget = open ? 1.0f : 0.0f;
}

void FrontWall::ToggleDoor()
{
    doorTarget = (doorTarget < 0.5f) ? 1.0f : 0.0f;
}

void FrontWall::Update(float dt)
{
    if (doorDuration <= 0.0001f) { doorT = doorTarget; return; }

    float step = dt / doorDuration;

    if (doorT < doorTarget) doorT = clamp01(doorT + step);
    else if (doorT > doorTarget) doorT = clamp01(doorT - step);
}

FrontWall::FrontWall(Point center, double height, double length, double width)
    : center(center),
      height(height),
      length(length),
      width(width),

      // ===== Ratios (same ones you used) =====
      // sideW  = 10% of width  -> parts (7)(8)
      // postW  =  6% of width  -> parts (3)(4)
      // doorW  = 28% of width  -> door gap (no cube)
      // top1H  = 20% of height -> part (1)
      // beam2H = 15% of height -> part (2)
      // baseH  = 18% of height -> parts (5)(6)

      // Helpful derived values:
      // winW = (width - 2*sideW - 2*postW - doorW) / 2
      //
      // IMPORTANT CHANGE:
      // Z FRONT PLANE IS ALREADY center.z (do NOT add length/2 - 2.4 again)

      wall1( // (1) TOP LONG BEAM
          Point(
              center.x,
              center.y + height*0.25,
              center.z
          ),
          (height )*0.31,
          length,
          width
      ),

      wall2( // (2) BEAM ABOVE DOOR
          Point(
              center.x,
              center.y ,
              center.z
          ),
          height * 0.25,
          5,
          (width * 0.28) + 2.0 * (width * 0.06) // doorW + 2*postW
      ),

      wall3( // (3) LEFT DOOR POST
          Point(
              center.x - ((width * 0.25) / 2.0),
              center.y - (height * 0.5),
              center.z
          ),
          height * 0.5 ,
          5,
          width * 0.15
      ),

      wall4( // (4) RIGHT DOOR POST
          Point(
              center.x + ((width * 0.25) / 2.0),
              center.y - (height * 0.5),
              center.z
          ),
          height - (height * 0.5),
          5,
          width * 0.15
      ),

      wall5( // (5) LEFT BOTTOM BASE
          Point(
              center.x - width*0.325,
              center.y - height*0.5 ,
              center.z
          ),
          height * 0.18,
          5,
          (width * 0.25)
      ),

      wall6( // (6) RIGHT BOTTOM BASE
          Point(
              center.x + width * 0.325
              ,
              center.y - height * 0.5,
              center.z
          ),
          height * 0.18,
          5,
          (width * 0.25)
      ),

      wall7( // (7) LEFT OUTER SIDE
          Point(
              center.x - width * 0.475,
              center.y - height * 0.5 ,
              center.z
          ),
          height - (height * 0.25),
          5,
          width * 0.05
      ),

      wall8( // (8) RIGHT OUTER SIDE
          Point(
              center.x + width * 0.475,
              center.y - height * 0.5,
              center.z
          ),
          height - (height * 0.25),
          5,
          width * 0.05
      ),
          leftGlass(Point(
              center.x - width * 0.325,
              center.y - height * 0.5,
              center.z),
              height * 0.75,
              5,
              (width * 0.25)),
          rightGlass(Point(
              center.x + width * 0.325,
              center.y - height * 0.5,
              center.z),
              height * 0.75,
              5,
              (width * 0.25)),

          leftEntryGlassDoor( 
          Point(
          center.x - width*0.025,
          center.y - (height * 0.5),
          center.z
          ),
          height * 0.5,
          2.5,
          width * 0.05
          ),
          rightEntryGlassDoor(
          Point(
          center.x + width * 0.025,
          center.y - (height * 0.5),
          center.z
          ),
          height * 0.5,
          2.5,
          width * 0.05
          )
{
    float doorW = (float)(width * 0.28); // عرض فتحة الباب
    float panelW = (float)(width * 0.05); // عرض لوح الزجاج نفسه

    doorMaxSlide = (doorW * 0.5f) - panelW*1.9f;
    if (doorMaxSlide < 0.0f) doorMaxSlide = 0.0f; 
}


void FrontWall::draw()
{
    // لو ما انحمل texture لأي سبب
    if (texture == 0) Init();

    wall1.drawWithTexture(texture, 1, 1);
    wall2.drawWithTexture(texture, 1, 1);
    wall3.drawWithTexture(texture, 1, 1);
    wall4.drawWithTexture(texture, 1, 1);
    wall7.drawWithTexture(texture, 1, 1);
    wall8.drawWithTexture(texture, 1, 1);

    //left
    leftGlass.drawGlassCube(0.6f, 0.8f, 1.0f, 0.35f);
    //right
    rightGlass.drawGlassCube(0.6f, 0.8f, 1.0f, 0.35f);

    /*leftEntryGlassDoor.drawGlassCube(0.6f, 0.8f, 1.0f, 0.35f);
    rightEntryGlassDoor.drawGlassCube(0.6f, 0.8f, 1.0f, 0.35f);*/

    GLfloat red = 0.6f,
        green = 0.8f,
        blue = 1.0f,
        alpha = 0.35f;
    float t = easeInOutSine(doorT);
    float dx = doorMaxSlide * t;

    // اليسار ينزلق يسار، اليمين ينزلق يمين
    glDisable(GL_CULL_FACE); // للزجاج غالباً أفضل يشوف من الطرفين

    leftEntryGlassDoor.drawTranslated(-dx, 0.0f, 0.0f,red,green,blue,alpha);
    rightEntryGlassDoor.drawTranslated(+dx, 0.0f, 0.0f, red, green, blue, alpha);


}








