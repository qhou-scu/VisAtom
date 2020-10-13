#include <windows.h>	// Standard windows include
#include <gl\gl.h>		// OpenGL library
#include <gl\glu.h>		// OpenGL library

#include <stdlib.h>

#include "Scene.h"
#include "Style.h"

Scene::~Scene()
{
	 glDisable(GL_CLIP_PLANE0);
	 glDisable(GL_CLIP_PLANE1);
	 glDisable(GL_CLIP_PLANE2);
     light1->Turnoff();
     light2->Turnoff();
     light3->Turnoff();
	 delete light1;
	 delete light2;
	 delete light3;
}


Scene::Scene()
{
	m_viewportx0 = m_viewporty0 = 0;
    m_viewportw  = m_viewporth  = 480;
	m_x0= -0.5f;
	m_x1=  0.5f;
	m_y0= -0.5f;
	m_y1=  0.5f;
	m_z0= -0.5f;
	m_z1=  0.5f;
	m_dist= 0.f;

	m_showbox = true;
	m_borderthick = 2.f;
	m_showfrontface[0] = m_showfrontface[1] = m_showfrontface[2] = 1;
	m_showbackface[0]  = m_showbackface[1]  = m_showbackface[2] = 1+2;
	m_backfaceopacity[0] = m_backfaceopacity[1] = m_backfaceopacity[2] = 0.f;

    action = 0;
	style  = scene_Style_ShowVolume|scene_Style_Perspective;
	update = 0;
    system = scene_System_Relative; 


	m_zoomPercent = 0.9f;
	m_zoomscal    = 1.f;
    m_rotationStepx = m_rotationStepy = m_rotationStepz = 10.f;
	m_rotationCx= m_rotationCy= m_rotationCz= 0.5f;
	m_itransStepx   = m_itransStepy = m_itransStepz   = 20.f;    
    m_itransX = m_itransY = m_itransZ = 0.f;

	iclipStepx    = 20.f;   //(x1-x0)/40.f;
	iclipStepy    = 20.f;   //(x1-x0)/40.f;
	iclipStepz    = 20.f;   // (x1-x0)/40.f;

	iclipThickx    = iclipStepx; 
	iclipThicky    = iclipStepy; 
	iclipThickz    = iclipStepz;
	clip1[0] = 1.f; clip1[1] = 0.f; clip1[2] = 0.f; clip1[3] = 0.f;
	clip2[0] = 0.f; clip2[1] = 1.f; clip2[2] = 0.f; clip2[3] = 0.f;
	clip3[0] = 0.f; clip3[1] = 0.f; clip3[2] = 1.f; clip3[3] = 0.f;
	clip1p[0] = -1.f; clip1p[1] = 0.f; clip1p[2] = 0.f; clip1p[3] = (m_x1-m_x0)/(float)iclipThickx;
	clip2p[0] = 0.f; clip2p[1] = -1.f; clip2p[2] = 0.f; clip2p[3] = (m_y1-m_y0)/(float)iclipThicky;
	clip3p[0] = 0.f; clip3p[1] = 0.f; clip3p[2] = -1.f; clip3p[3] = (m_z1-m_z0)/(float)iclipThickz;
	clipState[0] = 0; 
	clipState[1] = 0; 
	clipState[2] = 0; 
	clipState[3] = 0; 
	clipState[4] = 0; 
	clipState[5] = 0; 
	curclip   = 0;

	light1 = new Light(1);
	light2 = new Light(2);
	light3 = new Light(3);
 	light1->Turnon();
	light2->Turnon();
    light2->ChangePosition(-1.f, -1.f, -1.f);
    light2->ChangeColorD(0.5f, 0.5f, 0.5f);
    light2->ChangeColorS(0.5f, 0.5f, 0.5f);
	glLoadIdentity();

}

Scene::Scene(float left, float right, float bottom, float top, float nearz, float farz, float dist)
{
	m_viewportx0 = m_viewporty0 = 0;
    m_viewportw  = m_viewporth = 480;

	m_x0= left;
	m_x1= right;
	m_y0= bottom;
	m_y1= top;
	m_z0= nearz;
	m_z1= farz;
	m_dist= dist;

	m_showbox = true;
	m_borderthick = 2.f;
	m_showfrontface[0] = m_showfrontface[1] = m_showfrontface[2] = 1;
	m_showbackface[0]  = m_showbackface[1]  = m_showbackface[2] = 1+2;
	m_backfaceopacity[0] = m_backfaceopacity[1] = m_backfaceopacity[2] = 0.5f;

    action = 0;
	style  = scene_Style_ShowVolume|scene_Style_Perspective;
	update = 0;
    system = scene_System_Relative; 


	m_zoomPercent = 0.9f;
	m_zoomscal    = 1.f;
    m_rotationStepx = m_rotationStepy = m_rotationStepz = 10.f;
    m_rotationCx= m_rotationCy= m_rotationCz= 0.f;
	m_itransStepx = m_itransStepy = m_itransStepz = 20.f;    
    m_itransX = m_itransY = m_itransZ = 0.f;

	iclipStepx     = 20.f;   //(x1-x0)/40.f;
	iclipStepy     = 20.f;   //(x1-x0)/40.f;
	iclipStepz     = 20.f;   // (x1-x0)/40.f;
	iclipThickx    = iclipStepx; 
	iclipThicky    = iclipStepy; 
	iclipThickz    = iclipStepz;
	clip1[0] = 1.f; clip1[1] = 0.f; clip1[2] = 0.f; clip1[3] = 0.f;
	clip2[0] = 0.f; clip2[1] = 1.f; clip2[2] = 0.f; clip2[3] = 0.f;
	clip3[0] = 0.f; clip3[1] = 0.f; clip3[2] = 1.f; clip3[3] = 0.f;
	clip1p[0] = -1.f; clip1p[1] = 0.f; clip1p[2] = 0.f; clip1p[3] = (m_x1-m_x0)/(float)iclipThickx;
	clip2p[0] = 0.f; clip2p[1] = -1.f; clip2p[2] = 0.f; clip2p[3] = (m_y1-m_y0)/(float)iclipThicky;
	clip3p[0] = 0.f; clip3p[1] = 0.f; clip3p[2] = -1.f; clip3p[3] = (m_z1-m_z0)/(float)iclipThickz;
	clipState[0] = 0; 
	clipState[1] = 0; 
	clipState[2] = 0; 
	clipState[3] = 0; 
	clipState[4] = 0; 
	clipState[5] = 0; 
	curclip   = 0;

	light1 = new Light(1);
	light2 = new Light(2);
	light3 = new Light(3);
 	light1->Turnon();
	light2->Turnon();
    light2->ChangePosition(-1.f, -1.f, -1.f);
    light2->ChangeColorD(0.5f, 0.5f, 0.5f);
    light2->ChangeColorS(0.5f, 0.5f, 0.5f);
	glLoadIdentity();
}


