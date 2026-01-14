#pragma once
#include "Point.h"
#include <GL/glut.h>
#include <string>
#include "Texture.h"

class Cube
{
public:
	Cube(Point center, double height, double length, double width);
	void draw();
	void drawWithTexture(GLuint textureID, int repeatX, int repeatY);

	Point center;
	double height, length, width;
	void drawWithTextureNoTopBottom(GLuint textureID, int repeatX, int repeatY);
	void drawWithTextureOnOneFace(GLuint textureID, const std::string& face, int repeatX, int repeatY);

};