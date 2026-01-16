#pragma once
#include "Point.h"
class Cylinder
{
public:
	Cylinder(Point center, float baseRadius, float topRadius, float height, int slices, int stacks);
	void drawWithTexture(GLint textureId);
	void draw();

private:
	Point center;
	float baseRadius, topRadius, height;
	int slices, stacks;
};