void Scene::CreateScene( )
{

    GLint		viewport[4];	/* Current viewport */
	float w, h;
    double nearz, farz, ang,width,height;

    glGetIntegerv(GL_VIEWPORT, viewport);
    w =(float)viewport[2];
    h =(float)viewport[3];
	if(w == 0 ) w = 1.f; 
	if(h == 0)  h = 1.f;

	width = (m_x1 - m_x0);
	if(width < 0.0) width = -width;
	height = (m_y1 - m_y0);
	if(height < 0.0) height = -height;
	width = std::max(width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if((style&scene_Style_Perspective) != scene_Style_Perspective)
	{
	  if (w <= h) 
		glOrtho (m_x0, m_x0+width, m_y0*h/w, (m_y0+width)*h/w, m_z0, m_z1);

	  else 
		glOrtho (m_x0*w/h, (m_x0+width)*w/h, m_y0, (m_y0+width), m_z0, m_z1); 
	   //glMatrixMode(GL_MODELVIEW);
 	   //glLoadIdentity();
	 }
	else
	{
	  nearz = m_z0;
	  if(nearz < 0.0) nearz = -nearz;
	  if(nearz == 0.0 ) nearz = 1.0; 
	  nearz = nearz+m_dist;
	  farz =  10.*(m_z1 - m_z0)+nearz; 
	  ang = atan((m_y1 - m_y0)/nearz/2.0)*180.0/3.1415926; 
      gluPerspective(ang, w/h, nearz, farz);

	   //glMatrixMode(GL_MODELVIEW);
 	   //glLoadIdentity();

	   glTranslatef(0.f, 0.f, (float)(-2.5*nearz + m_z0));
	   
	 }
	 glClipPlane(GL_CLIP_PLANE0, clip1);
	 glClipPlane(GL_CLIP_PLANE1, clip2);
	 glClipPlane(GL_CLIP_PLANE2, clip3);
	 glClipPlane(GL_CLIP_PLANE3, clip1p);
	 glClipPlane(GL_CLIP_PLANE4, clip2p);
	 glClipPlane(GL_CLIP_PLANE5, clip3p);
}


void Scene::CreateScene(float left, float right, float bottom, float top, float nearz, float farz, float dist)
{

	m_x0 = left;
	m_x1 = right;
	m_y0 = bottom;
	m_y1 = top;
	m_z0 = nearz;
	m_z1 = farz;
	m_dist = dist;

    clip1[3] = 0.f;
    clip2[3] = 0.f;
    clip3[3] = 0.f;
    clip1p[3] = (m_x1 - m_x0)/(float)iclipThickx;
    clip2p[3] = (m_y1 - m_y0)/(float)iclipThicky;
    clip3p[3] = (m_z1 - m_z0)/(float)iclipThickz;
    CreateScene();
}


int Scene::Response(int char1)
{

    int flag;

	flag = 0;
	switch(char1)
	{
	case action_None:
		 action = action_None;
		 break;
	case action_Object_RotationX:
		 action = action_Object_RotationX;
		 break;
	case action_Object_RotationY:
		 action = action_Object_RotationY;
		 break;
	case action_Object_RotationZ:
		 action = action_Object_RotationZ;
		 break;
	case action_Object_TranslateX:
		 action = action_Object_TranslateX;
		 break;
	case action_Object_TranslateY:
		 action = action_Object_TranslateY;
		 break;
	case action_Object_TranslateZ:
		 action = action_Object_TranslateZ;
		 break;
	case action_Object_Zoom:
		 action = action_Object_Zoom;
		 break;
	case action_Object_Clipx:
		 action = action_Object_Clipx;
		 curclip = 1;
  		 break;
	case action_Object_Clipy:
		 action = action_Object_Clipy;
 		 curclip = 2;
 		 break;
	case action_Object_Clipz:
		 action = action_Object_Clipz;
		 curclip = 3;
  		 break;
    case action_Object_ClipR:
	     ChangeClipPlane();
		 flag = 1;
		 break;
    case action_Object_ClipE:
 	     EnableClipPlane();
		 flag = 1;
		 break;
    case action_Object_ClipEP:
 	     EnableClipPlanePair();
		 flag = 1;
		 break;
   case action_Object_ClipD:
	     DisableClipPlane();
		 flag = 1;
		 break;
   case action_Object_ClipDP:
	     DisableClipPlanePair();
		 flag = 1;
		 break;
	}


	if((char1 ==Key_UP)|(char1 ==Key_DOWN)|(char1 ==Key_LEFT)|(char1 ==Key_RIGHT))
	{
	  switch (action)
	  {
	   case action_None:
		   break;
       case action_Object_Select:
		   break; 
       case action_Object_RotationX:
		   if((char1 ==Key_UP)|(char1 ==Key_RIGHT)) {  RotateXf(); flag = 1; }
		   else if((char1 ==Key_DOWN)|(char1 ==Key_LEFT)) {RotateXb(); flag = 1;}
		   break; 
       case action_Object_RotationY:
		   if((char1 ==Key_UP)|(char1 ==Key_RIGHT)) { RotateYf(); flag = 1;}
		   else if((char1 ==Key_DOWN)|(char1 ==Key_LEFT)) { RotateYb(); flag = 1;}
		   break; 
       case action_Object_RotationZ:
		   if((char1 ==Key_UP)|(char1 ==Key_RIGHT)) { RotateZf(); flag = 1;}
		   else if((char1 ==Key_DOWN)|(char1 ==Key_LEFT)) { RotateZb(); flag = 1;}
		   break; 
       case action_Object_TranslateX:
		   if((char1 ==Key_UP)|(char1 ==Key_RIGHT)) { TransR(); flag = 1;}
		   else if((char1 ==Key_DOWN)|(char1 ==Key_LEFT)) { TransL(); flag = 1;}
		   break; 
       case action_Object_TranslateY:
		   if((char1 ==Key_UP)|(char1 ==Key_RIGHT)) { TransU(); flag =1;}
		   else if((char1 ==Key_DOWN)|(char1 ==Key_LEFT)) { TransD(); flag = 1;}
		   break; 
       case action_Object_TranslateZ:
		   if((char1 ==Key_UP)|(char1 ==Key_RIGHT)) { TransN(); flag =1;}
		   else if((char1 ==Key_DOWN)|(char1 ==Key_LEFT)) { TransF(); flag = 1;}
		   break; 
       case action_Object_Zoom:
		   if((char1 ==Key_UP)|(char1 ==Key_RIGHT)) { ZoomOut(); flag = 1;}
		   else if((char1 ==Key_DOWN)|(char1 ==Key_LEFT)) { ZoomIn(); flag = 1;}
		   break; 
       case action_Object_Clipx:
       case action_Object_Clipy:
       case action_Object_Clipz:
		   switch(char1)
		   {
		   case Key_UP:
		   case Key_RIGHT:
			    if(MoveClipPlaneP()) flag = 1;
				break;
		   case Key_DOWN:
		   case Key_LEFT:
			    if(MoveClipPlaneM()) flag = 1;
				break;
		   }
		   break; 
	   }
	  //light[0].Turnon();
	  //light[1].Turnon();
	}
	return flag;
}

void Scene::ZoomIn( )
{

	float p2;

	/*x0 = x0*zoomPercent;
	x1 = x1*zoomPercent;
	y0 = y0*zoomPercent;
	y1 = y1*zoomPercent;
	z0 = z0*zoomPercent; 
	z1 = z1*zoomPercent; */
	/*p2 = 1.f/zoomPercent;
	if(p2 < 1.f) p2 = 1.f/p2;

	glOrtho (-p2, p2, -p2, p2, p2, -p2); 
 	x0 = x0/p2;
	x1 = x1/p2;
	y0 = y0/p2;
	y1 = y1/p2;
	z0 = z0/p2;
	z1 = z1/p2; */


	p2 = m_zoomPercent;
	if(p2>1.f) p2 = 1.f/p2;
	m_zoomscal = m_zoomscal*p2;
	glScalef(p2,p2,p2);
	glClipPlane(GL_CLIP_PLANE0, clip1);
	glClipPlane(GL_CLIP_PLANE1, clip2);
	glClipPlane(GL_CLIP_PLANE2, clip3);
	glClipPlane(GL_CLIP_PLANE3, clip1p);
	glClipPlane(GL_CLIP_PLANE4, clip2p);
	glClipPlane(GL_CLIP_PLANE5, clip3p);

}

void Scene::ZoomOut( )
{

	float p2;

	/*x0 = x0*zoomPercent;
	x1 = x1*zoomPercent;
	y0 = y0*zoomPercent;
	y1 = y1*zoomPercent;
	z0 = z0*zoomPercent;
	z1 = z1*zoomPercent; */
	/*p2 = 1.f/zoomPercent;
	if(p2 > 1.f) p2 = 1.f/p2;

	glOrtho (-p2, p2, -p2, p2, p2, -p2); 
	x0 = x0/p2;
	x1 = x1/p2;
	y0 = y0/p2;
	y1 = y1/p2;
	z0 = z0/p2;
	z1 = z1/p2; */

	p2 = m_zoomPercent;
	if(p2<1.f) p2 = 1.f/p2;
	m_zoomscal = m_zoomscal*p2;
	glScalef(p2,p2,p2); 
	glClipPlane(GL_CLIP_PLANE0, clip1);
	glClipPlane(GL_CLIP_PLANE1, clip2);
	glClipPlane(GL_CLIP_PLANE2, clip3);
	glClipPlane(GL_CLIP_PLANE3, clip1p);
	glClipPlane(GL_CLIP_PLANE4, clip2p);
	glClipPlane(GL_CLIP_PLANE5, clip3p);

}

void Scene::GetLabX(float*xl, float*yl, float*zl)
{
   
  double len;
  double vx[3],	vy[3], vz[3];
  double mat[16];

	   //to get current transform matrix
	   glGetDoublev(GL_MODELVIEW_MATRIX, mat);

	   //to transform the coodrinate of atoms to current coordinate
	   vx[0] = mat[0];	    
	   vx[1] = mat[1];	    
	   vx[2] = mat[2];	    
	   /*len = vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2];
  	   len = sqrt(len);
	   vx[0] = vx[0]/len;	    
 	   vx[1] = vx[1]/len;	    
 	   vx[2] = vx[2]/len;	    
	   */

	   vy[0] = mat[4];	    
	   vy[1] = mat[5];	    
	   vy[2] = mat[6];	    
 	   /*len = vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2];
  	   len = sqrt(len);
 	   vy[0] = vy[0]/len;	    
 	   vy[1] = vy[1]/len;	    
 	   vy[2] = vy[2]/len;	    
		 */
	   vz[0] = mat[8];	    
	   vz[1] = mat[9];	    
	   vz[2] = mat[10];	    
  	   len = vz[0]*vz[0]+vz[1]*vz[1]+vz[2]*vz[2];
  	   len = sqrt((double)len);
 	   /*vz[0] = vz[0]/len;	    
 	   vz[1] = vz[1]/len;	    
 	   vz[2] = vz[2]/len;	    
		 */
	   *xl = (float)vx[0];
	   *yl = (float)vy[0];
	   *zl = (float)vz[0];

	   return;
}


void Scene::GetLabY(float*xl, float*yl, float*zl)
{
   
  double vx[3],	vy[3], vz[3];
  double mat[16];

	   //to get current transform matrix
	   glGetDoublev(GL_MODELVIEW_MATRIX, mat);

	   //to transform the coodrinate of atoms to current coordinate
	   vx[0] = mat[0];	    
	   vx[1] = mat[1];	    
	   vx[2] = mat[2];	    
 	  /* len = vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2];
  	   len = sqrt((double)len);
 	   vx[0] = vx[0]/len;	    
 	   vx[1] = vx[1]/len;	    
 	   vx[2] = vx[2]/len;	    
		*/
	   vy[0] = mat[4];	    
	   vy[1] = mat[5];	    
	   vy[2] = mat[6];	    
 	   /*len = vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2];
  	   len = sqrt((double)len);
 	   vy[0] = vy[0]/len;	    
 	   vy[1] = vy[1]/len;	    
 	   vy[2] = vy[2]/len;	    
		 */
	   vz[0] = mat[8];	    
	   vz[1] = mat[9];	    
	   vz[2] = mat[10];	    
  	   /*len = vz[0]*vz[0]+vz[1]*vz[1]+vz[2]*vz[2];
  	   len = sqrt((double)len);
 	   vz[0] = vz[0]/len;	    
 	   vz[1] = vz[1]/len;	    
 	   vz[2] = vz[2]/len;	    
	   	 */
	   *xl = (float)vx[1];
	   *yl = (float)vy[1];
	   *zl = (float)vz[1];

	   return;
}


void Scene::GetLabZ(float*xl, float*yl, float*zl)
{
   
  double len;
  double vx[3],	vy[3], vz[3];
  GLdouble mat[16];

	   //to get current transform matrix
	   glGetDoublev(GL_MODELVIEW_MATRIX, mat);

	   //to transform the coodrinate of atoms to current coordinate
	   vx[0] = mat[0];	    
	   vx[1] = mat[1];	    
	   vx[2] = mat[2];	    
 	   len = vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2];
  	   len = sqrt((double)len);
 	   vx[0] = vx[0]/len;	    
 	   vx[1] = vx[1]/len;	    
 	   vx[2] = vx[2]/len;	    

	   vy[0] = mat[4];	    
	   vy[1] = mat[5];	    
	   vy[2] = mat[6];	    
 	   len = vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2];
  	   len = sqrt((double)len);
 	   vy[0] = vy[0]/len;	    
 	   vy[1] = vy[1]/len;	    
 	   vy[2] = vy[2]/len;	    

	   vz[0] = mat[8];	    
	   vz[1] = mat[9];	    
	   vz[2] = mat[10];	    
  	   len = vz[0]*vz[0]+vz[1]*vz[1]+vz[2]*vz[2];
  	   len = sqrt((double)len);
 	   vz[0] = vz[0]/len;	    
 	   vz[1] = vz[1]/len;	    
 	   vz[2] = vz[2]/len;	    

	   *xl = (float)vx[2];
	   *yl = (float)vy[2];
	   *zl = (float)vz[2];

	   return;
}


