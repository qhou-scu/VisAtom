
#include <math.h>
#include "Arrow.h"

#define DEFAUT_NUMFAI      16
#define DEFAUT_CONERAD     5
#define DEFAUT_CONERATIO   2.5
#define DEFAUT_R_R_Ratio   0.4
void Arrow::Set(int nfai)
{
	int i;
	double ang,step;

	if( m_nfai == nfai)  return;
	  m_nfai = nfai;

	  if(m_sinfai != NULL)   delete m_sinfai;   m_sinfai = NULL;
	  if(m_cosfai != NULL)   delete m_cosfai;   m_cosfai = NULL;
	  if(m_conetx != NULL)   delete m_conetx;   m_conetx = NULL;
	  if(m_conety != NULL)   delete m_conety;   m_conety = NULL;
	  if(m_cylindtx != NULL) delete m_cylindtx; m_cylindtx = NULL;
	  if(m_cylindty != NULL) delete m_cylindty; m_cylindty = NULL;

	  m_sinfai   = new float[m_nfai];
	  m_cosfai   = new float[m_nfai];
	  m_conetx   = new float[m_nfai];
	  m_conety   = new float[m_nfai];
	  m_cylindtx = new float[m_nfai];
	  m_cylindty = new float[m_nfai];

	  step = (2.*3.14159265359)/ (double) (m_nfai-1);
	  for(i=0; i<m_nfai; i++)
	  {
		 ang = step *(double) i;
		 m_cosfai[i] = cos(ang);
		 m_sinfai[i] = sin(ang);
	  }
	  m_cosfai[m_nfai-1] = m_cosfai[0];
	  m_sinfai[m_nfai-1] = m_sinfai[0];

	  for(i=0; i<m_nfai; i++)
	  {
		 m_conetx[i]   = m_coneR*m_cosfai[i];
		 m_conety[i]   = m_coneR*m_sinfai[i];
		 m_cylindtx[i] = m_cylindR*m_cosfai[i];
		 m_cylindty[i] = m_cylindR*m_sinfai[i];
	  }
    return;
}


Arrow::~Arrow()
{
	if(m_cosfai != NULL)   delete m_cosfai;   m_cosfai = NULL;
	if(m_sinfai != NULL)   delete m_sinfai;   m_sinfai = NULL;
	if(m_conetx != NULL)   delete m_conetx;   m_conetx = NULL;
	if(m_conety != NULL)   delete m_conety;   m_conety = NULL;
	if(m_cylindtx != NULL) delete m_cylindtx; m_cylindtx = NULL;
	if(m_cylindty != NULL) delete m_cylindty; m_cylindty = NULL;
}

Arrow::Arrow():Line()
{
	m_ratio = DEFAUT_CONERATIO;
	m_coneR = size = DEFAUT_CONERAD;
    m_coneL = m_ratio*m_coneR;
	m_cylindR = m_coneR*DEFAUT_R_R_Ratio;

	m_nfai = 0;
	m_sinfai   = NULL;
	m_cosfai   = NULL;
	m_conetx   = NULL;
	m_conety   = NULL;
	m_cylindtx = NULL;
	m_cylindty = NULL;

	Set(DEFAUT_NUMFAI);
    ResetStartPoint(Startx, Starty, Startz);
    ResetEndPoint(Endx, Endy, Endz);
}

Arrow::Arrow(float x0, float y0, float z0, float x1, float y1, float z1):Line(x0,y0,z0,x1,y1,z1)
{
	m_ratio = DEFAUT_CONERATIO;
	m_coneR = size = DEFAUT_CONERAD;
    m_coneL = m_ratio*m_coneR;
	m_cylindR = m_coneR*DEFAUT_R_R_Ratio;

	m_nfai = 0;
	m_sinfai   = NULL;
	m_cosfai   = NULL;
	m_conetx   = NULL;
	m_conety   = NULL;
	m_cylindtx = NULL;
	m_cylindty = NULL;

	Set(DEFAUT_NUMFAI);
	ResetStartPoint(Startx, Starty, Startz);
    ResetEndPoint(Endx, Endy, Endz);
}




