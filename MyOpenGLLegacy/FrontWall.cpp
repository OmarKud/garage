#include "FrontWall.h"
#include <GL/gl.h>
#include "CollisionUtils.h"
#include "Collision.h"
#include "FrontWall.h"
#include <GL/gl.h>
#include "Cube.h"

// لازم يكون عندك الدالة معرفة في مكان ما

void FrontWall::Init()
{
        texture = LoadTexture2D("assets/textures/gray2.jpg"); 
}

// FrontWall.cpp


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
          winLeft(
              Point(center.x - width * 0.325, center.y - height * 0.5, center.z),
              height * 0.75, 5, (width * 0.25)
          ),
          winRight(
              Point(center.x + width * 0.325, center.y - height * 0.5, center.z),
              height * 0.75, 5, (width * 0.25)
          )

{
    // length might be unused now (that's fine)
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
    drawGlassCube(Point(
        center.x - width * 0.325,
        center.y - height*0.5,
        center.z),
        height * 0.75,
        5,
        (width * 0.25));
    //right
    drawGlassCube(Point(
        center.x + width * 0.325,
        center.y - height * 0.5,
        center.z),
        height * 0.75,
        5,
        (width * 0.25));
}

void FrontWall::BuildColliders(CollisionWorld& cw) const
{
    // كل القطع الصلبة
    cw.AddAABB(AABBFromCube(wall1));
    cw.AddAABB(AABBFromCube(wall2));
    cw.AddAABB(AABBFromCube(wall3));
    cw.AddAABB(AABBFromCube(wall4));
    cw.AddAABB(AABBFromCube(wall7));
    cw.AddAABB(AABBFromCube(wall8));

    // القواعد (إذا بدك تمنع المرور منها)
    cw.AddAABB(AABBFromCube(wall5));
    cw.AddAABB(AABBFromCube(wall6));

    // الزجاج: إذا بدك يكون “جدار” (يمنع المرور) ضيفه
   /* cw.AddAABB(AABBFromCube(winLeft));
    cw.AddAABB(AABBFromCube(winRight));*/

    // الباب: ما في Cube له ⇒ ما في Collider ⇒ فتحة مرور جاهزة
}


void FrontWall::drawGlassCube(Point center, float height, float length, float width)
{
    // اعزل كل التغييرات داخل الدالة
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // للشفافية: ما نكتب على depth
    glDepthMask(GL_FALSE);

    // لون البلور (أزرق خفيف)
    glColor4f(0.6f, 0.8f, 1.0f, 0.35f);

    // لمعان (Specular) — بس ما نفعّل/نطفّي الإضاءة هون
    GLfloat spec[]  = { 0.9f, 0.9f, 1.0f, 1.0f };
    GLfloat shine[] = { 90.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

    Cube glassCube(center, height, length, width);
    glassCube.draw();

    glPopAttrib(); // يرجّع كل شيء كما كان
}



