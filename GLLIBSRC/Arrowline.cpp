
#include <math.h>
#include "Arrowline.h"

#define DEFAUT_NUMFAI      8
#define DEFAUT_CONERAD     5
#define DEFAUT_CONERATIO   2.5
#define DEFAUT_R_R_Ratio   0.4
void Arrowline::Set(int nfai)
{
	int i;
	double ang,step;

	if( m_nfai == nfai)  return;
	  m_nfai = nfai;

	  if(m_sinfai != NULL)   delete m_sinfai;   m_sinfai = NULL;
	  if(m_cosfai != NULL)   delete m_cosfai;   m_cosfai = NULL;
	  if(m_conetx != NULL)   delete m_conetx;   m_conetx = NULL;
	  if(m_conety != NULL)   delete m_conety;   m_conety = NULL;

	  m_sinfai   = new float[m_nfai];
	  m_cosfai   = new float[m_nfai];
	  m_conetx   = new float[m_nfai];
	  m_conety   = new float[m_nfai];

	  step = (2.*3.14159265359)/ (double) m_nfai;
	  for(i=0; i<m_nfai; i++)
	  {
		 ang = step *(double) i;
		 m_cosfai[i] = cos(ang);
		 m_sinfai[i] = sin(ang);
	  }

	  for(i=0; i<m_nfai; i++)
	  {
		 m_conetx[i]   = m_coneR*m_cosfai[i];
		 m_conety[i]   = m_coneR*m_sinfai[i];
	  }
    return;
}


Arrowline::~Arrowline()
{
	if(m_cosfai != NULL)   delete m_cosfai;   m_cosfai = NULL;
	if(m_sinfai != NULL)   delete m_sinfai;   m_sinfai = NULL;
	if(m_conetx != NULL)   delete m_conetx;   m_conetx = NULL;
	if(m_conety != NULL)   delete m_conety;   m_conety = NULL;
}

Arrowline::Arrowline():Line()
{
	m_ratio = DEFAUT_CONERATIO;
	m_coneR = size = DEFAUT_CONERAD;
    m_coneL = m_ratio*m_coneR;

	m_nfai = 0;
	m_sinfai   = NULL;
	m_cosfai   = NULL;
	m_conetx   = NULL;
	m_conety   = NULL;

	Set(DEFAUT_NUMFAI);
    ResetStartPoint(Startx, Starty, Startz);
    ResetEndPoint(Endx, Endy, Endz);
}

Arrowline::Arrowline(float x0, float y0, float z0, float x1, float y1, float z1):Line(x0,y0,z0,x1,y1,z1)
{
	m_ratio = DEFAUT_CONERATIO;
	m_coneR = size = DEFAUT_CONERAD;
    m_coneL = m_ratio*m_coneR;

	m_nfai = 0;
	m_sinfai   = NULL;
	m_cosfai   = NULL;
	m_conetx   = NULL;
	m_conety   = NULL;

	Set(DEFAUT_NUMFAI);
	ResetStartPoint(Startx, Starty, Startz);
    ResetEndPoint(Endx, Endy, Endz);
}


#define PI_DEGREE 57.2957795 

void Arrowline::PaintVertex()
{
	if(m_fullL <= 0.f) return;

	int i;
	double len,dx,dy,dz,ang;

	dx = (Endx-Startx);
	dy = (Endy-Starty);
	dz = (Endz-Startz);
	ang = acos(dz/m_fullL)*PI_DEGREE;

	glPushMatrix();
	glTranslatef(Startx, Starty, Startz);
	glRotatef((float)ang, -(float)dy, (float)dx, 0.f);

	float ratio = m_coneL/m_fullL;
	
	if(ratio<1.f)
	{

	 //Then draw the center line
	   glBegin(GL_LINES);
		  glVertex3f(0.f, 0.f, m_fullL);
		  glVertex3f(0.f, 0.f, 0.f);

      glEnd();
	 //Then draw the cone 
	   glTranslatef(0.f, 0.f, m_fullL-m_coneL);
       glBegin(GL_LINES);
	  	 for(i=0; i<m_nfai; i++)
		 {
		     glVertex3f(0.f, 0.f, m_coneL);
			 glVertex3f(m_conetx[i], m_conety[i], 0.0);
		 }
       glEnd();
	}
	else
	{
      ratio = 1.f/ratio;
	  // draw the cone bottom 
	   glBegin(GL_LINES);
	  	 for(i=0; i<m_nfai; i++)
		 {
		     glVertex3f(0.f, 0.f, m_coneL*ratio);
			 glVertex3f(m_conetx[i]*ratio, m_conety[i]*ratio, 0.f);
		 }
       glEnd();
	}
	glPopMatrix();
	return;
}

void Arrowline::Paint()
{
	glDisable(GL_LIGHTING);
    glColor4f(R,G,B,A);
	PaintVertex();
	glEnable(GL_LIGHTING);
	return;
}


