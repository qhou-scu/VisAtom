//#include <windows.h>	// Standard windows include
//#include <gl\gl.h>		// OpenGL library



#ifndef _SPHERE
#define _SPHERE
#include "Point.h"
#include "Texture.h"
class Sphere: public Point
{
private:
	  static int fai,theta; 
	  int list;
public:
	Sphere();
	Sphere(float);
	Sphere(float*);
	Sphere(float,float,float);
	Sphere(float,float*);
	Sphere(float,float,float,float);
	Sphere(float,float*,float*);
	Sphere(float,float,float,float,float*);
	Sphere(float,float,float,float,float,float, float);
	~Sphere();
	static void Set(int, int);
	virtual void Paint();
	virtual void Paint(Texture*);
	virtual int  MakeList();

};

#endif