#pragma once
#include "Point.h"
class Pyramid
{
public:
	Pyramid(Point center, unsigned int type, double hight, double base_angle);
	void draw();
	unsigned int type;
	double hight, base_angle;
	Point center;
private:
	void draw_part(double hight);
};