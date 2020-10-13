/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#include <math.h>
#include <stdio.h>
#include "Scene.h"
#include "Style.h"
#include "VisAtomCliper.h"


VisAtomCliper::~VisAtomCliper()
{
}

VisAtomCliper::VisAtomCliper()
{
   
	 this->m_Style = CLIPPING_STYLE_OR;
	 this->m_Scene = NULL;
	 this->m_Inv[0]   = m_Inv[1] = m_Inv[2] =0;
     this->m_Axsis[0] = m_Axsis[1] = m_Axsis[2] = 0;
     this->m_Pos[0] = m_Pos[1] = m_Pos[2] = 0;
	 this->m_CStep[0] = m_CStep[1] = m_CStep[2] = 0.1;
	 this->m_ShowFrontface[0] = m_ShowFrontface[1] = m_ShowFrontface[2] = 1;
	 this->m_ShowBackface[0] = m_ShowBackface[1] = m_ShowBackface[2] = 1;
	 this->m_Opacity = 0.3;
	 this->EnableClip(false);
	 return;
 }

void VisAtomCliper::SetParameters(VisAtomCliper*sor)
{
	int i;
	for(i=0; i<3; i++)
	{
	   this->m_ClipStatu[i] = sor->m_ClipStatu[i];
  	   this->m_Inv[i]   = sor->m_Inv[i];
	   this->m_Axsis[i] = sor->m_Axsis[i];
	   this->m_CStep[i] = sor->m_CStep[i];
	   this->m_Pos[i]   = sor->m_Pos[i];
	   this->m_ShowFrontface[i] = sor->m_ShowFrontface[i];
	   this->m_ShowBackface[i] = sor->m_ShowBackface[i];
	   this->m_Opacity = sor->m_Opacity;
	}
    return;    
}

bool VisAtomCliper::IsParameterChanged(VisAtomCliper*sor)
{
	int i;
	for(i=0; i<3; i++)
	{
	   if(this->m_ClipStatu[i] != sor->m_ClipStatu[i]) return true;
  	   if(this->m_Inv[i]   != sor->m_Inv[i]) return true;
	   if(abs(this->m_Axsis[i] - sor->m_Axsis[i]) > 1.e-5) return true;
	   if(this->m_CStep[i] != sor->m_CStep[i]) return true;
	}
	return false;
}

void VisAtomCliper::GetClipThickness(double thickness[])
{
	int i;
	if(!this->m_Scene)
	{
		this->GetClipStep(thickness);
		return;
	}
	else
	{
		/*double dx = this->m_Scene->GetVolumeX1() - this->m_Scene->GetVolumeX0();
		double dy = this->m_Scene->GetVolumeY1() - this->m_Scene->GetVolumeY0();
		double dz = this->m_Scene->GetVolumeZ1() - this->m_Scene->GetVolumeZ0();
		thickness[0] = dx*this->m_CStep[0];
		thickness[1] = dy*this->m_CStep[1];
		thickness[2] = dz*this->m_CStep[2];*/
		this->GetClipStep(thickness);
		return;
	}
}

void VisAtomCliper::GetClipAbsolutePos(double pos[])
{
	if(!this->m_Scene)
	{
		this->GetClipPos(pos);
		return;
	}
	else
	{
		/*double dx = this->m_Scene->GetVolumeX1() - this->m_Scene->GetVolumeX0();
		double dy = this->m_Scene->GetVolumeY1() - this->m_Scene->GetVolumeY0();
		double dz = this->m_Scene->GetVolumeZ1() - this->m_Scene->GetVolumeZ0();
		pos[0] = dx*this->m_Pos[0];
		pos[1] = dy*this->m_Pos[1];
		pos[2] = dz*this->m_Pos[2];*/
        this->GetClipPos(pos);
		return;
	}
}

