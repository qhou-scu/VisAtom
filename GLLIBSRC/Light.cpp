#include <math.h>

#include "Light.h"


#define labelLight GL_LIGHT0+(n-1)

int Light::number=0;

Light::~Light()
{
	if(number > 0) number--;
	glDisable(n);
}

Light::Light()
{
	A_light[0] = 0.2f;	
	A_light[1] = 0.2f;	
	A_light[2] = 0.2f;	
	A_light[3] = 1.f;

	D_light[0] = 1.f;	
	D_light[1] = 1.f;	
	D_light[2] = 1.f;	
	D_light[3] = 1.f;

	S_light[0] = 1.f;	
	S_light[1] = 1.f;	
	S_light[2] = 1.f;	
	S_light[3] = 1.f;

	SPOT_light[0] = 1.f;	
	SPOT_light[1] = 1.f;	
	SPOT_light[2] = 1.f;	
	SPOT_light[3] = 1.f;

	Pos[0]  = 1.f;
	Pos[1]  = 1.f;
	Pos[2]  = 1.f;
	Pos[3]  = 0.f;
    
    number ++;	
	n = number;
	if(n > 8) n = 8;
	n = labelLight;
	 

}


Light::Light(int I)
{
	A_light[0] = 0.2f;	
	A_light[1] = 0.2f;	
	A_light[2] = 0.2f;	
	A_light[3] = 1.f;

	D_light[0] = 1.f;	
	D_light[1] = 1.f;	
	D_light[2] = 1.f;	
	D_light[3] = 1.f;

	S_light[0] = 1.f;	
	S_light[1] = 1.f;	
	S_light[2] = 1.f;	
	S_light[3] = 1.f;

	SPOT_light[0] = 1.f;	
	SPOT_light[1] = 1.f;	
	SPOT_light[2] = 1.f;	
	SPOT_light[3] = 1.f;

	Pos[0]  = 1.f;
	Pos[1]  = 1.f;
	Pos[2]  = 1.f;
	Pos[3]  = 0.f;
    
   	n = I;
	n = labelLight;

}


void Light::ChangeColorA(float Red, float Green, float Blue)
{
	A_light[0] = Red;	
	A_light[1] = Green;	
	A_light[2] = Blue;	
	A_light[3] = 1.f;
	glLightfv(n, GL_AMBIENT,A_light);
}

void Light::ChangeColorD(float Red, float Green, float Blue)
{
	D_light[0] = Red;	
	D_light[1] = Green;	
	D_light[2] = Blue;	
	D_light[3] = 1.f;
	glLightfv(n, GL_DIFFUSE,D_light);
}

void Light::ChangeColorS(float Red, float Green, float Blue)
{
	S_light[0] = Red;	
	S_light[1] = Green;	
	S_light[2] = Blue;	
	S_light[3] = 1.f;
	glLightfv(n, GL_SPECULAR,S_light);
}


void Light::ChangeColorA(float color[3])
{
	A_light[0] = color[0];	
	A_light[1] = color[1];	
	A_light[2] = color[2];	
	A_light[3] = 1.f;
	glLightfv(n, GL_AMBIENT,A_light);

}

void Light::ChangeColorD(float color[3])
{
	D_light[0] = color[0];	
	D_light[1] = color[1];	
	D_light[2] = color[2];	
	D_light[3] = 1.f;
	glLightfv(n, GL_DIFFUSE,D_light);

}

void Light::ChangeColorS(float color[3])
{
	S_light[0] = color[0];	
	S_light[1] = color[1];	
	S_light[2] = color[2];	
	S_light[3] = 1.f;
	glLightfv(n, GL_SPECULAR,S_light);

}



void Light::ChangePosition(float x, float y, float z)
{
	Pos[0]  = x;
	Pos[1]  = y;
	Pos[2]  = z;
    glLightfv(n, GL_POSITION, Pos);
}

void Light::ChangePosition(float x, float y, float z, float a)
{
	Pos[0]  = x;
	Pos[1]  = y;
	Pos[2]  = z;
	Pos[3] =  a;
    glLightfv(n, GL_POSITION, Pos);

}


void Light::Turnon()
{

	glEnable(GL_LIGHTING);
	glLightfv(n, GL_AMBIENT,A_light);
	glLightfv(n, GL_DIFFUSE,D_light);
	glLightfv(n, GL_SPECULAR,S_light);
    glLightfv(n, GL_POSITION, Pos);
    glLightfv(n, GL_SPOT_DIRECTION, SPOT_light);
	glEnable(n);
	return;
}

void Light::Turnoff()
{

    glDisable(n);
}

void Light::KeepMove()
{
    glLightfv(n, GL_POSITION, Pos);
	return;
}