//#include <windows.h>	// Standard windows include
//#include <gl\gl.h>		// OpenGL library
#include "GlcObj.h"

#ifndef _POINT
#define _POINT

class Point:public GlcObj
{
protected:
      GLfloat x,y,z;
	  GLfloat size;
	  GLfloat R,G,B,A;
public:
	Point();
	Point(float,float);
	Point(float,float,float);
	Point(float,float,float,float,float,float);
	virtual ~Point(){};
	virtual void Paint();
	virtual int  GetList(){return 0;};


	void ChangeAlpha(float NewA){A=NewA;};
	float GetAlpha(){ return A;};
	void ChangeSize(float NewSize){size = NewSize;};

    void Moveto(float Newx, float Newy, float Newz) { x = Newx; y = Newy; z = Newz; return;}
    void MoveTo(float Newx, float Newy, float Newz) {x = Newx; y = Newy; z = Newz; return;}
	void Moveto(float Newv[3]) { x = Newv[0]; y = Newv[1]; z = Newv[2]; return;}
    void Translate(float dx, float dy, float dz) { x += dx; y += dy; z += dz; return;}
    void Translate(float dr[3]) { x += dr[0]; y += dr[1]; z += dr[2]; return;}
    void GetPosition(float*x0, float*y0, float*z0) { *x0 = x; *y0 = y; *z0 = z; return;}
	void ChangeColor(float NewR, float NewG, float NewB) {R = NewR; G = NewG; B = NewB; return; }
    void ChangeColor(float Color[3]) {R = Color[0]; G = Color[1]; B = Color[2]; return;}
    void InvertColor() { R = 1-R; G = 1-G; B = 1-B; return;}
	void GetColor(float*oldR, float*oldG, float*oldB) {	*oldR = R; 	*oldG = G;  *oldB = B; 	return;}

};

#endif