void VisAtomCliper::GetClipPlaneContour(double x0, double y0, double z0, double x1,double y1,double z1,
										double vnx, double vny, double vnz, double rr, int&npt, double ppt[12][3])
{

	    double x, y, z, dis, pt[12][3];
		int i, j, ii, flag[12];



       //get the intersects
         npt = 0;
         if(abs(vnz) > 1.e-8) 
		 {
           z = (rr - vnx*x0 - vny*y0)/vnz;
		   if( z >=z0 && z<=z1) { pt[npt][0] = x0; pt[npt][1] = y0; pt[npt][2] = z; npt++;}

           z = (rr - vnx*x0 - vny*y1)/vnz;
		   if( z >=z0 && z<=z1) { pt[npt][0] = x0; pt[npt][1] = y1; pt[npt][2] = z; npt++;}

           z = (rr - vnx*x1 - vny*y0)/vnz;
		   if( z >=z0 && z<=z1) { pt[npt][0] = x1; pt[npt][1] = y0; pt[npt][2] = z; npt++;}

           z = (rr - vnx*x1 - vny*y1)/vnz;
		   if( z >=z0 && z<=z1) { pt[npt][0] = x1; pt[npt][1] = y1; pt[npt][2] = z; npt++;}
		 }
		
         if(abs(vnx) > 1.e-8) 
		 {
           x = (rr - vny*y0 - vnz*z0)/vnx;
		   if( x >=x0 && x<=x1) { pt[npt][0] = x; pt[npt][1] = y0; pt[npt][2] = z0; npt++;}

		   x = (rr - vny*y0 - vnz*z1)/vnx;
		   if( x >=x0 && x<=x1) { pt[npt][0] = x; pt[npt][1] = y0; pt[npt][2] = z1; npt++;}

           x = (rr - vny*y1 - vnz*z0)/vnx;
		   if( x >=x0 && x<=x1) { pt[npt][0] = x; pt[npt][1] = y1; pt[npt][2] = z0; npt++;}

		   x = (rr - vny*y1 - vnz*z1)/vnx;
		   if( x >=x0 && x<=x1) { pt[npt][0] = x; pt[npt][1] = y1; pt[npt][2] = z1; npt++;}

  		 }
		
         if(abs(vny) > 1.e-8) 
		 {
           y = (rr - vnz*z0 - vnx*x0)/vny;
		   if( y >=y0 && y<=y1) { pt[npt][0] = x0; pt[npt][1] = y; pt[npt][2] = z0; npt++;}

		   y = (rr - vnz*z0 - vnx*x1)/vny;
		   if( y >=y0 && y<=y1) { pt[npt][0] = x1; pt[npt][1] = y; pt[npt][2] = z0; npt++;}

           y = (rr - vnz*z1 - vnx*x0)/vny;
		   if( y >=y0 && y<=y1) { pt[npt][0] = x0; pt[npt][1] = y; pt[npt][2] = z1; npt++;}

		   y = (rr - vnz*z1 - vnx*x1)/vny;
		   if( y >=y0 && y<=y1) { pt[npt][0] = x1; pt[npt][1] = y; pt[npt][2] = z1; npt++;}
  		 }
          //sort the intersects		
		  ppt[0][0] = pt[0][0];
		  ppt[0][1] = pt[0][1];
		  ppt[0][2] = pt[0][2];
		  flag[0]   = 1;
		  for(i=1; i<npt; i++) flag[i] = 0;

		  for(j=0; j<npt; j++)
		  {
			  rr=1.e8;
			  for(i=0; i<npt; i++)
			  {
				 if(flag[i]) continue;
             
				 dis = (pt[i][0] - ppt[j][0])*(pt[i][0] - ppt[j][0]) + 
                       (pt[i][1] - ppt[j][1])*(pt[i][1] - ppt[j][1]) +
                       (pt[i][2] - ppt[j][2])*(pt[i][2] - ppt[j][2]); 
				 if(dis < rr) { ii = i; rr = dis;}
			  }
			  ppt[j+1][0] = pt[ii][0];
			  ppt[j+1][1] = pt[ii][1];
			  ppt[j+1][2] = pt[ii][2];
			  flag[ii] = 1;
		  }
  
}