void Scene::RotateX(float theta)
{
   if(system&scene_System_Labrotary) 
   {
	 //to get the lab x axsis in current coordinate
       float xo,yo,zo;

        GetLabX(&xo, &yo, &zo);
        glTranslatef(m_rotationCx, m_rotationCy, m_rotationCz);
        glRotatef(theta, xo, yo, zo);
        glTranslatef(-m_rotationCx, -m_rotationCy, -m_rotationCz);
   }
   else
   {
        glTranslatef(m_rotationCx, m_rotationCy, m_rotationCz);
 		glRotatef(theta, 1.f, 0.f, 0.f);
        glTranslatef(-m_rotationCx, -m_rotationCy, -m_rotationCz);
   }


    glClipPlane(GL_CLIP_PLANE0, clip1);
	glClipPlane(GL_CLIP_PLANE1, clip2);
	glClipPlane(GL_CLIP_PLANE2, clip3);
	glClipPlane(GL_CLIP_PLANE3, clip1p);
	glClipPlane(GL_CLIP_PLANE4, clip2p);
	glClipPlane(GL_CLIP_PLANE5, clip3p);
   

}

void Scene::RotateXf()
{
     RotateX(m_rotationStepx);
}
void Scene::RotateXb()
{
     RotateX(-m_rotationStepx);
}

void Scene::RotateY(float theta)
{
   
   if(system&scene_System_Labrotary) 
   {
	 //to get the lab y axsis in current coordinate
       float xo,yo,zo;

        GetLabY(&xo, &yo, &zo);
        glTranslatef(m_rotationCx, m_rotationCy, m_rotationCz);
        glRotatef(theta, xo, yo, zo);
        glTranslatef(-m_rotationCx, -m_rotationCy, -m_rotationCz);
   }
   else
   {
     glTranslatef(m_rotationCx, m_rotationCy, m_rotationCz);
     glRotatef(theta, 0.f, 1.f, 0.f);
     glTranslatef(-m_rotationCx, -m_rotationCy, -m_rotationCz);
   }

	glClipPlane(GL_CLIP_PLANE0, clip1);
	glClipPlane(GL_CLIP_PLANE1, clip2);
	glClipPlane(GL_CLIP_PLANE2, clip3);
	glClipPlane(GL_CLIP_PLANE3, clip1p);
	glClipPlane(GL_CLIP_PLANE4, clip2p);
	glClipPlane(GL_CLIP_PLANE5, clip3p);
}

void Scene::RotateYf()
{
     RotateY(m_rotationStepy);

}

void Scene::RotateYb()
{
     RotateY(-m_rotationStepy);

}

void Scene::RotateZ(float theta)
{
   if(system&scene_System_Labrotary) 
   {
	 //to get the lab z axsis in current coordinate
       float xo,yo,zo;

        GetLabZ(&xo, &yo, &zo);
        glTranslatef(m_rotationCx, m_rotationCy, m_rotationCz);
        glRotatef(theta, xo, yo, zo);
        glTranslatef(-m_rotationCx, -m_rotationCy, -m_rotationCz);

   }
   else
   {
        glTranslatef(m_rotationCx, m_rotationCy, m_rotationCz);
        glRotatef(theta, 0.f, 0.f, 1.f);
        glTranslatef(-m_rotationCx, -m_rotationCy, -m_rotationCz);
   }
	glClipPlane(GL_CLIP_PLANE0, clip1);
	glClipPlane(GL_CLIP_PLANE1, clip2);
	glClipPlane(GL_CLIP_PLANE2, clip3);
	glClipPlane(GL_CLIP_PLANE3, clip1p);
	glClipPlane(GL_CLIP_PLANE4, clip2p);
	glClipPlane(GL_CLIP_PLANE5, clip3p);
}

void Scene::RotateZf()
{
     RotateZ(m_rotationStepz);

}

void Scene::RotateZb()
{
     RotateZ(-m_rotationStepz);

}


void Scene::TransL(float d)
{
   
  float xo,yo,zo,len;

     if(system&scene_System_Labrotary) 
     {
	   //to get the lab x axsis in current coordinate
        GetLabX(&xo, &yo, &zo);
     }
     else
     {
        xo = 1.f;
		yo = 0.f;
		zo = 0.f;
     }

	    len = xo*xo+yo*yo+zo*zo;
		len = (float)sqrt((double)len);
		len = -d/len;
		xo  = len*xo;
		yo  = len*yo;
		zo  = len*zo;
	    glTranslatef(xo, yo, zo);
		glClipPlane(GL_CLIP_PLANE0, clip1);
		glClipPlane(GL_CLIP_PLANE1, clip2);
		glClipPlane(GL_CLIP_PLANE2, clip3);
		glClipPlane(GL_CLIP_PLANE3, clip1p);
		glClipPlane(GL_CLIP_PLANE4, clip2p);
		glClipPlane(GL_CLIP_PLANE5, clip3p);

}

