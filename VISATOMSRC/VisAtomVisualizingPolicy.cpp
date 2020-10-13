/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#include <math.h>
#include <stdio.h>

#include "VisAtomVisualizingPolicy.h"

//default color table
static const int CORLOR_NUM = 10;
static const float CORLOR_TABLE[CORLOR_NUM][4] =
{
	  0.5f, 0.0f, 0.0f, 1.f,
	  0.0f, 0.3f, 0.0f, 1.f,
	  0.0f, 0.0f, 0.3f, 1.f,
	  0.5f, 0.3f, 0.0f, 1.f,
	  0.5f, 0.0f, 0.3f, 1.f,
	  0.0f, 0.3f, 0.3f, 1.f,
	  0.5f, 0.3f, 0.3f, 1.f,
	  0.1f, 0.0f, 0.0f, 1.f,
	  0.0f, 0.1f, 0.0f, 1.f,
	  0.0f, 0.0f, 0.1f, 1.f,
};

static const int CORLOR_MAP_SIZE  = VisAtomVisualizingPolicy::CORLOR_MAP_SIZE;
static const int CORLOR_MAP_SIZEM = CORLOR_MAP_SIZE-1;
static const float MAP_TABLE[CORLOR_MAP_SIZE][3] = 
{
    { 0.0, 0.0, 1.0, },
    { 0.0, 1.0, 1.0, },
    { 0.0, 1.0, 0.0, },
    { 1.0, 1.0, 0.0, },
    { 1.0, 0.5, 0.0, },
	{ 1.0, 0.0, 0.0, },
};
//size mapping parameters
int   VisAtomVisualizingPolicy::m_radcol = -1;
float VisAtomVisualizingPolicy::m_radscal = 1;
float*VisAtomVisualizingPolicy::m_raddata = NULL;

//color mapping parameters
int   VisAtomVisualizingPolicy::m_scalarcol  = -1;
int   VisAtomVisualizingPolicy::m_colormap   = 0; 
float VisAtomVisualizingPolicy::m_scalarmin  = 0.f;
float VisAtomVisualizingPolicy::m_scalarmax  = 1.f;      
float VisAtomVisualizingPolicy::m_scalarrange = VisAtomVisualizingPolicy::m_scalarmax - VisAtomVisualizingPolicy::m_scalarmin;
//float VisAtomVisualizingPolicy::m_wscalarmin  = m_scalarmin;
//float VisAtomVisualizingPolicy::m_wscalarmax  = m_scalarmax;      
//float VisAtomVisualizingPolicy::m_wscalarrange = VisAtomVisualizingPolicy::m_wscalarmax - VisAtomVisualizingPolicy::m_wscalarmin;
float*VisAtomVisualizingPolicy::m_mappingdata = NULL;

float VisAtomVisualizingPolicy::m_wscalartab[CORLOR_MAP_SIZE]  = {0.f, 0.f, 0.f, 0.f, 0.f, 1.f}; 


//vector visualization parameteters
float VisAtomVisualizingPolicy::m_vlenscal  = -1.0f;
float VisAtomVisualizingPolicy::m_vrad      = 0.1f;
float VisAtomVisualizingPolicy::m_arrowl    = 0.2f;
float VisAtomVisualizingPolicy::m_arrowr    = 0.2f;   
int   VisAtomVisualizingPolicy::m_vcol[3]={-1,-1,-1}; 

//trajectory visualization parameters
VisAtomDataSheet* VisAtomVisualizingPolicy::m_datasheet = NULL;


//projection visualization parameters
int VisAtomVisualizingPolicy::m_autoBox = 1;
float VisAtomVisualizingPolicy::m_projectionBox[6]={0.f, 0.f, 0.f, 0.f, 0.f, 0.f};            



VisAtomVisualizingPolicy::~VisAtomVisualizingPolicy()
{
}

VisAtomVisualizingPolicy::VisAtomVisualizingPolicy()
{
	   
       m_geomStyle = GEOM_STYLE_ATOM | GEOM_STYLE_3D; //|GEOM_STYLE_PROJECT;
       //display property of atoms in the subsample
	   m_shapeStyle= SHAPE_STYLE_DOT;
	   m_radius    = 0.5f;
	   m_unifyradiu = 1;

	   m_vectorStyle = VECTOR_STYLE_LINE;
	   m_trajectoryStyle = TRAJECTORY_STYLE_STARTSITE|TRAJECTORY_STYLE_ENDSITE|TRAJECTORY_STYLE_HASLINE;

	   m_colorStyle = COLOR_STYLE_SINGLE;     
	   m_red        = CORLOR_TABLE[0][0];
	   m_green      = CORLOR_TABLE[0][1];
	   m_blue       = CORLOR_TABLE[0][2];
	   m_alpha      = CORLOR_TABLE[0][3];

	   m_tLinethick = 1.f;
	   m_tRadiusscal= 0.25f;

	   m_projectionStyle = VisAtomVisualizingPolicy::PROJECTION_STYLE_X0 
	                     + VisAtomVisualizingPolicy::PROJECTION_STYLE_Y0    
                         + VisAtomVisualizingPolicy::PROJECTION_STYLE_Z0;
	   m_3dStyle = VisAtomVisualizingPolicy::D3_STYLE_INBOX;

       return;
}

