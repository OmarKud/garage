#include "LuxuryRoom.h"
#include "Texture.h"      


void LuxuryRoom::DrawFakeSpotOnFloor(float lx, float ly, float lz,
    float floorY, float cutoffDeg, float intensity)
{
    float h = ly - floorY;
    if (h <= 0.01f) return;

    float rad = tanf(cutoffDeg * 3.1415926f / 180.0f) * h;
    rad *= 1.35f;

    const int slices = 36;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.9f, 0.35f, intensity);
    glVertex3f(lx, floorY + 0.01f, lz);

    for (int i = 0; i <= slices; ++i)
    {
        float a = (float)i / (float)slices * 2.0f * 3.1415926f;
        float x = lx + cosf(a) * rad;
        float z = lz + sinf(a) * rad;

        glColor4f(1.0f, 0.6f, 0.35f, 0.0f);
        glVertex3f(x, floorY + 0.01f, z);
    }
    glEnd();
}

void LuxuryRoom::DrawFakeSpotOnBackWall(float lx, float ly, float lz,
    float wallZ, float cutoffDeg, float intensity)
{
    float d = fabsf(lz - wallZ);
    if (d <= 0.01f) return;

    float rad = tanf(cutoffDeg * 3.1415926f / 180.0f) * d;
    rad *= 1.35f;

    const int slices = 36;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.9f, 0.35f, intensity);
    glVertex3f(lx, ly - d, wallZ + 0.01f); // تقريب مركز البقعة

    for (int i = 0; i <= slices; ++i)
    {
        float a = (float)i / (float)slices * 2.0f * 3.1415926f;

        float x = lx + cosf(a) * rad;
        float y = (ly - d) + sinf(a) * rad;

        glColor4f(1.0f, 0.9f, 0.35f, 0.0f);
        glVertex3f(x, y, wallZ + 0.01f);
    }
    glEnd();
}

void LuxuryRoom::DrawFakeSpotOnLeftWall(float lx, float ly, float lz,
    float wallX, float cutoffDeg, float intensity)
{
    float d = fabsf(lx - wallX);
    if (d <= 0.01f) return;

    float rad = tanf(cutoffDeg * 3.1415926f / 180.0f) * d;
    rad *= 1.35f;

    const int slices = 36;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.9f, 0.35f, intensity);
    glVertex3f(wallX + 0.01f, ly - d, lz);

    for (int i = 0; i <= slices; ++i)
    {
        float a = (float)i / (float)slices * 2.0f * 3.1415926f;

        float y = (ly - d) + sinf(a) * rad;
        float z = lz + cosf(a) * rad;

        glColor4f(1.0f, 0.9f, 0.35f, 0.0f);
        glVertex3f(wallX + 0.01f, y, z);
    }
    glEnd();
}

void LuxuryRoom::DrawFakeSpotOnRightWall(float lx, float ly, float lz,
    float wallX, float cutoffDeg, float intensity)
{
    float d = fabsf(lx - wallX);
    if (d <= 0.01f) return;

    float rad = tanf(cutoffDeg * 3.1415926f / 180.0f) * d;
    rad *= 1.35f;

    const int slices = 36;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.9f, 0.35f, intensity);
    glVertex3f(wallX - 0.01f, ly - d, lz);

    for (int i = 0; i <= slices; ++i)
    {
        float a = (float)i / (float)slices * 2.0f * 3.1415926f;

        float y = (ly - d) + sinf(a) * rad;
        float z = lz + cosf(a) * rad;

        glColor4f(1.0f, 0.9f, 0.35f, 0.0f);
        glVertex3f(wallX - 0.01f, y, z);
    }
    glEnd();
}