void Scene::TransL()
{
	TransL((float)(m_x1 - m_x0)/(float)m_itransStepx);
}

void Scene::TransR(float d)
{
     TransL(-d);
}

void Scene::TransR()
{
     TransR((float)(m_x1 - m_x0)/(float)m_itransStepx);
}

void Scene::TransU(float d)
{
   
  float xo,yo,zo,len;

	   //to get the lab x axsis in current coordinate
     if(system&scene_System_Labrotary) 
     {
	   //to get the lab x axsis in current coordinate
        GetLabY(&xo, &yo, &zo);
     }
     else
     {
        xo = 0.f;
		yo = 1.f;
		zo = 0.f;
     }
		len = xo*xo+yo*yo+zo*zo;
		len = (float)sqrt((double)len);
		len = d/len;
		xo  = len*xo;
		yo  = len*yo;
		zo  = len*zo;
	    glTranslatef(xo, yo, zo);
		glClipPlane(GL_CLIP_PLANE0, clip1);
		glClipPlane(GL_CLIP_PLANE1, clip2);
		glClipPlane(GL_CLIP_PLANE2, clip3);
		glClipPlane(GL_CLIP_PLANE3, clip1p);
		glClipPlane(GL_CLIP_PLANE4, clip2p);
		glClipPlane(GL_CLIP_PLANE5, clip3p);
}

void Scene::TransU()
{
     TransU((float)(m_y1 - m_y0)/(float)m_itransStepy);
}

void Scene::TransD(float d)
{
     TransU(-d);
}

void Scene::TransD()
{
     TransD((float)(m_y1 - m_y0)/(float)m_itransStepy);
}

void Scene::TransN(float d)
{
   
  float xo,yo,zo,len;

     if(system&scene_System_Labrotary) 
     {
	   //to get the lab x axsis in current coordinate
        GetLabZ(&xo, &yo, &zo);
     }
     else
     {
        xo = 0.f;
		yo = 0.f;
		zo = 1.f;
     }

		len = xo*xo+yo*yo+zo*zo;
		len = (float)sqrt((double)len);
		len = -d/len;
		xo  = len*xo;
		yo  = len*yo;
		zo  = len*zo;
	    glTranslatef(xo, yo, zo);
		glClipPlane(GL_CLIP_PLANE0, clip1);
		glClipPlane(GL_CLIP_PLANE1, clip2);
		glClipPlane(GL_CLIP_PLANE2, clip3);
		glClipPlane(GL_CLIP_PLANE3, clip1p);
		glClipPlane(GL_CLIP_PLANE4, clip2p);
		glClipPlane(GL_CLIP_PLANE5, clip3p);

}

void Scene::TransN()
{
     TransN((float)(m_z1 - m_z0)/(float)m_itransStepz);
}

void Scene::TransF(float d)
{
     TransN(-d);
}

void Scene::TransF()
{
     TransF((float)(m_z1 - m_z0)/(float)m_itransStepz);
}


// the function concerning clip planes
void Scene::EnableClipPlanePair()
{
     switch (curclip)
	 {
	 case 1:
         clipState[0]=clipState[0]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE0, clip1);
	     glEnable(GL_CLIP_PLANE0);
         clipState[3]=clipState[3]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE3, clip1p);
	     glEnable(GL_CLIP_PLANE3);
		 break;
	 case 2:
         clipState[1]=clipState[1]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE1, clip2);
	     glEnable(GL_CLIP_PLANE1);
         clipState[4]=clipState[4]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE4, clip2p);
	     glEnable(GL_CLIP_PLANE4);
		 break;
	 case 3:
         clipState[2]=clipState[2]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE2, clip3);
	     glEnable(GL_CLIP_PLANE2);
         clipState[5]=clipState[5]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE5, clip3p);
	     glEnable(GL_CLIP_PLANE5);
		 break;
	 }
}

void Scene::EnableClipPlane()
{
     switch (curclip)
	 {
	 case 1:
         clipState[0]=clipState[0]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE0, clip1);
	     glEnable(GL_CLIP_PLANE0);
		 break;
	 case 2:
         clipState[1]=clipState[1]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE1, clip2);
	     glEnable(GL_CLIP_PLANE1);
		 break;
	 case 3:
         clipState[2]=clipState[2]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE2, clip3);
	     glEnable(GL_CLIP_PLANE2);
		 break;
	 case 4:
         clipState[3]=clipState[3]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE3, clip1p);
	     glEnable(GL_CLIP_PLANE3);
		 break;
	 case 5:
         clipState[4]=clipState[4]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE4, clip2p);
	     glEnable(GL_CLIP_PLANE5);
		 break;
	 case 6:
         clipState[5]=clipState[5]|clip_State_Enabled;
         glClipPlane(GL_CLIP_PLANE5, clip3p);
	     glEnable(GL_CLIP_PLANE5);
		 break;
	 }
}


void Scene::DisableClipPlane()
{
   int i;
	switch (curclip)
	 {
	 case 1:
         i = clipState[0]>>1;
		 clipState[0] = i<<1;
	     glDisable(GL_CLIP_PLANE0);
		 break;
	 case 2:
         i = clipState[1]>>1;
		 clipState[1] = i<<1;
	     glDisable(GL_CLIP_PLANE1);
		 break;
	 case 3:
         i = clipState[2]>>1;
		 clipState[2] = i<<1;
	     glDisable(GL_CLIP_PLANE2);
		 break;
	 case 4:
         i = clipState[3]>>1;
		 clipState[3] = i<<1;
	     glDisable(GL_CLIP_PLANE3);
		 break;
	 case 5:
         i = clipState[4]>>1;
		 clipState[4] = i<<1;
	     glDisable(GL_CLIP_PLANE4);
		 break;
	 case 6:
         i = clipState[5]>>1;
		 clipState[5] = i<<1;
	     glDisable(GL_CLIP_PLANE5);
		 break;
	 }
}

void Scene::DisableClipPlanePair()
{
   int i;
	switch (curclip)
	 {
	 case 1:
         i = clipState[0]>>1;
		 clipState[0] = i<<1;
	     glDisable(GL_CLIP_PLANE0);
         i = clipState[3]>>1;
		 clipState[3] = i<<1;
	     glDisable(GL_CLIP_PLANE3);
		 break;
		 break;
	 case 2:
         i = clipState[1]>>1;
		 clipState[1] = i<<1;
	     glDisable(GL_CLIP_PLANE1);
         i = clipState[4]>>1;
		 clipState[4] = i<<1;
	     glDisable(GL_CLIP_PLANE4);
		 break;
		 break;
	 case 3:
         i = clipState[2]>>1;
		 clipState[2] = i<<1;
	     glDisable(GL_CLIP_PLANE2);
         i = clipState[5]>>1;
		 clipState[5] = i<<1;
	     glDisable(GL_CLIP_PLANE5);
		 break;
	 }
}


int Scene::IsClipPlaneEnabled(int id)
{
	int i=0;
     switch (id)
	 {
	 case 1:
	     if(clipState[0]&clip_State_Enabled) i++;
	     if(clipState[3]&clip_State_Enabled) i++;
		 break;
	 case 2:
	     if(clipState[1]&clip_State_Enabled) i++;
	     if(clipState[4]&clip_State_Enabled) i++;
		 break;
	 case 3:
	     if(clipState[2]&clip_State_Enabled) i++;
	     if(clipState[5]&clip_State_Enabled) i++;
		 break;
	 case 4:
	     if(clipState[0]&clip_State_Enabled) i++;
	     if(clipState[3]&clip_State_Enabled) i++;
		 break;
	 case 5:
	     if(clipState[1]&clip_State_Enabled) i++;
	     if(clipState[4]&clip_State_Enabled) i++;
		 break;
	 case 6:
	     if(clipState[2]&clip_State_Enabled) i++;
	     if(clipState[5]&clip_State_Enabled) i++;
		 break;
	 }
	 return i;
}

int Scene::IsClipPlaneEnabled()
{
	int i;

	for(i=1; i<7; i++)
	   if(IsClipPlaneEnabled(i) > 0) return IsClipPlaneEnabled(i);
	return 0;
}


int Scene::IsCurrentClipPlaneEnabled()
{
     return IsClipPlaneEnabled(curclip);
}


