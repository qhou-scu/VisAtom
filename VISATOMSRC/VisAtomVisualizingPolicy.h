/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _VISATOMVISUALIZINGPOLICY_H
#define _VISATOMVISUALIZINGPOLICY_H
#include "GlcObj.h"
 
#include "VisAtomDataSheet.h"

//*********************************************************************
class VisAtomVisualizingPolicy:public GlcObj
{
	private:
    protected:

       //
       int  m_geomStyle;
       //display property of atoms in the subsample
	   int   m_shapeStyle;
	   float m_radius;                                  //sphere and circle parameter
	   int   m_unifyradiu;
	   //the static variable used when atoms have variable radiaa
       static int   m_radcol;
	   static float m_radscal;
       static float*m_raddata;
	   
	   //static parameter for vectors
	   //should be the same for all atoms
	   int    m_vectorStyle;
	   static float m_vlenscal, m_vrad, m_arrowl, m_arrowr;     //vector parameter 
	   static int   m_vcol[3];                                  //identification for vector data if has vector
	   
	   //parameters for coloring
	   int   m_colorStyle;                              //color style
	   float m_red, m_green, m_blue, m_alpha;           //color parameter for single color

	   //static color-mapping paramters, should be
	   //same for all atoms
	   static int   m_scalarcol;                        //indification for scarlar data, used if using scalar data for coloring
	   static int   m_colormap;                         //index for color mapping table
	   static float m_scalarmin,  m_scalarmax,   m_scalarrange;   //
	   //static float m_wscalarmin, m_wscalarmax, m_wscalarrange;   //
	   static float*m_mappingdata;
	   static float m_wscalartab[6];                    //scalar table mapping to color maping tbale 

      
	   static VisAtomDataSheet*m_datasheet;            //datasheet used for colr mapping

	   //trajectory style
	   int m_trajectoryStyle;
	   float m_tLinethick;                            //line for drawing trajectory
	   float m_tRadiusscal;                           //the scal relative to m_radius for drawing of sites

	   //projection style
       static int   m_autoBox;
	   static float m_projectionBox[6];               //the box on its side faces, the sample would be projected on, 
	   int m_projectionStyle;                         //the style of projection, determine which side faces the subsample will be projetec on

	   //3D scene style
	   int m_3dStyle;

    public:
		 static const int CORLOR_MAP_SIZE = 6;
	     enum VISATOMGEOMETRYSTYLE
		 {
              GEOM_STYLE_ATOM    =   1, 
              GEOM_STYLE_VECTOR  =   2,
              GEOM_STYLE_TRAJECT =   4,
			  GEOM_STYLE_PROJECT =   8,
			  GEOM_STYLE_3D      =   16
		 };

		 enum VISATOMGEOMSTYLE
		 {
              SHAPE_STYLE_DOT  =   1, 
              SHAPE_STYLE_LINE =   2, 
              SHAPE_STYLE_WIRE =   4, 
              SHAPE_STYLE_SOLID=   8,
			  SHAPE_STYLE_MASK = 0x0000000F,
		 };

		 enum VISATOMCOLORSTYLE
		 {
              COLOR_STYLE_SINGLE  = 0, 
              COLOR_STYLE_MAP     = 1 
		 };

		 enum VISATOMVECTORSTYLE
		 {
			  VECTOR_STYLE_ENDATOM  = 32,     //line vector  
			  VECTOR_STYLE_LINE     = 64,     //line vector  
              VECTOR_STYLE_SOLID    = 128,     //3D vector
              VECTOR_STYLE_ARROW    = 256,     //has arrow
 			  VECTOR_STYLE_MASK = 0x000000F0,
		 };

		 enum VISATOMP3DSTYLE
		 {
			 D3_STYLE_INBOX  = 1048576,      //8192*64*2
			 D3_STYLE_OUTBOX = 1048576 * 2,
			 D3_STYLE_MASK = D3_STYLE_INBOX + D3_STYLE_OUTBOX,

		 };

