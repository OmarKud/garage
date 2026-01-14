<<<<<<< HEAD
#include "Cube.h"
#include <GL/gl.h>  
#include <AABB.h>
=======
﻿#include "Cube.h"
#include <GL/gl.h>  
>>>>>>> BETA
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

void Cube::drawWithTexture(GLuint textureID, int repeatX, int repeatY) {
    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw front and back faces (GL_QUADS)
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

    // Draw left and right faces
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

    // Draw top and bottom faces
    // Bottom face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y, center.z + length / 2);
    glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y, center.z + length / 2);
    glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y, center.z - length / 2);
    glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y, center.z - length / 2);
    glEnd();

    // Top face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3d(center.x - width / 2, center.y + height, center.z + length / 2);
    glTexCoord2f(repeatX, 0.0f); glVertex3d(center.x + width / 2, center.y + height, center.z + length / 2);
    glTexCoord2f(repeatX, repeatY); glVertex3d(center.x + width / 2, center.y + height, center.z - length / 2);
    glTexCoord2f(0.0f, repeatY); glVertex3d(center.x - width / 2, center.y + height, center.z - length / 2);
    glEnd();

    // Disable texturing
    glDisable(GL_TEXTURE_2D);
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