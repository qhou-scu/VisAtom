/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/
#include <windows.h>	// Standard windows include
#include <gl\gl.h>		// OpenGL library
#include <gl\glu.h>		// OpenGL library

#include <stdlib.h>
#include <stdio.h>


#include "Arrow.h"
#include "Arrowline.h"
#include "Circle.h"
#include "SolidCircle.h"
#include "Style.h"
#include "Sphere.h"
#include "VisAtomDataSheet.h"
#include "VisAtomSubSample.h"
#include "VisAtomSample.h"
#include "VisAtomCliper.h"
#include "VisAtomVisualizingPolicy.h"
#include "VisAtomTrajectory.h"


void VisAtomSample::Clear(bool keepsubsample)
{
	if(!m_aType)       delete m_aType;m_aType= NULL;
	if(!m_aPx)         delete m_aPx; m_aPx = NULL;
	if(!m_aPy)         delete m_aPy; m_aPy = NULL;
	if(!m_aPz)         delete m_aPz; m_aPz = NULL;
	if(!m_aRadius)     delete m_aRadius; m_aRadius = NULL;
	if(!m_aRed)        delete m_aRed; m_aRed = NULL;
	if(!m_aGreen)      delete m_aGreen; m_aGreen = NULL;
	if(!m_aBlue)       delete m_aBlue; m_aBlue = NULL;
	if(!m_aRedex)      delete m_aRedex; m_aRedex = NULL;
	if(!m_aGreenex)    delete m_aGreenex; m_aGreenex = NULL;
	if(!m_aBlueex)     delete m_aBlueex; m_aBlueex = NULL;
    if(!m_aGeomStyle)  delete m_aGeomStyle; m_aGeomStyle = NULL;
	if(!m_aVisStat)    delete m_aVisStat; m_aVisStat = NULL;

    if(m_vPx)          delete m_vPx; m_vPx = NULL;
    if(m_vPy)          delete m_vPy; m_vPy = NULL;
    if(m_vPz)          delete m_vPz; m_vPz = NULL;
	
    if( !keepsubsample)
	{
	   while(!this->m_subSamples.isEmpty())
	  {
           VisAtomSubSample*p = this->m_subSamples[0];
		   delete p;
		   this->m_subSamples.removeAt(0);
		   this->m_selSubsamples.removeAt(0);
	   }
	}

	//dettach from the data sheet
    m_pData = NULL;
	return;
}

VisAtomSample::~VisAtomSample()
{

	Clear();
	m_pickedAtomID = -1;
    m_numAtom = 0;
	m_pCliper.clear();
}

VisAtomSample::VisAtomSample()
{

    m_pData = NULL;
	m_aType = NULL;
	m_aPx   = NULL;
	m_aPy   = NULL;
	m_aPz   = NULL;
	m_aXmi = m_aXmx = m_aYmi = m_aYmx =  m_aZmi = m_aZmx = 0;

	m_aRadius = NULL;
	m_aRed = NULL;  
	m_aGreen = NULL;   
	m_aBlue = NULL;
	m_aRedex = NULL;  
	m_aGreenex = NULL;   
	m_aBlueex = NULL;
    m_aGeomStyle = NULL;
	m_aVisStat = NULL;

	m_vPx = NULL; 
	m_vPy = NULL; 
	m_vPz = NULL; 

	m_numAtom = 0;
    m_curSubsample = -1;

	m_pickedAtomID = -1;
    m_pick_style = atom_pick_Option_Single;
	m_pick_statu = atom_pick_Statu_nopicked;

	m_pTrajectory = NULL;

}

void VisAtomSample::Paint()
{
	
	 glMatrixMode(GL_MODELVIEW);
     glEnable(GL_DEPTH_TEST);
	 Draw();
	 if(this->m_pTrajectory) 
	 {
		 this->m_pTrajectory->Paint();
	 }
     return;
}

//********************************************
void VisAtomSample::Draw()
{
   if(m_numAtom <= 0) return;
       
	   DrawProjCirleAtoms(-1, 0, 0);
	   DrawProjCirleAtoms( 1, 0, 0);
	   DrawProjCirleAtoms( 0, 1, 0);
	   DrawProjCirleAtoms( 0,-1, 0);
	   DrawProjCirleAtoms( 0, 0, 1);
	   DrawProjCirleAtoms( 0, 0,-1);

	   DrawProjSolidAtoms(-1, 0, 0);
	   DrawProjSolidAtoms( 1, 0, 0);
	   DrawProjSolidAtoms( 0, 1, 0);
	   DrawProjSolidAtoms( 0,-1, 0);
	   DrawProjSolidAtoms( 0, 0, 1);
	   DrawProjSolidAtoms( 0, 0,-1);

	   DrawProjDotAtoms(-1, 0, 0);
	   DrawProjDotAtoms( 1, 0, 0);
	   DrawProjDotAtoms( 0, 1, 0);
	   DrawProjDotAtoms( 0,-1, 0);
	   DrawProjDotAtoms( 0, 0, 1);
	   DrawProjDotAtoms( 0, 0,-1);

	   glEnable(GL_DEPTH_TEST);
	   DrawCircleAtoms(VisAtomVisualizingPolicy::D3_STYLE_INBOX);
	   DrawSolidAtoms(VisAtomVisualizingPolicy::D3_STYLE_INBOX);
	   DrawDotAtoms(VisAtomVisualizingPolicy::D3_STYLE_INBOX);
	   DrawLineVectors(VisAtomVisualizingPolicy::D3_STYLE_INBOX);
	   DrawSolidVectors(VisAtomVisualizingPolicy::D3_STYLE_INBOX);

	   DrawCircleAtoms(VisAtomVisualizingPolicy::D3_STYLE_OUTBOX);
	   DrawSolidAtoms(VisAtomVisualizingPolicy::D3_STYLE_OUTBOX);
	   DrawDotAtoms(VisAtomVisualizingPolicy::D3_STYLE_OUTBOX);
	   DrawLineVectors(VisAtomVisualizingPolicy::D3_STYLE_OUTBOX);
	   DrawSolidVectors(VisAtomVisualizingPolicy::D3_STYLE_OUTBOX);

	   return;
}
//*********************************************
//to high-light the slected atoms
void VisAtomSample::HighlightSelectedAtoms()
{
	int j, list;
	Style  Sty;
	Sphere Balls = Sphere();
	bool checkin, in;
	float x0, x1, y0, y1, z0, z1;

	glEnable(GL_LIGHTING);
	Sty.SolidFrontOnly();
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	list = Balls.MakeList();
	for (j = 0; j<m_numAtom; j++)
	{
		if (this->m_aVisStat[j] & atom_Statu_Selected) 
		{
   	    glColor4f(0.7f, 0.7f, 0.0f, 0.5f);
		glPushMatrix();
		glTranslatef(m_aPx[j], m_aPy[j], m_aPz[j]);
		glScalef(1.01f*m_aRadius[j], 1.01f*m_aRadius[j], 1.01f*m_aRadius[j]);
		glCallList(list);
		glPopMatrix();
		}
	}
	return;
}

void VisAtomSample::HighlightFoundAtoms()
{
	int j, list;
	Style  Sty;
	Sphere Balls = Sphere();
	bool checkin, in;
	float x0, x1, y0, y1, z0, z1;

	glEnable(GL_LIGHTING);
	Sty.SolidFrontOnly();
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	list = Balls.MakeList();
	for (j = 0; j<m_numAtom; j++)
	{
		if (this->m_aVisStat[j] & atom_Statu_MarkedFound)
		{
			glColor4f(0.7f, 0.7f, 0.7f, 0.5f);
			glPushMatrix();
			glTranslatef(m_aPx[j], m_aPy[j], m_aPz[j]);
			glScalef(1.01f*m_aRadius[j], 1.01f*m_aRadius[j], 1.01f*m_aRadius[j]);
			glCallList(list);
			glPopMatrix();
		}
	}
	return;
}

//********************************************
//To draw atom in style of solid sphere
void VisAtomSample::DrawSolidAtoms(int inbox)
{
  int j, list;
  Style  Sty;
  Sphere Balls=Sphere();
  bool checkin, in;
  float x0, x1, y0, y1, z0, z1;
  
		  glEnable(GL_LIGHTING);
	      Sty.SolidFrontOnly();

	      VisAtomVisualizingPolicy::GetProjectionBox(x0, x1, y0, y1, z0, z1);
		  if (inbox == VisAtomVisualizingPolicy::D3_STYLE_INBOX) checkin = true;
		  else checkin = false;

		   float shine[] = {0.5f, 0.5f, 0.5f, 1.0f};
	       glMaterialfv(GL_FRONT, GL_SPECULAR,shine);
	       glMateriali(GL_FRONT,GL_SHININESS,100);
	       glEnable(GL_COLOR_MATERIAL);
	       glEnable(GL_NORMALIZE);
		   list = Balls.MakeList();  
		   for(j=0; j<m_numAtom; j++)
		   {
			   if ( (this->m_aVisStat[j] & atom_Statu_Visiable || this->m_aVisStat[j] & atom_Statu_MarkedFound) &&
				   (this->m_aGeomStyle[j] & inbox) &&
				   (this->m_aGeomStyle[j] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK) == VisAtomVisualizingPolicy::SHAPE_STYLE_SOLID)

			   {
				   in = (x0 <= m_aPx[j] && m_aPx[j] <= x1) && (y0 <= m_aPy[j] && m_aPy[j] <= y1) && (z0 <= m_aPz[j] && m_aPz[j] <= z1);
				   if (in == checkin)
				   {
				   glPushName(j);
				   glColor4f(m_aRed[j], m_aGreen[j], m_aBlue[j], 1.f);
				   glPushMatrix();
				   glTranslatef(m_aPx[j], m_aPy[j], m_aPz[j]);
				   glScalef(m_aRadius[j], m_aRadius[j], m_aRadius[j]);
				   glCallList(list);
				   glPopMatrix();
				   glPopName();
				   }
			   }
		   }
	return;
}

