
#ifndef VISATOMWIDGET_H
#define VISATOMWIDGET_H

#include <QGLWidget>
#include "VisAtomSample.h"
class Scene;
class Bitmap;
class VisAtomDataSheet;
class VisAtomCliper;
class VisAtom_AtomInfo; 
class VisAtomTrajectory;
class VisAtomExtendObject;
class VisAtomWidget : public QGLWidget
{
    Q_OBJECT

public:
    VisAtomWidget(QWidget *parent = 0);
    ~VisAtomWidget();

	enum ACTION_OTPION
	{
		ACTION_ROTATION_X = 0,
		ACTION_ROTATION_Y = 1,
		ACTION_ROTATION_Z = 2,
		ACTION_ZOOM_SCENE = 3,
		ACTION_VIEWPORT_CENTER = 4
	};

	enum ACTION_MOUSE_STATE
	{
		BUTTON_RELEASED = 0,
		LBUTTON_PRESSED  = 1,
		RBUTTON_PRESSED  = 2
	};
    

public slots:
    

signals:

protected:
    void initializeGL();
    void paintGL();
	void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent( QMouseEvent * event); 
    void mouseMoveEvent(QMouseEvent *event);
	void hideEvent(QHideEvent *event); 

	//VisAtomData type
    Scene*m_Scene;
    Scene*m_ColorbarView;
	VisAtomSample*m_Sample;
    //VisAtomCliper**m_Cliper;
	QList<VisAtomCliper*>*m_pCliper;
	VisAtomCliper*m_curCliper;
	int m_curCliperStyle;

	//extension objects
	VisAtomTrajectory*m_Trajectory;
	QList<VisAtomExtendObject*>m_ExtObjs;

	//Properties for concerning scene
	BOOL  m_AutoNormalizingView;
	int   m_MouseAction;
	int   m_MouseState;
	float m_Backgroud[4];

	QTimer *m_Timer;
	int m_Timestep;

	//Methods for VisAtom
	void NormalizingView();
	void ProcessCurrentAction();

	VisAtom_AtomInfo*m_pickedAtomInfo;


private slots:
	void SelectMouseAction(int action);
	void OnChangeSceneBackground();
	void OnProjecttypeOrtho();
	void OnProjecttypePerspect();
	void SetSceneDistance(int range, int value);
	void OnCoordinatBody();
	void OnCoordinatLab();
	void OnBoxDisplayOption(QTreeWidgetItem*);
	void OnShowSceneBox(int show);
	void OnShowSceneBackfaceX(int show);
	void OnShowSceneBackfaceY(int show);
	void OnShowSceneBackfaceZ(int show);
	void OnShowSceneFrontfaceX(int show);
	void OnShowSceneFrontfaceY(int show);
	void OnShowSceneFrontfaceZ(int show);
	void OnClipXBack();
	void OnClipXForward();
	void OnClipYBack();
	void OnClipYForward();
	void OnClipZBack();
	void OnClipZForward();
	void OnRotationXStep(int step);
	void OnRotationYStep(int step);
	void OnRotationZStep(int step);
	void OnZoomStep(int step);
	void OnShowClipANYDIRFrontface(bool show);
	void OnShowClipANYDIRBackface(bool show);


	void OnTimerout();

	void OnSelectShapeDot(bool checked);
	void OnSelectShapeSolidSphere(bool checked);
	void OnSelectShapeWireSphere(bool checked);
	void OnSelectShapeCircle(bool checked);
	void OnChangeAtomColor();
	void OnChangeGraphQuality(int nfai);

	void SetAtomRadiu(int range,  int value, bool all=false);
	void SetAtomShape(int shape, bool all);
    void SetGeomStyleVectorVisible(bool show, int style, bool all);
	void SetVariableAtomRadiu(bool variable,bool all);
	void ChangeGraphQuality(int nfai);

	void OnVectorLenMinus();
	void OnVectorLenPlus();
	void OnVectorSizeMinus();
	void OnVectorSizePlus();
	void OnSelectVectorStyleLine(bool checked);
	void OnSelectVectorStyleSolid(bool checked);

	void SetGeomStyleAtomVisible(bool show,int style,bool all);
	void SetGeomStyleTrajectyoryVisible(bool show,int style,bool all);
	void SetTrajectoryThickChanged(double thick,bool all);

    void OnOpenExtendedObject(VisAtomExtendObject*ptrObj, QString fname);

private:
    
    QPoint lastPos;

public:
	VisAtomSample * GetSample()
	{
		return m_Sample;
	};
	VisAtomTrajectory*GetTrajectory()
	{
		return m_Trajectory;
	};
	void SetSampleDataSheet(VisAtomDataSheet*pData, BOOL redraw=true);

	BOOL IfAutoNormalizingView()
	{
		return m_AutoNormalizingView;
	};
	void SetAutoNormalizingView()
	{
		m_AutoNormalizingView = true;
	};
	void SetNoAutoNormalizingView()
	{
		m_AutoNormalizingView = false;
	};

	void FitGraphView();
	void GetSceneVolume(float&xmin, float&xmax, float&ymin, float&ymax, float&zmin, float&zmax);
	void SetSceneVolume(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);


	//Clipping routines
	int  CurrentClipperStyle()
	{
		return m_curCliperStyle;
	};
	void  SetCurrentClipperStyle(int curstyle, bool redraw=true);

	VisAtomCliper* SetCurrentClipper(int index);

	VisAtomCliper* CurrentClipper()
	{
		return m_curCliper;
	};
	void  SetCurrentCliperParameters(VisAtomCliper*sor);
	VisAtomCliper*NewCliper();

	//IO
	void RenderToPixmap(QPixmap&pixmap);
	void RenderToBitmap(Bitmap&bitmap); 

	void OnActionFindatom();
};

#endif
