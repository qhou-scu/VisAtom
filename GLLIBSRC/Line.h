
#include "GlcObj.h"

#ifndef _LINE
#define _LINE

class Line:public GlcObj
{ 
protected:
      GLfloat Startx,Starty,Startz;
      GLfloat Endx,Endy,Endz;
	  GLfloat size;
	  GLfloat R,G,B,A;
public:
	Line();
	Line(float,float,float,float);
	Line(float,float,float,float,float,float);
	virtual ~Line(){};
	virtual void Paint();
    void Translate(float, float, float);
    void MoveTo(float, float, float);
	void ChangeColor(float, float, float);
	void ChangeColor(float, float, float, float);
	void ChangeColor(float*);
	void ChangeColorA(float a) {A=a;};
	float GetColorA(){return A;};
	void ChangeTransparency(float t) {A=t;};
	float GetTransparency() {return A;};
	void ChangeSize(float);
	void GetStartPoint(float*, float*, float*);
	void GetEndPoint(float*, float*, float*);
	void ResetStartPoint(float, float, float);
	void ResetStartPoint(float*);
	void ResetEndPoint(float, float, float);
	void ResetEndPoint(float*);
	float GetLen();
    float GetLenX(){return Endx-Startx;};
    float GetLenY(){return Endy-Starty;};
    float GetLenZ(){return Endz-Startz;};
    float GetSize(){return size;};

};

#endif