//********************************************
//To draw atom of type I in style of solid sphere
void VisAtomSample::DrawWireAtoms(int inbox)
{
           int j;
           Style  Sty;
           Sphere Balls=Sphere();
		   int Balllist;
		   bool checkin, in;
		   float x0, x1, y0, y1, z0, z1;

		   glEnable(GL_LIGHTING);
	       Sty.Wire();
		   VisAtomVisualizingPolicy::GetProjectionBox(x0, x1, y0, y1, z0, z1);
		   if (inbox == VisAtomVisualizingPolicy::D3_STYLE_INBOX) checkin = true;
		   else checkin = false;

		   Balllist = Balls.MakeList();
		   for(j=0; j<m_numAtom; j++)
		   {
              if((m_aVisStat[j]&atom_Statu_Visiable || this->m_aVisStat[j] & atom_Statu_MarkedFound) &&
				  (this->m_aGeomStyle[j] & inbox) &&
				  (this->m_aGeomStyle[j] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_WIRE)
		      {
				  in = (x0 <= m_aPx[j] && m_aPx[j] <= x1) && (y0 <= m_aPy[j] && m_aPy[j] <= y1) && (z0 <= m_aPz[j] && m_aPz[j] <= z1);
				  if (in == checkin)
				  {
			  	      glPushName(j);
					  glColor4f(this->m_aRed[j], this->m_aGreen[j], this->m_aBlue[j], 1.f);
					  glPushMatrix();
					  glTranslatef(this->m_aPx[j], this->m_aPy[j], this->m_aPz[j]);
					  glScalef(this->m_aRadius[j], this->m_aRadius[j], this->m_aRadius[j]);
					  glCallList(Balllist);
					  glPopMatrix();
					  glPopName();
				  }
				 }
		   }

	return;
}


//********************************************
//To draw atom of type I in style of dot
void VisAtomSample::DrawDotAtoms(int inbox)
{
	
           int j;
		   bool checkin, in;
		   float x0, x1, y0, y1, z0, z1;

		   VisAtomVisualizingPolicy::GetProjectionBox(x0, x1, y0, y1, z0, z1);
		   if (inbox == VisAtomVisualizingPolicy::D3_STYLE_INBOX) checkin = true;
		   else checkin = false;

           glDisable(GL_LIGHTING);
           glPointSize(2.f);
		   for(j=0; j<m_numAtom; j++)
	       {
              if( (m_aVisStat[j]&atom_Statu_Visiable || this->m_aVisStat[j] & atom_Statu_MarkedFound) &&
				  (this->m_aGeomStyle[j] & inbox) &&
				  (this->m_aGeomStyle[j] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_DOT)
		      {
				  in = (x0 <= m_aPx[j] && m_aPx[j] <= x1) && (y0 <= m_aPy[j] && m_aPy[j] <= y1) && (z0 <= m_aPz[j] && m_aPz[j] <= z1);
				  if (in == checkin)
				  {
					  glColor4f(this->m_aRed[j], this->m_aGreen[j], this->m_aBlue[j], 1.f);
					  glPushName(j);
					  glBegin(GL_POINTS);
					  glVertex3f(m_aPx[j], m_aPy[j], m_aPz[j]);
					  glEnd();
					  glPopName();
				  }
					  
			  }
		    }

	return;
}

//*******************************************************************************
// to draw atoms in circles
void VisAtomSample::DrawCircleAtoms(int inbox)
{
   
   if(m_numAtom <= 0) return;

       double winx, winy, winz;
       double winx1, winy1, winz1;
	   GLdouble modelMatrix[16];
	   GLdouble projMatrix[16];
	   GLint viewport[4];
	   bool checkin, in;
	   float bx0, bx1, by0, by1, bz0, bz1;

	   VisAtomVisualizingPolicy::GetProjectionBox(bx0, bx1, by0, by1, bz0, bz1);
	   if (inbox == VisAtomVisualizingPolicy::D3_STYLE_INBOX) checkin = true;
	   else checkin = false;

	   int i, k, n, NA;
       Circle BallsLine=Circle();
       SolidCircle BallsPie=SolidCircle();
       Style Sty=Style();

	   glDisable(GL_LIGHTING);
	   Sty.SolidFrontOnly();
	   //to get current transform matrix
	   glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	   glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	   glGetIntegerv(GL_VIEWPORT, viewport);


	   //to determine the rotation
       double x0, y0, z0, xx0, yy0, zz0, len;
       double vx[3],	vy[3], vz[3];
       double theta;

	   vx[0] = modelMatrix[0];	    
	   vx[1] = modelMatrix[1];	    
	   vx[2] = modelMatrix[2];	    
 	   len = vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2];
  	   len = sqrt((double)len);
 	   vx[0] = vx[0]/len;	    
 	   vx[1] = vx[1]/len;	    
 	   vx[2] = vx[2]/len;	    

	   vy[0] = modelMatrix[4];	    
	   vy[1] = modelMatrix[5];	    
	   vy[2] = modelMatrix[6];	    
 	   len = vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2];
  	   len = sqrt((double)len);
 	   vy[0] = vy[0]/len;	    
 	   vy[1] = vy[1]/len;	    
 	   vy[2] = vy[2]/len;	    

	   vz[0] = modelMatrix[8];	    
	   vz[1] = modelMatrix[9];	    
	   vz[2] = modelMatrix[10];	    
  	   len = vz[0]*vz[0]+vz[1]*vz[1]+vz[2]*vz[2];
  	   len = sqrt((double)len);
 	   vz[0] = vz[0]/len;	    
 	   vz[1] = vz[1]/len;	    
 	   vz[2] = vz[2]/len;	    

	   x0 = vx[2];
	   y0 = vy[2];
	   z0 = vz[2];

	   theta = x0*vz[0]+y0*vz[1]+z0*vz[2];
	   theta = acos(z0)*(180.0/3.14159265359);
	   //The normal of plane (Z,Z') in (X, Y, Z) coordinate system:
       xx0 = -y0;	    
       yy0 =  x0; 	    
       zz0 =  0.0;	    
	   BallsLine.SetBorderThick(1.f);
	   int list1 = BallsPie.MakeList();
	   int list2 = BallsLine.MakeList();
	   float rad;
	   for(i=0; i<m_numAtom; i++)
	   {
          if( (this->m_aVisStat[i] & atom_Statu_Visiable || this->m_aVisStat[i] & atom_Statu_MarkedFound) &&
			  (this->m_aGeomStyle[i] & inbox) &&
			  (((this->m_aGeomStyle[i] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_LINE)||
			  ((this->m_aGeomStyle[i]  & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_WIRE)) )
		  {

			  in = (bx0 <= m_aPx[i] && m_aPx[i] <= bx1) && (by0 <= m_aPy[i] && m_aPy[i] <= by1) && (bz0 <= m_aPz[i] && m_aPz[i] <= bz1);
			  if (in == checkin)
			  {
				  int j = i;
				  glPushName(j);
				  glPushMatrix();
				  glTranslatef(m_aPx[j], m_aPy[j], m_aPz[j]);
				  glRotated(theta, xx0, yy0, zz0);
				  //draw pie
				  rad = this->m_aRadius[j] * 0.98;
				  glPushMatrix();
				  glScalef(rad, rad, rad);
				  if ((this->m_aGeomStyle[j] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK) == VisAtomVisualizingPolicy::SHAPE_STYLE_LINE)
					  glColor3f(1.f, 1.f, 1.f);
				  else
					  glColor3f(m_aRed[j], m_aGreen[j], m_aBlue[j]);
				  glCallList(list1);
				  glPopMatrix();
				  //draw edge
				  glPushMatrix();
				  glScalef(rad, rad, rad);
				  if ((this->m_aGeomStyle[j] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK) == VisAtomVisualizingPolicy::SHAPE_STYLE_LINE)
					  glColor3f(m_aRed[j], m_aGreen[j], m_aBlue[j]);
				  else
					  glColor3f(m_aRedex[j], m_aGreenex[j], m_aBlueex[j]);
				  glCallList(list2);
				  glPopMatrix();

				  glPopMatrix();
				  glPopName();
			  }
		 }
	   }
	   
	   //delete id; 
	   glEnable(GL_LIGHTING);

	return;
}

//*******************************************************************************
// to draw colormaping bar
void VisAtomSample::DrawColorMappinBar(int lablepos[])
{
#define BARW   16
#define BARH   16*6

      double winx, winy, winz;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];
	  double LB[3], RT[3];

	  glGetIntegerv(GL_VIEWPORT, viewport);
	  glDisable(GL_DEPTH_TEST);
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glLoadIdentity();
	  glMatrixMode(GL_PROJECTION);
	  glPushMatrix();
	  glLoadIdentity();

	  glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	  //get the object coodination of left-bottom corner of viewport
	  winx = (double)viewport[0];
	  winy = (double)viewport[1]; //(double)(h - viewport[1]);
	  winz = 0;
	  gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport,
		  &LB[0], &LB[1], &LB[2]);
	  //get the object coodination of left-bottom corner of viewport
	  winx = winx + (double)viewport[2];
	  winy = winy + (double)viewport[3];
	  winz = 0;
	  gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport,
		  &RT[0], &RT[1], &RT[2]);

	  int nbar = VisAtomVisualizingPolicy::GetColorMappingNumBar();
	  double hh = RT[1] - LB[1];
	  double step = hh / (nbar - 1);
	  float R, G, B;
	  double x1, y1, z1, x2, y2, z2;
	  x1 = LB[0];
	  y1 = LB[1];
	  z1 = 0.5*(LB[2] + RT[2]);
	  x2 = RT[0];
	  y2 = y1 + step;
	  z2 = z1;
	  glEnable(GL_COLOR_MATERIAL);
	  int i;
	  for (i = 0; i<nbar - 1; i++)
	  {
		  VisAtomVisualizingPolicy::GetColorMappingBar(i, R, G, B);
		  glBegin(GL_POLYGON);
		  glColor3f(R, G, B);
		  glVertex2d(x1 + 0.1*(x2 - x1), y1); // , z1);
		  glVertex2d(x2, y1); // , z1);

		  VisAtomVisualizingPolicy::GetColorMappingBar(i + 1, R, G, B);
		  glColor3f(R, G, B);
		  glVertex2d(x2, y2); // , z1);
		  glVertex2d(x1 + 0.1*(x2 - x1), y2); // , z1);

							  //VisAtomVisualizingPolicy::GetColorMappingBar(i, R, G, B);
							  //glColor3f(R, G, B);
							  //glVertex3d(x1, y1, z1);
		  glEnd();

		  VisAtomVisualizingPolicy::GetColorMappingBar(i, R, G, B);
		  glBegin(GL_LINES);
		  glColor3f(R, G, B);
		  glVertex2d(x1, y1);// , z1);
		  glVertex2d(x2, y1); // , z1);

		  VisAtomVisualizingPolicy::GetColorMappingBar(i + 1, R, G, B);
		  glColor3f(R, G, B);
		  glVertex2d(x2, y2);// , z1);
		  glVertex2d(x1, y2);// , z1);
		  glEnd();

		  //to get the lable position
		  gluProject(x1, y1, z1, modelMatrix, projMatrix, viewport,
			  &winx, &winy, &winz);
		  lablepos[i] = winy;
		  //move to next 
		  y1 = y2;
		  y2 = y2 + step;
	  }
	  gluProject(x1, y1, z1, modelMatrix, projMatrix, viewport,
		  &winx, &winy, &winz);
	  lablepos[i] = winy;

	  glMatrixMode(GL_PROJECTION);
	  glPopMatrix();
	  glMatrixMode(GL_MODELVIEW);
	  glPopMatrix();

      
	  /*glGetIntegerv(GL_VIEWPORT, viewport);
	  glDisable(GL_DEPTH_TEST);
      glPushMatrix();
	      glMatrixMode(GL_MODELVIEW);	
		  glLoadIdentity();

		  float vw = viewport[1] - viewport[0];//   (SCENE->GetVolumeX1() - SCENE->GetVolumeX0());
		  float vh = viewport[3] - viewport[0]; // (SCENE->GetVolumeY1() - SCENE->GetVolumeY0());

		  float w0 = (SCENE->GetVolumeX1() - SCENE->GetVolumeX0());
		  float h0 = (SCENE->GetVolumeY1() - SCENE->GetVolumeY0());

		  double x1, y1, z1, x2, y2, z2;

	      int nbar = VisAtomVisualizingPolicy::GetColorMappingNumBar();
		  double step, hh;
		  if(h0>w0) hh = h0;
		  else      hh = w0;
		  step = hh / (nbar - 1);
		  float R, G, B;
		  x1 = -w0*0.5; 
		  y1 = -0.5*hh; // SCENE->GetVolumeY0(); 
		  z1 = SCENE->GetVolumeZ1();
		  x2 = w0*0.5;  
		  y2 = y1 + step; 
		  z2 = z1;
	      glEnable(GL_COLOR_MATERIAL);

  		  for(int i=0; i<nbar-1; i++)
		  {
			 VisAtomVisualizingPolicy::GetColorMappingBar(i, R, G, B);
		     glBegin(GL_POLYGON);
			 glColor3f(R, G, B);
			 glVertex2d(x1, y1); // , z1);
			 glVertex2d(x2, y1); // , z1);

			 VisAtomVisualizingPolicy::GetColorMappingBar(i+1, R, G, B);
			 glColor3f(R, G, B);
			 glVertex2d(x2, y2); // , z1);
			 glVertex2d(x1, y2); // , z1);

			 //VisAtomVisualizingPolicy::GetColorMappingBar(i, R, G, B);
			 //glColor3f(R, G, B);
			 //glVertex3d(x1, y1, z1);
			 glEnd();			  

			 VisAtomVisualizingPolicy::GetColorMappingBar(i, R, G, B);
		     glBegin(GL_LINES);
			 glColor3f(R, G, B);
			 glVertex2d(x1 - 0.1*(x2 - x1), y1);// , z1);
			 glVertex2d(x2, y1); // , z1);

			 VisAtomVisualizingPolicy::GetColorMappingBar(i+1, R, G, B);
			 glColor3f(R, G, B);
			 glVertex2d(x2, y2);// , z1);
			 glVertex2d(x1 - 0.1*(x2 - x1), y2);// , z1);
			 glEnd();			  

             y1 = y2;
			 y2 = y2 + step;
		  }
	  glPopMatrix();*/
	  return;

}

//*******************************************************************************
// to draw line vectors of atoms 
void VisAtomSample::DrawLineVectors(int inbox)
{
     if(!VisAtomVisualizingPolicy::HasVectorData()) return;

	 bool checkin, in;
	 float bx0, bx1, by0, by1, bz0, bz1;

	 VisAtomVisualizingPolicy::GetProjectionBox(bx0, bx1, by0, by1, bz0, bz1);
	 if (inbox == VisAtomVisualizingPolicy::D3_STYLE_INBOX) checkin = true;
	 else checkin = false;

     Arrowline  V=Arrowline();
	 glEnable(GL_COLOR_MATERIAL);
   
	  int j;
	  float vrad, vlenscal,arrowl, arrowr;
	  VisAtomVisualizingPolicy::GetVectorSize(vrad, vlenscal,arrowl, arrowr);
	  V.ChangeSize(arrowr);

	  glDisable(GL_LIGHTING);
      glLineWidth(1.f); 
	  for(j=0; j<m_numAtom; j++)
	  {
          if(m_aVisStat[j]&atom_Statu_Visiable &&
			(this->m_aGeomStyle[j] & inbox) &&
			(this->m_aGeomStyle[j]&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK)==VisAtomVisualizingPolicy::VECTOR_STYLE_LINE)
		  {
			 in = (bx0 <= m_aPx[j] && m_aPx[j] <= bx1) && (by0 <= m_aPy[j] && m_aPy[j] <= by1) && (bz0 <= m_aPz[j] && m_aPz[j] <= bz1);
			 if (in == checkin)
			 {
				 V.ResetStartPoint(m_aPx[j], m_aPy[j], m_aPz[j]);
				 V.ResetEndPoint(m_aPx[j] + m_vPx[j], m_aPy[j] + m_vPy[j], m_aPz[j] + m_vPz[j]);
				 glColor3f(m_aRed[j], m_aGreen[j], m_aBlue[j]);
				 glPushName(j);
				 V.PaintVertex();
				 glPopName();
			 }
		  }
	   } 
	  glEnable(GL_LIGHTING);
      return;
}

//*******************************************************************************
// to draw solid vectors of atoms 
void VisAtomSample::DrawSolidVectors(int inbox)
{
     if(!VisAtomVisualizingPolicy::HasVectorData()) return;

     Style  Sty;
     Arrow  V=Arrow();
	 bool checkin, in;
	 float bx0, bx1, by0, by1, bz0, bz1;

	 VisAtomVisualizingPolicy::GetProjectionBox(bx0, bx1, by0, by1, bz0, bz1);
	 if (inbox == VisAtomVisualizingPolicy::D3_STYLE_INBOX) checkin = true;
	 else checkin = false;

     glEnable(GL_LIGHTING);
     Sty.SolidFrontOnly();

	float shine[] = {0.5f, 0.5f, 0.5f, 0.6f};
	glMaterialfv(GL_FRONT, GL_SPECULAR,shine);
	glMateriali(GL_FRONT,GL_SHININESS,80);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
   
	  int j;
	  float vrad, vlenscal,arrowl, arrowr;
	  VisAtomVisualizingPolicy::GetVectorSize(vrad, vlenscal,arrowl, arrowr);
	  V.ChangeSize(arrowr);

	  for(j=0; j<m_numAtom; j++)
	  {
          if(m_aVisStat[j]&atom_Statu_Visiable &&
			(this->m_aGeomStyle[j] & inbox) &&
			(this->m_aGeomStyle[j]&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK)==VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID)
		  {
			  in = (bx0 <= m_aPx[j] && m_aPx[j] <= bx1) && (by0 <= m_aPy[j] && m_aPy[j] <= by1) && (bz0 <= m_aPz[j] && m_aPz[j] <= bz1);
			  if (in == checkin)
			  {
				  V.ChangeColor(m_aRed[j], m_aGreen[j], m_aBlue[j]);
				  V.ResetStartPoint(m_aPx[j], m_aPy[j], m_aPz[j]);
				  V.ResetEndPoint(m_aPx[j] + m_vPx[j], m_aPy[j] + m_vPy[j], m_aPz[j] + m_vPz[j]);
				  glPushName(j);
				  V.Paint();
				  glPopName();
			  }
		  }
	   }
	  return;
}

