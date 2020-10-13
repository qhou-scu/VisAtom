#include <windows.h>	// Standard windows include
#include <gl\gl.h>		// OpenGL library
#include "xyPoint.h"
#include "Texture.h"

#ifndef _CIRCLE
#define _CIRCLE

class Circle: public xyPoint
{
private:
	  static int fai;
	  int list;
protected:
	  float borderThick;
public:
	Circle();
	Circle(float);
	Circle(float,float);
	Circle(float,float,float);
	Circle(float,float,float,float,float,float);
	~Circle();
	static void Set(int);
	virtual void Paint();
	virtual int  MakeList();
	int  GetList(){return list;};
//**
	void SetBorderThick(float thick){borderThick = thick;};
	float GetBorderThick(){return borderThick;};
};
#endif