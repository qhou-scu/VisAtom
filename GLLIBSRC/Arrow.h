#ifndef _ARROW
#define _ARROW
#include "Line.h"

class Arrow:public Line
{
	private:
	   float m_ratio;
     protected:
	   int m_nfai;
	   float m_cylindR, m_cylindL, m_coneR, m_coneL, m_fullL;
	   float*m_cosfai,   *m_sinfai;
	   float*m_cylindtx, *m_cylindty;
	   float*m_conetx,   *m_conety;

     public:
	    ~Arrow();
	    Arrow();
	    Arrow(float,float,float,float,float,float);
		virtual void Paint();
		void Set(int nfai);
	    void ResetStartPoint(float, float, float);
	    void ResetEndPoint(float, float, float);
	    void ChangeColor(float, float, float);
	    void ChangeColor(float*);
	    void ChangeSize(float);
};

inline void Arrow::ResetStartPoint(float x, float y, float z)
{
    float dx,dy,dz;
	Startx = x;
	Starty = y;
	Startz = z;

    dx = Endx - Startx;
    dy = Endy - Starty;
    dz = Endz - Startz;

	m_fullL = (float)sqrt(dx*dx + dy*dy + dz*dz);
	return;
}

inline void Arrow::ResetEndPoint(float x, float y, float z)
{

    float dx,dy,dz;
	Endx = x;
	Endy = y;
	Endz = z;
    dx = Endx - Startx;
    dy = Endy - Starty;
    dz = Endz - Startz;

	m_fullL = (float)sqrt(dx*dx + dy*dy + dz*dz);
	return;
}


inline void Arrow::ChangeColor(float NewR, float NewG, float NewB)
{

	R = NewR;
	G = NewG;
	B = NewB;
	return;
}

inline void Arrow::ChangeColor(float Color[3])
{

	R = Color[0];
	G = Color[1];
	B = Color[2];
	return;
}


#endif