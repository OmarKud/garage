#pragma once
<<<<<<< HEAD
#include <Point.h>
#include <GL/glut.h>
#include <Cube.h>
#include "AABB.h"
#include <LuxuryRoom.h>



class Building {
	
	public:
		Building(Point center, double height, double length, double width);
		void draw();
		Point center;

		double height, length, width;
		double innerWallLength = length / 2;
		double wallThickness = 2.0;
		double innerThickness = 2.0;
		LuxuryRoom luxuryRoom;

		Cube leftWall, rightWall, behindWall, roof, floor,innerWall1, innerWall2;
		void Init();

		GLuint texture = 0;
	private:
		

};
=======
#include <GL/glut.h>
#include "Point.h"
#include "Cube.h"
#include "FrontWall.h"

class Building {
public:
    Building(Point center, double height, double length, double width);
    void Init();
    void draw();

private:
    Point center;
    double height, length, width;

    Cube leftWall, rightWall, behindWall, roof, floor, innerWall1, innerWall2;
    FrontWall frontWall;

    GLuint floorTexture = 0;
    GLuint wallTexture = 0;
    GLuint roofTexture = 0;
};
>>>>>>> BETA
