#pragma once
#include <Point.h>
#include <GL/glut.h>
#include <Cube.h>


class Building {
	public:
		Building(Point center, double height, double length, double width);
		void draw();
		Point center;
		double height, length, width;
		Cube leftWall, rightWall, behindWall, roof, floor;
		void Init();
		GLuint texture = 0;
	private:

};