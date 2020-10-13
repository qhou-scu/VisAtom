#include <windows.h>	// Standard windows include
#include <gl\gl.h>		// OpenGL library
#include "GlcObj.h"

#ifndef _XYPOINT
#define _XYPOINT

class xyPoint:public GlcObj
{
protected:
      GLfloat x,y;
	  GLfloat size;
	  GLfloat R,G,B,A;
public:
	xyPoint();
	xyPoint(float,float);
	xyPoint(float,float,float,float,float);
	virtual ~xyPoint(){};

	virtual void Copy(xyPoint*);
	virtual void Paste(xyPoint*);

	virtual void Paint();
	virtual int MakeList();
	virtual int  GetList(){return 0;};
    virtual void Moveto(float, float);
	void ChangeColor(float, float, float);
	void GetColor(float*, float*, float*);
	void ChangeAlpha(float NewA){A = NewA;};
	float GetAlpha(){ return A;};
	virtual void ChangeSize(float NewSize){size = NewSize;};
	virtual float GetSize(){return size;};

	virtual void SetBorderThick(float thick){};
	virtual void SetBarHight(float h){};
	virtual void SetBarWidth(float w){};
	virtual void SetBarCapW (float w){};
	virtual void SetBarCapH (float h){};
	virtual float GetBorderThick(){return 0.f;};
	virtual float GetBarHight(){return 0.f;};
	virtual float GetBarWidth(){return 0.f;};
	virtual float GetBarCapW (){return 0.f;};
	virtual float GetBarCapH (){return 0.f;};

};

#endif