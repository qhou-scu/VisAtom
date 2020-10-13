#include <math.h>
#include "Circle.h"
//
#define c_MXNFAI 64
#define DEFAUT_NUMFAI   12
static float cosfai[c_MXNFAI], sinfai[c_MXNFAI];

int Circle::fai=0;


void Circle::Set(int Nf)
{
	int i;
	double ang,step;

	if( fai == Nf) return;

	fai = Nf;
    if(fai   > c_MXNFAI)   fai   = c_MXNFAI;

	  step = (2.*3.14159265359)/ (double) (fai-1);
	  for(i=0; i<fai; i++)
	  {
		 ang = step *(double) i;
		 cosfai[i] = cos(ang);
		 sinfai[i] = sin(ang);
	  }
    return;
}


Circle::~Circle()
{
    if(list> 0) glDeleteLists(list,1);
}

Circle::Circle()
{
	x = 0.f;
	y = 0.f;
	size = 1.f;
	borderThick = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	if(Circle::fai <= 0) Set(DEFAUT_NUMFAI);
    list = 0;

}

Circle::Circle(float radiu)
{
	x = 0.f;
	y = 0.f;
	size = radiu;
	borderThick = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
   if(Circle::fai <= 0) Set(DEFAUT_NUMFAI);
    list = 0;
}


Circle::Circle(float X, float Y)
{
	x = X;
	y = Y;
	size = 1.f;
	borderThick = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
    if(Circle::fai <= 0) Set(DEFAUT_NUMFAI);
    list = 0;
}

Circle::Circle(float radiu, float X, float Y)
{
	x = X;
	y = Y;
	size = radiu;
	borderThick = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
    if(Circle::fai <= 0) Set(DEFAUT_NUMFAI);
    list = 0;
}


Circle::Circle(float radiu, float X, float Y, float Red, float Green, float Blue)
{
	x = X;
	y = Y;
	size = radiu;
	borderThick = 1.f;
    R = Red;
	G = Green;
	B = Blue;
    if(Circle::fai <= 0) Set(DEFAUT_NUMFAI);
    list = 0;
}


void Circle::Paint()
{
   int i;

	glPushMatrix();
	glTranslatef(x,y,0.f);

      if(list > 0)
	  {
        glScalef(size,size,size);
        glCallList(list);
	    glPopMatrix();
	    return;
	  }
      glEnable(GL_LINE_SMOOTH);
	  glEnable(GL_COLOR_MATERIAL);
      glColor4f(R,G,B,A);
      glLineWidth(borderThick); 
	  glBegin(GL_LINE_LOOP);
	     for(i=0; i<fai; i++) glVertex2d(size*cosfai[i], size*sinfai[i]);
      glEnd();
      glLineWidth(1.f); 
	glPopMatrix();
	return;
}

int Circle::MakeList()
{
    glNewList(list = glGenLists(1), GL_COMPILE);
    int i;

      glEnable(GL_LINE_SMOOTH);
      glLineWidth(borderThick); 
	  glBegin(GL_LINE_LOOP);
	     for(i=0; i<fai; i++) glVertex2d(cosfai[i], sinfai[i]);
      glEnd();
      glLineWidth(1.f); 
	glEndList();
	return list;
}





