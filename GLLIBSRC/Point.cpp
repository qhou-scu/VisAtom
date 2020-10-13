#include "Point.h"

Point::Point()
{
	x=0.f;
	y=0.f;
	z=0.f;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;

}


Point::Point(float x0, float y0)
{
	x=x0;
	y=y0;
	z=0.f;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;

}

Point::Point(float x0, float y0, float z0)
{
	x=x0;
	y=y0;
	z=z0;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;
}

Point::Point(float x0, float y0, float z0, float r, float g, float b)
{
	x=x0;
	y=y0;
	z=z0;
	size = 1.f;
    R = r;
	G = g;
	B = b;
	A = 1.f;
}

void Point::Paint()
{
    glEnable(GL_POINT_SMOOTH);
	glPointSize(size);
	glColor4f(R,G,B,A);
	glBegin(GL_POINTS);
	       glVertex3f(x, y, z);
    glEnd();
//	glFlush();
	return;
}


 