int Scene::MoveClipPlaneP()
{
    int i=0; 
	float d;

	switch (curclip)
	 {
	 case 1:
		 if( (clipState[0]&clip_State_Enabled)||clipState[3]&clip_State_Enabled ) 
		 {
		   d = (m_x1 - m_x0)/(float)iclipStepx;
		   clip1[3]=clip1[3]+d;
		   clip1p[3]=clip1p[3]-d;
		   //if(clip1[3]>x1)clip1[3]=x1; 
           glClipPlane(GL_CLIP_PLANE0, clip1);
           glClipPlane(GL_CLIP_PLANE3, clip1p);
		   i = clipState[0]&clip_State_Enabled;
		   i = clipState[3]&clip_State_Enabled;
	       if(clipState[0]&clip_State_Enabled) glEnable(GL_CLIP_PLANE0);
	       if(clipState[3]&clip_State_Enabled) glEnable(GL_CLIP_PLANE3);
		   i = 1;
		 }
		 break;
	 case 2:
		 if( (clipState[1]&clip_State_Enabled)||(clipState[4]&clip_State_Enabled))
		 {
			d = (m_y1 - m_y0)/(float)iclipStepy;
		    clip2[3]=clip2[3]+d;
		    clip2p[3]=clip2p[3]-d;
 		    //if(clip2[3]>y1)clip2[3]=y1; 
            glClipPlane(GL_CLIP_PLANE1, clip2);
            glClipPlane(GL_CLIP_PLANE4, clip2p);
	        if(clipState[1]&clip_State_Enabled) glEnable(GL_CLIP_PLANE1);
	        if(clipState[4]&clip_State_Enabled) glEnable(GL_CLIP_PLANE4);
		    i = 1;
		 }
		 break;
	 case 3:
		 if( (clipState[2]&clip_State_Enabled) ||(clipState[5]&clip_State_Enabled))
		 {
			d = (m_z1 - m_z0)/(float)iclipStepz; 
		    clip3[3]=clip3[3]+d;
		    clip3p[3]=clip3p[3]-d;
 		    //if(clip3[3]>z1)clip3[3]=z1; 
            glClipPlane(GL_CLIP_PLANE2, clip3);
            glClipPlane(GL_CLIP_PLANE5, clip3p);
	        if(clipState[2]&clip_State_Enabled) glEnable(GL_CLIP_PLANE2);
	        if(clipState[5]&clip_State_Enabled) glEnable(GL_CLIP_PLANE5);
		    i = 1;
		 }
		 break;
	 }
	return i;
}

int Scene::MoveClipPlaneM()
{
    int i=0; 
	float d;

	switch (curclip)
	 {
	 case 1:
		 if( (clipState[0]&clip_State_Enabled)||clipState[3]&clip_State_Enabled ) 
		 {
		   d = -(m_x1 - m_x0)/(float)iclipStepx;
		   clip1[3]=clip1[3]+d;
		   clip1p[3]=clip1p[3]-d;
		   //if(clip1[3]>x1)clip1[3]=x1; 
           glClipPlane(GL_CLIP_PLANE0, clip1);
           glClipPlane(GL_CLIP_PLANE3, clip1p);
	       if(clipState[0]&clip_State_Enabled) glEnable(GL_CLIP_PLANE0);
	       if(clipState[3]&clip_State_Enabled) glEnable(GL_CLIP_PLANE3);
		   i = 1;
		 }
		 break;
	 case 2:
		 if( (clipState[1]&clip_State_Enabled)||(clipState[4]&clip_State_Enabled))
		 {
			d = -(m_y1 - m_y0)/(float)iclipStepy;
		    clip2[3]=clip2[3]+d;
		    clip2p[3]=clip2p[3]-d;
 		    //if(clip2[3]>y1)clip2[3]=y1; 
            glClipPlane(GL_CLIP_PLANE1, clip2);
            glClipPlane(GL_CLIP_PLANE4, clip2p);
	        if(clipState[1]&clip_State_Enabled) glEnable(GL_CLIP_PLANE1);
	        if(clipState[4]&clip_State_Enabled) glEnable(GL_CLIP_PLANE4);
		    i = 1;
		 }
		 break;
	 case 3:
		 if( (clipState[2]&clip_State_Enabled) ||(clipState[5]&clip_State_Enabled))
		 {
			d = -(m_z1 - m_z0)/(float)iclipStepz; 
		    clip3[3]=clip3[3]+d;
		    clip3p[3]=clip3p[3]-d;
 		    //if(clip3[3]>z1)clip3[3]=z1; 
            glClipPlane(GL_CLIP_PLANE2, clip3);
            glClipPlane(GL_CLIP_PLANE5, clip3p);
	        if(clipState[2]&clip_State_Enabled) glEnable(GL_CLIP_PLANE2);
	        if(clipState[5]&clip_State_Enabled) glEnable(GL_CLIP_PLANE5);
		    i = 1;
		 }
		 break;
	 }
	return i;
}


int Scene::ChangeClipPlane()
{
     int i=0;

	 switch (curclip)
	 {
	 case 1:
		 if( (clipState[0]&clip_State_Enabled)&&(clipState[3]&clip_State_Enabled)) return i;
		 if( clipState[0]&clip_State_Enabled)
		 {
		   i = clipState[0] >>1;
		   clipState[0] = i<<1;
		   clipState[3] = clipState[3]|clip_State_Enabled;
           glClipPlane(GL_CLIP_PLANE0, clip1);
           glClipPlane(GL_CLIP_PLANE3, clip1p);
		   glDisable(GL_CLIP_PLANE0);
		   glEnable(GL_CLIP_PLANE3);
		   i = 1;
		   break;
		  }
		 if( clipState[3]&clip_State_Enabled)
		 {
		   i = clipState[3] >>1;
		   clipState[3] = i<<1;
		   clipState[0] = clipState[0]|clip_State_Enabled;
           glClipPlane(GL_CLIP_PLANE0, clip1);
           glClipPlane(GL_CLIP_PLANE3, clip1p);
		   glDisable(GL_CLIP_PLANE3);
		   glEnable(GL_CLIP_PLANE0);
		   i = 1;
		   break;
		  }
		 break;
	 case 2:
		 if( (clipState[1]&clip_State_Enabled)&&(clipState[4]&clip_State_Enabled)) return i;
		 if( clipState[1]&clip_State_Enabled)
		 {
		   i = clipState[1] >>1;
		   clipState[1] = i<<1;
		   clipState[4] = clipState[4]|clip_State_Enabled;
           glClipPlane(GL_CLIP_PLANE1, clip2);
           glClipPlane(GL_CLIP_PLANE4, clip2p);
		   glDisable(GL_CLIP_PLANE1);
		   glEnable(GL_CLIP_PLANE4);
		   i = 1;
		   break;
		  }
		 if( clipState[4]&clip_State_Enabled)
		 {
		   i = clipState[4] >>1;
		   clipState[4] = i<<1;
		   clipState[1] = clipState[1]|clip_State_Enabled;
           glClipPlane(GL_CLIP_PLANE1, clip2);
           glClipPlane(GL_CLIP_PLANE4, clip2p);
		   glDisable(GL_CLIP_PLANE4);
		   glEnable(GL_CLIP_PLANE1);
		   i = 1;
		   break;
		  }
		 break;
	 case 3:
		 if( (clipState[2]&clip_State_Enabled)&&(clipState[5]&clip_State_Enabled)) return i;
		 if( clipState[2]&clip_State_Enabled)
		 {
		   i = clipState[2] >>1;
		   clipState[2] = i<<1;
		   clipState[5] = clipState[5]|clip_State_Enabled;
           glClipPlane(GL_CLIP_PLANE2, clip3);
           glClipPlane(GL_CLIP_PLANE5, clip3p);
		   glDisable(GL_CLIP_PLANE2);
		   glEnable(GL_CLIP_PLANE5);
		   i = 1;
		   break;
		  }
		 if( clipState[5]&clip_State_Enabled)
		 {
		   i = clipState[5] >>1;
		   clipState[5] = i<<1;
		   clipState[2] = clipState[2]|clip_State_Enabled;
           glClipPlane(GL_CLIP_PLANE2, clip3);
           glClipPlane(GL_CLIP_PLANE5, clip3p);
		   glDisable(GL_CLIP_PLANE5);
		   glEnable(GL_CLIP_PLANE2);
		   i = 1;
		   break;
		  }
		 break;

	 }
	 return i;
}

void Scene::ChangeClipPairThickx(float iclipThick)
{
     int i=0;

	 double  od = (m_x1 - m_x0)/(float)iclipThickx;
	 double  nd = (m_x1 - m_x0)/(float)iclipThick;

	 clip1p[3]=clip1p[3]+(nd-od);
	 iclipThickx = iclipThick;
}

void Scene::ChangeClipPairThicky(float iclipThick)
{
     int i=0;

	 double od = (m_y1 - m_y0)/(float)iclipThicky;
	 double nd = (m_y1 - m_y0)/(float)iclipThick;

	 clip2p[3]=clip2p[3]+(nd-od);
	 iclipThicky = iclipThick;
}

