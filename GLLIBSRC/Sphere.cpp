#include <math.h>

#include "Sphere.h"

int Sphere::fai=0;
int Sphere::theta=0;

#define c_MXNFAI 64
#define c_MXNTHETA 64
#define DEFAUT_NUMFAI   12
#define DEFAUT_NUMTHETA 12

static float cosfai[c_MXNFAI], sinfai[c_MXNFAI];
static float costheta[c_MXNTHETA],sintheta[c_MXNTHETA]; 
static float tx1[c_MXNFAI], tx2[c_MXNFAI]; 
static float ty1[c_MXNFAI], ty2[c_MXNFAI]; 
static float tz1[c_MXNFAI], tz2[c_MXNFAI];
static float pjx[c_MXNTHETA][c_MXNFAI];
static float pjy[c_MXNTHETA][c_MXNFAI];

void Sphere::Set(int Nf, int Nt)
{
	int i, j;
	double ang,step;

	if( (fai == Nf) && (theta == Nt)) return;

	  fai = Nf;
	  theta = Nt;
	  if(fai   > c_MXNFAI)   fai   = c_MXNFAI;
	  if(theta > c_MXNTHETA) theta = c_MXNTHETA;

	  step = (2.*3.14159265359)/ (double) (fai-1);
	  for(i=0; i<fai; i++)
	  {
		 ang = step *(double) i;
		 cosfai[i] = cos(ang);
		 sinfai[i] = sin(ang);
	  }

	  step = 3.14159265359/ (double) (theta-1);
	  for(i=0; i<theta; i++)
	  {
		 ang = step *(double) i;
		 costheta[i] = cos(ang);
		 sintheta[i] = sin(ang);
	  }

      for(i=0; i<theta; i++)
	  {
		  for(j=0; j<fai; j++) 
		  {
			  pjx[i][j] = sintheta[i]*cosfai[j];
			  pjy[i][j] = sintheta[i]*sinfai[j];
		  }
	  }
    return;
}


Sphere::~Sphere()
{
      if(list> 0) glDeleteLists(list,1);
}

Sphere::Sphere()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;

}

Sphere::Sphere(float radiu)
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	size = radiu;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}

Sphere::Sphere(float center[3])
{
	x = center[0];
	y = center[1];
	z = center[2];
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}

Sphere::Sphere(float radiu, float center[3])
{
	x = center[0];
	y = center[1];
	z = center[2];
	size = radiu;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}

Sphere::Sphere(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
	size = 1.f;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}

Sphere::Sphere(float radiu, float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
	size = radiu;
    R = 1.f;
	G = 1.f;
	B = 1.f;
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}

Sphere::Sphere(float radiu, float center[3], float color[3])
{
	x = center[0];
	y = center[1];
	z = center[2];
	size = radiu;
    R = color[0];
	G = color[1];
	B = color[2];
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}


Sphere::Sphere(float radiu, float X, float Y, float Z, float color[3])
{
	x = X;
	y = Y;
	z = Z;
	size = radiu;
    R = color[0];
	G = color[1];
	B = color[2];
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}


Sphere::Sphere(float radiu, float X, float Y, float Z, float Red, float Green, float Blue)
{
	x = X;
	y = Y;
	z = Z;
	size = radiu;
    R = Red;
	G = Green;
	B = Blue;
	if(Sphere::fai <= 0 || Sphere::theta <= 0) Set(DEFAUT_NUMFAI,DEFAUT_NUMTHETA);
    list = 0;
}

inline void Sphere::Paint()
{
   int i,j;

	glPushMatrix();
	glTranslatef(x,y,z);

    if(list > 0)
	{
       glScalef(size,size,size);
       glCallList(list);
	   glPopMatrix();
	   return;
	}
    //direct paint
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
    glColor4f(R,G,B,A);
	for(i=0; i<fai; i++)
	{
	    tx1[i] = size*pjx[0][i]; //sintheta[0]*cosfai[i];
	    ty1[i] = size*pjy[0][i]; //sintheta[0]*sinfai[i];
	    tz1[i] = size*costheta[0];
	}

	//**** to draw the ball
	for(j=1; j<theta; j++)
	{
	    for(i=0; i<fai; i++)
	    {
	        tx2[i] = size*pjx[j][i];
	        ty2[i] = size*pjy[j][i];
	        tz2[i] = size*costheta[j];
	    }
        // to draw the strips
        glBegin(GL_TRIANGLE_STRIP);
        for(i=0; i<fai; i++)
        {
		    glNormal3f(tx1[i], ty1[i], tz1[i]);                 
		    glVertex3f(tx1[i], ty1[i], tz1[i]);
		    glNormal3f(tx2[i], ty2[i], tz2[i]);                 
		    glVertex3f(tx2[i], ty2[i], tz2[i]);
		 }
         glEnd();
		 for(i=0; i<fai; i++)
		 {
		     tx1[i] = tx2[i];
		     ty1[i] = ty2[i];
		     tz1[i] = tz2[i];
		  }
	}
	glPopMatrix();
	return;
}


