#ifndef _ARROWLINE
#define _ARROWLINE
#include "Line.h"

class Arrowline:public Line
{
  private:
	   float m_ratio;
  protected:
	   int m_nfai;
	   float m_coneR, m_coneL, m_fullL;
	   float*m_cosfai,   *m_sinfai;
	   float*m_conetx,   *m_conety;

   public:
	   ~Arrowline();
	    Arrowline();
	    Arrowline(float,float,float,float,float,float);
        void PaintVertex();
		virtual void Paint();
		void Set(int nfai);
	    void ResetStartPoint(float, float, float);
	    void ResetEndPoint(float, float, float);
	    void ChangeColor(float, float, float);
	    void ChangeColor(float*);
	    void ChangeSize(float);
};


inline void Arrowline::ResetStartPoint(float x, float y, float z)
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

inline void Arrowline::ResetEndPoint(float x, float y, float z)
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


inline void Arrowline::ChangeSize(float NewSize)
{

	if(m_fullL == 0.f) return;

	m_coneR  = size = NewSize;
	m_coneL   = m_ratio*m_coneR;

	for(int i=0; i<m_nfai; i++)
	{
	    m_conetx[i]   = m_coneR*m_cosfai[i];
		m_conety[i]   = m_coneR*m_sinfai[i];
	}
	return;
}


inline void Arrowline::ChangeColor(float NewR, float NewG, float NewB)
{

	R = NewR;
	G = NewG;
	B = NewB;
	return;
}

inline void Arrowline::ChangeColor(float Color[3])
{

	R = Color[0];
	G = Color[1];
	B = Color[2];
	return;
}


#endif