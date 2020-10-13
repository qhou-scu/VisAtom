
#ifndef _SCENE
#define _SCENE
#include  "Light.h"

#define  action_None               0 
#define  action_Object_Select      1 
#define  action_Object_RotationX   2001 
#define  action_Object_RotationY   2010 
#define  action_Object_RotationZ   2100 
#define  action_Object_Translate   3 
#define  action_Object_TranslateX  3001 
#define  action_Object_TranslateY  3010 
#define  action_Object_TranslateZ  3100 
#define  action_Object_Zoom        4 
#define  action_Object_Zoomin      4001 
#define  action_Object_Zoomout     4002 
#define  action_Object_ClipE       61 
#define  action_Object_ClipD       62 
#define  action_Object_ClipEP      612 
#define  action_Object_ClipDP      622 
#define  action_Object_Clipx       6001 
#define  action_Object_Clipy       6010 
#define  action_Object_Clipz       6100 
#define  action_Object_ClipR       6111 
#define  action_Object_Light       7 
#define  action_Object_Light1      7001 
#define  action_Object_Light2      7002 
#define  action_Object_Light3      7003 

#define  clip_State_Enabled        1 
#define  clip_State_Shown          2 

#define  scene_Style_ShowNone      0 
#define  scene_Style_ShowVolume    1 
#define  scene_Style_Perspective   2 

#define  scene_Update_Automatic    0 
#define  scene_Update_ByRequire    1 

#define  scene_System_Relative     0 
#define  scene_System_Labrotary    1 


#define  Key_LEFT                0x25
#define  Key_UP                  0x26
#define  Key_RIGHT               0x27
#define  Key_DOWN                0x28

 
class Scene:public GlcObj
{
	 private:
     protected:
       int    m_viewportx0, m_viewporty0, m_viewportw, m_viewporth;
       double m_x0, m_x1, m_y0, m_y1, m_z0, m_z1, m_dist;
	   int    action;
	   int    style;
	   int    update;
	   int    system;
	   float  m_zoomPercent, m_zoomscal;
	   float  m_rotationStepx, m_rotationStepy, m_rotationStepz;
	   float  m_rotationCx, m_rotationCy, m_rotationCz;           //center for rotation
	   float  m_itransStepx, m_itransStepy, m_itransStepz;
	   float  m_itransX, m_itransY, m_itransZ;
	   float  iclipStepx,  iclipStepy, iclipStepz;
	   float  iclipThickx,  iclipThicky, iclipThickz;
	   double clip1[4],clip2[4],clip3[4];
	   double clip1p[4],clip2p[4],clip3p[4];
	   int    clipState[6], curclip;

	   Light*light1, *light2, *light3;
	   int   curlight;

	   //Display property
	   int  m_showbox, m_showbackface[3],m_showfrontface[3];
	   float m_borderthick, m_backfaceopacity[3];
     public:
		 void ShowBoxBorder(bool show=true){m_showbox = show;}
		 bool IsShowBoxborder(){ return m_showbox;}

		 void ShowBackfaceX(int show){ m_showbackface[0] = show;}
		 int  IsShowBackfaceX(){ return m_showbackface[0];}
		 void ShowBackfaceY(int show){ m_showbackface[1] = show;}
		 int  IsShowBackfaceY(){ return m_showbackface[1];}
		 void ShowBackfaceZ(int show){ m_showbackface[2] = show;}
		 int IsShowBackfaceZ(){ return m_showbackface[2];}
		 void ShowAllBackfaces(int show){m_showbackface[0]=m_showbackface[1]=m_showbackface[2]=show;}


		 void ShowFrontfaceX(int show){ m_showfrontface[0] = show;}
		 int IsShowFrontfaceX(){ return m_showfrontface[0];}
		 void ShowFrontfaceY(int show){ m_showfrontface[1] = show;}
		 int IsShowFrontfaceY(){ return m_showfrontface[1];}
		 void ShowFrontfaceZ(int show){ m_showfrontface[2] = show;}
		 int IsShowFrontfaceZ(){ return m_showfrontface[2];}
		 void ShowAllFrontfaces(int show){m_showfrontface[0]=m_showfrontface[1]=m_showfrontface[2]=show;}

     public:
	    Scene();
	    Scene(float left, float right, float bottom, float top, float nearz, float farz, float dist=0);
	    ~Scene();

		void  SetViewport( int x0, int y0, int w, int h)
		{ 
		  m_viewportx0 = x0; m_viewporty0 = y0;
          m_viewportw  = w;  m_viewporth = h;
		}

		void  GetViewport( int&x0, int&y0, int&w, int&h)
		{ 
		  x0 = m_viewportx0; y0 = m_viewporty0;
          w  = m_viewportw;  h = m_viewporth;
		}

		void  MakeCurrent()
		{
			glViewport(m_viewportx0, m_viewporty0, m_viewportw, m_viewporth);
		}