void Sphere::Paint(Texture*tex)
{
   int i,j;
   float *texx;
   float texy1, texy2;
   float texdx, texdy;

	glPushMatrix();
	glTranslatef(x,y,z);

      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      glCallList(tex->List());
	  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	  glDisable(GL_TEXTURE_GEN_S);
	  glDisable(GL_TEXTURE_GEN_T);
      glEnable(tex->Dim());	 

	  glEnable(GL_COLOR_MATERIAL);
	  glEnable(GL_NORMALIZE);
      glColor4f(R,G,B,A);

	  texx = new float[fai];
	  texdx = 1.0/(float) (fai-1);
	  texdy = 1.0/(float) (theta-1);

	     for(i=0; i<fai; i++)
		 {
		     tx1[i] = size*pjx[0][i]; //sintheta[0]*cosfai[i];
		     ty1[i] = size*pjy[0][i]; //sintheta[0]*sinfai[i];
		     tz1[i] = size*costheta[0];
			 texx[i] = (float)i*texdx;
		 }
		 texy1= 0;

		 //**** to draw the ball
	       for(j=1; j<theta; j++)
		   {
	          for(i=0; i<fai; i++)
		      {

		         tx2[i] = size*pjx[j][i]; //sintheta[j]*cosfai[i];
		         ty2[i] = size*pjy[j][i]; //sintheta[j]*sinfai[i];
		         tz2[i] = size*costheta[j];
			  }
			  texy2 = (float)j*texdy;
              // to draw the strips
              glBegin(GL_TRIANGLE_STRIP);
	            for(i=0; i<fai; i++)
		         {
				    glNormal3f(tx1[i], ty1[i], tz1[i]);                 
					glTexCoord2f(texx[i], texy1);
				    glVertex3f(tx1[i], ty1[i], tz1[i]);
				    glNormal3f(tx2[i], ty2[i], tz2[i]);                 
					glTexCoord2f(texx[i], texy2);
		            glVertex3f(tx2[i], ty2[i], tz2[i]);
			     }
              glEnd();
			  for(i=0; i<fai; i++)
			  {
				  tx1[i] = tx2[i];
				  ty1[i] = ty2[i];
				  tz1[i] = tz2[i];
			  }
			  texy1 = texy2;
		   }
      glDisable(tex->Dim()); 
	  delete texx;
	glPopMatrix();

	return; 
}

int Sphere::MakeList()
{
    int i, j;

	if(list>0) return list;
		
      glNewList(list = glGenLists(1), GL_COMPILE);

	     for(i=0; i<fai; i++)
		 {
		     tx1[i] = pjx[0][i]; //sintheta[0]*cosfai[i];
		     ty1[i] = pjy[0][i]; //sintheta[0]*sinfai[i];
		     tz1[i] = costheta[0];
		 }

		 //**** to draw the ball
	       for(j=1; j<theta; j++)
		   {
	          for(i=0; i<fai; i++)
		      {

		         tx2[i] = pjx[j][i]; //sintheta[j]*cosfai[i];
		         ty2[i] = pjy[j][i]; //sintheta[j]*sinfai[i];
		         tz2[i] = costheta[j];
			  }
              // to draw the strips
              glBegin(GL_TRIANGLE_STRIP);
	            for(i=0; i<fai; i++)
		         {
				    glNormal3f(tx1[i], ty1[i], tz1[i]);                 
				    glVertex3f(tx1[i], ty1[i], tz1[i]);
				    glNormal3f(tx2[i], ty2[i], tz2[i]);                 
			        glVertex3f(tx2[i], ty2[i], tz2[i]);
			     }
              glEnd();
			  for(i=0; i<fai; i++)
			  {
				  tx1[i] = tx2[i];
				  ty1[i] = ty2[i];
				  tz1[i] = tz2[i];
			  }
		   }
	glEndList(); 		
   return list;
}