void Scene::ChangeClipPairThickz(float iclipThick)
{
     int i=0;

	 double od = (m_z1 - m_z0)/(float)iclipThickz;
	 double nd = (m_z1 - m_z0)/(float)iclipThick;

	 clip3p[3]=clip3p[3]+(nd-od);
	 iclipThickz = iclipThick;
}

void Scene::TurnLighton(int n)
{
    if(n>3) return;  
	switch(n)
	{
	case 1:
		light1->Turnon();
		break;
	case 2:
		light2->Turnon();
		break;
	case 3:
		light3->Turnon();
		break;
	}
}

void Scene::TurnLightoff(int n)
{
     if(n>3) return;  
	 switch (n)
	 {
	 case  1:
		 light1->Turnoff();
		 break;
	 case  2:
		 light2->Turnoff();
		 break;
	 case  3:
		 light3->Turnoff();
		 break;
	 }

}

void Scene::ChangeLightColorA(int n, float R, float G, float B)
{
     if(n>3) return;  
	 switch (n)
	 {
	 case  1:
		 light1->ChangeColorA(R,G,B);
		 break;
	 case  2:
		 light2->ChangeColorA(R,G,B);
		 break;
	 case  3:
		 light3->ChangeColorA(R,G,B);
		 break;
	 }
}

void Scene::ChangeLightColorD(int n, float R, float G, float B)
{
     if(n>3) return;  
	 switch (n)
	 {
	 case  1:
		 light1->ChangeColorD(R,G,B);
		 break;
	 case  2:
		 light2->ChangeColorD(R,G,B);
		 break;
	 case  3:
		 light3->ChangeColorD(R,G,B);
		 break;
	 }
}

void Scene::ChangeLightColorS(int n, float R, float G, float B)
{
     if(n>3) return;  
	 switch (n)
	 {
	 case  1:
		 light1->ChangeColorS(R,G,B);
		 break;
	 case  2:
		 light2->ChangeColorS(R,G,B);
		 break;
	 case  3:
		 light3->ChangeColorS(R,G,B);
		 break;
	 }
}


void Scene::ChangeLightPosition(int n, float x, float y, float z)
{
     if(n>3) return;  
	 switch (n)
	 {
	 case  1:
		 light1->ChangePosition(x,y,z);
		 break;
	 case  2:
		 light2->ChangePosition(x,y,z);
		 break;
	 case  3:
		 light3->ChangePosition(x,y,z);
		 break;
	 }
}


void Scene::Paint()
{
     Style  Sty;
	 Sty.TranspSolid();

     glEnable(GL_DEPTH_TEST);
     glEnable(GL_COLOR_MATERIAL);
     glEnable(GL_NORMALIZE);
	 glLineWidth(this->m_borderthick);

	//if(style&scene_Style_ShowVolume) 
/*	 if(this->m_showbox)
	 {
	   
       glBegin(GL_LINES);
	     //glColor4f(0.8f, 0.40f,0.4f,0.6f);
	     glColor4f(1.f, 0.f, 0.f, 0.6f);
		 glVertex3d(m_x0*0.995f, m_y0*0.995f, m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, m_y0*0.995f, m_z0*0.995f);
		 glVertex3d(m_x0*0.995f, m_y1*0.995f, m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, m_y1*0.995f, m_z0*0.995f);
		 glVertex3d(m_x0*0.995f, m_y0*0.995f, m_z1*0.995f);
		 glVertex3d(m_x1*0.995f, m_y0*0.995f, m_z1*0.995f);
		 glVertex3d(m_x0*0.995f, m_y1*0.995f, m_z1*0.995f);
		 glVertex3d(m_x1*0.995f, m_y1*0.995f, m_z1*0.995f);

  	     //glColor4f(0.4f, 0.80f,0.40f,0.6f);
  	     glColor4f(0.f, 1.f, 0.f, 0.6f);
		 glVertex3d(m_x0*0.995f, m_y0*0.995f, m_z0*0.995f);
		 glVertex3d(m_x0*0.995f, m_y1*0.995f, m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, m_y0*0.995f, m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, m_y1*0.995f, m_z0*0.995f);
		 glVertex3d(m_x0*0.995f, m_y0*0.995f, m_z1*0.995f);
		 glVertex3d(m_x0*0.995f, m_y1*0.995f, m_z1*0.995f);
		 glVertex3d(m_x1*0.995f, m_y0*0.995f, m_z1*0.995f);
		 glVertex3d(m_x1*0.995f, m_y1*0.995f, m_z1*0.995f);


     	 //glColor4f(0.4f, 0.4f,0.8f,0.6f);
     	 glColor4f(0.f, 0.f, 1.f, 0.6f);
		 glVertex3d(m_x0*0.995f, m_y0*0.995f, m_z0*0.995f);
		 glVertex3d(m_x0*0.995f, m_y0*0.995f, m_z1*0.995f);
		 glVertex3d(m_x1*0.995f, m_y0*0.995f, m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, m_y0*0.995f, m_z1*0.995f);
		 glVertex3d(m_x0*0.995f, m_y1*0.995f, m_z0*0.995f);
		 glVertex3d(m_x0*0.995f, m_y1*0.995f, m_z1*0.995f);
		 glVertex3d(m_x1*0.995f, m_y1*0.995f, m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, m_y1*0.995f, m_z1*0.995f);

       glEnd(); 
	 }*/

	 if(this->m_showbackface[0])
	 {
		 if (m_showbackface[0] & 1)
		 {
			 glBegin(GL_LINES);
			 glColor4f(0.f, 1.f, 0.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z0);
			 glVertex3d(m_x0, m_y1, m_z0);
			 glVertex3d(m_x0, m_y0, m_z1);
			 glVertex3d(m_x0, m_y1, m_z1);

			 glColor4f(0.f, 0.f, 1.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z0);
			 glVertex3d(m_x0, m_y0, m_z1);
			 glVertex3d(m_x0, m_y1, m_z0);
			 glVertex3d(m_x0, m_y1, m_z1);
			 glEnd();
		 }
	 }

	 if(this->m_showfrontface[0])
	 {
		 if (m_showfrontface[0] & 1)
		 {
			 glBegin(GL_LINES);
			 glColor4f(0.f, 1.f, 0.f, 0.6f);
			 glVertex3d(m_x1, m_y0, m_z0);
			 glVertex3d(m_x1, m_y1, m_z0);
			 glVertex3d(m_x1, m_y0, m_z1);
			 glVertex3d(m_x1, m_y1, m_z1);

			 glColor4f(0.f, 0.f, 1.f, 0.6f);
			 glVertex3d(m_x1, m_y0, m_z0);
			 glVertex3d(m_x1, m_y0, m_z1);
			 glVertex3d(m_x1, m_y1, m_z0);
			 glVertex3d(m_x1, m_y1, m_z1);
			 glEnd();

		 }
	 }

	 if(this->m_showbackface[1])
	 {
		 if (m_showbackface[1] & 1)
		 {
			 glBegin(GL_LINES);
			 glColor4f(0.f, 0.f, 1.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z0);
			 glVertex3d(m_x0, m_y0, m_z1);
			 glVertex3d(m_x1, m_y0, m_z0);
			 glVertex3d(m_x1, m_y0, m_z1);

			 glColor4f(1.f, 0.f, 0.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z0);
			 glVertex3d(m_x1, m_y0, m_z0);
			 glVertex3d(m_x0, m_y0, m_z1);
			 glVertex3d(m_x1, m_y0, m_z1);
			 glEnd();

		 }
	 }
	 if(this->m_showfrontface[1])
	 {
		 if (m_showfrontface[1] & 1)
		 {
			 glBegin(GL_LINES);
			 glColor4f(0.f, 0.f, 1.f, 0.6f);
			 glVertex3d(m_x0, m_y1, m_z0);
			 glVertex3d(m_x0, m_y1, m_z1);
			 glVertex3d(m_x1, m_y1, m_z0);
			 glVertex3d(m_x1, m_y1, m_z1);

			 glColor4f(1.f, 0.f, 0.f, 0.6f);
			 glVertex3d(m_x0, m_y1, m_z0);
			 glVertex3d(m_x1, m_y1, m_z0);
			 glVertex3d(m_x0, m_y1, m_z1);
			 glVertex3d(m_x1, m_y1, m_z1);
			 glEnd();
		 }
	 }
	 if(this->m_showbackface[2])
	 {
		 if (this->m_showbackface[2] & 1)
		 {
			 glBegin(GL_LINES);
			 glColor4f(1.f, 0.f, 0.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z0);
			 glVertex3d(m_x1, m_y0, m_z0);
			 glVertex3d(m_x0, m_y1, m_z0);
			 glVertex3d(m_x1, m_y1, m_z0);

			 glColor4f(0.f, 1.f, 0.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z0);
			 glVertex3d(m_x0, m_y1, m_z0);
			 glVertex3d(m_x1, m_y0, m_z0);
			 glVertex3d(m_x1, m_y1, m_z0);
			 glEnd();

		 }
	 }

	 if(this->m_showfrontface[2])
	 {
		 if (this->m_showfrontface[2] & 1)
		 {
			 glBegin(GL_LINES);
			 glColor4f(1.f, 0.f, 0.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z1);
			 glVertex3d(m_x1, m_y0, m_z1);
			 glVertex3d(m_x0, m_y1, m_z1);
			 glVertex3d(m_x1, m_y1, m_z1);

			 glColor4f(0.f, 1.f, 0.f, 0.6f);
			 glVertex3d(m_x0, m_y0, m_z1);
			 glVertex3d(m_x0, m_y1, m_z1);
			 glVertex3d(m_x1, m_y0, m_z1);
			 glVertex3d(m_x1, m_y1, m_z1);
			 glEnd();
		 }
	 }

	 if (m_showbackface[0] & 2)
	 {
		 glColor4f(1.f, 0.0f, 0.0f, 0.1f);
		 glBegin(GL_POLYGON);
		 glNormal3d(-1.0, 0.0, 0.0);
		 glVertex3d(m_x0, m_y0, m_z0);
		 glVertex3d(m_x0, m_y1, m_z0);
		 glVertex3d(m_x0, m_y1, m_z1);
		 glVertex3d(m_x0, m_y0, m_z1);
		 glEnd();
	 }
	 if (m_showbackface[1] & 2)
	 {
		 glColor4f(0.0f, 1.0f, 0.0f, 0.1f);
		 glBegin(GL_POLYGON);
		 glNormal3d(0.0, -1.0, 0.0);
		 glVertex3d(m_x0, m_y0, m_z0);
		 glVertex3d(m_x0, m_y0, m_z1);
		 glVertex3d(m_x1, m_y0, m_z1);
		 glVertex3d(m_x1, m_y0, m_z0);
		 glEnd();
	 }
	 if (this->m_showbackface[2] & 2)
	 {
		 glColor4f(0.0f, 0.0f, 1.0f, 0.1f);
		 glBegin(GL_POLYGON);
		 glNormal3d(0.0, 0.0, -1.0);
		 glVertex3d(m_x0, m_y0, m_z0);
		 glVertex3d(m_x1, m_y0, m_z0);
		 glVertex3d(m_x1, m_y1, m_z0);
		 glVertex3d(m_x0, m_y1, m_z0);
		 glEnd();
	 }
	 if (m_showfrontface[0] & 2)
	 {
		 glColor4f(1.f, 0.0f, 0.0f, 0.1f);
		 glBegin(GL_POLYGON);
		 glNormal3d(1.0, 0.0, 0.0);
		 glVertex3d(m_x1, m_y0, m_z0);
		 glVertex3d(m_x1, m_y1, m_z0);
		 glVertex3d(m_x1, m_y1, m_z1);
		 glVertex3d(m_x1, m_y0, m_z1);
		 glEnd();
	 }
	 if (m_showfrontface[1] & 2)
	 {
		 glColor4f(0.0f, 1.0f, 0.0f, 0.1f);
		 glBegin(GL_POLYGON);
		 glNormal3d(0.0, 1.0, 0.0);
		 glVertex3d(m_x0, m_y1, m_z0);
		 glVertex3d(m_x0, m_y1, m_z1);
		 glVertex3d(m_x1, m_y1, m_z1);
		 glVertex3d(m_x1, m_y1, m_z0);
		 glEnd();
	 }
	 if (this->m_showfrontface[2] & 2)
	 {
		 glColor4f(0.0f, 0.0f, 1.0f, 0.1f);
		 glBegin(GL_POLYGON);
		 glNormal3d(0.0, 0.0, 1.0);
		 glVertex3d(m_x0, m_y0, m_z1);
		 glVertex3d(m_x1, m_y0, m_z1);
		 glVertex3d(m_x1, m_y1, m_z1);
		 glVertex3d(m_x0, m_y1, m_z1);
		 glEnd();
	 }


     PaintClipPlane();
	 glDisable(GL_BLEND);

}