VisAtomVisualizingPolicy::VisAtomVisualizingPolicy(VisAtomVisualizingPolicy*sor)
{
	Paste(sor);
}

void VisAtomVisualizingPolicy::SetColorByDefaultTable(int index)
{
	int it = index;
	if(it >= CORLOR_NUM) it = 0;

	m_red        = CORLOR_TABLE[it][0];
	m_green      = CORLOR_TABLE[it][1];
	m_blue       = CORLOR_TABLE[it][2];
	m_alpha      = CORLOR_TABLE[it][3];
    return;
}

//***********************************************************************************
// return the color of atom index
void VisAtomVisualizingPolicy::GetColor(float&R, float&G, float&B, float&A, int index, float*exscalar)
{ 

	 double scalar = exscalar[index];
	 if(scalar <= m_wscalartab[0])
	 {
		 R = MAP_TABLE[0][0];
		 G = MAP_TABLE[0][1];
		 B = MAP_TABLE[0][2];
		 return;
	 }
	 else if(scalar >= m_wscalartab[CORLOR_MAP_SIZEM])
	 {
		 R = MAP_TABLE[CORLOR_MAP_SIZEM][0];
		 G = MAP_TABLE[CORLOR_MAP_SIZEM][1];
		 B = MAP_TABLE[CORLOR_MAP_SIZEM][2];
		 return;
	 }

     int  i, it, it1;
	 for(i=1; i<CORLOR_MAP_SIZE; i++)
	 {
		 if(scalar < m_wscalartab[i]) break;
	 }
     it1 = i;
	 it  = it1-1;
     float dv = (scalar - m_wscalartab[it])/( m_wscalartab[it1] - m_wscalartab[it]); 
	 R =   MAP_TABLE[it][0] + dv*(MAP_TABLE[it1][0]- MAP_TABLE[it][0]); 
	 G =   MAP_TABLE[it][1] + dv*(MAP_TABLE[it1][1]- MAP_TABLE[it][1]); 
	 B =   MAP_TABLE[it][2] + dv*(MAP_TABLE[it1][2]- MAP_TABLE[it][2]); 
	 return;
}


//***********************************************************************************
// return the color of atom index
void VisAtomVisualizingPolicy::GetColor(float&R, float&G, float&B, float&A, int index)
{ 
    
	if(this->m_colorStyle == VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)
	{
       this->GetColor(R, G, B, A);
	   return;
	}

	VisAtomVisualizingPolicy::GetColor(R, G, B, A, index, this->m_mappingdata);
	return;
}

//***********************************************************************************
void VisAtomVisualizingPolicy::Paste(VisAtomVisualizingPolicy*sor)
{
	
    //display property of atoms in the subsample
	m_shapeStyle = sor->m_shapeStyle;
	m_radius     = sor->m_radius;                               
	m_vlenscal   = sor->m_vlenscal;
	m_vrad       = sor->m_vrad;
	m_arrowl     = sor->m_arrowl;
	m_arrowr     = sor->m_arrowr; 
	m_vcol[1]    = sor->m_vcol[1];                              
	m_vcol[2]    = sor->m_vcol[2];                              
	m_vcol[3]    = sor->m_vcol[3];                              

	m_colorStyle = sor->m_colorStyle;    
	m_red        = sor->m_red;
	m_green      = sor->m_green;
	m_blue       = sor->m_blue;
	m_alpha      = sor->m_alpha;

	m_trajectoryStyle = sor->m_trajectoryStyle;
	m_tLinethick      = sor->m_tLinethick;
	m_tRadiusscal     = sor->m_tRadiusscal;

	m_projectionStyle = sor->m_projectionStyle; 
	m_3dStyle         = sor->m_3dStyle;
}

int  VisAtomVisualizingPolicy::GetColorMappingNumBar()
{
	return CORLOR_MAP_SIZE;
}

void VisAtomVisualizingPolicy::GetColorMappingBar(int index, float&R, float&G, float&B)
{
	R = MAP_TABLE[index][0];
	G = MAP_TABLE[index][1];
	B = MAP_TABLE[index][2];
}

void VisAtomVisualizingPolicy::SetScalarMappingTab(float smin, float smax)
{    
     float  dv = (smax - smin)/(float)CORLOR_MAP_SIZEM;
     for(int i=0; i<CORLOR_MAP_SIZE; i++) m_wscalartab[i] = smin+(float)i*dv;
}
//		static void SetScalarWindow(float min, float max) { m_wscalarmin = min; m_wscalarmax = max; m_wscalarrange = m_wscalarmax - m_wscalarmin;}
//		static void SetScalarWindowMin(float min) { m_wscalarmin = min; m_wscalarrange = m_wscalarmax - m_wscalarmin;}//
//		static void SetScalarWindowMax(float max) { m_wscalarmax = max; m_wscalarrange = m_wscalarmax - m_wscalarmin;}
//		static void GetScalarWindow(float&min, float&max) { min = m_wscalarmin;  max = m_wscalarmax;}