//*******************************************************************************
// to draw atomsprojected on box side face
void VisAtomSample::DrawProjSolidAtoms(int nx, int ny, int nz)
{
   
   if(m_numAtom <= 0) return;


       glDisable(GL_DEPTH_TEST);
       Style Sty=Style();
	   glDisable(GL_LIGHTING);
       glEnable(GL_COLOR_MATERIAL);

	   Sty.Solid();

       Circle BallsLine=Circle();
       SolidCircle BallsPie=SolidCircle();

	   int list1 = BallsPie.MakeList();

	   //the face postion and rotation axsis
	   int i, k, tv[3], flag;
	   float x0, x1, y0, y1, z0, z1, fx, fy, fz, rx, ry, rz, rad, theta;
	   bool  in;
	   VisAtomVisualizingPolicy::GetProjectionBox(x0, x1, y0, y1, z0, z1);


	   tv[0] = 1 - abs(nx);
	   tv[1] = 1 - abs(ny);
	   tv[2] = 1 - abs(nz);

	   fx = 0.f;
	   if(nx > 0)      { fx = x1; rx= 0; ry = 1.f; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_X1; }
	   else if(nx < 0) { fx = x0; rx= 0; ry =-1.f; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_X0;}

	   fy = 0.f;
	   if(ny > 0)      { fy = y1; rx= 1.f; ry = 0; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Y1;}
	   else if(ny < 0) { fy = y0; rx=-1.f; ry = 0; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Y0;}

	   fz = 0.f;
	   if(nz > 0)      { fz = z1; rx= 0; ry = 0; rz = 1.f;theta =0.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Z1;}
       else if(nz < 0) { fz = z0; rx= 0; ry = 0; rz = 1.f;theta =0.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Z0;}


	   for(i=0; i<m_numAtom; i++)
	   {
          if(( this->m_aVisStat[i]&atom_Statu_Visiable) && ((this->m_aGeomStyle[i]&flag)== flag) &&
			  ((this->m_aGeomStyle[i]&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_SOLID ))
		  {
			  in = (x0 <= m_aPx[i] && m_aPx[i] <= x1) && (y0 <= m_aPy[i] && m_aPy[i] <= y1) && (z0 <= m_aPz[i] && m_aPz[i] <= z1);
			  if (in)
			  {
				  glPushName(i);
				  glPushMatrix();
				  glTranslatef(m_aPx[i] * tv[0] + fx * 0.99, m_aPy[i] * tv[1] + fy * 0.99, m_aPz[i] * tv[2] + fz * 0.99);
				  glRotatef(theta, rx, ry, rz);
				  //draw pie
				  glColor4f(m_aRed[i], m_aGreen[i], m_aBlue[i], 1.f);
				  glPushMatrix();
				  glScalef(m_aRadius[i], m_aRadius[i], m_aRadius[i]);
				  glCallList(list1);
				  glPopMatrix();

				  glPopMatrix();
				  glPopName();
			  }
		 }
	   }
	   //delete id; 
	   glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

	return;
}


//*******************************************************************************
// to draw atomsprojected on box side face
void VisAtomSample::DrawProjCirleAtoms(int nx, int ny, int nz)
{
   
   if(m_numAtom <= 0) return;


       glDisable(GL_DEPTH_TEST);
       Style Sty=Style();
	   glDisable(GL_LIGHTING);
       glEnable(GL_COLOR_MATERIAL);

	   Sty.Solid();

       Circle BallsLine=Circle();
       SolidCircle BallsPie=SolidCircle();

	   BallsLine.SetBorderThick(1.f);
	   int list1 = BallsPie.MakeList();
	   int list2 = BallsLine.MakeList();

	   //the face postion and rotation axsis
	   int i, k, n, NA, tv[3], flag;
	   float x0, x1, y0, y1, z0, z1, fx, fy, fz, rx, ry, rz, rad, theta;
	   bool in;
	   VisAtomVisualizingPolicy::GetProjectionBox(x0, x1, y0, y1, z0, z1);

	   tv[0] = 1 - abs(nx);
	   tv[1] = 1 - abs(ny);
	   tv[2] = 1 - abs(nz);

	   fx = 0.f;
	   if(nx > 0)      { fx = x1; rx= 0; ry = 1.f; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_X1; }
	   else if(nx < 0) { fx = x0; rx= 0; ry =-1.f; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_X0;}

	   fy = 0.f;
	   if(ny > 0)      { fy = y1; rx= 1.f; ry = 0; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Y1;}
	   else if(ny < 0) { fy = y0; rx=-1.f; ry = 0; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Y0;}

	   fz = 0.f;
	   if(nz > 0)      { fz = z1; rx= 0; ry = 0; rz = 1.f;theta =0.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Z1;}
       else if(nz < 0) { fz = z0; rx= 0; ry = 0; rz = 1.f;theta =0.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Z0;}


	   for(i=0; i<m_numAtom; i++)
	   {
          if(( this->m_aVisStat[i]&atom_Statu_Visiable) && ((this->m_aGeomStyle[i]&flag)== flag) &&
			  (((this->m_aGeomStyle[i]&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_LINE)||
			  ((this->m_aGeomStyle[i]&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_WIRE)) )
		  {
			  in = (x0 <= m_aPx[i] && m_aPx[i] <= x1) && (y0 <= m_aPy[i] && m_aPy[i] <= y1) && (z0 <= m_aPz[i] && m_aPz[i] <= z1);
			  if (in)
			  {
				  glPushName(i);
				  //draw cricles
				  glPushMatrix();
				  glTranslatef(m_aPx[i] * tv[0] + fx * 0.99, m_aPy[i] * tv[1] + fy * 0.99, m_aPz[i] * tv[2] + fz * 0.99);
				  glRotatef(theta, rx, ry, rz);
				  //draw pie
				  rad = this->m_aRadius[i] * 0.98;
				  glPushMatrix();
				  glScalef(rad, rad, rad);
				  if ((this->m_aGeomStyle[i] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK) == VisAtomVisualizingPolicy::SHAPE_STYLE_LINE)
					  glColor3f(1.f, 1.f, 1.f);
				  else
					  glColor3f(m_aRed[i], m_aGreen[i], m_aBlue[i]);
				  glCallList(list1);
				  glPopMatrix();
				  //draw edge
				  glPushMatrix();
				  glScalef(rad, rad, rad);
				  if ((this->m_aGeomStyle[i] & VisAtomVisualizingPolicy::SHAPE_STYLE_MASK) == VisAtomVisualizingPolicy::SHAPE_STYLE_LINE)
					  glColor3f(m_aRed[i], m_aGreen[i], m_aBlue[i]);
				  else
					  glColor3f(m_aRedex[i], m_aGreenex[i], m_aBlueex[i]);
				  glCallList(list2);
				  glPopMatrix();

				  glPopMatrix();
				  glPopName();

			  }
		 }
	   }
	   
	   //delete id; 
	   glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

	return;
}


//*******************************************************************************
// to draw atomsprojected on box side face
void VisAtomSample::DrawProjDotAtoms(int nx, int ny, int nz)
{
   
   if(m_numAtom <= 0) return;

       glDisable(GL_DEPTH_TEST);
	   glDisable(GL_LIGHTING);
       glEnable(GL_COLOR_MATERIAL);

	   //the face postion and rotation axsis
	   int i, k, tv[3], flag;
	   float x0, x1, y0, y1, z0, z1, fx, fy, fz, rx, ry, rz, theta;
	   bool in;
	   VisAtomVisualizingPolicy::GetProjectionBox(x0, x1, y0, y1, z0, z1);

	   tv[0] = 1 - abs(nx);
	   tv[1] = 1 - abs(ny);
	   tv[2] = 1 - abs(nz);

	   fx = 0.f;
	   if(nx > 0)      { fx = x1; rx= 0; ry = 1.f; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_X1; }
	   else if(nx < 0) { fx = x0; rx= 0; ry =-1.f; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_X0;}

	   fy = 0.f;
	   if(ny > 0)      { fy = y1; rx= 1.f; ry = 0; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Y1;}
	   else if(ny < 0) { fy = y0; rx=-1.f; ry = 0; rz = 0;theta =90.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Y0;}

	   fz = 0.f;
	   if(nz > 0)      { fz = z1; rx= 0; ry = 0; rz = 1.f;theta =0.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Z1;}
       else if(nz < 0) { fz = z0; rx= 0; ry = 0; rz = 1.f;theta =0.f; flag = VisAtomVisualizingPolicy::PROJECTION_STYLE_Z0;}


	   for(i=0; i<m_numAtom; i++)
	   {
          if(( this->m_aVisStat[i]&atom_Statu_Visiable) && ((this->m_aGeomStyle[i]&flag)== flag) && 
			  (this->m_aGeomStyle[i]&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)==VisAtomVisualizingPolicy::SHAPE_STYLE_DOT)
		  {
			  in = (x0 <= m_aPx[i] && m_aPx[i] <= x1) && (y0 <= m_aPy[i] && m_aPy[i] <= y1) && (z0 <= m_aPz[i] && m_aPz[i] <= z1);
			  if (in)
			  {
				  glColor4f(this->m_aRed[i], this->m_aGreen[i], this->m_aBlue[i], 1.f);
				  glPushName(i);
				  glBegin(GL_POINTS);
				  glVertex3f(m_aPx[i] * tv[0] + fx * 0.99, m_aPy[i] * tv[1] + fy * 0.99, m_aPz[i] * tv[2] + fz * 0.99);
				  glEnd();
				  glPopName();
			  }
		 }
	   }
	   
	   //delete id; 
	  glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

	return;
}



//To get the ID of current picked atom (in current sample)
int VisAtomSample::GetCurAtom()
{
	  return m_pickedAtomID;
}

//To set the atom id as current picked atom (in current sample)
void VisAtomSample::SetCurAtom(int id)
{
	  if(id > m_numAtom) return;
	  m_pickedAtomID = id;
}

//To get the type of current picked atom (in current sample)
int VisAtomSample::GetTypeofCurAtom()
{
	  if(m_pickedAtomID <= 0) return -1;
	  return m_aType[m_pickedAtomID-1];
}

//To get the X coordinate of current picked atom (in current sample)
float VisAtomSample::GetXofCurAtom()
{
	  	if(m_pickedAtomID <=0) return -(float)1.E38;
		return m_aPx[m_pickedAtomID-1];
}


//To get the Y coordinate of current picked atom (in current sample)
float VisAtomSample::GetYofCurAtom()
{
	  if(m_pickedAtomID <=0) return -(float)1.E38;
	  return m_aPy[m_pickedAtomID-1];
}


//To get the Z coordinate of current picked atom (in current sample)
float VisAtomSample::GetZofCurAtom()
{
	  if(m_pickedAtomID <=0) return -(float)1.E38;
	  return m_aPz[m_pickedAtomID-1];
}



//To set atom id as selected
void  VisAtomSample::Select_aAtom(int id)
{
	 if(id > m_numAtom) return;
	 if(id <= 0) return;
	 int i = id-1;
	 this->m_aVisStat[i] = this->m_aVisStat[i]|atom_Statu_Selected;
	 return;
}

//To set the current atom as selected
void  VisAtomSample::Select_aAtom()
{
	  int id = GetCurAtom();
	  if(id <=0) return;
      Select_aAtom(id);
}

void  VisAtomSample::MarkAtomsFound(int from, int to, int step)
{
	for (int i = from; i <= to; i+=step)
	{
		if (1 <= i && i <= m_numAtom)
		{
			this->m_aVisStat[i - 1] = this->m_aVisStat[i - 1] | atom_Statu_MarkedFound;
		}
	}
	return;
}

void  VisAtomSample::MarkGroupAtomsFound(int group, int from, int to)
{
	int ip = 0;
	for (int i = 0; i < this->m_numAtom; i++)
	{
		if (this->m_aType[i] == group)
		{
			ip++;
			if (ip >= from && ip <= to)
			{
				this->m_aVisStat[i] = this->m_aVisStat[i] | atom_Statu_MarkedFound;
			}
		}
	}
	return;
}

void  VisAtomSample::GeNextAtomMarkedFound(int ith, int&gid, int&ingr, float&posx, float&posy, float&posz)
{   
	int ip, i0, i;

	i0 = ith;
	if (ith <= 0) i0 = 0;

	gid  = 0;
	ingr = 0;
	ip   = 0;
	for (int i = 0; i < this->m_numAtom; i++)
	{
		if( this->m_aVisStat[i] & atom_Statu_MarkedFound )
		{
			if (ip == i0)
			{
				gid  = i + 1;
				ingr = this->m_aType[i];
				posx = this->m_aPx[i];
				posy = this->m_aPy[i];
				posz = this->m_aPz[i];
				break;
			}
			ip++;
		}
	}
}



//To set an atom as unselected
void  VisAtomSample::unSelect_aAtom(int id)
{
	 if(id > m_numAtom) return;
	 if(id <= 0) return;
	 int i = id-1;
	 int j,k;
	 k = this->m_aVisStat[i]&atom_Statu_Visiable;
	 j = (this->m_aVisStat[i]>>2);
	 this->m_aVisStat[i] = (j<<2);
	 this->m_aVisStat[i] = this->m_aVisStat[i]|k;
	 //astatu[i] = astatu[i]|atom_Statu_Selected;
	 return;
}

//To set the current picked atom as unselected
void  VisAtomSample::unSelect_aAtom()
{
	  int id = GetCurAtom();
	  if(id <=0) return;
      unSelect_aAtom(id);
}

void  VisAtomSample::unSelect_Atoms(int from, int to)
{
	for (int i = from; i <= to; i++)
	{
		this->unSelect_aAtom(i);
	}
	return;
}


//To set an atom as unselected
void  VisAtomSample::unSelect_AllAtoms()
{
	 int i,j, k;
	 for(i=0; i<m_numAtom; i++)
	 {
	   k = this->m_aVisStat[i]&atom_Statu_Visiable;
	   j = (this->m_aVisStat[i]>>2);
	   this->m_aVisStat[i] = (j<<2);
	   this->m_aVisStat[i] = this->m_aVisStat[i]|k;
	 }
	 m_pickedAtomID = -1;
	return;
}

//To remove the found mark of atoms
void  VisAtomSample::RemoveAtomsFoundMark()
{
	int i, j, k;
	for (i = 0; i<m_numAtom; i++)
	{
		if (this->m_aVisStat[i] & atom_Statu_MarkedFound)
		{
			k = this->m_aVisStat[i] & atom_Statu_Visiable;
			j = (this->m_aVisStat[i] >> 3);
			this->m_aVisStat[i] = (j << 3);
			this->m_aVisStat[i] = this->m_aVisStat[i] | k;
		}
	}
	return;
}



//To get the number of atoms that are visiable
int  VisAtomSample::NumberofVisiableAtom()
{
	int i, count;

	count = 0;
	for(i=0; i<m_numAtom; i++) 
      if(this->m_aVisStat[i]&atom_Statu_Visiable) count ++; 

	return count;
}


//To get if the samples changed
int  VisAtomSample::IfChanged()
{
	 return statu;
}

//To set all the samples in unchanged statu
void  VisAtomSample::FinishChange()
{
	statu = sample_Statu_noChange;
	return;
}


//To move a atom picked
int  VisAtomSample::Move_aAtom(float dx, float dy, float dz)
{
	 if(m_pickedAtomID <= 0) return 0;
	 int i = m_pickedAtomID-1;
	 m_aPx[i] = m_aPx[i]+dx;
	 m_aPy[i] = m_aPy[i]+dy;
	 m_aPz[i] = m_aPz[i]+dz;
	 statu = sample_Statu_Changed;
	 return m_pickedAtomID;
}

//To move atoms that are selected
void  VisAtomSample::Move_SelectedAtoms(float dx, float dy, float dz)
{
	 if(m_pickedAtomID <= 0) return;
	 int i;
	 for(i=0; i<m_numAtom; i++)
	 {
	  if( this->m_aVisStat[i]&atom_Statu_Selected)
	  {
	   m_aPx[i] = m_aPx[i]+dx;
	   m_aPy[i] = m_aPy[i]+dy;
	   m_aPz[i] = m_aPz[i]+dz;
	  }
	 }
	 statu = sample_Statu_Changed;
	 return;
}


//To translate the atoms(subsample) of type I 
void  VisAtomSample::Move_aAtomGroup(int I, float dx, float dy, float dz)
{
	  int i;
	  for(i=0; i<m_numAtom; i++)
	  {
	   if(m_aType[i] == I)
	   {
 	     m_aPx[i] = m_aPx[i]+dx;
	     m_aPy[i] = m_aPy[i]+dy;
	     m_aPz[i] = m_aPz[i]+dz;
	    }
	   }
}

//To translate the atoms(subsample) of type of currrent selected atom
void  VisAtomSample::Move_aAtomGroup(float dx, float dy, float dz)
{
	  if(m_pickedAtomID <=0) return;
	  int i;
	  int I = m_aType[m_pickedAtomID-1];
	  for(i=0; i<m_numAtom; i++)
	  {
	   if(m_aType[i] == I)
	   {
 	     m_aPx[i] = m_aPx[i]+dx;
	     m_aPy[i] = m_aPy[i]+dy;
	     m_aPz[i] = m_aPz[i]+dz;
	    }
	   }
}


//To get the current pick statu
int  VisAtomSample::GetPickStatu()
{
	 return m_pick_statu;
}

//To clear current  pick statu
void VisAtomSample::ClearPickStatu()
{
	  m_pick_statu = atom_pick_Statu_nopicked;
}


//To get the current pick option
int  VisAtomSample::GetPickOption()
{
	 return m_pick_style;
}

//To set the current pick option
void  VisAtomSample::SetPickOption(int nStyle)
{
	  m_pick_style = nStyle;
	  return;
}


//To process pick single atom in active sample
int  VisAtomSample::PickAtom(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
{
	 switch(m_pick_style)
	 {
	 case atom_pick_Option_Single:
	 case atom_pick_Option_Subsample:
			 return PickSingle(xPos, yPos, SCENE, curX, curY, curZ);

	 case atom_pick_Option_Multiple:
			 return PickMultiple(xPos, yPos, SCENE, curX, curY, curZ);

	 case atom_pick_Option_Tripoints:
			 return PickTriPoints(xPos, yPos, SCENE, curX, curY, curZ);

	 case atom_pick_Option_Square:
		   switch (m_pick_statu)
		   {
		   case atom_pick_Statu_nopicked:
                return PickSquare(0, xPos, yPos, SCENE, curX, curY, curZ);
		   }
	 }

	return atom_pick_Statu_nopicked;
}

//To process pick single atom in current sample
int VisAtomSample::PickSingle(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	// Space for selection buffer
	GLuint*selectBuff;
   	int i,j,id,n;
	GLuint flag;
	// Hit counter and viewport storeage
	GLint hits, viewport[4];
	double xx0, yy0, zz0, xx1, yy1, zz1, dist;
	int scenestyle;

	if(m_numAtom <= 0) return atom_pick_Statu_nopicked;
	// store the current position
	m_pick_posx = xPos;
	m_pick_posy = yPos;
	// Setup selection buffer
	n = 4*m_numAtom;
	selectBuff = NULL;
	selectBuff = new GLuint[n];
	glSelectBuffer(n, selectBuff);
	// Change render mode
	glRenderMode(GL_SELECT);
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);
	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Establish new clipping volume to be unit cube around
	// mouse cursor point (xPos, yPos) and extending two pixels
	// in the vertical and horzontal direction
	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3]-yPos, 3, 3, viewport);

	// Apply perspective matrix 
	 xx0 = SCENE->GetVolumeX0();
	 yy0 = SCENE->GetVolumeY0();
	 zz0 = SCENE->GetVolumeZ0();
	 xx1 = SCENE->GetVolumeX1();
	 yy1 = SCENE->GetVolumeY1();
	 zz1 = SCENE->GetVolumeZ1();
	 dist = SCENE->GetDistance();
	 scenestyle = SCENE->GetStyle();
	 //be careful to adpted this part according to SCEN->create
     float w =(float)viewport[2];
     float h =(float)viewport[3];
	 if(w == 0.f ) w = 1.f; 
	 if(h == 0.f)  h = 1.f;
	 if((scenestyle&scene_Style_Perspective) != scene_Style_Perspective)
	 {
       double width, height;
	   width = (xx1-xx0);
	   if(width < 0.0) width = -width;
	   height = (yy1-yy0);
	   if(height < 0.0) height = -height;
	   width = std::max(width, height);
	   if (w <= h) 
		glOrtho (xx0, xx0+width, yy0*h/w, (yy0+width)*h/w, zz0, zz1);
	   else 
		glOrtho (xx0*w/h, (xx0+width)*w/h, yy0, (yy0+width), zz0, zz1); 
      }
	 else
	  {
	    double nearz = zz0; 
	    if(nearz < 0.0) nearz = -nearz;
	    if(nearz == 0.0 ) nearz = 1.0; 
		nearz = nearz+dist;
	    double farz =  10.*(zz1-zz0)+nearz; 
	    double ang = atan((yy1-yy0)/nearz/2.0)*180.0/3.1415926; 
        gluPerspective(ang, w/h, nearz, farz);
        glTranslatef(0.f, 0.f, (float)(-2.5*nearz + zz0));
	   }
	 //Initialize names
	 glInitNames();
	 // Draw the scene
     this->Paint();
     glFlush();
	 // Collect the hits
	 hits = glRenderMode(GL_RENDER);
	 if(hits>0)
	 {
	 // To determine which one on the top
        flag = selectBuff[1];
	    id = selectBuff[3];
        for(i=1; i<hits; i++)
	    {
	        j=i*4+1;
	        if(selectBuff[j] < flag) 
		    {
			   flag = selectBuff[j];
			   id = selectBuff[j+2];
		     }
	     } 
		//To check if the atom pciked twice, if no, sset focuse  to other atom
		 if(m_pickedAtomID != (id+1))
		 {
		    //To un select the old picked atom
		    if(m_pickedAtomID > 0)
		    {
		     flag = m_pickedAtomID-1;
	         i = this->m_aVisStat[flag]&atom_Statu_Visiable;
	         j = (this->m_aVisStat[flag]>>2);
	         this->m_aVisStat[flag] = (j<<2);
	         this->m_aVisStat[flag] = this->m_aVisStat[flag]|i;
		    }
		    //To set the picked atom as current and make it as selected
   		    m_pickedAtomID = id+1; 
            this->m_aVisStat[id] = this->m_aVisStat[id]|atom_Statu_Selected;          
	        m_pick_statu = atom_pick_Statu_picked;
		 }
		 else // the current atom has been picked twice
		 {
			//To set the picked atom as current and make it as selected
			id = m_pickedAtomID - 1;
			this->m_aVisStat[id] = this->m_aVisStat[id] | atom_Statu_Selected;
			m_pick_statu = atom_pick_Statu_doublepicked;
		 }
		 // To get the position of current atom
         (*curX) = GetXofCurAtom(); 		 
         (*curY) = GetYofCurAtom();	 
         (*curZ) = GetZofCurAtom();	 
	  }
	  else 
	  {
		 //To un select the old picked atom
		if(m_pickedAtomID > 0)
		{
		   flag = m_pickedAtomID-1;
	       i = this->m_aVisStat[flag]&atom_Statu_Visiable;
	       j = (this->m_aVisStat[flag]>>2);
	       this->m_aVisStat[flag] = (j<<2);
	       this->m_aVisStat[flag] = this->m_aVisStat[flag]|i;
		 }
	     m_pickedAtomID = -1;
	     m_pick_statu = atom_pick_Statu_nopicked;
	  }

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
    delete selectBuff;

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);
	return m_pick_statu;
}