		 enum VISATOMTRAJECTORYSTYLE
		 {
			  TRAJECTORY_STYLE_STARTSITE  = 1024,    //has start site
			  TRAJECTORY_STYLE_ENDSITE    = 2048,    //has endsite  
              TRAJECTORY_STYLE_HASLINE    = 4096,    //line
              TRAJECTORY_STYLE_HASPOINT   = 8192,    //line
 			  TRAJECTORY_STYLE_MASK = 0x0000FC00,
		 };

		 enum VISATOMPROJECTIONSTYLE
		 {
			  PROJECTION_STYLE_X0  = 8192*2,    
			  PROJECTION_STYLE_X1  = 8192*4,    
			  PROJECTION_STYLE_Y0  = 8192*8,    
			  PROJECTION_STYLE_Y1  = 8192*16,   
			  PROJECTION_STYLE_Z0  = 8192*32,   
			  PROJECTION_STYLE_Z1  = 8192*64,   
  			  PROJECTION_STYLE_MASK = 8192*(2+4+8+16+32+64),

		 };


		VisAtomVisualizingPolicy();
		VisAtomVisualizingPolicy(VisAtomVisualizingPolicy*sor);
	    ~VisAtomVisualizingPolicy();

		void Paste(VisAtomVisualizingPolicy*sor);

		//********** color functions
		void SetColorByDefaultTable(int index);
		void SetColor(float R, float G, float B, float A)
		{    m_red = R; m_green = G; m_blue = B; m_alpha = A;}

		//get color of atoms in single-color style
		void GetColor(float&R, float&G, float&B, float&A)
		{    R = m_red; G = m_green; B = m_blue; A = m_alpha; return;}

		//get color of atoms in mapping-color style
		//index : the id of the atom 
		void GetColor(float&R, float&G, float&B, float&A, int index);

		//get color of atoms in mapping-color style, with given scalar data sheet
		//index : the id of the atom 
		static void GetColor(float&R, float&G, float&B, float&A, int index, float*exscalar);

		void SetColorStyle(int colorstyle){ m_colorStyle = colorstyle;};
		int  GetColorStyle(){ return m_colorStyle;};

        //size functions for atoms
		void SetAtomShapeStyle(int geom){ m_shapeStyle = geom;};
		int  GetAtomShapeStyle(){  return m_shapeStyle;};
		void  SetAtomSize(float r){m_radius = r;};
		float GetAtomSize(){ return m_radius; };
		float GetAtomSize(int i)
		{
			if(m_unifyradiu) return m_radius; 
			else return m_raddata[i];
		};
		void  SetAtomHaveUnifiedSize(bool yes){ m_unifyradiu = yes;};
		bool  IsAtomHaveUnifiedSize(){return m_unifyradiu;};
		static void  SetAtomSizeCol(int col) { m_radcol = col; m_raddata = m_datasheet->GetData(col);}
		static int   GetAtomSizeCol() { return m_radcol;}

        //*** functions concerning geometry style		
		int  GetGeomStyle()
		{   int style = 0;
			if(m_geomStyle&GEOM_STYLE_ATOM)     style = style|m_shapeStyle;
			if(m_geomStyle&GEOM_STYLE_VECTOR)   style = style|m_vectorStyle;
			if(m_geomStyle&GEOM_STYLE_TRAJECT)  style = style|m_trajectoryStyle;
			if (m_geomStyle&GEOM_STYLE_PROJECT) style = style|m_projectionStyle;
			if (m_geomStyle&GEOM_STYLE_3D)      style = style|m_3dStyle;
			return style;
		};

		void SetGeomStyleAtomVisible(bool visible) 
		{ int style = 0; 
		  //if(m_geomStyle&GEOM_STYLE_VECTOR) style = style| GEOM_STYLE_VECTOR;
		  //if(visible) style = style | GEOM_STYLE_ATOM;
		  style = style|(m_geomStyle&GEOM_STYLE_VECTOR);
		  style = style|(m_geomStyle&GEOM_STYLE_TRAJECT);
		  style = style|(m_geomStyle&GEOM_STYLE_PROJECT);
		  style = style|(m_geomStyle&GEOM_STYLE_3D);
		  if(visible) style = style | GEOM_STYLE_ATOM;
          m_geomStyle = style;
		}
		bool IsGeomStyleAtomVisible(){ return m_geomStyle&GEOM_STYLE_ATOM;} 
		