void VisAtomCliper::PaintEdge()
{

     Style  Sty;
     glEnable(GL_LIGHTING);
     Sty.Solid();


	double x0, x1, y0, y1, z0, z1, x, y, z, pos[3];
	x0 = this->m_Scene->GetVolumeX0();
	x1 = this->m_Scene->GetVolumeX1();
	y0 = this->m_Scene->GetVolumeY0();
	y1 = this->m_Scene->GetVolumeY1();
	z0 = this->m_Scene->GetVolumeZ0();
	z1 = this->m_Scene->GetVolumeZ1();
	pos[0] = this->m_Pos[0]; //*(x1 - x0);
	pos[1] = this->m_Pos[1]; //*(y1 - y0);
	pos[2] = this->m_Pos[2]; //*(z1 - z0);

	if(this->m_Style == VisAtomCliper::CLIPPING_STYLE_ANYDIR)
	{
		double norm, rr, thick[3], vnx,vny, vnz, pt[12][3], ppt[12][3];
		int i, j, ii, npt, flag[12];
        //Without normal of the clip plane
		if(abs(this->m_Axsis[0]) < 0.0001 && 
		   abs(this->m_Axsis[1]) < 0.0001 && 
		   abs(this->m_Axsis[2]) < 0.0001) return;
       
       //get normal of the clip plane
        norm = this->m_Axsis[0]*this->m_Axsis[0] +
		       this->m_Axsis[1]*this->m_Axsis[1] +
			   this->m_Axsis[2]*this->m_Axsis[2];
		norm = pow(norm, 0.5);
		vnx  = this->m_Axsis[0]/norm;
		vny  = this->m_Axsis[1]/norm;
		vnz  = this->m_Axsis[2]/norm;


	   //get the distance to origion
		this->GetClipThickness(thick);
		rr = vnx*pos[0] + vny*pos[1] + vnz*pos[2];

		if(this->m_ShowFrontface[0])
		{
         //get the intersects1
		 GetClipPlaneContour(x0, y0, z0, x1, y1, z1, vnx, vny, vnz, rr+0.5*thick[0], npt, ppt);

          //Begin the paint
         glEnable(GL_COLOR_MATERIAL);
         glEnable(GL_NORMALIZE);
	     glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		 glLineWidth(1.f); 
		 glDisable(GL_DEPTH_TEST);

		 glColor4f(0.f, 0.f, 0.f, 0.6f);
		 glBegin(GL_LINE_LOOP);
		 for (i = 0; i<npt; i++) glVertex3d(ppt[i][0], ppt[i][1], ppt[i][2]);
		 glVertex3d(ppt[0][0], ppt[0][1], ppt[0][2]);
		 glEnd();
		}

		 if( this->IsEnabledDClipX()||this->IsEnabledDClipY()||this->IsEnabledDClipZ())
		 {
			if(this->m_ShowBackface[0])
			{
               //get the intersects2
		       GetClipPlaneContour(x0, y0, z0, x1, y1, z1, vnx, vny, vnz, rr-0.5*thick[0], npt, ppt);

			  glColor4f(0.f, 0.f, 0.f, 0.6f);
			  glBegin(GL_LINE_LOOP);
			  for (i = 0; i<npt; i++) glVertex3d(ppt[i][0], ppt[i][1], ppt[i][2]);
			  glVertex3d(ppt[0][0], ppt[0][1], ppt[0][2]);
			  glEnd();
			}
		 }
	}
	//glEnable(GL_LIGHTING);
    return;
}