//To process pick mutiple atoms in current sample
int VisAtomSample::PickMultiple(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	// Space for selection buffer
	GLuint*selectBuff;
   	int i,j,id,n;
	GLuint flag;
	// Hit counter and viewport storeage
	GLint hits, viewport[4];
	double xx0, yy0, zz0, xx1, yy1, zz1;
	int scenestyle;

	if(m_numAtom <= 0) return atom_pick_Statu_nopicked;
	// store the current position
	m_pick_posx = xPos;
	m_pick_posy = yPos;
	// Setup selection buffer
	n = 4*m_numAtom;
	selectBuff = NULL;
	selectBuff = new GLuint[n];
	glSelectBuffer(n, selectBuff);
	// Change render mode
	glRenderMode(GL_SELECT);
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);
	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3]-yPos, 3, 3, viewport);

	// Apply perspective matrix 
	 xx0 = SCENE->GetVolumeX0();
	 yy0 = SCENE->GetVolumeY0();
	 zz0 = SCENE->GetVolumeZ0();
	 xx1 = SCENE->GetVolumeX1();
	 yy1 = SCENE->GetVolumeY1();
	 zz1 = SCENE->GetVolumeZ1();
	 scenestyle = SCENE->GetStyle();
     float w =(float)viewport[2];
     float h =(float)viewport[3];
	 if(w == 0.f ) w = 1.f; 
	 if(h == 0.f)  h = 1.f;
	 if((scenestyle&scene_Style_Perspective) != scene_Style_Perspective)
	 {
       double width, height;
	   width = (xx1-xx0);
	   if(width < 0.0) width = -width;
	   height = (yy1-yy0);
	   if(height < 0.0) height = -height;
	   width = std::max(width, height);
	   if (w <= h) 
		glOrtho (xx0, xx0+width, yy0*h/w, (yy0+width)*h/w, zz0, zz1);
	   else 
		glOrtho (xx0*w/h, (xx0+width)*w/h, yy0, (yy0+width), zz0, zz1); 
      }
	 else
	  {
	    double nearz = zz0; 
	    if(nearz < 0.0) nearz = -nearz;
	    if(nearz == 0.0 ) nearz = 1.0; 
	    double farz =  10.*(zz1-zz0)+nearz; 
	    double ang = atan((yy1-yy0)/nearz/2.0)*180.0/3.1415926; 
        gluPerspective(ang, w/h, nearz, farz);
	   }
	 //Initialize names
	 glInitNames();
	 // Draw the scene
     this->Paint();
     glFlush();
	 // Collect the hits
	 hits = glRenderMode(GL_RENDER);
	 if(hits>0)
	 {
	 // To determine which one on the top
        flag = selectBuff[1];
	    id = selectBuff[3];
        for(i=1; i<hits; i++)
	    {
	        j=i*4+1;
	        if(selectBuff[j] < flag) 
		    {
			   flag = selectBuff[j];
			   id = selectBuff[j+2];
		     }
	     } 
		//To check if the atom pciked twice, if no, set focuse  to other atom
		 if(m_pickedAtomID != (id+1))
		 {
		    //To set the picked atom as current and make it as selected
            m_pickedAtomID = id+1; 
			if(this->m_aVisStat[id]&atom_Statu_Selected)
			{
			  m_pick_statu = atom_pick_Statu_doublepicked;
			}
			else
			{
              this->m_aVisStat[id] = this->m_aVisStat[id]|atom_Statu_Selected;          
	          m_pick_statu = atom_pick_Statu_picked;
			}
		 }
		 else // the current atom has been picked twice
		 {
			 id = m_pickedAtomID - 1;
			 this->m_aVisStat[id] = this->m_aVisStat[id] | atom_Statu_Selected;
			 m_pick_statu = atom_pick_Statu_doublepicked;
		 }
		 // To get the position of current atom
         (*curX) = GetXofCurAtom(); 		 
         (*curY) = GetYofCurAtom(); 		 
         (*curZ) = GetZofCurAtom(); 		 
	  }
	  else 
	  {
		 //To un select the old picked atoms
		for(id=0; id<m_numAtom; id++)
		{
	       if(this->m_aVisStat[id]&atom_Statu_Selected)
		   {
		    i = this->m_aVisStat[id]&atom_Statu_Visiable;
	        j = (this->m_aVisStat[id]>>2);
	        this->m_aVisStat[id] = (j<<2);
	        this->m_aVisStat[id] = this->m_aVisStat[id]|i;
		   }
		 }
	     m_pickedAtomID = -1;
	     m_pick_statu = atom_pick_Statu_nopicked;
	  }

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
    delete selectBuff;

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);
	return m_pick_statu;
}


