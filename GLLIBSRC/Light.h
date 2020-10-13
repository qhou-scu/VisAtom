//#include <windows.h>	// Standard windows include
//#include <gl\gl.h>		// OpenGL library


#ifndef _LIGHT
#define _LIGHT
#include "GlcObj.h"
class Light: public GlcObj
{
private:
      GLfloat Pos[4];
	  GLfloat A_light[4];
      GLfloat D_light[4];
      GLfloat S_light[4];
      GLfloat SPOT_light[4];
      int n;
	  static int number;

public:
	Light();
	Light(int);
	Light(float,float, float,float,float,float);
	Light(float, float, float, float*);
	Light(float*, float,float, float);
	Light(float*, float*, float*);
	~Light();
	void Turnon();
	void Turnoff();
	void ChangeColorA(float, float, float);
	void ChangeColorA(float*);
	void ChangeColorD(float, float, float);
	void ChangeColorD(float*);
	void ChangeColorS(float, float, float);
	void ChangeColorS(float*);
	void ChangePosition(float, float, float);
	void ChangePosition(float, float, float, float);
	void KeepMove();

};

#endif
