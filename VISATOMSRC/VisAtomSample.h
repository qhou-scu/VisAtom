/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _VISATOMSAMPLE_H
#define _VISATOMSAMPLE_H
#include  "GlcObj.h"
#include  "Scene.h"
#include  "VisAtomSubSample.h"


#define  atom_Statu_Invisiable  0
#define  atom_Statu_Visiable    1
#define  atom_Statu_Selected    2
#define  atom_Statu_MarkedFound 4
#define  atom_Statu_Deleted     8


#define  file_format_Simple   0

#define  atom_pick_Option_Single 0
#define  atom_pick_Option_Subsample 1
#define  atom_pick_Option_Multiple  2
#define  atom_pick_Option_Square    3
#define  atom_pick_Option_Tripoints 4

#define  atom_pick_Statu_nopicked 0
#define  atom_pick_Statu_picked      1
#define  atom_pick_Statu_doublepicked 2
#define  atom_pick_Statu_droped      4

#define  sample_Statu_noChange    0
#define  sample_Statu_Changed     1
 
class VisAtomDataSheet;
class VisAtomCliper;
class VisAtomTrajectory;

class VisAtomSample:public GlcObj
{
	private:
       //************ New or modified  2013-09 ************************
		//
	    void ResetVisibilityByCliper();
	    void ResetVisibilityByCliper_OR(VisAtomCliper*cliper);
		void ResetVisibilityByCliper_OR_ALSO(VisAtomCliper*cliper);
	    void ResetVisibilityByCliper_AND(VisAtomCliper*cliper);
	    void ResetVisibilityByCliper_ANYDIR();

		bool IsCliped(double c, double n, float p) {if((p-c)*n > 0) return true; else return false;}
		void SetStatAsHide(char&visstat){ char j; j = (visstat>>1); visstat = (j<<1);}

    protected:
       VisAtomDataSheet*m_pData;
	   QList<VisAtomCliper*>m_pCliper;


	   int   m_numAtom;
	   char  *m_aType;
	   float *m_aPx, *m_aPy, *m_aPz;
	   //display parameter for atoms
	   char  *m_aVisStat;
	   float *m_aRadius;
	   float *m_aRed, *m_aGreen, *m_aBlue;
	   float *m_aRedex, *m_aGreenex, *m_aBlueex;
	   int   *m_aGeomStyle;
	   //display parameter for vectors
	   float *m_vPx, *m_vPy, *m_vPz;

	   // the box range automatically determined by position of atoms
	   float m_aXmi, m_aXmx, m_aYmi, m_aYmx, m_aZmi, m_aZmx;       


	   QList<VisAtomSubSample*> m_subSamples;
	   QList<int> m_selSubsamples;
	   int  m_curSubsample;

	   int  m_pick_style, m_pick_statu;
	   int  m_pick_posx, m_pick_posy;      
       int  m_pickedAtomID;
	   int  statu;

       VisAtomTrajectory*m_pTrajectory;
       friend class VisAtomTrajectory;

    public:
	    VisAtomSample();
	    ~VisAtomSample();

		void Clear(bool keepsubsample=false);
		void SetDataSheet(VisAtomDataSheet*pData);
		VisAtomDataSheet*GetDataSheet(){ return m_pData;}

		int   NumberofAtom(){return m_numAtom;}; 
		int   GetSampleType(int*);
		int   GetSampleX(float*);
		int   GetSampleY(float*);
		int   GetSampleZ(float*);
		char* GetSampleTypeP(){if(m_aType != NULL) return m_aType; else return NULL;};
		float*GetSampleXP(){if(m_aPx != NULL) return m_aPx; else return NULL;};
		float*GetSampleYP(){if(m_aPy != NULL) return m_aPy; else return NULL;};
		float*GetSampleZP(){if(m_aPz != NULL) return m_aPz; else return NULL;};

        void  AutoSampleBoxRange();
		void  GetSampleBoxRangeX(float&xmi, float&xmx){xmi = m_aXmi; xmx = m_aXmx;};
        void  GetSampleBoxRangeY(float&ymi, float&ymx){ymi = m_aYmi; ymx = m_aYmx;};
        void  GetSampleBoxRangeZ(float&zmi, float&zmx){zmi = m_aZmi; zmx = m_aZmx;};
		void  GetDefaultAtomRadiu(float&radiu, float&scal);
    	void  GetDefaultVectorSize(float&lenscal, float&rad, float&arrowl, float&arrowr);


		int   NumberofSubsample(){ return m_subSamples.count();};
		VisAtomSubSample*GetSubsample(int i){return m_subSamples[i];};
		VisAtomSubSample*GetSubsample(QString name)
		{ for(int i=0; i<m_subSamples.count(); i++) 
		      if(m_subSamples[i]->GetName() == name) return m_subSamples[i];
		  return NULL;
		}; 

		void  SetCurrentSubsample(int subsample){m_curSubsample = subsample; };
		int   SetCurrentSubsample(QString name)
		      {  for(int i=0; i<m_subSamples.count(); i++) 
		              if(m_subSamples[i]->GetName() == name) { m_curSubsample = i; return m_curSubsample;}
		          return -1;  };
		int   GetCurrentSubsampleID() { return m_curSubsample; };
		VisAtomSubSample*GetCurrentSubsample(){ return m_subSamples[m_curSubsample]; };
		bool IsCurrentSample(QString name)
		{
			if(name == m_subSamples[m_curSubsample]->GetName() ) return true;
			else return false;
		}

		int   SetSelSubsample(QString name)
		      {  for(int i=0; i<m_subSamples.count(); i++) 
		              if(m_subSamples[i]->GetName() == name) { m_selSubsamples[i] = 1; return i;}
		          return -1;  };