//To pick atoms define by a square
int VisAtomSample::PickSquare(int flag, int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	 static float  left, right, top, bottom;

     double winx, winy, winz;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];

	  glGetIntegerv(GL_VIEWPORT, viewport);

      winx = (double)xPos;
	  winy = (double)(viewport[3]-yPos);
	  winz = 100.0;

	 switch (flag)
	 {
	 case 0:  //start pick
		  unSelect_AllAtoms();
	      left = (float)winx;
	      top =  (float)winy;
		  m_pick_statu = atom_pick_Statu_nopicked;
	      return m_pick_statu;
     case 1:  //Move
	      right = (float)winx;
	      bottom = (float)winy;
		  m_pick_statu = atom_pick_Statu_nopicked;
	      return m_pick_statu;
     case 2:
		 {
		  float xmi, xma, ymi, yma;
		  xmi = std::min(left, right);
		  xma = std::max(left, right);
		  ymi = std::min(top, bottom);
		  yma = std::max(top, bottom);

		  int i;
	      glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	      glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

		  for(i=0; i<m_numAtom; i++)
		  {
	        if(this->m_aVisStat[i]&atom_Statu_Visiable)
			{
               gluProject(m_aPx[i], m_aPy[i], m_aPz[i], modelMatrix, projMatrix, viewport, 
		           &winx, &winy, &winz);
			   if((winx >= xmi) && (winx <= xma)&&(winy >= ymi) && (winy <= yma))
				  this->m_aVisStat[i] = this->m_aVisStat[i]|atom_Statu_Selected;
			}
		  }
		  m_pick_statu = atom_pick_Statu_picked;
	      return m_pick_statu;
		 }
     case 3:
		 {
	      // store the current position
		  float xmi, xma, ymi, yma;
		  xmi = std::min(left, right);
		  xma = std::max(left, right);
		  ymi = std::min(top, bottom);
		  yma = std::max(top, bottom);

		  if((winx >= xmi) && (winx <= xma)&&(winy >= ymi) && (winy <= yma))
		  {
	       m_pick_posx = xPos;
	       m_pick_posy = yPos;
		   m_pick_statu = atom_pick_Statu_doublepicked;
		  }
		  return m_pick_statu;
		 }

	 }
   return atom_pick_Statu_nopicked;
}

//To process pick tri-atoms 
int VisAtomSample::PickTriPoints(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	// Space for selection buffer
	GLuint*selectBuff;
   	int i,j,id,n;
	GLuint flag;
	// Hit counter and viewport storeage
	GLint hits, viewport[4];
	double xx0, yy0, zz0, xx1, yy1, zz1;
	int scenestyle;
	static int tristatu = 0;
	static float v1[3], v2[3], v3[3];

	if(m_numAtom <= 0) return atom_pick_Statu_nopicked;
	if(m_pick_statu == atom_pick_Statu_nopicked) tristatu = 0;
	if(tristatu == 0) unSelect_AllAtoms();

	// store the current position
	m_pick_posx = xPos;
	m_pick_posy = yPos;
	// Setup selection buffer
	n = 4*m_numAtom;
	selectBuff = NULL;
	selectBuff = new GLuint[n];
	glSelectBuffer(n, selectBuff);
	// Change render mode
	glRenderMode(GL_SELECT);
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);
	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3]-yPos, 3, 3, viewport);

	// Apply perspective matrix 
	 xx0 = SCENE->GetVolumeX0();
	 yy0 = SCENE->GetVolumeY0();
	 zz0 = SCENE->GetVolumeZ0();
	 xx1 = SCENE->GetVolumeX1();
	 yy1 = SCENE->GetVolumeY1();
	 zz1 = SCENE->GetVolumeZ1();
	 scenestyle = SCENE->GetStyle();
     float w =(float)viewport[2];
     float h =(float)viewport[3];
	 if(w == 0.f ) w = 1.f; 
	 if(h == 0.f)  h = 1.f;
	 if((scenestyle&scene_Style_Perspective) != scene_Style_Perspective)
	 {
       double width, height;
	   width = (xx1-xx0);
	   if(width < 0.0) width = -width;
	   height = (yy1-yy0);
	   if(height < 0.0) height = -height;
	   width = std::max(width, height);
	   if (w <= h) 
		glOrtho (xx0, xx0+width, yy0*h/w, (yy0+width)*h/w, zz0, zz1);
	   else 
		glOrtho (xx0*w/h, (xx0+width)*w/h, yy0, (yy0+width), zz0, zz1); 
      }
	 else
	  {
	    double nearz = zz0; 
	    if(nearz < 0.0) nearz = -nearz;
	    if(nearz == 0.0 ) nearz = 1.0; 
	    double farz =  10.*(zz1-zz0)+nearz; 
	    double ang = atan((yy1-yy0)/nearz/2.0)*180.0/3.1415926; 
        gluPerspective(ang, w/h, nearz, farz);
	   }
	 //Initialize names
	 glInitNames();
	 // Draw the scene
     this->Paint();
     glFlush();
	 // Collect the hits
	 hits = glRenderMode(GL_RENDER);
	 if(hits>0)
	 {
	 // To determine which one on the top
        flag = selectBuff[1];
	    id = selectBuff[3];
        for(i=1; i<hits; i++)
	    {
	        j=i*4+1;
	        if(selectBuff[j] < flag) 
		    {
			   flag = selectBuff[j];
			   id = selectBuff[j+2];
		     }
	     } 
		//To check if the atom pciked twice, if no, set focuse  to other atom
		 if(m_pickedAtomID != (id+1) )   
		 {
		    //To set the picked atom as current and make it as selected
            m_pickedAtomID = id+1; 
			if(!(this->m_aVisStat[id]&atom_Statu_Selected) )
			{
              this->m_aVisStat[id] = this->m_aVisStat[id]|atom_Statu_Selected;          
			  tristatu ++;
	          m_pick_statu = atom_pick_Statu_picked;
			}
		 }

		 // To get the position of current atom
         (*curX) = GetXofCurAtom(); 		 
         (*curY) = GetYofCurAtom();	 
         (*curZ) = GetZofCurAtom();	 

		 switch (tristatu)
		 {
		 case 1: 
			 {
				 v1[0] = m_aPx[id];
				 v1[1] = m_aPy[id];
				 v1[2] = m_aPz[id];
				 break;
			 }
		 case 2: 
			 {
				 v2[0] = m_aPx[id];
				 v2[1] = m_aPy[id];
				 v2[2] = m_aPz[id];
				 break;
			 }
		 case 3: 
			 {
				 v3[0] = m_aPx[id];
				 v3[1] = m_aPy[id];
				 v3[2] = m_aPz[id];

				 float vv1[3], vv2[3];
				 vv1[0] = v1[0]-v3[0];
				 vv1[1] = v1[1]-v3[1];
				 vv1[2] = v1[2]-v3[2];
				 vv2[0] = v2[0]-v3[0];
				 vv2[1] = v2[1]-v3[1];
				 vv2[2] = v2[2]-v3[2];

				 float a1, a2, a3, d, d1;

				 a1 = vv1[1]*vv2[2]-vv1[2]*vv2[1];
				 a2 = vv1[2]*vv2[0]-vv1[0]*vv2[2];
				 a3 = vv1[0]*vv2[1]-vv1[1]*vv2[0];

				 d = a1*v3[0]+a2*v3[1]+a3*v3[2];
				 for(i=0; i<m_numAtom; i++)
				 {
					d1 = a1*m_aPx[i]+a2*m_aPy[i]+a3*m_aPz[i];
					if(d >= 0)
					{
					  if(d1 >= d) this->m_aVisStat[i] = this->m_aVisStat[i]|atom_Statu_Selected;
					}
					else 
					{
					  if(d1 <= d) this->m_aVisStat[i] = this->m_aVisStat[i]|atom_Statu_Selected;
					}
				 }

		         m_pick_statu = atom_pick_Statu_doublepicked; 
			     tristatu = 0;
				 break;
			 }
		 }
	  }

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
    delete selectBuff;

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);
	return m_pick_statu;
}


//To process move single picked
int VisAtomSample::MovePickedAtom(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	 
	 if(m_pick_statu == atom_pick_Statu_doublepicked) 
	 {
	   switch(m_pick_style)
	    {
	     case atom_pick_Option_Single:
		      return MoveSinglePicked(xPos, yPos, SCENE, curX, curY, curZ);
	     case atom_pick_Option_Multiple:
		      return MoveMultiplePicked(xPos, yPos, SCENE, curX, curY, curZ);
	     case atom_pick_Option_Subsample:
		      return MoveSubsamplePicked(xPos, yPos, SCENE, curX, curY, curZ);
	     //case atom_pick_Option_Square:
         //     return MoveSquarePicked(xPos, yPos, SCENE);
	   }
	  }
	  else
	  {
        switch(m_pick_style)
	    {
	     case atom_pick_Option_Square:
		      return PickSquare(1, xPos, yPos, SCENE,curX, curY, curZ);
	    }
	  }
	return 0;
}


//To process move single picked
int VisAtomSample::MoveSinglePicked(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	  
	 float  x0,y0,z0;
	 float  x1,y1,z1;
     double winx, winy, winz, objx, objy, objz;
	  int ID, Type;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];

	  glGetIntegerv(GL_VIEWPORT, viewport);
	  glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
  
      InformPickedAtom(&ID, &Type, &x0, &y0, &z0);

	  objx = x0;
	  objy = y0;
      objz = z0;
      gluProject(objx, objy, objz, modelMatrix, projMatrix, viewport, 
		           &winx, &winy, &winz);

	  winx = (double)xPos;
	  winy = (double)(viewport[3]-yPos);

      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &objx, &objy, &objz);

	  x1 = objx - x0;
	  y1 = objy - y0;
	  z1 = objz - z0;

      (*curX) = objx;
      (*curY) = objy;
      (*curZ) = objz;
	 if(xPos == m_pick_posx|yPos == m_pick_posy) return sample_Statu_noChange;

	 return Move_aAtom(x1, y1, z1);
}


//To process move single picked
int VisAtomSample::MoveMultiplePicked(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	  
	 float  x0,y0,z0;
	 float  x1,y1,z1;
     double winx, winy, winz, objx, objy, objz;
	  int ID, Type;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];

	  glGetIntegerv(GL_VIEWPORT, viewport);
	  glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
  
      InformPickedAtom(&ID, &Type, &x0, &y0, &z0);

	  objx = x0;
	  objy = y0;
      objz = z0;
      gluProject(objx, objy, objz, modelMatrix, projMatrix, viewport, 
		           &winx, &winy, &winz);

	  winx = (double)xPos;
	  winy = (double)(viewport[3]-yPos);

      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &objx, &objy, &objz);

	  x1 = (float)(objx - x0);
	  y1 = (float)(objy - y0);
	  z1 = (float)(objz - z0);

	  (*curX) = objx;
      (*curY) = objy;
      (*curZ) = objz;

	 if(xPos == m_pick_posx|yPos == m_pick_posy) return sample_Statu_noChange;

	 Move_SelectedAtoms(x1, y1, z1);
	 return 1; 
}

//To process move subsample  picked
int VisAtomSample::MoveSubsamplePicked(int xPos, int yPos, Scene*SCENE, float*curX, float*curY, float*curZ)
	{
	  
	 float  x0,y0,z0;
	 float  x1,y1,z1;
     double winx, winy, winz, objx, objy, objz;
	  int ID, Type;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];

	  glGetIntegerv(GL_VIEWPORT, viewport);
	  glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
  
      InformPickedAtom(&ID, &Type, &x0, &y0, &z0);

	  objx = x0;
	  objy = y0;
      objz = z0;
      gluProject(objx, objy, objz, modelMatrix, projMatrix, viewport, 
		           &winx, &winy, &winz);

	  winx = (double)xPos;
	  winy = (double)(viewport[3]-yPos);

      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &objx, &objy, &objz);

	  x1 = (float)(objx - x0);
	  y1 = (float)(objy - y0);
	  z1 = (float)(objz - z0);


	  (*curX) = objx;
      (*curY) = objy;
      (*curZ) = objz;

	 if(xPos == m_pick_posx|yPos == m_pick_posy) return sample_Statu_noChange;

	 Move_aAtomGroup(x1, y1, z1);
	 return 1; 
}



//To process move and drop 
int VisAtomSample::DropPickedAtom(int xPos, int yPos, Scene*SCENE,float*curX, float*curY, float*curZ)
	{
	 
	  if(m_pick_statu == atom_pick_Statu_doublepicked) 
	  {
	   switch(m_pick_style)
	    {
	     case atom_pick_Option_Single:
	     case atom_pick_Option_Multiple:
	     case atom_pick_Option_Subsample:
         case  atom_pick_Option_Square:
			   return DropSinglePicked(xPos, yPos, SCENE);
         case  atom_pick_Option_Tripoints:
			   return DropSinglePicked(xPos, yPos, SCENE);
	    }
	  }
	 else
	 {
	   if(m_pick_statu == atom_pick_Statu_nopicked)
	   {
	     switch(m_pick_style)
	     {
           case  atom_pick_Option_Square:
		 	     PickSquare(2, xPos, yPos, SCENE, curX, curY, curZ);
				 m_pick_statu = atom_pick_Statu_nopicked;
                 statu = sample_Statu_noChange;
			 return statu;
		 }
	   }
	   ///
	  }
	return 0;
}


//To drop the picked atom to position
int VisAtomSample::DropSinglePicked(int xPos, int yPos, Scene*SCENE)
{
   //int i = MoveSinglePicked(xPos, yPos, SCENE);
   if(m_pick_statu == atom_pick_Statu_nopicked) return sample_Statu_noChange;
   if(xPos == m_pick_posx|yPos == m_pick_posy)    return sample_Statu_noChange;
   m_pick_statu = atom_pick_Statu_nopicked;
   statu = sample_Statu_noChange;
   return sample_Statu_Changed;
}