		void  CreateScene();
		void  CreateScene(float left, float right, float bottom, float top, float nearz, float farz, float dist=0);
		int   GetStyle(){ return style;};
		float GetVolumeX0(){ return (float)m_x0;};
		float GetVolumeY0(){ return (float)m_y0;};
		float GetVolumeZ0(){ return (float)m_z0;};
		float GetVolumeX1(){ return (float)m_x1;};
		float GetVolumeY1(){ return (float)m_y1;};
		float GetVolumeZ1(){ return (float)m_z1;};
		void  SetVolumeX0(float X0){ m_x0 = X0;};
		void  SetVolumeX1(float X1){ m_x1 = X1;};
		void  SetVolumeY0(float Y0){ m_y0 = Y0;};
		void  SetVolumeY1(float Y1){ m_y1 = Y1;};
		void  SetVolumeZ0(float Z0){ m_z0 = Z0;};
		void  SetVolumeZ1(float Z1){ m_z1 = Z1;};
		float GetDistance(){ return (float)m_dist;};
		void  SetDistance(float dist) { m_dist = dist;};


		void  SetAsPerspective();
		void  SetAsOrtho();
		void  SetAsUpdateAuto(){update = scene_Update_Automatic;}; 
		void  SetAsUpdateByRequire(){update = scene_Update_ByRequire;}; 
		int   GetUpdateStyle(){ return update;}; 
		void  SetAsLaboratorySystem(){system = scene_System_Labrotary;};
		void  SetAsRelativeSystem(){system = scene_System_Relative;};
		int   GetSystemStyle(){ return system;}; 

		
		void  GetLabX(float*, float*, float*);
		void  GetLabY(float*, float*, float*);
		void  GetLabZ(float*, float*, float*);
		void  RotateX(float);
		void  RotateY(float);
		void  RotateZ(float);
		void  RotateXf();
		void  RotateYf();
		void  RotateZf();
		void  RotateXb();
		void  RotateYb();
		void  RotateZb();
		void  TransL();
		void  TransR();
		void  TransU();
		void  TransD();
		void  TransN();
		void  TransF();
		void  TransL(float);
		void  TransR(float);
		void  TransU(float);
		void  TransD(float);
		void  TransN(float);
		void  TransF(float);
		void  ResetFocus(float,float,float);     //reset the rotation center
		void  GetFocus(float&,float&,float&);    //get the rotation center
		void  TransDevice(int, int, int, int);
		void  TransDevice(int, int);
		void  TransDevice();
		void  ZoomIn();
		void  ZoomOut();
		int   MoveClipPlaneP();
		int   MoveClipPlaneM();
        int   ChangeClipPlane();
        void  EnableClipPlane();
        void  EnableClipPlanePair();
        void  DisableClipPlane();
        void  DisableClipPlanePair();
        int   IsCurrentClipPlaneEnabled();
        int   IsClipPlaneEnabled();
        int   IsClipPlaneEnabled(int);
        int   IfClipPlaneVisiable(int);
        void  SetClipPlaneShown(int);
        void  SetClipPlaneHiden(int);
        void  SetVolumeShown();
        void  SetVolumeHiden();

		int   Response(int);

		void  ChangeLightColorA(int, float, float, float);
		void  ChangeLightColorD(int, float, float, float);
		void  ChangeLightColorS(int, float, float, float);
		void  ChangeLightPosition(int, float, float, float);
		void  TurnLighton(int);
		void  TurnLightoff(int);

		virtual void Paint();
        void         PaintClipPlane();

		void  ChangeAction(int Action){ action = Action; }; 
		int   CurrentAction(){  return action; }; 

		void  ChangeZoomPercent(float ZoomP ){ m_zoomPercent = ZoomP; }; 
		float CurrentZoomPercent(){  return m_zoomPercent; };
		float CurrentZoomScale(){  return m_zoomscal; };
		void  ChangeZoomScale(float zoomscal){  m_zoomscal = zoomscal; };
		
		void  ChangeRotationStepx(float Rstep ){ m_rotationStepx = Rstep; }; 
		float CurrentRotationStepx(){ return m_rotationStepx; }; 
		void  ChangeRotationStepy(float Rstep ){ m_rotationStepy = Rstep; }; 
		float CurrentRotationStepy(){ return m_rotationStepy; }; 
		void  ChangeRotationStepz(float Rstep ){ m_rotationStepz = Rstep; }; 
		float CurrentRotationStepz(){ return m_rotationStepz; }; 
		void  ChangeTransStepx(float Tstep ){ m_itransStepx = Tstep; }; 
		float CurrentTransStepx(){ return m_itransStepx; }; 
		void  ChangeTransStepy(float Tstep ){ m_itransStepy = Tstep; }; 
		float CurrentTransStepy(){ return m_itransStepy; }; 
		void  ChangeTransStepz(float Tstep ){ m_itransStepz = Tstep; }; 
		float CurrentTransStepz(){ return m_itransStepz; }; 

		void  ChangeClipStepx(float Cstep ){ iclipStepx = Cstep; }; 
		float CurrentClipStepx(){ return iclipStepx; }; 
		void  ChangeClipStepy(float Cstep ){ iclipStepy = Cstep; }; 
		float CurrentClipStepy(){ return iclipStepy; }; 
		void  ChangeClipStepz(float Cstep ){ iclipStepz = Cstep; }; 
		float CurrentClipStepz(){ return iclipStepz; }; 
		
		void  ChangeClipPairThickx(float); 
		float CurrentClipPairThickx(){ return iclipThickx; }; 
		void  ChangeClipPairThicky(float); 
		float CurrentClipPairThicky(){ return iclipThicky; }; 
		void  ChangeClipPairThickz(float); 
		float CurrentClipPairThickz(){ return iclipThickz; }; 


		void  Save(FILE*);
		void  Read(FILE*);
};

#endif