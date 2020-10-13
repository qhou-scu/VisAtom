//#include <windows.h>	// Standard windows include
//#include <gl\gl.h>		// OpenGL library

#ifndef _GLCOBJ
#define _GLCOBJ
#include <QtOpenGL>

class GlcObj
{
protected:
public:
	GlcObj(){};
	virtual ~GlcObj(){};
	virtual void Paint(){};
    virtual void Move(float, float, float){};
    virtual void Translate(float, float, float){};
	virtual void ChangeColor(float, float, float){};
	virtual float MinX(){ return 0.f;};
	virtual float MinY(){ return 0.f;};
	virtual float MinZ(){ return 0.f;};
	virtual float MaxX(){ return 0.f;};
	virtual float MaxY(){ return 0.f;};
	virtual float MaxZ(){ return 0.f;};
	virtual void  SetXscal(float xscal){};
	virtual void  SetYscal(float yscal){};
	virtual void  SetZscal(float zscal){};
	virtual void  SetStyle(int NewStyle){};
	virtual int  GetStyle(){ return 0;};
};

#endif
