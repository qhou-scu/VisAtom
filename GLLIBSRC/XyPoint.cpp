#include "xyPoint.h"

xyPoint::xyPoint()
{
	x=0.f;
	y=0.f;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;
}


xyPoint::xyPoint(float x0, float y0)
{
	x=x0;
	y=y0;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	A = 1.f;

}


xyPoint::xyPoint(float x0, float y0, float r, float g, float b)
{
	x=x0;
	y=y0;
	size = 1.f;
    R = r;
	G = g;
	B = b;
	A = 1.f;
}

void xyPoint::Paint()
{
    glEnable(GL_POINT_SMOOTH);
	glPointSize(size);
	glColor4f(R,G,B,A);
	glBegin(GL_POINTS);
	       glVertex2f(x, y);
    glEnd();
//	glFlush();
	return;
}

int xyPoint::MakeList()
{
    return 0;
}

void xyPoint::Moveto(float Newx, float Newy)
{

	x = Newx;
	y = Newy;
	return;
}

void xyPoint::ChangeColor(float NewR, float NewG, float NewB)
{

	R = NewR;
	G = NewG;
	B = NewB;
	return;
}

void xyPoint::GetColor(float*oldR, float*oldG, float*oldB)
{

	*oldR = R;
	*oldG = G;
	*oldB = B;
	return;
}

void xyPoint::Copy(xyPoint*target)
{

	if(target == NULL) return;
	  target->size = size;
	  target->R    = R;
	  target->G    = G;
	  target->B    = B;
      target->x    = x;
	  target->y    = y;

	return;
}

void xyPoint::Paste(xyPoint*source)
{

    if(source == NULL ) return;
	  size = source->size;
	  R    = source->R;
	  G    = source->G;
	  B    = source->B;
	  x    = source->x;
	  y    = source->y;

	return;
}


