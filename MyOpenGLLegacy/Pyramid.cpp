#include "Pyramid.h"
#include <GL/glut.h>
#include <bits/stdc++.h>
#define pi 3.141592653589793238462643383279502884197

Pyramid::Pyramid(Point center, unsigned int type, double hight, double base_angle)
{
	this->center = center;
	this->type = type;
	this->hight = hight;
	base_angle /= 2;
	this->base_angle = base_angle;
}

void Pyramid::draw()
{
	double degree = 360 / type;
	degree /= 2;
	glPushMatrix();
	glRotatef(degree, 0, 1, 0);
	this->draw_part(0);
	this->draw_part(hight);
	glPopMatrix();
}

void Pyramid::draw_part(double hight)
{
	double rad = 2 * pi / type;
	double x, y, z;
	y = center.y + hight;
	glBegin(GL_QUAD_STRIP);
	for (double i = 0; i <= 2 * pi; i += rad)
	{
		x = base_angle * cos(i) + center.x;
		z = base_angle * sin(i) + center.z;
		glVertex3d(x, center.y, z);
		glVertex3d(center.x, y, center.z);
	}
	glEnd();
}