		int   SetUnselSubsample(QString name)
		      {  for(int i=0; i<m_subSamples.count(); i++) 
		              if(m_subSamples[i]->GetName() == name) { m_selSubsamples[i] = 0; return i;}
		          return -1;  };

		int  IsSelectedSubsample(int i)
		      {  return m_selSubsamples[i];}


		//This part concerning display property
 		virtual void Paint();
		void Draw();
		void DrawSolidAtoms(int inbox);
		void DrawWireAtoms(int inbox);
		void DrawDotAtoms(int inbox);
		void DrawCircleAtoms(int inbox);

		void DrawLineVectors(int inbox);
		void DrawSolidVectors(int inbox);
		void DrawColorMappinBar(int*lablepos);
		void DrawWignerSeizVolume(QString fname);


		void DrawProjCirleAtoms(int nx, int ny, int nz);
		void DrawProjSolidAtoms(int nx, int ny, int nz);
		void DrawProjDotAtoms(int nx, int ny, int nz);

		void HighlightSelectedAtoms();
		void HighlightFoundAtoms();

		void Save_Summary(FILE*);
		void Save_Config_Selected(FILE*);
		void Save_Config_PickedSubsample(FILE*);
		void Save_Config_VisiableAtoms(FILE*);
		void Save_Config(FILE*);
		int  NumberofVisiableAtom(); 


		//Part concerning statu
		int   IfChanged(); 
		void  FinishChange(); 

        int  GetPickStatu();
        int  GetPickOption();
        void ClearPickStatu();
        void SetPickOption(int);
        int  PickAtom(int, int, Scene*, float*, float*, float*);
        int  PickSingle(int, int, Scene*, float*, float*, float*);
        int  PickMultiple(int, int, Scene*, float*, float*, float*);
        int  PickSquare(int, int, int, Scene*, float*, float*, float*);
        int  PickTriPoints(int, int, Scene*, float*, float*, float*);
        int  MovePickedAtom(int, int, Scene*, float*, float*, float*);
        int  MoveSinglePicked(int, int, Scene*, float*, float*, float*);
        int  MoveMultiplePicked(int, int, Scene*, float*, float*, float*);
        int  MoveSubsamplePicked(int, int, Scene*, float*, float*, float*);
        int  DropPickedAtom(int, int, Scene*,float*, float*, float*);
        int  DropSinglePicked(int, int, Scene*);
        int  InformPickedAtom(int*, int*, float*, float*, float*);
		int  Inf_of_SinglePicked(int*, int*, float*, float*, float*);
        int  Move_aAtom(float, float, float);
        void Move_SelectedAtoms(float, float, float);
        void Move_aAtomGroup(int, float, float, float);
        void Move_aAtomGroup(float, float, float);


		//*** this part concering the operation on atoms
		int   GetCurAtom();
		void  SetCurAtom(int id);
        int   GetTypeofCurAtom();
        float GetXofCurAtom();
        float GetYofCurAtom();
        float GetZofCurAtom();

        void  Show_aAtom(int);
        void  Show_aAtom();
        void  Hide_aAtom(int);
        void  Hide_aAtom();
        void  Hide_SelectedAtoms();
        void  Hide_unSelectedAtoms();
        void  Show_SelectedAtoms();
        void  Show_unSelectedAtoms();
        void  Show_AllAtoms();
        void  Hide_AllAtoms();
		void  Select_aAtom(int);
		void  Select_aAtom();
		void  unSelect_aAtom(int);
        void  unSelect_aAtom();
		void  unSelect_Atoms(int from, int to);
        void  unSelect_aAtomGroup(int);
        void  unSelect_aAtomGroup();
        void  unSelect_AllAtoms();

        //************ New or modified  2013-09 ************************
		//
        //*** this part concerning the clipping of scene
		void  SetCliper(QList<VisAtomCliper*>clip);
		void  AddCliper(VisAtomCliper*clip);
		void  ResetVisibility();
        

		//*** this part concering the operation on atoms
        void  LoadSubsampleProp(int is);
		void  LoadCurSubsampleProp(){if(m_curSubsample>=0) LoadSubsampleProp(m_curSubsample);}

		void  SetAtomRadiusForSubsample(int is, float radius){ m_subSamples[is]->SetCurAtomSize(radius);};
		void  SetAtomShapeStyleForSubsample(int  is, int style){ m_subSamples[is]->SetCurAtomShapeStyle(style);};
		void  SetColorForSubsample(int  is, float R, float G, float B, float A=1.f){ m_subSamples[is]->SetCurColor(R, G, B, A);};


		void  SetAtomRadiusForCurrentSubsample(float radius)
		     {if(m_curSubsample>=0) SetAtomRadiusForSubsample(m_curSubsample, radius);};

		void  SetAtomShapeStyleForCurrentSubsample(int style)
		     {if(m_curSubsample>=0) SetAtomShapeStyleForSubsample(m_curSubsample, style);};

		void  SetColorForCurrentSubsample(float R, float G, float B, float A=1.f)
		     {if(m_curSubsample>=0) SetColorForSubsample(m_curSubsample, R, G, B);};


		VisAtomVisualizingPolicy*GetSubsampleVisualPolicy(QString name, int policy);
        //****
		void AttachTrajectory(VisAtomTrajectory*pTraject);
		VisAtomTrajectory*GetTrajectoryData(){return m_pTrajectory;};

		//***
		void  MarkAtomsFound(int from, int to, int step);
		void  MarkGroupAtomsFound(int group, int from, int to);
		void  RemoveAtomsFoundMark();
		void  SetShowOnlyAtomsFound(int showonly);
		void  GeNextAtomMarkedFound(int here, int&gid, int&ingr, float&posx, float&posy, float&posz);

 };

 

#endif