//To get information about the picked atom
int VisAtomSample::InformPickedAtom(int*ID, int*Type, float*xPos, float*yPos, float*zPos)
{
	 
	 if( (m_pick_statu == atom_pick_Statu_picked) ||(m_pick_statu == atom_pick_Statu_doublepicked)) 
	 {
	   switch(m_pick_style)
	    {
	     case atom_pick_Option_Single:
	     case atom_pick_Option_Multiple:
	     case atom_pick_Option_Subsample:
              return Inf_of_SinglePicked(ID, Type, xPos, yPos, zPos);
	    }
	 }
	return 0;
}

//To get information about the picked atom
int VisAtomSample::Inf_of_SinglePicked(int*ID, int*Type, float*xPos, float*yPos, float*zPos)
{
   if(m_pick_statu == atom_pick_Statu_nopicked) return atom_pick_Statu_nopicked;
   if(m_pickedAtomID <= 0) return atom_pick_Statu_nopicked;
   int i= m_pickedAtomID-1;
   (*ID) = m_pickedAtomID;
   (*Type) = m_aType[i];
   (*xPos) = m_aPx[i];
   (*yPos) = m_aPy[i];
   (*zPos) = m_aPz[i];
   return m_pick_statu;
}


//To get the type of atoms in sample
int VisAtomSample::GetSampleType(int*TYPE)
{
	// i: the index of subsample
	   int i;
	   for(i=0; i<m_numAtom; i++) TYPE[i] = m_aType[i];
	   return m_numAtom;
}
//To get the x coordination of sample x 
int VisAtomSample::GetSampleX(float*X)
{
	// i: the index of subsample
	   int i;
	   for(i=0; i<m_numAtom; i++) X[i] = m_aPx[i];
	   return m_numAtom;
}

//To get the y coordination of sample y 
int VisAtomSample::GetSampleY(float*Y)
{
	// i: the index of subsample
	   int i;
	   for(i=0; i<m_numAtom; i++) Y[i] = m_aPy[i];
	   return m_numAtom;
}

//To get the z coordination of sample z 
int VisAtomSample::GetSampleZ(float*Z)
{
	// i: the index of subsample
	   int i;
	   for(i=0; i<m_numAtom; i++) Z[i] = m_aPz[i];
	   return m_numAtom;
}


//To show a atom with id
inline void  VisAtomSample::Show_aAtom(int id)
{
	int i = id-1;
	if(this->m_aVisStat[i]&atom_Statu_Deleted) return;
	this->m_aVisStat[i] = this->m_aVisStat[i]|atom_Statu_Visiable;
	return;
}
//To show a atom of current picked
inline void  VisAtomSample::Show_aAtom()
{
	 if(m_pickedAtomID <=0) return;
     Show_aAtom(m_pickedAtomID);
}

//To hide a atom with id
inline void  VisAtomSample::Hide_aAtom(int id)
{
	int i = id-1;
	int j;
	j = (this->m_aVisStat[i]>>1);
	this->m_aVisStat[i] = (j<<1);
	return;
}
//To hide current picked atom
inline void  VisAtomSample::Hide_aAtom()
{
	if(m_pickedAtomID <=0) return;
    Hide_aAtom(m_pickedAtomID);
}


//To show the selected atoms
void VisAtomSample::Show_SelectedAtoms( )
{
	   int i; 
	   for(i=0; i<m_numAtom; i++)
	      if(this->m_aVisStat[i]&atom_Statu_Selected)Show_aAtom(i+1); 
	   return;

}

//To hide the selected atoms
void VisAtomSample::Hide_SelectedAtoms( )
{
	   int i,j; 
	   for(i=0; i<m_numAtom; i++)
	   {
	      if(this->m_aVisStat[i]&atom_Statu_Selected)
		  {  
	        j = (this->m_aVisStat[i]>>2);
	        this->m_aVisStat[i] = (j<<2);
		  }
	   }
	   return;
}


//To show the unselected atoms
void VisAtomSample::Show_unSelectedAtoms( )
{
	   int i; 
	   for(i=0; i<m_numAtom; i++)
	      if(!(this->m_aVisStat[i]&atom_Statu_Selected)) Show_aAtom(i+1); 
	   return;
}

//To show the unselected atoms
void VisAtomSample::Hide_unSelectedAtoms( )
{
	   int i; 
	   for(i=0; i<m_numAtom; i++)
	      if(!(this->m_aVisStat[i]&atom_Statu_Selected)) Hide_aAtom(i+1); 
	   return;
}

//To show all atoms in active sample
void VisAtomSample::Show_AllAtoms( )
{
	   int i; 
	   for(i=0; i<m_numAtom; i++) Show_aAtom(i+1); 
	   return;
}

//To hide all atoms in active sample
void VisAtomSample::Hide_AllAtoms( )
{
	   int i; 
	   for(i=0; i<m_numAtom; i++) Hide_aAtom(i+1); 
	   return;
}

/****************************************************************
  New 2013 -09
  to import temp data in to bufff. the data will be selected 
  interactively, for positions energies etc, for atoms
*/
void  VisAtomSample::SetDataSheet(VisAtomDataSheet*pData)
{     
     //if we have old datasheet, the reserve the display policy
	 //for the present datasheet
	  this->Clear(true);
	  this->m_pData = pData;
    
	  this->m_numAtom    = pData->GetNumRow();
	  this->m_aType      = new char[m_numAtom];   
	  this->m_aPx        = new float[m_numAtom];  
	  this->m_aPy        = new float[m_numAtom];
	  this->m_aPz        = new float[m_numAtom];  
	  this->m_aVisStat   = new char [m_numAtom]; 
	  this->m_aRadius    = new float[m_numAtom];
	  this->m_aRed       = new float[m_numAtom];
	  this->m_aGreen     = new float[m_numAtom];
	  this->m_aBlue      = new float[m_numAtom];
	  this->m_aRedex     = new float[m_numAtom];
	  this->m_aGreenex   = new float[m_numAtom];
	  this->m_aBlueex    = new float[m_numAtom];
	  this->m_aGeomStyle = new int[m_numAtom];
	  this->m_vPx        = new float[m_numAtom];  memset(this->m_vPx, 0, sizeof(float)*this->m_numAtom);
	  this->m_vPy        = new float[m_numAtom];  memset(this->m_vPy, 0, sizeof(float)*this->m_numAtom);
	  this->m_vPz        = new float[m_numAtom];  memset(this->m_vPz, 0, sizeof(float)*this->m_numAtom);

       //We also set the data sheet for mapping-color
	   VisAtomVisualizingPolicy::SetDataSheet(pData);

	  int i,j, col;
	  float*dcol;

	  //set type
	  col = m_pData->GetTypeCol();
	  if(col >= 0)
	  {
	     dcol = m_pData->GetData(col);
	     for(i=0; i<m_numAtom; i++)m_aType[i] = dcol[i]+0.00001;
	  }
	  else
         for(i=0; i<m_numAtom; i++)m_aType[i] = 1;

	  //set position
	  col = m_pData->GetPosXCol();
	  dcol = m_pData->GetData(col);
	  for(i=0; i<m_numAtom; i++)m_aPx[i] = dcol[i];
	  
	  col = m_pData->GetPosYCol();
	  dcol = m_pData->GetData(col);
	  for(i=0; i<m_numAtom; i++)m_aPy[i] = dcol[i];
	  
	  col = m_pData->GetPosZCol();
	  dcol = m_pData->GetData(col);
	  for(i=0; i<m_numAtom; i++)m_aPz[i] = dcol[i];

	  //Update the sample size
	  if(pData->HasBoxSize()) 
	  {
		  pData->GetBoxSize(this->m_aXmi, this->m_aXmx, 
		                    this->m_aYmi, this->m_aYmx, 
		                    this->m_aZmi, this->m_aZmx); 
	  }
	  else
	     this->AutoSampleBoxRange();
      
	  //set all visible
	  for(i=0; i<m_numAtom; i++) this->m_aVisStat[i] = VisAtomSubSample::SUBSAMPLE_VISIABLE;
	  //determine how many kind of atom we have
      int nmin,nmax;
	  nmax = 0; 	nmin = 10000000;
	  for(i=0; i<m_numAtom; i++)  
	  {
	  	 if(m_aType[i] > nmax) nmax = m_aType[i];
			else if(m_aType[i] < nmin) nmin = m_aType[i];
	  }
      //if there is already subsample list, use old subsample information
      //otherwise, create defaut subsamples,a subsample is constructed by atoms of the same type
	  int ns = this->m_subSamples.count()+1;
      float rad, scal;
	  this->GetDefaultAtomRadiu(rad, scal);
      for(i=ns; i<=nmax; i++)
	  {
          VisAtomSubSample*p = new VisAtomSubSample(i);
	      this->m_subSamples.append(p);
		  this->m_selSubsamples.append(0);
          // create default properties for this subsample
		  p->SetCurAtomSize(rad);
	   }
	   //restore the display policy  of subsamples
	  for(i=0; i<this->m_subSamples.count(); i++)
	  {
		  this->m_subSamples[i]->SetDataSheet(pData);
		  if(VisAtomVisualizingPolicy::GetScalarCol() < 0)
		  {
		     this->m_subSamples[i]->SetCurVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		  }
		  //this->m_subSamples[i]->SetCurVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
 	      this->LoadSubsampleProp(i);
	  }
	   this->SetCurrentSubsample(0); 


   	  //to update the projection box size
	  if(VisAtomVisualizingPolicy::IsAutoProjectionBox()) 
  	  {
		VisAtomVisualizingPolicy::SetProjectionBox(this->m_aXmi, this->m_aXmx, this->m_aYmi, this->m_aYmx, this->m_aZmi, this->m_aZmx);
	  }
      return;
}

void  VisAtomSample::LoadSubsampleProp(int is)
{
       float r, g, b, a, rad;
	   int i,  geomstyle;
	   VisAtomSubSample*p = this->m_subSamples[is];
	   geomstyle = p->GetCurGeometryStyle();

	   int col[3], hasvect;
	   float*vpx, *vpy, *vpz, vrad, vlenscal;
	   VisAtomVisualizingPolicy::GetVectorCol(col);
	   hasvect = VisAtomVisualizingPolicy::HasVectorData();
	   if(hasvect)
	   {
	      vpx = this->m_pData->GetData(col[0]);
	      vpy = this->m_pData->GetData(col[1]);
	      vpz = this->m_pData->GetData(col[2]);
	      VisAtomVisualizingPolicy::GetVectorSize(vrad, vlenscal);
	   }
		   
          for(i=0; i<this->m_numAtom; i++) 
	      {
			  if(p->CheckMembership(this->m_aType[i], this->m_aPx[i], this->m_aPy[i], this->m_aPz[i]))
			  {
                //load basic atom properties
				p->GetCurAtomSize(i, this->m_aRadius[i]);
		        //this->m_aRadius[i]  = rad;
	            this->m_aGeomStyle[i] = geomstyle;
				if((geomstyle&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK) == VisAtomVisualizingPolicy::SHAPE_STYLE_WIRE)
				{
				   if(p->GetCurVisualPolicy()->GetColorStyle() == VisAtomVisualizingPolicy::COLOR_STYLE_MAP)
				   {
	                  p->GetCurColor(r, g, b, a, i);
		              this->m_aRed[i]     = r;
		              this->m_aGreen[i]   = g;
		              this->m_aBlue[i]    = b;

	                   p->GetColor(r, g, b, a, i, 0);
		              this->m_aRedex[i]     = r;
		              this->m_aGreenex[i]   = g;
		              this->m_aBlueex[i]    = b;
				    }   
				    else if(p->GetCurVisualPolicy()->GetColorStyle() == VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)
				   {
	                  p->GetCurColor(r, g, b, a, i);
		              this->m_aRed[i]     = r ;
		              this->m_aGreen[i]   = g;
		              this->m_aBlue[i]    = b;

		              this->m_aRedex[i]   = 1.f - r;
		              this->m_aGreenex[i] = 1.f - g;
		              this->m_aBlueex[i]  = 1.f - b;
			       }
				}
				else
				{
	               p->GetCurColor(r, g, b, a, i);
		           this->m_aRed[i]     = r;
		           this->m_aGreen[i]   = g;
		           this->m_aBlue[i]    = b;
				}
			  }
			  //load vector properties if we have vector data
			  if(hasvect)
			  {
				  this->m_vPx[i] = vpx[i]*vlenscal;
				  this->m_vPy[i] = vpy[i]*vlenscal;
				  this->m_vPz[i] = vpz[i]*vlenscal;
			  }
              
		  }
		  return;
}

void  VisAtomSample::SetCliper(QList<VisAtomCliper*>cliper)
{
	
	int i;

	this->m_pCliper.clear();
	for(i=0; i<cliper.size(); i++)
	{
		this->AddCliper(cliper[i]);
	}
	this->ResetVisibility();
}


void  VisAtomSample::AddCliper(VisAtomCliper*cliper)
{
	switch (cliper->GetStyle())
	{
  	   case VisAtomCliper::CLIPPING_STYLE_OR:
	   {
		   this->m_pCliper.clear(); 
		   break;
	   }
	   case VisAtomCliper::CLIPPING_STYLE_AND:
	   {
		   this->m_pCliper.clear(); 
		   break;
	   }
	   case VisAtomCliper::CLIPPING_STYLE_ANYDIR:
	   {
		    break;
	   }

	 }
	 this->m_pCliper.append(cliper);
}


