#include "RoomTest.h"
#include <GL/glut.h>
#include <World.h>


void RoomTest::RegisterColliders(CollisionWorld& cw) const
{
    // غرفة 20x20 تقريباً على أرضية y=0، ارتفاع 3
    // جدران (سميكة 0.5)
    cw.Add({ {-10,0,-10}, { 10,3,-9.5f} }, "wall"); // خلف
    cw.Add({ {-10,0, 9.5f}, { 10,3,10} }, "wall");  // أمام
    cw.Add({ {-10,0,-10}, {-9.5f,3,10} }, "wall");  // يسار
    cw.Add({ { 9.5f,0,-10}, {10,3,10} }, "wall");   // يمين

    // باب على الجدار الخلفي (يتعطل تصادمه لما يفتح)
    // إذا الباب شبه مفتوح، خليه بدون تصادم
    bool doorClosed = (door.OpenRate() < 0.1f);
    if (doorClosed)
        cw.Add({ {-1.0f,0,-10}, {1.0f,2.5f,-9.5f} }, "door");
}

void RoomTest::Update(float dt)
{
    door.Update(dt);
}

void RoomTest::TryInteract(const Vec3& playerPos)
{
    if (door.CanInteract(playerPos))
        door.Interact();
}

static void DrawGround()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.18f, 0.18f, 0.18f);
    glBegin(GL_QUADS);
    glVertex3f(-50, 0, -50);
    glVertex3f(50, 0, -50);
    glVertex3f(50, 0, 50);
    glVertex3f(-50, 0, 50);
    glEnd();
}

static void DrawWalls()
{
    glColor3f(0.35f, 0.35f, 0.40f);
    // أربع جدران كـ quads (مبسطة)
    glBegin(GL_QUADS);

    // خلف z=-10
    glVertex3f(-10, 0, -10); glVertex3f(10, 0, -10); glVertex3f(10, 3, -10); glVertex3f(-10, 3, -10);
    // أمام z=10
    glVertex3f(-10, 0, 10); glVertex3f(10, 0, 10); glVertex3f(10, 3, 10); glVertex3f(-10, 3, 10);
    // يسار x=-10
    glVertex3f(-10, 0, -10); glVertex3f(-10, 0, 10); glVertex3f(-10, 3, 10); glVertex3f(-10, 3, -10);
    // يمين x=10
    glVertex3f(10, 0, -10); glVertex3f(10, 0, 10); glVertex3f(10, 3, 10); glVertex3f(10, 3, -10);

    glEnd();
}

static void DrawDoor(float openRate)
{
    // باب بسيط على الجدار الخلفي: نعمله “يفتح” بتحريك/دوران رمزي
    // هنا عملناه ينزلق جانبياً حسب openRate (أسهل من hinge)
    float shift = openRate * 2.2f;

    glColor3f(0.6f, 0.45f, 0.25f);
    glPushMatrix();
    glTranslatef(shift, 0.0f, 0.0f);

    // الباب في فتحة الجدار الخلفي عند z=-10
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 0.0f, -10.0f);
    glVertex3f(1.0f, 0.0f, -10.0f);
    glVertex3f(1.0f, 2.5f, -10.0f);
    glVertex3f(-1.0f, 2.5f, -10.0f);
    glEnd();

    glPopMatrix();
}

void RoomTest::Render() const
{
    DrawGround();
    DrawWalls();
    DrawDoor(door.OpenRate());

    // مكعب مرجع بالوسط
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glColor3f(0.45f, 0.45f, 0.50f);
    glutSolidCube(2.0);
    glPopMatrix();
}
