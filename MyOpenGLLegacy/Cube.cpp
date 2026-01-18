#include "Cube.h"
#include <GL/gl.h>  
Cube::Cube(Point center, double height, double length, double width)
{
    this->center = center;
    this->height = height;
    this->length = length;
    this->width = width;
}

void Cube::draw()
{
    // draw sides
    glBegin(GL_QUAD_STRIP);
    glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glEnd();

    glBegin(GL_QUADS);
    // draw lower face
    glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
    // draw upper face
    glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    glEnd();
}

void Cube::drawWithTexture(GLuint textureID, int repeatX, int repeatY)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    double x0 = center.x - width * 0.5;
    double x1 = center.x + width * 0.5;
    double y0 = center.y;
    double y1 = center.y + height;
    double z0 = center.z - length * 0.5;
    double z1 = center.z + length * 0.5;

    glBegin(GL_QUADS);

    // Front (+Z)
    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(0.f, 0.f);         glVertex3d(x0, y0, z1);
    glTexCoord2f((float)repeatX, 0.f); glVertex3d(x1, y0, z1);
    glTexCoord2f((float)repeatX, (float)repeatY); glVertex3d(x1, y1, z1);
    glTexCoord2f(0.f, (float)repeatY); glVertex3d(x0, y1, z1);

    // Back (-Z)
    glNormal3f(0.f, 0.f, -1.f);
    glTexCoord2f(0.f, 0.f);         glVertex3d(x1, y0, z0);
    glTexCoord2f((float)repeatX, 0.f); glVertex3d(x0, y0, z0);
    glTexCoord2f((float)repeatX, (float)repeatY); glVertex3d(x0, y1, z0);
    glTexCoord2f(0.f, (float)repeatY); glVertex3d(x1, y1, z0);

    // Left (-X)
    glNormal3f(-1.f, 0.f, 0.f);
    glTexCoord2f(0.f, 0.f);         glVertex3d(x0, y0, z0);
    glTexCoord2f((float)repeatX, 0.f); glVertex3d(x0, y0, z1);
    glTexCoord2f((float)repeatX, (float)repeatY); glVertex3d(x0, y1, z1);
    glTexCoord2f(0.f, (float)repeatY); glVertex3d(x0, y1, z0);

    // Right (+X)
    glNormal3f(1.f, 0.f, 0.f);
    glTexCoord2f(0.f, 0.f);         glVertex3d(x1, y0, z1);
    glTexCoord2f((float)repeatX, 0.f); glVertex3d(x1, y0, z0);
    glTexCoord2f((float)repeatX, (float)repeatY); glVertex3d(x1, y1, z0);
    glTexCoord2f(0.f, (float)repeatY); glVertex3d(x1, y1, z1);

    // Bottom (-Y)
    glNormal3f(0.f, -1.f, 0.f);
    glTexCoord2f(0.f, 0.f);         glVertex3d(x0, y0, z1);
    glTexCoord2f((float)repeatX, 0.f); glVertex3d(x1, y0, z1);
    glTexCoord2f((float)repeatX, (float)repeatY); glVertex3d(x1, y0, z0);
    glTexCoord2f(0.f, (float)repeatY); glVertex3d(x0, y0, z0);

    // Top (+Y)
    glNormal3f(0.f, 1.f, 0.f);
    glTexCoord2f(0.f, 0.f);         glVertex3d(x0, y1, z0);
    glTexCoord2f((float)repeatX, 0.f); glVertex3d(x1, y1, z0);
    glTexCoord2f((float)repeatX, (float)repeatY); glVertex3d(x1, y1, z1);
    glTexCoord2f(0.f, (float)repeatY); glVertex3d(x0, y1, z1);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Cube::drawGlassCube(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);

    glColor4f(red, green, blue, alpha);

    GLfloat spec[] = { 0.9f, 0.9f, 1.0f, 1.0f };
    GLfloat shine[] = { 90.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

    draw();

    glPopAttrib(); 

}
void Cube::drawWithTextureNoTopBottom(GLuint textureID, int repeatX, int repeatY)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Front face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
    glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
    glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
    glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
    glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    glEnd();

    // Left face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
    glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glTexCoord2f(repeatX, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    glEnd();

    // Right face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
    glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
    glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
    glTexCoord2f(0.0f, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    glEnd();

    // Draw top face (always untextured)
    glBegin(GL_QUADS);
    glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    glEnd();

    // Draw bottom face (always untextured)
    glBegin(GL_QUADS);
    glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Cube::drawTranslated(float dx, float dy, float dz,GLfloat red =1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f) const
{
    glPushMatrix();
    glTranslatef(dx, dy, dz);   
    const_cast<Cube*>(this)->drawGlassCube(red,green,blue,alpha);
    glPopMatrix();              
}

void Cube::drawWithTextureTranslated(GLuint tex, int rx, int ry, float dx, float dy, float dz) const
{
    glPushMatrix();
    glTranslatef(dx, dy, dz);
    const_cast<Cube*>(this)->drawWithTexture(tex, rx, ry);
    glPopMatrix();
}




void Cube::drawWithTextureOnOneFace(GLuint textureID, const std::string& face, int repeatX, int repeatY)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw front face
    glBegin(GL_QUADS);
    if (face == "front")
    {
        // Apply texture to the front face
        glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
        glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
        glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
        glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    }
    else
    {
        // Front face without texture
        glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
        glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
        glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
        glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    }
    glEnd();

    // Draw back face
    glBegin(GL_QUADS);
    if (face == "back")
    {
        // Apply texture to the back face
        glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
        glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
        glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
        glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    }
    else
    {
        // Back face without texture
        glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
        glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
        glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
        glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    }
    glEnd();

    // Draw left face
    glBegin(GL_QUADS);
    if (face == "left")
    {
        // Apply texture to the left face
        glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
        glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
        glTexCoord2f(repeatX, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
        glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    }
    else
    {
        // Left face without texture
        glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
        glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
        glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
        glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    }
    glEnd();

    // Draw right face
    glBegin(GL_QUADS);
    if (face == "right")
    {
        // Apply texture to the right face
        glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
        glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
        glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
        glTexCoord2f(0.0f, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    }
    else
    {
        // Right face without texture
        glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
        glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
        glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
        glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    }
    glEnd();

    // Draw top face (always untextured)
    glBegin(GL_QUADS);
    glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    glEnd();

    // Draw bottom face (always untextured)
    glBegin(GL_QUADS);
    glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
    glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
    glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}