void Scene::PaintClipPlane()
{
	// to draw clipplane if they are enabled and shown
	float d;

    glColor4f(0.8f, 0.4f,0.4f,0.6f);
	if( (clipState[0]&clip_State_Enabled)&&(clipState[0]&clip_State_Shown) )
	{
		if(clip1[0]>0) d = -clip1[3]*0.995f; 
		else d = clip1[3]*0.995f;
        glBegin(GL_LINE_LOOP);
		 glVertex3d(d, m_y0*0.995f, m_z0*0.995f);
		 glVertex3d(d, m_y1*0.995f, m_z0*0.995f);
		 glVertex3d(d, m_y1*0.995f, m_z1*0.995f);
		 glVertex3d(d, m_y0*0.995f, m_z1*0.995f);
        glEnd(); 
		if(clipState[3]&clip_State_Enabled)
		{
		   if(clip1p[0]>0) d = -clip1p[3]*0.995f; 
		   else d = clip1p[3]*0.995f;
           glBegin(GL_LINE_LOOP);
		    glVertex3d(d, m_y0*0.995f, m_z0*0.995f);
		    glVertex3d(d, m_y1*0.995f, m_z0*0.995f);
		    glVertex3d(d, m_y1*0.995f, m_z1*0.995f);
		    glVertex3d(d, m_y0*0.995f, m_z1*0.995f);
           glEnd(); 

		}
	 }

    glColor4f(0.4f, 0.8f,0.4f,0.6f);
	if( (clipState[1]&clip_State_Enabled)&&(clipState[1]&clip_State_Shown) )
	{
		if(clip2[1]>0) d = -clip2[3]*0.995f; 
		else d = clip2[3]*0.995f;
        glBegin(GL_LINE_LOOP);
		 glVertex3d(m_x0*0.995f, d,  m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, d,  m_z0*0.995f);
		 glVertex3d(m_x1*0.995f, d,  m_z1*0.995f);
		 glVertex3d(m_x0*0.995f, d,  m_z1*0.995f);
        glEnd(); 
		if (clipState[4]&clip_State_Enabled)
		{
		  if(clip2p[1]>0) d = -clip2p[3]*0.995f; 
		  else d = clip2p[3]*0.995f;
          glBegin(GL_LINE_LOOP);
		    glVertex3d(m_x0*0.995f, d,  m_z0*0.995f);
		    glVertex3d(m_x1*0.995f, d,  m_z0*0.995f);
		    glVertex3d(m_x1*0.995f, d,  m_z1*0.995f);
		    glVertex3d(m_x0*0.995f, d,  m_z1*0.995f);
          glEnd(); 
		}

	 }

    glColor4f(0.4f, 0.4f,0.8f,0.6f);
	if( (clipState[2]&clip_State_Enabled)&&(clipState[2]&clip_State_Shown) )
	{
		if(clip3[2]>0) d = -clip3[3]*0.995f; 
		else d = clip3[3]*0.995f;
        glBegin(GL_LINE_LOOP);
		 glVertex3d(m_x0*0.995f, m_y0*0.995f, d);
		 glVertex3d(m_x1*0.995f, m_y0*0.995f, d);
		 glVertex3d(m_x1*0.995f, m_y1*0.995f, d);
		 glVertex3d(m_x0*0.995f, m_y1*0.995f, d);
        glEnd(); 
		if(clipState[5]&clip_State_Enabled)
		{
		  if(clip3[2]>0) d = -clip3p[3]*0.995f; 
		  else d = clip3p[3]*0.995f;
          glBegin(GL_LINE_LOOP);
		    glVertex3d(m_x0*0.995f, m_y0*0.995f, d);
		    glVertex3d(m_x1*0.995f, m_y0*0.995f, d);
		    glVertex3d(m_x1*0.995f, m_y1*0.995f, d);
		    glVertex3d(m_x0*0.995f, m_y1*0.995f, d);
          glEnd(); 
		}
	 }
}

void Scene::SetClipPlaneShown(int id)
{

	 switch (id)
	 {
	 case 1:
		 clipState[0]= clipState[0]|clip_State_Shown;
		 break;
	 case 2:
		 clipState[1]= clipState[1]|clip_State_Shown;
		 break;
	 case 3:
		 clipState[2]= clipState[2]|clip_State_Shown;
		 break;
	 }
}


int Scene::IfClipPlaneVisiable(int id)
{

	 int i = id-1;
	 return (clipState[i]&clip_State_Shown);
}


