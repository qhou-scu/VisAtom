/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _VISATOMCLIPER_H
#define _VISATOMCLIPER_H
#include "GlcObj.h"
#include "QList.h"
 
class Scene;
//************************************************************
class VisAtomCliper
{
   public:
          enum VISATOMCLIPERSTYLE
		  {
			  CLIPPING_STYLE_OR      = 0,
			  CLIPPING_STYLE_AND     = 1,
			  CLIPPING_STYLE_ANYDIR  = 2,
			  CLIPPING_STYLE_NUM     = 3
		   };
  protected:
	     enum VISCLIPERSTATU
	     {
		  CLIPPING_DISABLE = 0,
		  CLIPPING_ENABLE1 = 1,
		  CLIPPING_ENABLE2 = 2,
	      };

	      Scene* m_Scene;
		  int    m_Style;
		  int    m_ClipStatu[3];
		  int    m_Inv[3];
          double m_Axsis[3];     //in style 1-3, when the value -1.e-6< and <1.e-6, no clipping is enabled, corresponding to style 
		                         // CLIPPING_NONE. otherwise clipping  is enabled. The sign of Axsis determing the direction
		                         // of the clipping planes
		                         //in style CLIPPING_ANYDIR, m_Axsis represent the direction of the clipping plane.

		  double m_Pos[3];       //position of the clipping planes, in style 1-3, the postions are the 
		                         //section point of clipping planes with three axsis.
		  double m_CStep[3];    //step size ot thickness for clipping planes, in unit of the minima size of scene volumn

		  int m_ShowFrontface[3];
		  int m_ShowBackface[3];
		  double m_Opacity;
		  //to get the intersect of the clip plane with SCEN cubic:
          void GetClipPlaneContour(double x0, double y0, double z0, double x1,double y1,double z1,
								   double vnx, double vny, double vnz, double r, int&npt, double ppt[12][3]);
   public:
	   VisAtomCliper();
	   ~VisAtomCliper();

       void PaintEdge();
	   void PaintFace();

	   void AttachScene(Scene*pScene)
	   {
		   m_Scene = pScene;
	   };
	   Scene* GetAttachedScene()
	   {
		   return m_Scene;
	   };

	   void SetStyle(int style)
	   {
		   m_Style = style;
	   };
	   int  GetStyle()
	   {
		   return m_Style;
	   };
	   void EnableClip(bool enabled, int i)
	   {
		   int old = m_ClipStatu[i] & VISCLIPERSTATU::CLIPPING_ENABLE2;
		   if (enabled)
		   {
			   m_ClipStatu[i] = VISCLIPERSTATU::CLIPPING_ENABLE1 + old;
		   }
		   else
		   {
			   m_ClipStatu[i] = VISCLIPERSTATU::CLIPPING_DISABLE + old;
		   }
	   }
	   void EnableClipX(bool enabled = true)
	   {
		   //if (enabled) m_Axsis[0] = 1; else m_Axsis[0] = 0;
		   EnableClip(enabled, 0);
	   };
	   void EnableClipY(bool enabled = true)
	   {
		   //if (enabled) m_Axsis[1] = 1; else m_Axsis[1] = 0;
		   EnableClip(enabled, 1);
	   };
	   void EnableClipZ(bool enabled = true)
	   {
		   //if (enabled) m_Axsis[2] = 1; else m_Axsis[2] = 0;
		   EnableClip(enabled, 2);
	   };

	   void EnableClip(bool enabled = true)
	   {
		   EnableClip(enabled, 0);
		   EnableClip(enabled, 1);
		   EnableClip(enabled, 2);
	   }
	   bool IsEnabledClip(int i)
	   {
		   return (m_ClipStatu[i] & VisAtomCliper::CLIPPING_ENABLE1) > 0;
	   }
	   bool IsEnabledClipX()
	   {
		   return IsEnabledClip(0);
	   };
	   bool IsEnabledClipY()
	   {
		   return IsEnabledClip(1);
	   };
	   bool IsEnabledClipZ()
	   {  
		   return IsEnabledClip(2);
	   };
	   bool IsEnabledClip()
	   {
		   return IsEnabledClipX() || IsEnabledClipY() || IsEnabledClipZ();
	   };

	   void EnableDClip(bool enabled, int i)
	   {
		   int old = m_ClipStatu[i] & VISCLIPERSTATU::CLIPPING_ENABLE1;
		   if (enabled)
		   {
			   m_ClipStatu[i] = VISCLIPERSTATU::CLIPPING_ENABLE2 + old;
		   }
		   else
		   {
			   m_ClipStatu[i] = VISCLIPERSTATU::CLIPPING_DISABLE + old;
		   }
	   }
	   void EnableDClip(bool enabled = true)
	   {
		   EnableDClip(enabled, 0);
		   EnableDClip(enabled, 1);
		   EnableDClip(enabled, 2);
	   };
	   void EnableDClipX(bool enabled = true)
	   {
		   EnableDClip(enabled, 0);
		   //m_DClip[0] = enabled;
	   };
	   void EnableDClipY(bool enabled = true)
	   {
		   EnableDClip(enabled, 1);
		   //m_DClip[1] = enabled;
	   };
	   void EnableDClipZ(bool enabled = true)
	   {
		   EnableDClip(enabled, 2);
		   //m_DClip[2] = enabled;
	   };
	   bool IsEnabledDClip(int i)
	   {
		   return (m_ClipStatu[i] & VisAtomCliper::CLIPPING_ENABLE2) > 0;
	   }

