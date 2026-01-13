#pragma once
#include <Point.h>
#include <GL/glut.h>
#include <Cube.h>
#include "AABB.h"



class Building {
	public:
		Building(Point center, double height, double length, double width);
		void draw();
		Point center;

		double height, length, width;
		double innerWallLength = length / 2;
		double wallThickness = 2.0;
		double innerThickness = 2.0;


		Cube leftWall, rightWall, behindWall, roof, floor,innerWall1, innerWall2;
		void Init();

		GLuint texture = 0;
	private:

};