void  VisAtomSample::ResetVisibility()
{
      //first we should make all atoms visible
      int i,j;
	  for(i = 0; i < this->m_numAtom; i++)
	  {
	     this->m_aVisStat[i] = atom_Statu_Visiable|this->m_aVisStat[i];
	  }

	  //check if hide by subsample property
	  for(j=0; j<this->m_subSamples.count(); j++)
	  {
		  VisAtomSubSample*p = this->m_subSamples[j];
		  if(!p->IsVisible())
		  {
	         for(i=0; i<this->m_numAtom; i++)
			 {   
				 if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				 if(p->CheckMembership(this->m_aType[i], this->m_aPx[i], this->m_aPy[i], this->m_aPz[i]))
                    this->SetStatAsHide(this->m_aVisStat[i]);   
			 }
		  }
	  }

      //check if atoms are clipped
	  if(!this->m_pCliper.isEmpty()) this->ResetVisibilityByCliper();

	  //check if the atom are in invisible region
	  return;
}


void  VisAtomSample::ResetVisibilityByCliper()
{

	  VisAtomCliper*cliper;
	  cliper = this->m_pCliper[0];
	  int style = cliper->GetStyle();
	  
      switch(style)
	  {
	      case VisAtomCliper::CLIPPING_STYLE_OR:
		  {
              //this->ResetVisibilityByCliper_OR(cliper);
			  this->ResetVisibilityByCliper_OR_ALSO(cliper);
			  break;
		  }
	      case VisAtomCliper::CLIPPING_STYLE_AND:
		  {
              this->ResetVisibilityByCliper_AND(cliper);
			  break;
		  }
		  case VisAtomCliper::CLIPPING_STYLE_ANYDIR:
		  {
		      ResetVisibilityByCliper_ANYDIR();
			  break;
		  }
	  }
	  return;
}

void  VisAtomSample::ResetVisibilityByCliper_OR(VisAtomCliper*cliper)
{
	  
	 if(!cliper->IsEnabledClipX() && 
	    !cliper->IsEnabledClipY() && 
	    !cliper->IsEnabledClipZ()) return;

	  int i;
	  double pos0[3], pos1[3], dp[3];
	  cliper->GetClipThickness(dp);
  	  cliper->GetClipPos(pos0);
	  for(i=0; i<3; i++)
	  {
		  pos0[i] = pos0[i] + dp[i]*0.5;
          pos1[i] = pos0[i] - dp[i];
	  }
     
      //NOTE: the atom has been made visible in root ResetVisibility
	  //**************************   Check in X axsis
	  if(cliper->IsEnabledClipX())
	  {
         //for double clipping
		 if(cliper->IsEnabledDClipX())
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos0[0],  1.0, this->m_aPx[i]) ||
				   IsCliped(pos1[0], -1.0, this->m_aPx[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		  }
		 else if(cliper->IsInverseClipX())
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos1[0], -1.0, this->m_aPx[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		 } 
		 else 
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos0[0], 1.0, this->m_aPx[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		 }  
	  }//******************  End of Checking in X axsis

	  //**********************  Check in Y axsis
	  if(cliper->IsEnabledClipY())
	  {
         //for double clipping
		 if(cliper->IsEnabledDClipY())
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos0[1],  1.0, this->m_aPy[i]) ||
				   IsCliped(pos1[1], -1.0, this->m_aPy[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		  }
		 else if(cliper->IsInverseClipY())
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos1[1], -1.0, this->m_aPy[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		 } 
		 else 
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos0[1], 1.0, this->m_aPy[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		 } 
	  } //********************************** End of Checking in Y axsis

	  //**********************  Check in Z axsis
	  if(cliper->IsEnabledClipZ())
	  {
         //for double clipping
		 if(cliper->IsEnabledDClipZ())
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos0[2],  1.0, this->m_aPz[i]) ||
				   IsCliped(pos1[2], -1.0, this->m_aPz[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		  }
		 else if(cliper->IsInverseClipZ())
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos1[2], -1.0, this->m_aPz[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		 } 
		 else 
		 {
            for(i=0; i<this->m_numAtom; i++) 
			{
				//if the atom is alread invisible, no clipping check is needed
				if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;
				if(IsCliped(pos0[2], 1.0, this->m_aPz[i])) this->SetStatAsHide(this->m_aVisStat[i]);
			}
		 }  //********************************** End of Checking in Z axsis
	   }
}

//**** An alternative algorithm for OR-clipping 
void VisAtomSample::ResetVisibilityByCliper_OR_ALSO(VisAtomCliper*cliper)
 {
	  
	 if(!cliper->IsEnabledClipX() && 
	    !cliper->IsEnabledClipY() && 
	    !cliper->IsEnabledClipZ()) return;


	  int i, stat, n0[3], n1[3];
	  double pos0[3], pos1[3], dp[3];


	  cliper->GetClipThickness(dp);
  	  cliper->GetClipPos(pos0);
	  for(i=0; i<3; i++)
	  {
		  pos0[i] = pos0[i] + dp[i]*0.5;
          pos1[i] = pos0[i] - dp[i];
		  n0[i] = 0;
		  n1[i] =0;
	  }

	  if(cliper->IsEnabledClipX())
	  {
		  if(cliper->IsEnabledDClipX()) { n0[0] = n1[0] =1;}
		  else if(cliper->IsInverseClipX()) { n0[0] = 0; n1[0] = 1;}
		  else { n0[0] = 1; n1[0] = 0; }
	  }

	  if(cliper->IsEnabledClipY())
	  {
		  if(cliper->IsEnabledDClipY()) { n0[1] = n1[1] =1;}
		  else if(cliper->IsInverseClipY()) { n0[1] = 0; n1[1] = 1;}
		  else { n0[1] = 1; n1[1] = 0; }
	  };
      
	  if(cliper->IsEnabledClipZ())
	  {
		  if(cliper->IsEnabledDClipZ()) { n0[2] = n1[2] =1;}
		  else if(cliper->IsInverseClipZ()) { n0[2] = 0; n1[2] = 1;}
		  else { n0[2] = 1; n1[2] = 0; }
	  };

	  for(i=0; i<this->m_numAtom; i++)
	  {
          if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;

		  //Check if the atom has been clipped
		  stat = 
			     (n0[0]&&IsCliped(pos0[0],  1.0, this->m_aPx[i]) ) |
			     (n0[1]&&IsCliped(pos0[1],  1.0, this->m_aPy[i]) ) |
			     (n0[2]&&IsCliped(pos0[2],  1.0, this->m_aPz[i]) ) |
			     (n1[0]&&IsCliped(pos1[0], -1.0, this->m_aPx[i]) ) |
				 (n1[1]&&IsCliped(pos1[1], -1.0, this->m_aPy[i]) ) |
			     (n1[2]&&IsCliped(pos1[2], -1.0, this->m_aPz[i]) ); 
         //if stat is true, hade the atom
		  if(stat) this->SetStatAsHide(this->m_aVisStat[i]);
	  }

 }


//**********************************************
 void VisAtomSample::ResetVisibilityByCliper_AND(VisAtomCliper*cliper)
 {
	  int i, stat, deb;
	  double pos0[3], pos1[3], dp[3],vol[8][3];

	  if(!cliper->IsEnabledClipX() && 
		 !cliper->IsEnabledClipY() && 
		 !cliper->IsEnabledClipZ()) return;

	  cliper->GetClipThickness(dp);
  	  cliper->GetClipPos(pos0);
	  for(i=0; i<3; i++)
	  {
		  pos0[i] = pos0[i] + dp[i]*0.5;
          pos1[i] = pos0[i] - dp[i];
	  }

      //**************
	  if(cliper->IsEnabledClipX())
	  {
		 if(!cliper->IsEnabledDClipX())
		 {
		    if(cliper->IsInverseClipX()) 
			{
				vol[0][0] = vol[2][0] = vol[4][0] = vol[6][0] = 1.e18;
				vol[1][0] = vol[3][0] = vol[5][0] = vol[7][0] = pos1[0];
			}
			else 
			{
				vol[0][0] = vol[2][0] = vol[4][0] = vol[6][0] = pos0[0];
				vol[1][0] = vol[3][0] = vol[5][0] = vol[7][0] = -1.e18;
			}
		 }
		 else
		 {
	      vol[0][0] = vol[2][0] = vol[4][0] = vol[6][0] = pos0[0];
	      vol[1][0] = vol[3][0] = vol[5][0] = vol[7][0] = pos1[0];
		 }
	  }
	  else
	  {
		  vol[0][0] = vol[2][0] = vol[4][0] = vol[6][0] = -1.e18;
	      vol[1][0] = vol[3][0] = vol[5][0] = vol[7][0] =  1.e18;
	  }

	  //**************
	  if(cliper->IsEnabledClipY())
	  {
		 if(!cliper->IsEnabledDClipY())
		 {
		    if(cliper->IsInverseClipY()) 
			{
				vol[0][1] = vol[1][1] = vol[4][1] = vol[5][1] = 1.e18;
				vol[2][1] = vol[3][1] = vol[6][1] = vol[7][1] = pos1[1];
			}
			else 
			{
				vol[0][1] = vol[1][1] = vol[4][1] = vol[5][1] = pos0[1];
				vol[2][1] = vol[3][1] = vol[6][1] = vol[7][1] = -1.e18;
			}
		 }
		 else
		 {
		    vol[0][1] = vol[1][1] = vol[4][1] = vol[5][1] = pos0[1];
	        vol[2][1] = vol[3][1] = vol[6][1] = vol[7][1] = pos1[1];
		 }
	  }
	  else
	  {
		  vol[0][1] = vol[1][1] = vol[4][1] = vol[5][1] = -1.e18;
	      vol[2][1] = vol[3][1] = vol[6][1] = vol[7][1] =  1.e18;
	  }

//**************
if (cliper->IsEnabledClipZ())
{
	if (!cliper->IsEnabledDClipZ())
	{
		if (cliper->IsInverseClipZ())
		{
			vol[0][2] = vol[1][2] = vol[2][2] = vol[3][2] = 1.e18;
			vol[4][2] = vol[5][2] = vol[6][2] = vol[7][2] = pos1[2];
		}
		else
		{
			vol[0][2] = vol[1][2] = vol[2][2] = vol[3][2] = pos0[2];
			vol[4][2] = vol[5][2] = vol[6][2] = vol[7][2] = -1.e18;
		}
	}
	else
	{
		vol[0][2] = vol[1][2] = vol[2][2] = vol[3][2] = pos0[2];
		vol[4][2] = vol[5][2] = vol[6][2] = vol[7][2] = pos1[2];
	}
}
else
{
	vol[0][2] = vol[1][2] = vol[2][2] = vol[3][2] = -1.e18;
	vol[4][2] = vol[5][2] = vol[6][2] = vol[7][2] = 1.e18;
}


//1st region
for (i = 0; i < this->m_numAtom; i++)
{
	if (!(this->m_aVisStat[i] & atom_Statu_Visiable)) continue;

	//Check if the atom has been clipped
	deb = 0;
	stat = (IsCliped(vol[0][0], 1.0, this->m_aPx[i])&
		IsCliped(vol[0][1], 1.0, this->m_aPy[i])&
		IsCliped(vol[0][2], 1.0, this->m_aPz[i]));
	deb = deb | stat;

	stat = (IsCliped(vol[1][0], -1.0, this->m_aPx[i])&
		IsCliped(vol[1][1], 1.0, this->m_aPy[i])&
		IsCliped(vol[1][2], 1.0, this->m_aPz[i]));
	deb = deb | stat;

	stat = (IsCliped(vol[2][0], 1.0, this->m_aPx[i])&
		IsCliped(vol[2][1], -1.0, this->m_aPy[i])&
		IsCliped(vol[2][2], 1.0, this->m_aPz[i]));
	deb = deb | stat;

	stat = (IsCliped(vol[3][0], -1.0, this->m_aPx[i])&
		IsCliped(vol[3][1], -1.0, this->m_aPy[i])&
		IsCliped(vol[3][2], 1.0, this->m_aPz[i]));
	deb = deb | stat;

	stat = (IsCliped(vol[4][0], 1.0, this->m_aPx[i])&
		IsCliped(vol[4][1], 1.0, this->m_aPy[i])&
		IsCliped(vol[4][2], -1.0, this->m_aPz[i]));
	deb = deb | stat;

	stat = (IsCliped(vol[5][0], -1.0, this->m_aPx[i])&
		IsCliped(vol[5][1], 1.0, this->m_aPy[i])&
		IsCliped(vol[5][2], -1.0, this->m_aPz[i]));
	deb = deb | stat;

	stat = (IsCliped(vol[6][0], 1.0, this->m_aPx[i])&
		IsCliped(vol[6][1], -1.0, this->m_aPy[i])&
		IsCliped(vol[6][2], -1.0, this->m_aPz[i]));
	deb = deb | stat;


	stat = (IsCliped(vol[7][0], -1.0, this->m_aPx[i])&
		IsCliped(vol[7][1], -1.0, this->m_aPy[i])&
		IsCliped(vol[7][2], -1.0, this->m_aPz[i]));
	deb = deb | stat;

	if (deb) this->SetStatAsHide(this->m_aVisStat[i]);
}

 }

 //********************************************************************************
 void VisAtomSample::ResetVisibilityByCliper_ANYDIR()
 {
	 VisAtomCliper*cliper;
	 int nc, i, j, stat, deb, inplane;
	 double dist, norm;
	 double *posx, *posy, *posz, *dirx, *diry, *dirz, *htu, *htl;
	 

	 nc = this->m_pCliper.size();
	 posx = new double[nc];
	 posy = new double[nc];
	 posz = new double[nc];
	 dirx = new double[nc];
	 diry = new double[nc];
	 dirz = new double[nc];
	 htl = new double[nc];
	 htu = new double[nc];
	 for(j = 0; j < nc; j++)
	 {
		 cliper = this->m_pCliper[j];
		 cliper->GetClipAxsis(dirx[j], diry[j], dirz[j]);

		 //normalize the norm vector
		 norm = dirx[j] * dirx[j] + diry[j] * diry[j] + dirz[j] * dirz[j];
		 if (norm < 0.0001) norm = 1;
		 norm = pow(norm, 0.5);
		 dirx[j] = dirx[j] / norm;
		 diry[j] = diry[j] / norm;
		 dirz[j] = dirz[j] / norm;
		 //NOTE: for this case, only one clipping plane, thus only dp[0] is the thickness;
		 if (cliper->IsEnabledClip())
		 {
			 if (cliper->IsEnabledDClip())
			 {
				 htl[j] = -cliper->GetClipThickness()*0.5;
				 htu[j] = cliper->GetClipThickness()*0.5;
			 }
			 else if (cliper->IsInverseClipX() || cliper->IsInverseClipY() || cliper->IsInverseClipZ())
			 {
				 htl[j] = -cliper->GetClipThickness()*0.5;
				 htu[j] = 1.e64;
			 }
			 else
			 {
				 htl[j] = -1.e64;
				 htu[j] = cliper->GetClipThickness()*0.5;
			 }
		 }
		 else
		 {
			 htl[j] =  1.e64;
			 htu[j] = -1.e64;
		 }
		 //Get the center of clipping plane
		 //cliper->GetClipAbsolutePos(pos0);
		 cliper->GetClipPos(posx[j], posy[j], posz[j]);
		 //if (abs(dir[0]) < 0.001 && abs(dir[1]) < 0.001 && abs(dir[2]) < 0.001) return;
      }

      
      for(i=0; i<this->m_numAtom; i++)
	  {
          if(!(this->m_aVisStat[i]&atom_Statu_Visiable)) continue;

		  inplane = 0;
          for(j=0; j<nc; j++)
		  {
			  dist  = (this->m_aPx[i] - posx[j])*dirx[j] +
				      (this->m_aPy[i] - posy[j])*diry[j] +
				      (this->m_aPz[i] - posz[j])*dirz[j];
			  if (htl[j] <= dist && dist <= htu[j])
			  {
				  inplane = j + 1;
			  }
		  }
		  if(!inplane) this->SetStatAsHide(this->m_aVisStat[i]);
		  /*dist = (this->m_aPx[i]-pos0[0])*dir[0] +
			     (this->m_aPy[i]-pos0[1])*dir[1] +
			     (this->m_aPz[i]-pos0[2])*dir[2];

		  if(cliper->IsEnabledDClipX()||cliper->IsEnabledDClipY()||cliper->IsEnabledDClipZ())
		  {
		     if( dist>ht || dist<-ht) this->SetStatAsHide(this->m_aVisStat[i]);
		  }
		  else if(cliper->IsInverseClipX()||cliper->IsInverseClipY()||cliper->IsInverseClipZ())
		  {
             if( dist< -ht) this->SetStatAsHide(this->m_aVisStat[i]);   
		  }
		  else
		  {
			 if( dist> ht) this->SetStatAsHide(this->m_aVisStat[i]);   
		  }*/

	  }
	  delete posx;
	  delete posy;
	  delete posz;
	  delete dirx;
	  delete diry;
	  delete dirz;
	  delete htl;
	  delete htu;
 }

