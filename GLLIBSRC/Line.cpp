#include "math.h"
#include "Line.h"


Line::Line()
{
	Startx=0.f;
	Starty=0.f;
	Startz=0.f;
	Endx=100.f;
	Endy=100.f;
	Endz=100.f;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;
}

Line::Line(float x0, float y0, float x1, float y1)
{
	Startx=x0;
	Starty=y0;
	Startz=0.f;
	Endx=x1;
	Endy=y1;
	Endz=0.f;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;
}


Line::Line(float x0, float y0, float z0, float x1, float y1, float z1)
{
	Startx=x0;
	Starty=y0;
	Startz=z0;
	Endx=x1;
	Endy=y1;
	Endz=z1;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;
}

void Line::Paint()
{
	//glLineSize(size);
	glColor4f(R,G,B,A);
	glBegin(GL_LINES);
	       glVertex3f(Startx, Starty, Startz);
	       glVertex3f(Endx, Endy, Endz);
    glEnd();
//	glFlush();
	return;
}


void Line::Translate(float Newx, float Newy, float Newz)
{

	Startx += Newx;
	Starty += Newy;
	Startz += Newz;
	Endx += Newx;
	Endy += Newy;
	Endz += Newz;
	return;
}

void Line::MoveTo(float Newx, float Newy, float Newz)
{
   float dx, dy, dz;
   
    dx = Newx - Startx;
    dy = Newy - Starty;
    dz = Newz - Startz;
	Startx = Newx;
	Starty = Newy;
	Startz = Newz;
	Endx += dx;
	Endy += dy;
	Endz += dz;
	return;
}

void Line::GetStartPoint(float*x, float*y, float*z)
{

	(*x) = Startx;
	(*y) = Starty;
	(*z) = Startz;
	return;
}


void Line::ResetStartPoint(float x, float y, float z)
{

	Startx = x;
	Starty = y;
	Startz = z;
	return;
}

void Line::ResetStartPoint(float v[3])
{

	Startx = v[0];
	Starty = v[1];
	Startz = v[2];
	return;
}

void Line::GetEndPoint(float*x, float*y, float*z)
{

	(*x) = Endx;
	(*y) = Endy;
	(*z) = Endz;
	return;
}

void Line::ResetEndPoint(float x, float y, float z)
{

	Endx = x;
	Endy = y;
	Endz = z;
	return;
}

void Line::ResetEndPoint(float v[3])
{

	Endx = v[0];
	Endy = v[1];
	Endz = v[2];
	return;
}


void Line::ChangeColor(float NewR, float NewG, float NewB)
{ 

	R = NewR;
	G = NewG;
	B = NewB;
	return;
}

void Line::ChangeColor(float NewR, float NewG, float NewB, float NewA)
{ 

	R = NewR;
	G = NewG;
	B = NewB;
	A = NewA;
	return;
}

void Line::ChangeColor(float Color[3])
{

	R = Color[0];
	G = Color[1];
	B = Color[2];
	return;
}

void Line::ChangeSize(float NewSize)
{

	size = NewSize;
	return;
}

float Line::GetLen()
{

    float dx = Endx - Startx;
    float dy = Endy - Starty;
    float dz = Endz - Startz;

	return (float)sqrt(dx*dx+dy*dy+dz*dz);
}