void VisAtomCliper::PaintFace()
{

	Style  Sty;
	glEnable(GL_LIGHTING);
	Sty.Solid();


	double x0, x1, y0, y1, z0, z1, x, y, z, pos[3];
	x0 = this->m_Scene->GetVolumeX0();
	x1 = this->m_Scene->GetVolumeX1();
	y0 = this->m_Scene->GetVolumeY0();
	y1 = this->m_Scene->GetVolumeY1();
	z0 = this->m_Scene->GetVolumeZ0();
	z1 = this->m_Scene->GetVolumeZ1();
	pos[0] = this->m_Pos[0]; //*(x1 - x0);
	pos[1] = this->m_Pos[1]; //*(y1 - y0);
	pos[2] = this->m_Pos[2]; //*(z1 - z0);

	if (this->m_Style == VisAtomCliper::CLIPPING_STYLE_ANYDIR)
	{
		double norm, rr, thick[3], vnx, vny, vnz, pt[12][3], ppt[12][3];
		int i, j, ii, npt, flag[12];
		//Without normal of the clip plane
		if (abs(this->m_Axsis[0]) < 0.0001 &&
			abs(this->m_Axsis[1]) < 0.0001 &&
			abs(this->m_Axsis[2]) < 0.0001) return;

		//get normal of the clip plane
		norm = this->m_Axsis[0] * this->m_Axsis[0] +
			this->m_Axsis[1] * this->m_Axsis[1] +
			this->m_Axsis[2] * this->m_Axsis[2];
		norm = pow(norm, 0.5);
		vnx = this->m_Axsis[0] / norm;
		vny = this->m_Axsis[1] / norm;
		vnz = this->m_Axsis[2] / norm;


		//get the distance to origion
		this->GetClipThickness(thick);
		rr = vnx * pos[0] + vny * pos[1] + vnz * pos[2];

		//draw point vector
		/*if( this->IsEnabledDClipX()||this->IsEnabledDClipY()||this->IsEnabledDClipZ())
		{

		glColor4f(1.f, 1.f, 1.f, 0.8f);
		glBegin(GL_LINES);
		glVertex3d(0., 0., 0.);
		glVertex3d(pos[0], pos[1], pos[2]);
		glEnd();
		}*/

		if (this->m_ShowFrontface[0])
		{
			//get the intersects1
			GetClipPlaneContour(x0, y0, z0, x1, y1, z1, vnx, vny, vnz, rr + 0.5*thick[0], npt, ppt);

			//Begin the paint
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_NORMALIZE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLineWidth(1.f);

			glEnable(GL_DEPTH_TEST);
			glColor4f(1.f, 1.f, 1.f, (float)this->m_Opacity);
			glBegin(GL_POLYGON);
			glNormal3d(vnx, vny, vnz);
			for (i = 0; i<npt; i++) glVertex3d(ppt[i][0], ppt[i][1], ppt[i][2]);
			glVertex3d(ppt[0][0], ppt[0][1], ppt[0][2]);
			glEnd();

			glColor4f(0.f, 0.f, 0.f, 0.6f);
			glBegin(GL_LINE_LOOP);
			for (i = 0; i<npt; i++) glVertex3d(ppt[i][0], ppt[i][1], ppt[i][2]);
			glVertex3d(ppt[0][0], ppt[0][1], ppt[0][2]);
			glEnd();
		}

		if (this->IsEnabledDClipX() || this->IsEnabledDClipY() || this->IsEnabledDClipZ())
		{
			if (this->m_ShowBackface[0])
			{
				//get the intersects2
				GetClipPlaneContour(x0, y0, z0, x1, y1, z1, vnx, vny, vnz, rr - 0.5*thick[0], npt, ppt);

				glColor4f(1.f, 1.f, 1.f, (float)this->m_Opacity);
				glEnable(GL_DEPTH_TEST);
				glBegin(GL_POLYGON);
				glNormal3d(vnx, vny, vnz);
				for (i = 0; i<npt; i++) glVertex3d(ppt[i][0], ppt[i][1], ppt[i][2]);
				glVertex3d(ppt[0][0], ppt[0][1], ppt[0][2]);
				glEnd();

				glColor4f(0.f, 0.f, 0.f, 0.6f);
				glBegin(GL_LINE_LOOP);
				for (i = 0; i<npt; i++) glVertex3d(ppt[i][0], ppt[i][1], ppt[i][2]);
				glVertex3d(ppt[0][0], ppt[0][1], ppt[0][2]);
				glEnd();
			}
		}
	}
	//glEnable(GL_LIGHTING);
	return;
}