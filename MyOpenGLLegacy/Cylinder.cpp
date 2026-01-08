#include <gl/glut.h>
#include "Cylinder.h"

void Cylinder::drawWithTexture(Point center, float baseRadius, float topRadius, float height, int slices, int stacks, GLint textureId) {
	GLUquadric* quad = gluNewQuadric();

	glPushMatrix();
	glTranslated(center.x, center.y, center.z);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluQuadricTexture(quad, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
	gluDisk(quad, 0, baseRadius, slices, slices); // ????? ????????? ??????
	glPushMatrix();
	glTranslatef(0, 0, height);
	gluDisk(quad, 0, topRadius, slices, slices); // ????? ????????? ??????
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	gluDeleteQuadric(quad);
}

void Cylinder::draw(Point center, float baseRadius, float topRadius, float height, int slices, int stacks) {
	GLUquadric* quad = gluNewQuadric();

	glPushMatrix();
	glTranslated(center.x, center.y, center.z);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
	gluDisk(quad, 0, baseRadius, slices, slices); // ????? ????????? ??????
	glPushMatrix();
	glTranslatef(0, 0, height);
	gluDisk(quad, 0, topRadius, slices, slices); // ????? ????????? ??????
	glPopMatrix();
	glPopMatrix();
	gluDeleteQuadric(quad);
}