	   bool IsEnabledDClipX()
	   {
		   return IsEnabledDClip(0); //return m_DClip[0];
	   };
	   bool IsEnabledDClipY()
	   {
		   return IsEnabledDClip(1); //return m_DClip[1];
	   };
	   bool IsEnabledDClipZ()
	   {
		   return IsEnabledDClip(2); //return m_DClip[2];
	   };
	   bool IsEnabledDClip()
	   {
		   return IsEnabledDClipX() || IsEnabledDClipY() || IsEnabledDClipZ();
	   }

	   void SetInverseClipX(bool enabled = true)
	   {
		   m_Inv[0] = enabled;
	   };
	   void SetInverseClipY(bool enabled = true)
	   {
		   m_Inv[1] = enabled;
	   };
	   void SetInverseClipZ(bool enabled = true)
	   {
		   m_Inv[2] = enabled;
	   };
	   bool IsInverseClipX()
	   {
		   return m_Inv[0];
	   };
	   bool IsInverseClipY()
	   {
		   return m_Inv[1];
	   };
	   bool IsInverseClipZ()
	   {
		   return m_Inv[2];
	   };

	
	   void SetClipPos(double pos)
	   {
		   m_Pos[0] = m_Pos[1] = m_Pos[2] = pos;
	   };
	   void SetClipPos(double pos[])
	   {
		   m_Pos[0] = pos[0]; m_Pos[1] = pos[1]; m_Pos[2] = pos[2];
	   };
	   void GetClipPos(double pos[])
	   {
		   pos[0] = m_Pos[0]; pos[1] = m_Pos[1]; pos[2] = m_Pos[2];
	   };
	   void GetClipPos(double&x, double&y, double&z)
	   {
		   x = m_Pos[0]; y = m_Pos[1]; z = m_Pos[2];
	   };

	    //get the position multiplied by box side
	   void GetClipAbsolutePos(double pos[]);

	   void SetClipStep(double step[])
	   {
		   m_CStep[0] = step[0]; m_CStep[1] = step[1]; m_CStep[2] = step[2];
	   };
	   void GetClipStep(double step[])
	   {
		   step[0] = m_CStep[0]; step[1] = m_CStep[1]; step[2] = m_CStep[2];
	   };
	   void    GetClipThickness(double thickness[]);
	   double  GetClipThickness()
	   {
		   return m_CStep[0];
	    };

	  //used in style of anydirection clipping 
	   void SetClipAxsis(double axsis[])
	   {
		   m_Axsis[0] = axsis[0]; m_Axsis[1] = axsis[1]; m_Axsis[2] = axsis[2];
	   };
	   void GetClipAxsis(double axsis[])
	   {
		   axsis[0] = m_Axsis[0]; axsis[1] = m_Axsis[1]; axsis[2] = m_Axsis[2];
	   };
	   void GetClipAxsis(double&x, double&y, double&z)
	   {
		   x = m_Axsis[0]; y = m_Axsis[1]; z = m_Axsis[2];
	   };

	   void SetParameters(VisAtomCliper*sor);
	   bool IsParameterChanged(VisAtomCliper*sor);

	   //used for set visualization parameters
	   void ShowFronFace(int show[])
	   {
		   m_ShowFrontface[0] = show[0]; m_ShowFrontface[1] = show[1]; m_ShowFrontface[2] = show[2];
	   };
	   void ShowFronFace(int show)
	   {
		   m_ShowFrontface[0] = m_ShowFrontface[1] = m_ShowFrontface[2] = show;
	   };

	   int IsShowFronFace()
	   {
		   return m_ShowFrontface[0];
	   };

	   void ShowBackFace(int show[])
	   {
		   m_ShowBackface[0] = show[0]; m_ShowBackface[1] = show[1]; m_ShowBackface[2] = show[2];
	   };
	   void ShowBackFace(int show)
	   {
		   m_ShowBackface[0] = m_ShowBackface[1] = m_ShowBackface[2] = show;
	   };
	   int IsShowBackFace()
	   {
		   return m_ShowBackface[0];
	   };

	   void SetFaceOpacity(double opacity)
	   {
		   m_Opacity = opacity;
	   };
	   double GetFaceOpacity()
	   {
		   return m_Opacity;
	   };
};

//************************************************************



#endif