//********************************************************************************
//To get the range of the whole box along x 
void VisAtomSample::AutoSampleBoxRange()
{
	// i: the index of subsample
	   int i;
	   this->m_aXmi = 9.E20;
	   this->m_aXmx =-9.E20;
	   this->m_aYmi = 9.E20;
	   this->m_aYmx =-9.E20;
	   this->m_aZmi = 9.E20;
	   this->m_aZmx =-9.E20;
	   for(i=0; i<this->m_numAtom; i++)
	   {
		   if(this->m_aXmi > this->m_aPx[i]) this->m_aXmi = this->m_aPx[i];
		   if(this->m_aXmx < this->m_aPx[i]) this->m_aXmx = this->m_aPx[i];

		   if(this->m_aYmi > this->m_aPy[i]) this->m_aYmi = this->m_aPy[i];
		   if(this->m_aYmx < this->m_aPy[i]) this->m_aYmx = this->m_aPy[i];

		   if(this->m_aZmi > this->m_aPz[i]) this->m_aZmi = this->m_aPz[i];
		   if(this->m_aZmx < this->m_aPz[i]) this->m_aZmx = this->m_aPz[i];
	   }
       this->m_aXmx = std::max(abs(this->m_aXmx), abs(this->m_aXmi));
       this->m_aXmi = -this->m_aXmx;
       this->m_aYmx = std::max(abs(this->m_aYmx), abs(this->m_aYmi));
       this->m_aYmi = -this->m_aYmx;
       this->m_aZmx = std::max(abs(this->m_aZmx), abs(this->m_aZmi));
       this->m_aZmi = -this->m_aZmx;
       return;
}

//********************************************************************************
void VisAtomSample::GetDefaultAtomRadiu(float&radiu, float&scal)
{
    //automatically set the radius of atoms
	float rn = pow((double)this->m_numAtom, 0.3333333);
	float ds = this->m_aXmx - this->m_aXmi;

		if(ds > this->m_aYmx - this->m_aYmi) ds = this->m_aYmx - this->m_aYmi;
		if(ds > this->m_aZmx - this->m_aZmi) ds = this->m_aZmx - this->m_aZmi;

		radiu = 0.5*ds/rn;
		scal  = radiu/(ds/4.0);
		return; 
}


//********************************************************************************
VisAtomVisualizingPolicy*VisAtomSample::GetSubsampleVisualPolicy(QString name, int policy)
{
	//to find out he subsample of name 
	int i;
	VisAtomSubSample*p;
	for(i=0; i<this->m_subSamples.count(); i++)
	{
		p =  this->m_subSamples[i];
        if(name == p->GetName()) break;
	}
	if(policy < 0) return p->GetCurVisualPolicy();
	else return p->GetVisualPolicy(policy);
}

//********************************************************************************
void  VisAtomSample::GetDefaultVectorSize(float&lenscal, float&rad, float&arrowl, float&arrowr)
{
    //automatically set the radius of atoms
	float rn = pow((double)this->m_numAtom, 0.3333333);
	float ds = this->m_aXmx - this->m_aXmi;

		if(ds > this->m_aYmx - this->m_aYmi) ds = this->m_aYmx - this->m_aYmi;
		if(ds > this->m_aZmx - this->m_aZmi) ds = this->m_aZmx - this->m_aZmi;
		rad    = 0.025*ds/rn;
		arrowr = rad*2.0;
		arrowl = 1.7321*arrowr;

		int col[3];
		VisAtomVisualizingPolicy::GetVectorCol(col);
		float*vx = this->m_pData->GetData(col[0]);
		float*vy = this->m_pData->GetData(col[1]);
		float*vz = this->m_pData->GetData(col[2]);
        int i;
		float len, lmx;

		lmx = -1.e20;
		for(i=0; i<this->m_pData->GetNumRow(); i++)
		{
            len = vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
			if(len > lmx) lmx = len;
		}
		len = pow((double)lmx, 0.5);
		lenscal = ds/rn/len;
		return; 
}
//********************************************************************************

#define MAXCOL  64
#define MAXSTRL 32
void VisAtomSample::DrawWignerSeizVolume(QString fname)
{
     

		 QFile file(fname);
         file.open(QIODevice::ReadOnly|QIODevice::Text);

		 Arrowline  V=Arrowline();
         Style  Sty;
         glEnable(GL_DEPTH_TEST);
		 glEnable(GL_LIGHTING);
	     Sty.Solid();
         glEnable(GL_COLOR_MATERIAL);
         glEnable(GL_NORMALIZE);
         glLineWidth(4.f); 
		 //to determine how many col for each line
         char buf[MAXCOL*MAXSTRL];
		 float pos0[3], norm1[3], norm0[3],verts[120][3];
		 int  id, nfaces, i, j, ip, nverts, i1, i2, index, ityp;
		 
		 ip = 0;
         while (!file.atEnd()) 
		 {
			 file.readLine(buf,  MAXCOL*MAXSTRL);
			 file.readLine(buf,  MAXCOL*MAXSTRL);
			 sscanf(buf, "%i %i %i %e %e %e", &id, &index, &ityp,  &pos0[0], &pos0[1], &pos0[2]);
			 file.readLine(buf,  MAXCOL*MAXSTRL);
			 sscanf(buf, "%i ", &nfaces);
			 glPushMatrix();
			 //glTranslatef(pos0[0], pos0[1], pos0[2]);
			 for(i=0; i<nfaces; i++)
			 {
			     file.readLine(buf,  MAXCOL*MAXSTRL);
			     sscanf(buf, "%i %i %e %e %e ", &nverts, &ityp, &norm0[0], &norm0[1], &norm0[2]);
				 for(j=0; j<nverts; j++)
				 {
			       file.readLine(buf,  MAXCOL*MAXSTRL);
			       sscanf(buf, "%i %i  %e %e %e", &i1, &i2, &verts[j][0], &verts[j][1], &verts[j][2]);
				 }
				 if(i > 0) continue;
				 //determine the normal
				 norm1[0] = (verts[1][1]-verts[0][1])*(verts[2][2]-verts[1][2])-(verts[1][2]-verts[0][2])*(verts[2][1]-verts[1][1]);
				 norm1[1] = (verts[1][2]-verts[0][2])*(verts[2][0]-verts[1][0])-(verts[1][0]-verts[0][0])*(verts[2][2]-verts[1][2]);
				 norm1[2] = (verts[1][0]-verts[0][0])*(verts[2][1]-verts[1][1])-(verts[1][1]-verts[0][1])*(verts[2][0]-verts[1][0]);

		         glBegin(GL_POLYGON);
			     glColor3f((float)i/5.f+0.1, 0.f, 0.f);
				 
				 if(norm1[0]*norm0[0]+norm1[1]*norm0[1]+norm1[2]*norm0[2]>0)
				 {
                    glNormal3f(norm0[0], norm0[1], norm0[2]);
				    for(j=0; j<nverts; j++) glVertex3d(verts[j][0], verts[j][1], verts[j][2]);
				 }
				 else
				 {
                    glNormal3f(-norm0[0], -norm0[1], -norm0[2]);
				    for(j=nverts-1; j>=0; j--) glVertex3d(verts[j][0], verts[j][1], verts[j][2]);
					}
				 glEnd();

				 //to begine draw the face
		          /*glBegin(GL_LINE_LOOP);
			      glColor3f(0.0f, 0.f, 0.f);
				  glNormal3f(norm0[0], norm0[1], norm0[2]);

				 for(j=0; j<nverts; j++)
				 {
					glVertex3d(verts[j][0], verts[j][1], verts[j][2]);
				 }
				 glEnd();*/
				 //if(i == 3)
				 {
				 glLineWidth(4.f); 
				 for(j=0; j<nverts-1; j++)
				 {
					 V.ResetStartPoint(verts[j][0], verts[j][1], verts[j][2]);
					 V.ResetEndPoint(verts[j+1][0], verts[j+1][1], verts[j+1][2]);
					 V.ChangeSize(0.01f);
					 V.Paint();
				 }
				 }

				 
			 }
			 glPopMatrix();

			 break;
		   ip++;
		   if(ip>=100) break; 	 
         }
		 file.close();
}



void VisAtomSample::AttachTrajectory(VisAtomTrajectory*pTraject)
{   
	 this->m_pTrajectory = pTraject;
}

void  VisAtomSample::SetShowOnlyAtomsFound(int showonly)
{
	if (!showonly)
	{
		this->ResetVisibility();
	 }
	else
	{
		int i;
		for (i = 0; i < this->m_numAtom; i++)
		{
			if (!(this->m_aVisStat[i] & atom_Statu_MarkedFound))
			{
				this->Hide_aAtom(i + 1);
			}
		}
	}

}

/*void VisAtomSample::DrawWignerSeizVolume(QString fname)
{
         Sphere Balls=Sphere();

		 QFile file(fname);
         file.open(QIODevice::ReadOnly|QIODevice::Text);

         Style  Sty;
         glEnable(GL_DEPTH_TEST);
		 glEnable(GL_LIGHTING);
	     Sty.Solid();
         glEnable(GL_COLOR_MATERIAL);
         glEnable(GL_NORMALIZE);
         glLineWidth(1.f); 
		 //to determine how many col for each line
         char buf[MAXCOL*MAXSTRL];
		 float pos0[3], norm1[3], norm0[3], verts[50][3];
		 float x, y, z;
		 int  id, nfaces, i, j, ityp, ip, nverts, gid, i1, i2, index;
		 
		 ip = 0;
         while (!file.atEnd()) 
		 {
			 file.readLine(buf,  MAXCOL*MAXSTRL);
			 file.readLine(buf,  MAXCOL*MAXSTRL);
			 sscanf(buf, "%i %i %i %e %e %e", &id, &index,&ityp, &pos0[0], &pos0[1], &pos0[2]);
			 file.readLine(buf,  MAXCOL*MAXSTRL);
			 sscanf(buf, "%i ", &nfaces);
			 glPushMatrix();
			 for(i=0; i<2; i++) //nfaces; i++)
			 {
			     file.readLine(buf,  MAXCOL*MAXSTRL);
			     sscanf(buf, "%i ", &nverts);
				 for(j=0; j<nverts; j++)
				 {
			       file.readLine(buf,  MAXCOL*MAXSTRL);
			       sscanf(buf, "%i %i %i %i %e %e %e", &id, &index, &gid, &ityp, &x, &y, &z);
			       verts[j][0] = x;
				   verts[j][1] = y;
				   verts[j][2] = z;
				 }
				 //determine the normal
				 norm1[0] = (verts[1][1]-verts[0][1])*(verts[2][2]-verts[1][2])-(verts[1][2]-verts[0][2])*(verts[2][1]-verts[1][1]);
				 norm1[1] = (verts[1][2]-verts[0][2])*(verts[2][0]-verts[1][0])-(verts[1][0]-verts[0][0])*(verts[2][2]-verts[1][2]);
				 norm1[2] = (verts[1][0]-verts[0][0])*(verts[2][1]-verts[1][1])-(verts[1][1]-verts[0][1])*(verts[2][0]-verts[1][0]);


				 //to begine draw the face
		          glBegin(GL_LINES);
			      glColor3f(1.0f, 0.f, 0.f);
				  for(j=0; j<nverts; j++)
				  {
					glVertex3d(0.f, 0.f, 0.f);
					glVertex3d(verts[j][0], verts[j][1], verts[j][2]);
				  }
				  glEnd();
			 }
			 glPopMatrix();

			 break;
		     ip++;
		   if(ip>=100) break; 	 
         }
		 file.close();
}*/

