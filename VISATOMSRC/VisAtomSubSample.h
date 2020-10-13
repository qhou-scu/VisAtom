/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _VISATOMSUBSAMPLE_H
#define _VISATOMSUBSAMPLE_H
#include "GlcObj.h"
#include "QList.h"
#include "VisAtomVisualizingPolicy.h"
 
class VisAtomDataSheet;
//*********************************************************************
class VisAtomRegionData
{
   public:
          enum VISATOMREGION
		  {
			  UNDEF_REGION     = 0,
			  SPHERE_REGION    = 1,
			  CUBIC_REGION     = 2,
			  DPLANE_REGION    = 3,
			  EQUATION_REGION  = 4
		  };
  protected:
		  int m_type;
          QList<double> m_param;

   public:
	   VisAtomRegionData(){ m_type = UNDEF_REGION;}
	   ~VisAtomRegionData(){m_param.clear();}

	   void SetSphereRegion(double px, double py, double pz, double r)
	   { 
		   m_type = SPHERE_REGION;
		   m_param.clear();
		   m_param<<px<<py<<pz<<r;
	   }
	   void SetSphereRegion(double r)
	   { 
		   m_type = SPHERE_REGION;
		   m_param.clear();
		   m_param<<0.0<<0.0<<0.0<<r;
	   }

	   void SetCubicRegion(double px, double py, double pz, double sx, double sy, double sz)
	   { 
		   m_type = CUBIC_REGION;
		   m_param.clear();
		   m_param<<px<<py<<pz<<sx<<sy<<sz;
	   }

	   void SetCubicRegion(double sx, double sy, double sz)
	   { 
            SetCubicRegion(0.0, 0.0, 0.0, sx, sy, sz);
	   }	  

	   void SetCubicRegion(double size)
	   { 
		  SetCubicRegion(size, size, size);
	   }


	   void SetDPleneRegion(double nx, double ny, double nz, double d1, double d2)
	   { 
		   //nx, ny, nz is the normal direction of the planes,
		   //d1, d2 is the distance from the origion
		   m_type = DPLANE_REGION;
		   m_param.clear();
		   m_param<<nx<<ny<<nz<<d1<<d2;
	   }

	   int GetType() { return m_type;}
	   int GetParamCount() { return m_param.size();}

	   void GetRegion(int&type, int &ndat, double param[])
	   { 
		   //nx, ny, nz is the normal direction of the planes,
		   //d1, d2 is the distance from the origion
		   type = m_type;
		   ndat = m_param.size();
		   for(int i=0; i<ndat; i++) param[i] = m_param[i];
	   }
};

//*********************************************************************

//*********************************************************************
class VisAtomSubSample:public GlcObj
{
	private:
    protected:
       VisAtomDataSheet*m_pData;
       
	   QString m_name;
	   int m_statu;
	   int m_curvispolicy;
	   QList<int> m_group;
	   QList<VisAtomRegionData*> m_region;              //space region defining the subsample
       QList<double> m_dataregion;                      //data  range defining the subsample
	   QList<int>    m_datacol;                         //colume of data datasheet giving the data region
	   QList<VisAtomVisualizingPolicy*> m_vispolicy;    //visual policies for the subsample


    public:
		enum VISATOMSUBSTATU
		{
			SUBSAMPLE_INVISIABLE = 0,
			SUBSAMPLE_VISIABLE   = 1
		};
		VisAtomSubSample(int atype);
		VisAtomSubSample();
	    ~VisAtomSubSample();
		 void Clear();

		 QString GetName(){return m_name;}
		 void    SetName(QString name) { m_name = name;};

		 void SetDataSheet(VisAtomDataSheet*pdata) {m_pData = pdata;}
		 void AddGroupID(int type){m_group.append(type);}
		 int  GetNumberofGroup()  { return m_group.count();}
		 int  GetGroupID(int i)   { return m_group[i];}

         VisAtomRegionData*GetRegion(int i){ return m_region[i];}
	     int GetNumberofRegions(){ return m_region.size();}
       
		 int GetNumberofScalarRegions(){ return m_dataregion.count()/2;}
	
		 //to check if an atom is in the subgroup
         bool CheckMembership(int atype, float x, float y, float z, float scalar=0);
         int  GetNumberofAtoms();

		 //properties of the subsample
		 int  IsVisible() {return m_statu;};
		 void SetVisible(int visible=true) { m_statu = visible;};

		 void AddVisualPolicy(VisAtomVisualizingPolicy*policy);
		 int GetNumberofVisualPolicy() { return this->m_vispolicy.count();}
		 VisAtomVisualizingPolicy*GetVisualPolicy(int i) { return this->m_vispolicy[i];}
		 VisAtomVisualizingPolicy*GetCurVisualPolicy() { return this->m_vispolicy[m_curvispolicy];}
		 int  GetCurVisualPolicyID() { return m_curvispolicy; }
		 void SetCurVisualPolicy(int policy) {m_curvispolicy = policy;}
		 void SetLastVisualPolicyAsCurrent() {SetCurVisualPolicy(m_vispolicy.count()-1);}