void Arrow::ChangeSize(float NewSize)
{

	if(m_fullL == 0.f) return;

	m_coneR  = size = NewSize;
	m_coneL   = m_ratio*m_coneR;
	m_cylindR = m_coneR*DEFAUT_R_R_Ratio;

	for(int i=0; i<m_nfai; i++)
	{
	    m_conetx[i]   = m_coneR*m_cosfai[i];
		m_conety[i]   = m_coneR*m_sinfai[i];
		m_cylindtx[i] = m_cylindR*m_cosfai[i];
		m_cylindty[i] = m_cylindR*m_sinfai[i];
	}
	return;
}


#define PI_DEGREE 57.2957795 
void Arrow::Paint()
{
	if(m_fullL <= 0.f) return;

	int i;
	double len,dx,dy,dz,ang;

	dx = (Endx-Startx);
	dy = (Endy-Starty);
	dz = (Endz-Startz);
	ang = acos(dz/m_fullL)*PI_DEGREE;

    glColor4f(R,G,B,A);
	glPushMatrix();
	glTranslatef(Startx, Starty, Startz);
	glRotatef((float)ang, -(float)dy, (float)dx, 0.f);

	float ratio = m_coneL/m_fullL;
	
	if(ratio<1.f)
	{
      //paint the cylinder bottom
       glNormal3f(0.f, 0.f, -1.f);                 
	   glBegin(GL_TRIANGLE_FAN);
	     glVertex3f(0.f, 0.f,  0.f);
	     for(i=m_nfai-1; i>=0; i--)
	  		   glVertex3f(m_cylindtx[i], m_cylindty[i], 0.f);
	   glEnd();

	 //Then draw the surround of cylinder
	   glBegin(GL_TRIANGLE_STRIP);
	     for(i=0; i<m_nfai; i++)
	     {
		  glNormal3f(m_cosfai[i], m_sinfai[i], 0.0);                 

		  glVertex3f(m_cylindtx[i], m_cylindty[i], m_fullL-m_coneL);
		  glVertex3f(m_cylindtx[i], m_cylindty[i], 0.f);
	     }
       glEnd();
	 //Then draw the cone 
	   glTranslatef(0.f, 0.f, m_fullL-m_coneL);
 	   glNormal3d(0., 0., -1.);                 
       glBegin(GL_TRIANGLE_FAN);
		 glVertex3d(0., 0.,  0.);
	     for(i=m_nfai-1; i>=0; i--)
		    glVertex3f(m_conetx[i], m_conety[i], 0.f);
       glEnd();

	   glBegin(GL_TRIANGLE_STRIP);
	  	 for(i=0; i<m_nfai; i++)
		 {
		     glNormal3f(m_cosfai[i], m_sinfai[i], 0.f);                 
		     glVertex3f(0.f, 0.f, m_coneL);
			 glVertex3f(m_conetx[i], m_conety[i], 0.0);
		 }
       glEnd();
	}
	else
	{
      ratio = 1.f/ratio;
	  // draw the cone bottom 
 	   glNormal3d(0., 0., -1.);                 
       glBegin(GL_TRIANGLE_FAN);
		 glVertex3d(0., 0.,  0.);
	     for(i=m_nfai-1; i>=0; i--)
		    glVertex3f(m_conetx[i]*ratio, m_conety[i]*ratio, 0.f);
       glEnd();

	   glBegin(GL_TRIANGLE_STRIP);
	  	 for(i=0; i<m_nfai; i++)
		 {
		     glNormal3f(m_cosfai[i], m_sinfai[i], 0.f);                 
		     glVertex3f(0.f, 0.f, m_coneL*ratio);
			 glVertex3f(m_conetx[i]*ratio, m_conety[i]*ratio, 0.f);
		 }
       glEnd();
	}
	glPopMatrix();
	return;
}


