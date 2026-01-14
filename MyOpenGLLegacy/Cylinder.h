#pragma once
#include "Point.h"
class Cylinder
{
public:
	void drawWithTexture(Point center, float baseRadius, float topRadius, float height, int slices, int stacks, GLint textureId);
	void draw(Point center, float baseRadius, float topRadius, float height, int slices, int stacks);
};