		//vector functions
		void  SetVectorStyle(int style) { m_vectorStyle = style;}
		int   GetVectorStyle() { return m_vectorStyle;}
		void SetGeomStyleVectorVisible(bool visible) 
		{ int style = 0; 
		  //if(m_geomStyle&GEOM_STYLE_ATOM) style = style| GEOM_STYLE_ATOM;
		  //if(visible) style = style | GEOM_STYLE_VECTOR;
		  style = style|(m_geomStyle&GEOM_STYLE_TRAJECT);
		  style = style|(m_geomStyle&GEOM_STYLE_ATOM);
		  style = style|(m_geomStyle&GEOM_STYLE_PROJECT);
		  style = style|(m_geomStyle&GEOM_STYLE_3D);
		  if(visible) style = style | GEOM_STYLE_VECTOR;

          m_geomStyle = style;
		}
		bool IsGeomStyleVectorVisible(){return m_geomStyle&GEOM_STYLE_VECTOR;}


		//trjectory functions
		void  SetTrajectoryStyle(int style) { m_trajectoryStyle = style;}
		int   GetTrajectoryStyle() { return m_trajectoryStyle;}
		void SetGeomStyleTrajectoryVisible(bool visible) 
		{ int style = 0; 
		  style = style|(m_geomStyle&GEOM_STYLE_ATOM);
		  style = style|(m_geomStyle&GEOM_STYLE_VECTOR);
		  style = style|(m_geomStyle&GEOM_STYLE_PROJECT);
		  style = style|(m_geomStyle&GEOM_STYLE_3D);
		  if(visible) style = style | GEOM_STYLE_TRAJECT;

          m_geomStyle = style;
		}
		bool IsGeomStyleTrajectoryVisible(){return m_geomStyle&GEOM_STYLE_TRAJECT;}
		void  SetTrajectoryLinethick(float thick){ m_tLinethick = thick;}
		float GetTrajectoryLinethick(){ return m_tLinethick;}
		void  SetTrajectorySitescal(float scal){ m_tRadiusscal = scal;}
		float GetTrajectorySitescal(){ return m_tRadiusscal;}
		float GetTrajectorySitesize(){ return m_tRadiusscal*m_radius;}

		//projecction functions
		void  SetProjectionStyle(int style) { m_projectionStyle = style;}
		int   GetProjectionStyle() { return m_projectionStyle;}
		void SetGeomStyleProjectionVisible(bool visible) 
		{ int style = 0; 
		  style = style|(m_geomStyle&GEOM_STYLE_ATOM);
		  style = style|(m_geomStyle&GEOM_STYLE_VECTOR);
		  style = style|(m_geomStyle&GEOM_STYLE_TRAJECT);
		  style = style|(m_geomStyle&GEOM_STYLE_3D);
		  if(visible) style = style | GEOM_STYLE_PROJECT;

          m_geomStyle = style;
		}
		bool IsGeomStyleProjectionVisible(){return m_geomStyle&GEOM_STYLE_PROJECT;}

		//3d functions
		void  Set3DStyle(int style) { m_3dStyle = style; }
		int   Get3DStyle() { return m_3dStyle; }
		void SetGeomStyle3DVisible(bool visible)
		{
			int style = 0;
			style = style | (m_geomStyle&GEOM_STYLE_ATOM);
			style = style | (m_geomStyle&GEOM_STYLE_VECTOR);
			style = style | (m_geomStyle&GEOM_STYLE_TRAJECT);
			style = style | (m_geomStyle&GEOM_STYLE_PROJECT);
			if (visible) style = style | GEOM_STYLE_3D;
			m_geomStyle = style;
		}
		bool IsGeomStyle3DVisible() { return m_geomStyle & GEOM_STYLE_3D; }


