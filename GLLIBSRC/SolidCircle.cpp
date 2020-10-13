#include <math.h>

#include "SolidCircle.h"


#define c_MXNFAI 64
#define DEFAUT_NUMFAI   12
static float cosfai[c_MXNFAI], sinfai[c_MXNFAI];
int SolidCircle::fai=0;

void SolidCircle::Set(int Nf)
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


SolidCircle::~SolidCircle()
{
    if(list> 0) glDeleteLists(list,1);
}

SolidCircle::SolidCircle()
{
	x = 0.f;
	y = 0.f;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
    if(SolidCircle::fai <= 0) Set(DEFAUT_NUMFAI);	
    list = 0;

}

SolidCircle::SolidCircle(float radiu)
{
	x = 0.f;
	y = 0.f;
	size = radiu;
    R = 1.f;
	G = 1.f;
	B = 1.f;
    if(SolidCircle::fai <= 0) Set(DEFAUT_NUMFAI);	
    list = 0;
}


SolidCircle::SolidCircle(float X, float Y)
{
	x = X;
	y = Y;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
    if(SolidCircle::fai <= 0) Set(DEFAUT_NUMFAI);	
    list = 0;
}

SolidCircle::SolidCircle(float radiu, float X, float Y)
{
	x = X;
	y = Y;
	size = radiu;
    R = 1.f;
	G = 1.f;
	B = 1.f;
    if(SolidCircle::fai <= 0) Set(DEFAUT_NUMFAI);	
    list = 0;
}


SolidCircle::SolidCircle(float radiu, float X, float Y, float Red, float Green, float Blue)
{
	x = X;
	y = Y;
	size = radiu;
    R = Red;
	G = Green;
	B = Blue;
    if(SolidCircle::fai <= 0) Set(DEFAUT_NUMFAI);	
    list = 0;
}


void SolidCircle::Paint()
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
	  glEnable(GL_COLOR_MATERIAL);
      glColor4f(R,G,B,A);
	  glBegin(GL_TRIANGLE_FAN);
         glVertex2d(0.f, 0.f);
	     for(i=0; i<fai; i++) glVertex2d(cosfai[i], sinfai[i]);
      glEnd();

	glPopMatrix();
	return;
}

int SolidCircle::MakeList()
{
   glNewList(list = glGenLists(1), GL_COMPILE);
   int i;

	  //glEnable(GL_COLOR_MATERIAL);
      //glColor4f(R,G,B,A);
	  glBegin(GL_TRIANGLE_FAN);
         glVertex2d(0.f, 0.f);
	     for(i=0; i<fai; i++) glVertex2d(cosfai[i], sinfai[i]);
      glEnd();

	glEndList();
	return list;
}





