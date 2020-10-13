#include <windows.h>	// Standard windows include
#include <gl\gl.h>		// OpenGL library
#include "xyPoint.h"
#include "Texture.h"

#ifndef _SOLIDCIRCLE
#define _SOLIDCIRCLE

class SolidCircle: public xyPoint
{
private:
	  static int fai;
	  int list;
protected:
public:
	SolidCircle();
	SolidCircle(float);
	SolidCircle(float,float);
	SolidCircle(float,float,float);
	SolidCircle(float,float,float,float,float,float);
	~SolidCircle();
	static void Set(int);
	virtual void Paint();
	virtual int MakeList();
	int  GetList(){return list;};
};

#endif