		//static functions:
		static void SetDataSheet(VisAtomDataSheet*datasheet)
		{ 
			m_datasheet = datasheet;
		    if(m_scalarcol >=0 && m_scalarcol < datasheet->GetNumCol()) m_mappingdata = m_datasheet->GetData(m_scalarcol); 
			else {m_scalarcol = -1; m_mappingdata = NULL;}

			if(m_radcol>=0 && m_radcol < datasheet->GetNumCol()) m_raddata = m_datasheet->GetData(m_radcol); 
			else {m_radcol = -1; m_raddata = NULL;}

		}
        //static functions concerning color mapping
		static void SetScalarCol(int col) {m_scalarcol = col; m_mappingdata = m_datasheet->GetData(m_scalarcol); }
		static int  GetScalarCol() { return m_scalarcol;}
		static void SetScalarRange(float min, float max) { m_scalarmin = min; m_scalarmax = max; m_scalarrange = m_scalarmax - m_scalarmin;}
		static void GetScalarRange(float&min, float&max) { min = m_scalarmin; max = m_scalarmax;}
		static int  GetColorMappingNumBar();
		static void GetColorMappingBar(int index, float&R, float&G, float&B);
		static void VisAtomVisualizingPolicy::SetScalarMappingTab(int index, float value)
        {    
	         VisAtomVisualizingPolicy::m_wscalartab[index] = value;
        }
		static float VisAtomVisualizingPolicy::GetScalarMappingTabValue(int index)
        {    
	         return VisAtomVisualizingPolicy::m_wscalartab[index];
        }
		static float VisAtomVisualizingPolicy::GetScalarMappingTabMinValue()
        {    
	         return VisAtomVisualizingPolicy::m_wscalartab[0];
        }
		static float VisAtomVisualizingPolicy::GetScalarMappingTabMaxValue()
        {    
	         return VisAtomVisualizingPolicy::m_wscalartab[sizeof(m_wscalartab)/sizeof(float)-1];
        }
        static void SetScalarMappingTab(float smin, float smax);

		//static functions concerning vector visualization 
		static bool HasVectorData()
		{
			bool yes = false;

			if(m_datasheet &&
			   (m_vcol[0] >= 0 && m_vcol[0] < m_datasheet->GetNumCol()) &&
			   (m_vcol[1] >= 0 && m_vcol[1] < m_datasheet->GetNumCol()) &&
			   (m_vcol[2] >= 0 && m_vcol[2] < m_datasheet->GetNumCol()) )
			   yes = true;

			return yes;
		}

		static void SetVectorCol(int vcol[3])
		{
			m_vcol[0] = vcol[0];
			m_vcol[1] = vcol[1];
			m_vcol[2] = vcol[2];
		}

		static void GetVectorCol(int vcol[3])
		{
			vcol[0] = m_vcol[0];
			vcol[1] = m_vcol[1];
			vcol[2] = m_vcol[2];
		}


		static void SetVectorSize(float vlenscal, float vrad, float arrowl, float arrowr)
		{
			   m_vlenscal = vlenscal;
			   m_vrad     = vrad;
			   m_arrowl   = arrowl;
			   m_arrowr   = arrowr;   
		}
		static void SetVectorSize(float vrad, float vlenscal){ m_vrad = vrad; m_vlenscal = vlenscal;}

		static void GetVectorSize(float&vlenscal, float&vrad, float&arrowl, float&arrowr)
		{
			   vlenscal = m_vlenscal;
			   vrad     = m_vrad;
			   arrowl   = m_arrowl;
			   arrowr   = m_arrowr;   
		}
		static void GetVectorSize(float&vrad, float&vlenscal){ vrad = m_vrad; vlenscal = m_vlenscal;}

		//static function concerning projection
		static int IsAutoProjectionBox()
		{
			   return m_autoBox;
		}

		static void SetAutoProjectionBox(bool autobox)
		{
			    m_autoBox = autobox;
		}

		static void SetProjectionBox(float x0, float x1, float y0, float y1, float z0, float z1)
		{
			   m_projectionBox[0] = x0;
			   m_projectionBox[1] = x1;
			   m_projectionBox[2] = y0;
			   m_projectionBox[3] = y1;
			   m_projectionBox[4] = z0;
			   m_projectionBox[5] = z1;
		}
		static void GetProjectionBox(float& x0, float& x1, float& y0, float& y1, float& z0, float& z1)
		{
			   x0 = m_projectionBox[0];
			   x1 = m_projectionBox[1];
			   y0 = m_projectionBox[2];
			   y1 = m_projectionBox[3];
			   z0 = m_projectionBox[4];
			   z1 = m_projectionBox[5];
		}

 };

#endif