void Scene::SetClipPlaneHiden(int id)
{
	 switch (id)
	 {
	 case 1:
           clipState[0] = clipState[0]&clip_State_Enabled;
		 break;
	 case 2:
           clipState[1] = clipState[1]&clip_State_Enabled;
		 break;
	 case 3:
           clipState[2] = clipState[2]&clip_State_Enabled;
		 break;
	 }
}


void Scene::SetVolumeShown()
{

     style = style|scene_Style_ShowVolume;
}

void Scene::SetVolumeHiden()
{
	 int I = style&scene_Style_Perspective;
     style = I;
}

void Scene::SetAsPerspective()
{
	 style = style|scene_Style_Perspective;
}


void Scene::SetAsOrtho()
{
	 style = style&scene_Style_ShowVolume;
}

void Scene::Save(FILE*pFile)
{

	  fprintf(pFile, "x0 = %f\n",m_x0);
	  fprintf(pFile, "y0 = %f\n",m_y0);
	  fprintf(pFile, "z0 = %f\n",m_z0);
	  fprintf(pFile, "x1 = %f\n",m_x1);
	  fprintf(pFile, "y1 = %f\n",m_y1);
	  fprintf(pFile, "z1 = %f\n",m_z1);

	  fprintf(pFile, "zoom percentage= %f\n",m_zoomPercent);
	  fprintf(pFile, "Rotation step along x axsis = %f\n",m_rotationStepx);
	  fprintf(pFile, "Rotation step along y axsis = %f\n",m_rotationStepy);
	  fprintf(pFile, "Rotation step along z axsis = %f\n",m_rotationStepz);

	  fprintf(pFile, "Translate step horizontally = %f\n",m_itransStepx);
	  fprintf(pFile, "Translation step vertically   = %f\n",m_itransStepy);

	  fprintf(pFile, "Clipplane move step along x = %f\n",iclipStepx);
	  fprintf(pFile, "Clipplane move step along y = %f\n",iclipStepy);
	  fprintf(pFile, "Clipplane move step along z = %f\n",iclipStepz);

	  fprintf(pFile, "Clipplane pair thick along x = %f\n",iclipThickx);
	  fprintf(pFile, "Clipplane pair thick along y = %f\n",iclipThicky);
	  fprintf(pFile, "Clipplane pair thick along z = %f\n",iclipThickz);
}


#define slen 80
void Scene::Read(FILE*pFile)
{
	
	char str[slen], nstr[slen];
	int c, i,j, len;


	// to get X0
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_x0 = atof(nstr);
	//to get Y0
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_y0 = atof(nstr);
	//to get Z0
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_z0 = atof(nstr);
	//to get X1
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_x1 = atof(nstr);
	//to get Y1
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_y1 = atof(nstr);
	//to get Z1
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_z1 = atof(nstr);
	//to get zoom
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_zoomPercent = atof(nstr);
	//to get rotation step x
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_rotationStepx = atof(nstr);
	//to get rotation step y
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_rotationStepy = atof(nstr);
	//to get rotation step z
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_rotationStepz = atof(nstr);
	//to get translation H
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_itransStepx = atof(nstr);
	//to get translation V
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	m_itransStepy = atof(nstr);
	//to get clip x step
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	iclipStepx = atof(nstr);
	//to get clip y step
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	iclipStepy = atof(nstr);
	//to get clip z step
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	iclipStepz = atof(nstr);
	//to get clip x thick
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	iclipThickx = atof(nstr);
	//to get clip y thick
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	iclipThicky = atof(nstr);
	//to get clip z thick
	for(i=0; i<slen; i++) 
	{
	   c = fgetc(pFile);
	   if(c == '\n') break;
	   str[i] = (char)c;
	}
	len = i;
	str[i] =NULL;
	j = strcspn( str, "=" )+1;
	for(i=j; i<len; i++) nstr[i-j] = str[i];
	iclipThickz = atof(nstr);
	      
}

void Scene::TransDevice(int fx, int fy, int tox, int toy)
{
/*	 GLint  viewport[4];
	
	 glGetIntegerv(GL_VIEWPORT, viewport);

	 float  w, h, dxp, dyp;

     w =(float)viewport[2];
     h =(float)viewport[3];

	 dxp = xpixels; //(float)(xpixels)-w/2.f;
	 dyp = ypixels; //h/2.f-(float)ypixels;

	 if(w == 0.f ) w = 1.f; 
	 if(h == 0.f)  h = 1.f;
	 if (w <= h) 
	 {
		h = (y1-y0)*w/h;
		w = (x1-x0);
	 }
	 else 
	 {
		w = (x1-x0)*h/w;
		h = (y1-y0);
	 }

	 dxp = dxp/(float)viewport[2]/w;
	 dyp = dyp/(float)viewport[3]/h;

	 int oldSys = system;
	 SetAsLaboratorySystem();
     TransL(dxp); 
     //TransU(dyp); 
	 system = oldSys;

*/	  
	 //float  x0,y0,z0;
	 //float  x1,y1,z1;


     double winx, winy, winz, oldx, oldy, oldz, newx, newy, newz;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];

	  glGetIntegerv(GL_VIEWPORT, viewport);
	  glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	  oldx = 0.0;
	  oldy = 0.0;
	  oldz = 0.0;
      gluProject(oldx, oldy, oldz, modelMatrix, projMatrix, viewport, 
		           &winx, &winy, &winz);


	  winx = (double)fx; //xPos;
	  winy = (double)(viewport[3]-fy); //yPos);
	  winz = 0.0;

      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &oldx, &oldy, &oldz);

	  winx = (double)tox; //xPos;
	  winy = (double)(viewport[3]-toy); //yPos);

      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &newx, &newy, &newz);
	  glTranslated(newx-oldx, newy-oldy, newz-oldz);
      m_itransX = m_itransX + newx-oldx; 
	  m_itransY = m_itransY + newy-oldy;
	  m_itransZ = m_itransZ + newz-oldz;

	 //int oldSys = system;
//	 SetAsLaboratorySystem();
     //TransL((float)(oldx-newx)); 
     //TransU((float)(oldy-newy)); 
	 //glTranslated(-oldx, -oldy, -oldz);
	 //system = oldSys;

}

void Scene::TransDevice(int fx, int fy)
{
      double winx, winy, winz, oldx, oldy, oldz, newx, newy, newz;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];

	  glGetIntegerv(GL_VIEWPORT, viewport);
	  glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	  winx = (double)fx; 
	  winy = (double)(viewport[3]-fy); 
	  winz = 0.0;

      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &oldx, &oldy, &oldz);

	  winx = (double)viewport[2]*0.5;
	  winy = (double)viewport[3]*0.5;

      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &newx, &newy, &newz);
	  glTranslated(newx-oldx, newy-oldy, newz-oldz);

      m_itransX = m_itransX + newx-oldx; 
	  m_itransY = m_itransY + newy-oldy;
	  m_itransZ = m_itransZ + newz-oldz;

     return;
}

void Scene::TransDevice()
{
     
      double winx, winy, winz, oldx, oldy, oldz, newx, newy, newz;
	  GLdouble modelMatrix[16];
	  GLdouble projMatrix[16];
	  GLint viewport[4];

	  glGetIntegerv(GL_VIEWPORT, viewport);
	  glGetDoublev(GL_MODELVIEW_MATRIX,  modelMatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	  oldx = 0.5*(m_x0 + m_x1); 
	  oldy = 0.5*(m_y0 + m_y1);
	  oldz = 0.5*(m_z0 + m_z1);
      gluProject(oldx, oldy, oldz, modelMatrix, projMatrix, viewport, 
		           &winx, &winy, &winz);

	  winx = (double)viewport[2]*0.5;
	  winy = (double)viewport[3]*0.5; 
      gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, 
		           &newx, &newy, &newz);

	  glTranslated(newx-oldx, newy-oldy, newz-oldz);

     return;
}

void Scene::ResetFocus(float newx,float newy,float newz)
{
	 float dx = newx - this->m_rotationCx; 
	 float dy = newy - this->m_rotationCy; 
	 float dz = newz - this->m_rotationCz; 

	    glTranslatef(dx, dy, dz);
		glClipPlane(GL_CLIP_PLANE0, clip1);
		glClipPlane(GL_CLIP_PLANE1, clip2);
		glClipPlane(GL_CLIP_PLANE2, clip3);
		glClipPlane(GL_CLIP_PLANE3, clip1p);
		glClipPlane(GL_CLIP_PLANE4, clip2p);
		glClipPlane(GL_CLIP_PLANE5, clip3p);

     this->m_rotationCx = newx;
     this->m_rotationCy = newy;
     this->m_rotationCz = newz;
	 return;
}