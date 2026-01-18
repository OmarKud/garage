#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "World.h"
#include "Texture.h"
#include <Cube.h>
#include <Building.h>

void World::Init(int w, int h, const Building &building)
{
    width = w;
    height = h;

    groundTex = LoadTexture2D("assets/textures/dirt.jpg");
    skyTex = LoadHDRToLDRTexture2D("assets/textures/sky.hdr", 0.15f);
    cubeTex = LoadTexture2D("assets/textures/stone.jpg");

    b = std::make_unique<Building>(building);
    collisions.Clear();
    if (b) b->BuildColliders(collisions);
#include <string>

    OutputDebugStringA(("Colliders count = " + std::to_string(collisions.boxes.size()) + "\n").c_str());


}

void World::Resize(int w, int h)
{
    width = (w <= 0) ? 1 : w;
    height = (h <= 0) ? 1 : h;
}

void World::Update(float dt, const Input& input)
{
    // 1) خزّن مكان الكاميرا قبل الحركة
    Vec3 oldPos = cam.pos;

    // 2) حدّث الكاميرا (بيعطيك pos جديد)
    cam.Update(dt, input);

    // 3) احسب الدلتا (قديش تحركت خلال هالفريم)
    Vec3 delta = cam.pos - oldPos;

    // 4) رجّع الكاميرا للمكان القديم، وبعدين امشِ على خطوات صغيرة
    cam.pos = oldPos;

    // قيم اللاعب
    const float playerRadius = 2.5f;
    const float playerHeight = 1.8f;   // خليه كبير لتتأكد ما يتجاهل الجدران (ارتفاع المبنى عندك كبير)

    // عدد الخطوات: كل ما زادت الدلتا زيد steps حتى ما يصير tunneling
    float d = length(delta);
    int steps = (int)std::ceil(d / 2.0f);   // كل 2 وحدات خطوة
    if (steps < 1) steps = 1;
    if (steps > 50) steps = 50;            // سقف أمان

    Vec3 step = delta / (float)steps;

    for (int i = 0; i < steps; ++i)
    {
        cam.pos = cam.pos + step;
        const float eyeY = cam.walkEyeY;   // 1.8
        collisions.ResolvePlayerCamera(cam.pos, playerRadius, eyeY, playerHeight);
    }

    if (b) b->Update(input);
}



void World::Apply3D() const
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)width / (double)height, 0.8, 1500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cam.ApplyView();
}
void World::DrawSkySphere(float radius, float yawOffsetDeg) const
{
    if (skyTex == 0)
        return;

    // sky لا لازم يأثر على باقي الرسم
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);

    glDepthMask(GL_FALSE);   // ما تكتب على depth
    glDisable(GL_CULL_FACE); // لأننا جوّا الكرة

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skyTex);

    // خلي السماء ما تتأثر بـ glColor
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glPushMatrix();

    // نثبت السماء بمكان الكاميرا (تلغي تأثير الترجمة)
    cam.ApplyView();
    glTranslatef(cam.pos.x, cam.pos.y, cam.pos.z);

    glRotatef(yawOffsetDeg, 0.f, 1.f, 0.f);

    const int stacks = 24;
    const int slices = 48;

    for (int i = 0; i < stacks; ++i)
    {
        float v0 = (float)i / stacks;
        float v1 = (float)(i + 1) / stacks;

        float theta0 = (v0 - 0.5f) * 3.1415926f; // -pi/2 .. +pi/2
        float theta1 = (v1 - 0.5f) * 3.1415926f;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j)
        {
            float u = (float)j / slices;
            float phi = u * 2.0f * 3.1415926f; // 0..2pi

            // point 0
            float x0 = cosf(theta0) * cosf(phi);
            float y0 = sinf(theta0);
            float z0 = cosf(theta0) * sinf(phi);

            glTexCoord2f(u, 1.0f - v0);
            glVertex3f(x0 * radius, y0 * radius, z0 * radius);

            // point 1
            float x1 = cosf(theta1) * cosf(phi);
            float y1 = sinf(theta1);
            float z1 = cosf(theta1) * sinf(phi);

            glTexCoord2f(u, 1.0f - v1);
            glVertex3f(x1 * radius, y1 * radius, z1 * radius);
        }
        glEnd();
    }

    glPopMatrix();

    // رجّع الـ states
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glDisable(GL_TEXTURE_2D);
    // glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

}

void World::DrawGround(float half, float y) const
{
    if (groundTex == 0)
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex3f(-half, y, -half);
        glVertex3f(half, y, -half);
        glVertex3f(half, y, half);
        glVertex3f(-half, y, half);
        glEnd();
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, groundTex);

    // ✅ مهم: الأرض لازم تكون MODULATE وتأكد اللون أبيض
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1, 1, 1);

    float tiling = 10.0f;
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-half, y, -half);
    glTexCoord2f(tiling, 0);
    glVertex3f(half, y, -half);
    glTexCoord2f(tiling, tiling);
    glVertex3f(half, y, half);
    glTexCoord2f(0, tiling);
    glVertex3f(-half, y, half);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void World::DrawGrid(float half, float step, float y) const
{
    glBegin(GL_LINES);
    for (float x = -half; x <= half; x += step)
    {
        glVertex3f(x, y + 0.001f, -half);
        glVertex3f(x, y + 0.001f, +half);
    }
    for (float z = -half; z <= half; z += step)
    {
        glVertex3f(-half, y + 0.001f, z);
        glVertex3f(+half, y + 0.001f, z);
    }
    glEnd();
}

void World::Render() const
{
    Apply3D();

    DrawSkySphere(1000.0f, 0.0f);
    DrawGround(1000.0f, 0.0f);

    if (b)
        b->draw();
}