         //coloring functions..  
      	 void SetColor(float R, float G, float B, float A, int policy)
         {
	        VisAtomVisualizingPolicy*p = this->m_vispolicy[policy];
	        p->SetColor(R, G, B, A); 
          };
		 void GetColor(float&R, float&G, float&B, float&A, int policy)
         {
	        VisAtomVisualizingPolicy*p = this->m_vispolicy[policy];
	        p->GetColor(R, G, B, A); 
          };
		 void GetColor(float&R, float&G, float&B, float&A, int index, int policy)
         {
	        VisAtomVisualizingPolicy*p = this->m_vispolicy[policy];
	        p->GetColor(R, G, B, A, index); 
          };
		 void SetCurColor(float R, float G, float B, float A) {SetColor(R, G, B, A, m_curvispolicy);}
		 void GetCurColor(float&R, float&G, float&B, float&A) {GetColor(R, G, B, A, m_curvispolicy);}
		 void GetCurColor(float&R, float&G, float&B, float&A, int index) {GetColor(R, G, B, A, index, m_curvispolicy);}
		 void  SetColorStyle(int style,int policy)
		 { 
             VisAtomVisualizingPolicy*p = m_vispolicy[policy];
			 return p->SetColorStyle(style);
		 }
		 int  GetColorStyle(int policy)
		 { 
             VisAtomVisualizingPolicy*p = m_vispolicy[policy];
			 return p->GetColorStyle();
		 }
		 void  SetCurColorStyle(int style) { SetColorStyle(style, m_curvispolicy);}
		 int   GetCurColorStyle() { return GetColorStyle(m_curvispolicy);}

		 //Geometry
		 void SetAtomShapeStyle(int style, int policy)
         {
	        VisAtomVisualizingPolicy*p = m_vispolicy[policy];
			p->SetAtomShapeStyle(style);
			return;
         };
		 void SetVectorStyle(int style, int policy)
         {
	        VisAtomVisualizingPolicy*p = m_vispolicy[policy];
			p->SetVectorStyle(style);
			return;
          };
		 int GetGeometryStyle(int policy)
         {
	        VisAtomVisualizingPolicy*p = m_vispolicy[policy];
			return p->GetGeomStyle(); 
          };
		 void SetCurAtomShapeStyle(int style) { SetAtomShapeStyle(style, m_curvispolicy); }
		 void SetCurVectorStyle(int style) { SetVectorStyle(style, m_curvispolicy); }
		 int  GetCurGeometryStyle() { return GetGeometryStyle(m_curvispolicy); }
         //size functions
		 void SetAtomSize(float rad, int policy)
         {
			this->m_vispolicy[policy]->SetAtomSize(rad);
          }
		 void GetAtomSize(float&rad, int policy)
         {
			rad = this->m_vispolicy[policy]->GetAtomSize();
          }
		 void GetAtomSize(int i, float&rad, int policy)
         {
			rad = this->m_vispolicy[policy]->GetAtomSize(i);
          }
		 void SetCurAtomSize(float rad){ SetAtomSize(rad, m_curvispolicy);}
		 void GetCurAtomSize(float&rad){ GetAtomSize(rad, m_curvispolicy);}
		 void GetCurAtomSize(int i, float&rad){ GetAtomSize(i, rad, m_curvispolicy);}
		 bool IsAtomHaveUnifiedSize(int policy){ return this->m_vispolicy[policy]->IsAtomHaveUnifiedSize();}
		 bool IsCurAtomHaveUnifiedSize(){ return IsAtomHaveUnifiedSize(m_curvispolicy);}
		 void SetAtomHaveUnifiedSize(bool yes, int policy)
		 { 
			 VisAtomVisualizingPolicy*p = this->m_vispolicy[policy];
			 p->SetAtomHaveUnifiedSize(yes);
		 }
		 void SetCurAtomHaveUnifiedSize(bool yes){this->SetAtomHaveUnifiedSize(yes, m_curvispolicy);};

	 
		 //trajectory propperty
		 void SetTrajectoryLinethick(float thick, int policy) 
		 {
			 this->m_vispolicy[policy]->SetTrajectoryLinethick(thick);
		 }
		 void SetTrajectoryLinethick(float thick) 
		 {
			 for(int i=0; i<this->m_vispolicy.count(); i++)
			     this->m_vispolicy[i]->SetTrajectoryLinethick(thick);
		 }
		 void SetCurTrajectoryLinethick(float thick) 
		 {
			     SetTrajectoryLinethick(thick, m_curvispolicy);
		 }
		 float GetTrajectoryLinethick(int policy) 
		 {
			   return this->m_vispolicy[policy]->GetTrajectoryLinethick();
		 }
		 float GetCurTrajectoryLinethick() 
		 {
			   return this->m_vispolicy[m_curvispolicy]->GetTrajectoryLinethick();
		 }
 };

#endif