LuxuryRoom::LuxuryRoom(Point floorCenter, double height, double length, double width)
    : floorCenter(floorCenter), height(height), length(length), width(width),

    floorPiece(Point(floorCenter.x, floorCenter.y + 0.1, floorCenter.z), 0.2, length, width),
    ceilingPiece(Point(floorCenter.x, floorCenter.y + height - 0.1, floorCenter.z), 0.2, length, width),

    backWall(
        Point(floorCenter.x, floorCenter.y + (height / 2.0) - 40, floorCenter.z - (length / 2.0 + t / 2.0)),
        height * 1.1, t, width),

    leftWall(
        Point(floorCenter.x - width / 2.0 + t / 2.0, floorCenter.y + (height / 2.0) - 40, floorCenter.z),
        height * 1.1, length, t),

    rightWall(
        Point(floorCenter.x, floorCenter.y + (height / 2.0) - 40, floorCenter.z - (length / 2.0 + t / 2.0) + 165),
        height * 1.1, t, width),

    lightSwitch(
        Point(floorCenter.x - width / 2.0 + 120,
            floorCenter.y + height * 0.1,
            floorCenter.z + length / 2.0 - 2.0),
        25.0f)
{
}

void LuxuryRoom::Init()
{
    wallTex = LoadTexture2D("assets/textures/wall.jpg");
    floorTex = LoadTexture2D("assets/textures/wood.jpg");
    roofTex = LoadTexture2D("assets/textures/roof.jpg");

    ceilingTex = wallTex;

    spots.clear();
    spots.resize(6);

    float y = (float)(floorCenter.y + height - 1.0f);
    float x = (float)floorCenter.x;
    float z = (float)floorCenter.z;

    float halfW = (float)width * 0.5f;
    float halfL = (float)length * 0.5f;

    float xs[3] = { x - halfW * 0.30f, x, x + halfW * 0.30f };
    float zs[2] = { z - halfL * 0.15f, z + halfL * 0.15f };

    int idx = 0;
    for (int r = 0; r < 2; ++r)
        for (int c = 0; c < 3; ++c)
        {
            SpotLight& s = spots[idx++];
            s.enabled = true;

            s.pos[0] = xs[c];
            s.pos[1] = y;
            s.pos[2] = zs[r];
            s.pos[3] = 1.0f;

            s.dir[0] = 0.0f;
            s.dir[1] = -1.0f;
            s.dir[2] = 0.0f;

            s.linearAtt = 0.0025f;
            s.quadAtt = 0.00005f;
            s.cutoff = 30.0f;
            s.exponent = 10.0f;
            s.constantAtt = 1.0f;
        }
}

void LuxuryRoom::Update(const Input& input)
{
    if (input.Pressed('L') || input.Pressed('l'))
        spotsOn = !spotsOn;
}

void LuxuryRoom::draw()
{
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1, 1, 1);

    floorPiece.drawWithTexture(floorTex, 8, 8);
    ceilingPiece.drawWithTexture(roofTex, 3, 3);
    backWall.drawWithTexture(wallTex, 1, 1);
    leftWall.drawWithTexture(wallTex, 1, 1);
    rightWall.drawWithTexture(wallTex, 1, 1);
    lightSwitch.Draw(floorTex);

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 0);
    for (int i = 0; i < 6; ++i)
    {
        Cube lamp(Point(spots[i].pos[0], spots[i].pos[1], spots[i].pos[2]), 2.0, 2.0, 2.0);
        lamp.draw();
    }
    glPopAttrib();

    if (spotsOn)
    {
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);     // additive
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        float intensity = 2.0f;

        float floorY = (float)(floorCenter.y + 0.11f);

        float backZ = (float)(floorCenter.z - (length / 2.0 + t / 2.0));
        float leftX = (float)(floorCenter.x - width / 2.0 + t / 2.0);
        float rightX = (float)(floorCenter.x + width / 2.0 - t / 2.0);

        for (int i = 0; i < 6; ++i)
        {
            float lx = spots[i].pos[0];
            float ly = spots[i].pos[1];
            float lz = spots[i].pos[2];

            DrawFakeSpotOnFloor(lx, ly, lz, floorY, spots[i].cutoff, intensity);
          
        }

        glDepthMask(GL_TRUE);
        glPopAttrib();
    }
}
