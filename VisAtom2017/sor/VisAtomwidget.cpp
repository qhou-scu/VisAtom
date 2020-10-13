
#include <QtGui>
#include <QtOpenGL>
#include <QColorDialog.h>

#include "Scene.h"
#include "Sphere.h"
#include "Circle.h"
#include "SolidCircle.h"
#include "Bitmap.h"
#include "Arrow.h"
#include "VisAtomwidget.h"
#include "VisAtomCliper.h"
#include "VisAtomVisualizingPolicy.h"
#include "VisAtom_NewVectorPolicyDlg.h"
#include "visatom_atominfo.h"
#include "VisAtomTrajectory.h"
#include "VisAtomExtendObject.h"

#define COLORMAPBARSZIEX     32
#define COLORMAPBARSZIEY     480
#define COORDSYSVIEWSIZE     148

VisAtomWidget::VisAtomWidget(QWidget *parent)
    : QGLWidget(parent)
{
	int i;
	m_Scene        = new Scene();
	m_ColorbarView = new Scene();
	m_Sample       = new VisAtomSample();

	//default action options for scene 
	m_AutoNormalizingView = true;
	m_MouseAction = ACTION_ZOOM_SCENE;
	m_MouseState  = BUTTON_RELEASED;

	//clipping
    m_Cliper = new VisAtomCliper*[VisAtomCliper::CLIPPING_STYLE_NUM];
	for(i=0; i<VisAtomCliper::CLIPPING_STYLE_NUM; i++) 
	{
		m_Cliper[i] = new VisAtomCliper();
		m_Cliper[i]->AttachScene(m_Scene);
		m_Cliper[i]->SetStyle(i);

	}
	m_currentCliper = 0;

	//default display options for scene
	m_Backgroud[0] = m_Backgroud[1] = m_Backgroud[2] = 1.f;
	m_Backgroud[4] = 1.f;

	//invoke a timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimerout()));
	m_Timestep = 300;
	timer->start(m_Timestep);

	//for picked atom infomation
	m_pickedAtomInfo = new VisAtom_AtomInfo();
	m_pickedAtomInfo->setVisible(false);

	//for atom trajectory
    m_Trajectory = new VisAtomTrajectory();
	m_Trajectory->AttachSample(m_Sample);
	m_Sample->AttachTrajectory(m_Trajectory);
   
}

VisAtomWidget::~VisAtomWidget()
{  
    makeCurrent(); 
	int i;
    if(m_Scene)  delete m_Scene; m_Scene=NULL;
	if(m_Sample) delete m_Sample; m_Sample=NULL;
	for(i=0; i<VisAtomCliper::CLIPPING_STYLE_NUM; i++) 
		if(m_Cliper[i]) delete m_Cliper[i]; delete m_Cliper; m_Cliper = NULL;

	if(m_pickedAtomInfo) delete m_pickedAtomInfo; m_pickedAtomInfo= NULL;
	if(m_Trajectory) delete m_Trajectory; m_Trajectory= NULL;
	
}

void VisAtomWidget::SetSampleDataSheet(VisAtomDataSheet*pData, BOOL redraw)
{    
	//Update the data
	if(m_Sample) m_Sample-> SetDataSheet(pData);

	if(this->m_AutoNormalizingView) 
       this->NormalizingView();

    if(redraw) this->updateGL();
}

void VisAtomWidget::NormalizingView()
{
    makeCurrent(); 
	//to atutomatically normalize the view
    float left, right,top, bottom,nearz, farz, z0;
    m_Sample->GetSampleBoxRangeX(left, right);
    m_Sample->GetSampleBoxRangeY(bottom, top);
    m_Sample->GetSampleBoxRangeZ(nearz, farz);
   
    z0 = std::max(fabs(nearz), fabs(farz));
    farz  = z0;
    nearz = -z0;

	m_Scene->CreateScene(left, right, bottom, top, nearz, farz);

	//to update the projection box size
	if(VisAtomVisualizingPolicy::IsAutoProjectionBox()) 
	{
		VisAtomVisualizingPolicy::SetProjectionBox(left, right, bottom, top, nearz, farz);
	}
    
	return;
}

void VisAtomWidget::FitGraphView()
{
    makeCurrent(); 
	glMatrixMode(GL_MODELVIEW);
 	glLoadIdentity();
	this->m_Scene->ChangeZoomScale(1.f);
	this->updateGL();
}

void VisAtomWidget::SelectMouseAction(int action)
{
	this->m_MouseAction = action;
}


void VisAtomWidget::initializeGL()
{
	m_Scene->CreateScene();
	m_Scene->TurnLighton(1);	
	m_Scene->TurnLighton(2);	
	m_Scene->TurnLighton(3);	
}

void VisAtomWidget::paintEvent(QPaintEvent *event)
{
	QGLWidget::paintEvent(event);
	return;
}

void VisAtomWidget::paintGL()
{
	
	int i;
	glClearColor(m_Backgroud[0],m_Backgroud[1], m_Backgroud[2], m_Backgroud[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->m_Scene->MakeCurrent();
	this->m_Sample->Paint();
	for(i=0; i<this->m_ExtObjs.size(); i++)
	{
		this->m_ExtObjs[i]->Paint();
	}
	
	//this->m_Trajectory->Paint();
	//delete this later
	//this->m_Sample->DrawWignerSeizVolume("..\\HE32000_2_a\\VTDATA__V_P0000_0001.0000");

	this->GetCurrentClipper()->Paint();
	this->m_Scene->Paint();


	//draw 2D colormap bar
	for( i=0; i<this->m_Sample->NumberofSubsample(); i++)
	{
        VisAtomVisualizingPolicy*p = this->m_Sample->GetSubsample(i)->GetCurVisualPolicy();
		if(p->GetColorStyle() == VisAtomVisualizingPolicy::COLOR_STYLE_MAP)
		{
			 int x0, y0, w0, h0;
			 int x1, y1, w1, h1;
			 int x, y;
			 int lablepos[VisAtomVisualizingPolicy::CORLOR_MAP_SIZE];
			 QFont font("arial", 9);
			 QFontMetrics metrics = QFontMetrics(font);
			 QRect rect;
			 QString label;

			 this->m_ColorbarView->GetViewport(x1, y1, w1, h1);
			 this->m_ColorbarView->MakeCurrent();
			 this->m_Sample->DrawColorMappinBar(lablepos);

			 this->m_Scene->GetViewport(x0, y0, w0, h0);
			 this->m_Scene->MakeCurrent();
			 glColor3f(1.f-m_Backgroud[0], 1.f-m_Backgroud[1], 1.f-m_Backgroud[2]);
			 for(int i= VisAtomVisualizingPolicy::GetColorMappingNumBar()-1; i>=0; i--)
			 {
				label.setNum(VisAtomVisualizingPolicy::GetScalarMappingTabValue(i), 'e', 2);
			    rect = metrics.boundingRect(label);
			    x = x1 - rect.width()-2;
				y = h0 - lablepos[i]; // +0.5*rect.height(); // h0 - y1 - i * step + 0.5*rect.height();
			    this->renderText (x, y, label, font);
			 }
			break;
		}
	}

	//draw coordinate axsis in a small viewport
	/*{
	  int x0, y0, w, h;
	  this->m_Scene->GetViewport( x0, y0, w, h);
	  if(w>h)
	     glViewport(0, 0, COORDSYSVIEWSIZE, COORDSYSVIEWSIZE*(float)h/(float)w);
	  else
	     glViewport(0, 0, COORDSYSVIEWSIZE*(float)w/(float)h, COORDSYSVIEWSIZE);

	  glDisable(GL_DEPTH_TEST);
      Arrow aX(0.f, 0.f, 0.f, 1.f, 0.f, 0.f);
      Arrow aY(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
      Arrow aZ(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	  float scal = this->m_Scene->CurrentZoomScale();
	  float sx = this->m_Scene->GetVolumeX1()*0.8/scal;
	  float sy = this->m_Scene->GetVolumeY1()*0.8/scal;
	  float sz = this->m_Scene->GetVolumeZ1()*0.8/scal;
	  //use the same length and size for axsis
	  aX.ResetEndPoint(sx,  0.f, 0.f);
	  aY.ResetEndPoint(0.f, sx,  0.f);
	  aZ.ResetEndPoint(0.f, 0.f,  sx);
      aX.ChangeSize(0.15f*sx); 
      aY.ChangeSize(0.15f*sx); 
      aZ.ChangeSize(0.15f*sx); 
      aX.ChangeColor(1.f, 0.f, 0.f); 
      aY.ChangeColor(0.f, 1.f, 0.f); 
      aZ.ChangeColor(0.f, 0.f, 1.f); 
	  glPushMatrix();
	    this->m_Scene->TransDevice();
   	    aX.Paint();
   	    aY.Paint();
   	    aZ.Paint();
	  glPopMatrix();
	}*/

    //restore the viewport to 3D render
    this->m_Scene->MakeCurrent();

	//draw the cover 
	if(!this->m_Sample->GetDataSheet())
	{
	qglColor(QColor(255*(1-m_Backgroud[0]), 255*(1-m_Backgroud[1]), 255*(1-m_Backgroud[2])));
	int x = 32;
	int y = 100;
	QFont font("times", 26);
    QFontMetrics metrics = QFontMetrics(font);
	QString str("Welcome to VisAtom2017");
	this->renderText (x, y, str, font) ;
	y = y + metrics.height()*1.5;
	QFont font1("times", 16);
    QFontMetrics metrics1 = QFontMetrics(font1);
	str = "HOU, Qing" ;
	this->renderText (x, y, str, font1) ;
	y = y + metrics1.height()*1.5;
	str = "Key Lab for Radiation Physics and Technology";
	this->renderText (x, y, str, font1) ;
	y = y + metrics1.height()*1.5;
	str = "Institute of Nuclear Science and Technology";
	this->renderText (x, y, str, font1) ;
	y = y + metrics1.height()*1.5;
	str = "Sichuan University";
	this->renderText (x, y, str, font1) ;
	}
}

void VisAtomWidget::RenderToPixmap(QPixmap&pixmap)
{
	 QSize s = this->size();
	 pixmap =  this->renderPixmap(s.width(), s.height());

   
}

void VisAtomWidget::RenderToBitmap(Bitmap&bitmap)
{
	//this->makeCurrent();
	this->updateGL();

	int x0, y0, w0, h0;
	this->m_Scene->GetViewport( x0, y0, w0, h0);
	bitmap.ReadFromViewport(x0, y0, w0,h0);
    return;     
}   

void VisAtomWidget::resizeGL(int width, int height)
{
   	//To store the old rotation matrix
	this->makeCurrent();

	if(height == 0) height = 1;
	if(width == 0) width = 1;
    //create the main scene for 3D redering
	m_Scene->SetViewport(0, 0, width, height);
	m_Scene->MakeCurrent();
	m_Scene->CreateScene();
    m_Scene->TurnLighton(1);	
    m_Scene->TurnLighton(2);	
    m_Scene->TurnLighton(3);	

    int clipState =   m_Scene->IsClipPlaneEnabled(1);
        m_Scene->Response(action_Object_Clipx);
        m_Scene->Response(action_Object_ClipDP);
        if( clipState ==2) 
	        m_Scene->Response(action_Object_ClipEP);
        else
            if(clipState == 1) m_Scene->Response(action_Object_ClipE);
		
        clipState =   m_Scene->IsClipPlaneEnabled(2);
        m_Scene->Response(action_Object_Clipy);
        m_Scene->Response(action_Object_ClipDP);
        if( clipState ==2) 
	        m_Scene->Response(action_Object_ClipEP);
        else
            if(clipState == 1) m_Scene->Response(action_Object_ClipE);
		  
        clipState =   m_Scene->IsClipPlaneEnabled(3);
        m_Scene->Response(action_Object_Clipz);
        m_Scene->Response(action_Object_ClipDP);
        if( clipState ==2) 
	        m_Scene->Response(action_Object_ClipEP);
        else
            if(clipState == 1) m_Scene->Response(action_Object_ClipE);

		//create the AUX scene for 2D redering
		this->m_ColorbarView->SetViewport(width-COLORMAPBARSZIEX-16, (height-COLORMAPBARSZIEY)/2, COLORMAPBARSZIEX, COLORMAPBARSZIEY);
		this->m_Scene->MakeCurrent();
	    return;	
}


void VisAtomWidget::mousePressEvent(QMouseEvent *event)
{
   	this->makeCurrent(); 
	  if(event->buttons() & Qt::LeftButton) this->m_MouseState  = LBUTTON_PRESSED;
	  else if(event->buttons() & Qt::RightButton) this->m_MouseState  = RBUTTON_PRESSED;

	  int pickedatom;
	  pickedatom = -1;
	  if( QApplication::queryKeyboardModifiers() == Qt::ControlModifier) 
	  {
        //to pick the atom  
		this->m_pickedAtomInfo-> StarPickAtomInfo(event->globalX(), event->globalY());
        float curX, curY, curZ;
	    this->m_Sample->PickAtom(event->pos().x(), event->pos().y(), this->m_Scene, &curX, &curY, &curZ);
		pickedatom = this->m_Sample->GetCurAtom();
		this->m_pickedAtomInfo->ShowAtomInfo(event->globalX(), event->globalY(), this->m_Sample, pickedatom);
		
	  }
	  else
	  {
	    switch(this->m_MouseAction)
	   {
		case this->ACTION_ROTATION_X:
		case this->ACTION_ROTATION_Y:
		case this->ACTION_ROTATION_Z:
		case this->ACTION_ZOOM_SCENE:
	         this->ProcessCurrentAction();
			 break;
	    case this->ACTION_VIEWPORT_CENTER:
			{
             if(this->m_MouseState == LBUTTON_PRESSED)
				 this->m_Scene->TransDevice(event->x(), event->y());

			 if(this->m_MouseState == RBUTTON_PRESSED) 
				 this->m_Scene->TransDevice();
			 break;
			}
	   }
	  }

	this->updateGL();
}

void VisAtomWidget::ProcessCurrentAction()
{
	  int keystate = QApplication::queryKeyboardModifiers();

	  switch(this->m_MouseAction)
	  {
		case this->ACTION_ROTATION_X:
			if(keystate == Qt::ShiftModifier)
			{
	         if(this->m_MouseState==LBUTTON_PRESSED) this->m_Scene->RotateX(90.f);
	         else if(this->m_MouseState==RBUTTON_PRESSED) this->m_Scene->RotateX(-90.f);
			}
			else
			{
	         if(this->m_MouseState==LBUTTON_PRESSED) this->m_Scene->RotateXf();
	         else if(this->m_MouseState==RBUTTON_PRESSED) this->m_Scene->RotateXb();
			}
			 break;
		case this->ACTION_ROTATION_Y:
			if(keystate == Qt::ShiftModifier)
			{
	         if(this->m_MouseState==LBUTTON_PRESSED) this->m_Scene->RotateY(90.f);
	         else if(this->m_MouseState==RBUTTON_PRESSED) this->m_Scene->RotateY(-90.f);
			}
			else
			{
	         if(this->m_MouseState==LBUTTON_PRESSED) this->m_Scene->RotateYf();
	         else if(this->m_MouseState==RBUTTON_PRESSED) this->m_Scene->RotateYb();
			}
			 break;
		case this->ACTION_ROTATION_Z:
			if(keystate == Qt::ShiftModifier)
			{
	         if(this->m_MouseState==LBUTTON_PRESSED) this->m_Scene->RotateZ(90.f);
	         else if(this->m_MouseState==RBUTTON_PRESSED) this->m_Scene->RotateZ(-90.f);
			}
			else
			{
	         if(this->m_MouseState==LBUTTON_PRESSED) this->m_Scene->RotateZf();
	         else if(this->m_MouseState==RBUTTON_PRESSED) this->m_Scene->RotateZb();
			}
			 break;
		case this->ACTION_ZOOM_SCENE:
	         if(this->m_MouseState==LBUTTON_PRESSED) this->m_Scene->ZoomIn();
	         else if(this->m_MouseState==RBUTTON_PRESSED) this->m_Scene->ZoomOut();
			 break;
	  }

}

void VisAtomWidget::mouseReleaseEvent ( QMouseEvent * event ) 
{
	this->m_MouseState  = BUTTON_RELEASED;
	this->m_pickedAtomInfo->hide();

}

void VisAtomWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    /*if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }*/
    lastPos = event->pos();
}

#define RGBSIZE 255
void VisAtomWidget::OnChangeSceneBackground()
{
	   int R = m_Backgroud[0]*RGBSIZE;
	   int G = m_Backgroud[1]*RGBSIZE;
	   int B = m_Backgroud[2]*RGBSIZE;
	   int A = m_Backgroud[3]*RGBSIZE;
	   QColorDialog colorsel(QColor(R, G, B, A), this);

	   if(colorsel.exec())
	   {
	   
		  QColor color = colorsel.currentColor(); 
	      color.getRgb(&R, &G, &B, &A);
	      m_Backgroud[0] = R/float(RGBSIZE);
	      m_Backgroud[1] = G/float(RGBSIZE);
	      m_Backgroud[2] = B/float(RGBSIZE);
	      m_Backgroud[3] = A/float(RGBSIZE);
	      this->makeCurrent();
          glClearColor(m_Backgroud[0],m_Backgroud[1], m_Backgroud[2], m_Backgroud[3]);
	      this->updateGL();
	   }
}   

void VisAtomWidget:: OnProjecttypeOrtho()
{
	this->makeCurrent();
	this->m_Scene->SetAsOrtho();
	this->m_Scene->CreateScene();
	this->updateGL();

}
void VisAtomWidget::OnProjecttypePerspect()
{
	this->makeCurrent();
	this->m_Scene->SetAsPerspective();
	this->m_Scene->CreateScene();
	this->updateGL();
}
void VisAtomWidget::SetSceneDistance(int range, int value)
{
	this->makeCurrent();
	float inc = 4.0*(this->m_Scene->GetVolumeZ1()-this->m_Scene->GetVolumeZ0())/float(range);
	float dist = value*inc;

	this->m_Scene->SetDistance(dist);
	this->m_Scene->CreateScene();
	this->updateGL();
}

void VisAtomWidget::OnBoxDisplayOption(QTreeWidgetItem*item)
{
	QString tname = item->text(0); 
	int checked1 = item->checkState(1);
	int checked2 = item->checkState(2);
	int style = 0;
	if (checked1) style = style + 1;
	if (checked2) style = style + 2;

    if (tname == tr("X-Y back"))
	{
		this->m_Scene->ShowBackfaceZ(style);
	}
	else if (tname == tr("Y-Z back"))
	{
		this->m_Scene->ShowBackfaceX(style);
	}
	else if (tname == tr("Z-X back"))
	{
		this->m_Scene->ShowBackfaceY(style);
	}
	else if (tname == tr("X-Y front"))
	{
		this->m_Scene->ShowFrontfaceZ(style);
	}
	else if (tname == tr("Y-Z front"))
	{
		this->m_Scene->ShowFrontfaceX(style);
	}
	else if (tname == tr("Z-X front"))
	{
		this->m_Scene->ShowFrontfaceY(style);
	}
	this->updateGL();
	return;
}
void VisAtomWidget::OnShowSceneBox(int show)
{
	this->m_Scene->ShowBoxBorder(show);
	this->updateGL();
}

void VisAtomWidget::OnShowSceneBackfaceX(int show)
{
	this->m_Scene->ShowBackfaceX(show);
	this->updateGL();
}

void VisAtomWidget::OnShowSceneBackfaceY(int show)
{
	this->m_Scene->ShowBackfaceY(show);
	this->updateGL();
}

void VisAtomWidget::OnShowSceneBackfaceZ(int show)
{
	this->m_Scene->ShowBackfaceZ(show);
	this->updateGL();
}

void VisAtomWidget::OnShowSceneFrontfaceX(int show)
{
	this->m_Scene->ShowFrontfaceX(show);
	this->updateGL();
}

void VisAtomWidget::OnShowSceneFrontfaceY(int show)
{
	this->m_Scene->ShowFrontfaceY(show);
	this->updateGL();
}

void VisAtomWidget::OnShowSceneFrontfaceZ(int show)
{
	this->m_Scene->ShowFrontfaceZ(show);
	this->updateGL();
}



void VisAtomWidget::SetCliperParameters(VisAtomCliper*sor)
{
	int style = sor->GetStyle();

	
	this->m_Cliper[style]->SetParameters(sor);  
	this->m_Sample->SetCliper(this->GetCurrentClipper());
	this->updateGL();
}

void VisAtomWidget::SetCurrentCliper(int current, bool redraw)
{
	if(current == this->m_currentCliper) return;
    this->m_currentCliper = current;
	this->m_Sample->SetCliper(this->GetCurrentClipper());
	if(redraw) this->updateGL();
}

void VisAtomWidget::OnClipXBack()
{
	VisAtomCliper*p = this->GetCurrentClipper();
	double pos[3], step[3];
	p->GetClipPos(pos);
	p->GetClipThickness(step);
	if(p->IsInverseClipX()) pos[0] = pos[0]+step[0];
	else                    pos[0] = pos[0]-step[0];

    //to prevent the sample moving out ofbox
	if(pos[0] < p->GetAttachedScene()->GetVolumeX0()) 
		pos[0] = p->GetAttachedScene()->GetVolumeX0();

	if(pos[0] > p->GetAttachedScene()->GetVolumeX1()) 
		pos[0] = p->GetAttachedScene()->GetVolumeX1();

	p->SetClipPos(pos);
	this->m_Sample->SetCliper(p);
	this->updateGL();
    return;
}

void VisAtomWidget::OnClipXForward()
{
	VisAtomCliper*p = this->GetCurrentClipper();
	double pos[3], step[3];
	p->GetClipPos(pos);
	p->GetClipThickness(step);
	if(p->IsInverseClipX()) pos[0] = pos[0]-step[0];
	else                    pos[0] = pos[0]+step[0];

    //to prevent the sample moving out ofbox
	if(pos[0] < p->GetAttachedScene()->GetVolumeX0()) 
		pos[0] = p->GetAttachedScene()->GetVolumeX0();

	if(pos[0] > p->GetAttachedScene()->GetVolumeX1()) 
		pos[0] = p->GetAttachedScene()->GetVolumeX1();

	p->SetClipPos(pos);
	this->m_Sample->SetCliper(p);
	this->updateGL();
    return;
}

void VisAtomWidget::OnClipYBack()
{
	VisAtomCliper*p = this->GetCurrentClipper();
	double pos[3], step[3];
	p->GetClipPos(pos);
	p->GetClipThickness(step);
	if(p->IsInverseClipY()) pos[1] = pos[1]+step[1];
	else                    pos[1] = pos[1]-step[1];

    //to prevent the sample moving out ofbox
	if(pos[1] < p->GetAttachedScene()->GetVolumeY0()) 
		pos[1] = p->GetAttachedScene()->GetVolumeY0();

	if(pos[1] > p->GetAttachedScene()->GetVolumeY1()) 
		pos[1] = p->GetAttachedScene()->GetVolumeY1();


	p->SetClipPos(pos);
	this->m_Sample->SetCliper(p);
	this->updateGL();
    return;
}

void VisAtomWidget::OnClipYForward()
{
	VisAtomCliper*p = this->GetCurrentClipper();
	double pos[3], step[3];
	p->GetClipPos(pos);
	p->GetClipThickness(step);
	if(p->IsInverseClipY()) pos[1] = pos[1]-step[1];
	else                    pos[1] = pos[1]+step[1];

    //to prevent the sample moving out ofbox
	if(pos[1] < p->GetAttachedScene()->GetVolumeY0()) 
		pos[1] = p->GetAttachedScene()->GetVolumeY0();

	if(pos[1] > p->GetAttachedScene()->GetVolumeY1()) 
		pos[1] = p->GetAttachedScene()->GetVolumeY1();

	p->SetClipPos(pos);
	this->m_Sample->SetCliper(p);
	this->updateGL();
    return;
}

void VisAtomWidget::OnClipZBack()
{
	VisAtomCliper*p = this->GetCurrentClipper();
	double pos[3], step[3];
	p->GetClipPos(pos);
	p->GetClipThickness(step);
	if(p->IsInverseClipZ()) pos[2] = pos[2]+step[2];
	else                    pos[2] = pos[2]-step[2];

    //to prevent the sample moving out ofbox
	if(pos[2] < p->GetAttachedScene()->GetVolumeZ0()) 
		pos[2] = p->GetAttachedScene()->GetVolumeZ0();

	if(pos[2] > p->GetAttachedScene()->GetVolumeZ1()) 
		pos[2] = p->GetAttachedScene()->GetVolumeZ1();


	p->SetClipPos(pos);
	this->m_Sample->SetCliper(p);
	this->updateGL();
    return;
}

void VisAtomWidget::OnClipZForward()
{
	VisAtomCliper*p = this->GetCurrentClipper();
	double pos[3], step[3];
	p->GetClipPos(pos);
	p->GetClipThickness(step);
	if(p->IsInverseClipZ()) pos[2] = pos[2]-step[2];
	else                    pos[2] = pos[2]+step[2];


    //to prevent the sample moving out ofbox
	if(pos[2] < p->GetAttachedScene()->GetVolumeZ0()) 
		pos[2] = p->GetAttachedScene()->GetVolumeZ0();

	if(pos[2] > p->GetAttachedScene()->GetVolumeZ1()) 
		pos[2] = p->GetAttachedScene()->GetVolumeZ1();


	p->SetClipPos(pos);
	this->m_Sample->SetCliper(p);
	this->updateGL();
    return;
}

void VisAtomWidget::OnShowClipANYDIRFrontface(bool show)
{
	VisAtomCliper*p = this->GetCurrentClipper();
	p->ShowFronFace(show);
	this->updateGL();
}

void VisAtomWidget::OnShowClipANYDIRBackface(bool show)
{
	VisAtomCliper*p = this->GetCurrentClipper();
	p->ShowBackFace(show);
	this->updateGL();
}


void VisAtomWidget::OnCoordinatBody()
{
	this->m_Scene->SetAsRelativeSystem();
}

void VisAtomWidget::OnCoordinatLab()
{
	this->m_Scene->SetAsLaboratorySystem();
}


void VisAtomWidget::OnTimerout()
{
	 if(this->m_pickedAtomInfo->isVisible()) return;
	  
	  if(this->m_MouseState == LBUTTON_PRESSED || this->m_MouseState == RBUTTON_PRESSED)
	  {
		  this->makeCurrent(); 
	      this->ProcessCurrentAction();
	      this->updateGL();
	  }
}


void VisAtomWidget::OnSelectShapeDot(bool checked)
{
	if(checked)
	this->SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_DOT, false);
}

void VisAtomWidget::OnSelectShapeSolidSphere(bool checked)
{
	if(checked)
	this->SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_SOLID, false);
}

void VisAtomWidget::OnSelectShapeWireSphere(bool checked)
{
	if(checked)
	this->SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_WIRE, false);
}

void VisAtomWidget::OnSelectShapeCircle(bool checked)
{
	if(checked)
	this->SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_LINE, false);
}


void VisAtomWidget::OnChangeAtomColor()
{
	   int i;

	   int R = 1.0*RGBSIZE;
	   int G = 1.0*RGBSIZE;
	   int B = 1.0*RGBSIZE;
	   int A = 1.0*RGBSIZE;

	   QColorDialog colorsel(QColor(R, G, B, A), this);

	   if(colorsel.exec())
	   {
	   
		  QColor color = colorsel.currentColor(); 
	      color.getRgb(&R, &G, &B, &A);
	      
		  this->m_Sample->SetColorForCurrentSubsample(R/float(RGBSIZE), G/float(RGBSIZE), B/float(RGBSIZE));
		  this->m_Sample->LoadCurSubsampleProp();
	      this->updateGL();
	   }
}

void VisAtomWidget::SetAtomRadiu(int range, int value, bool all)
{
	      float ratio = (float)value/(float)range;

		  float ds = this->m_Scene->GetVolumeX1() - this->m_Scene->GetVolumeX0();
		  if(ds > this->m_Scene->GetVolumeY1() - this->m_Scene->GetVolumeY0()) 
			  ds = this->m_Scene->GetVolumeY1() - this->m_Scene->GetVolumeY0();
		  if(ds > this->m_Scene->GetVolumeZ1() - this->m_Scene->GetVolumeZ0()) 
			  ds = this->m_Scene->GetVolumeZ1() - this->m_Scene->GetVolumeZ0();
		  ds = (ds/4.0)*ratio; 

		  float rad;
		  VisAtomSubSample*sub = this->m_Sample->GetCurrentSubsample();
          sub->GetCurAtomSize(rad);

		  if(abs(ds-rad) >0.00001)
		  {
           if(!all)
		   {
			  /*sub->SetAtomSize(ds,VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE); 
	          sub->SetAtomSize(ds,VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
		      this->m_Sample->LoadCurSubsampleProp();*/
	          for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	          {
		          if(this->m_Sample->IsSelectedSubsample(i))
		          {
	                 sub = this->m_Sample->GetSubsample(i);
			         sub->SetAtomSize(ds,VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE); 
	                 sub->SetAtomSize(ds,VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
                     this->m_Sample->LoadSubsampleProp(i);
		          }
	          }
	          this->updateGL();
		   }
		   else
		   {
			   for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
			   {
				   sub = this->m_Sample->GetSubsample(i);
				   sub->SetAtomSize(ds, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
				   sub->SetAtomSize(ds, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
                   this->m_Sample->LoadSubsampleProp(i);
			   }
		   }
	       this->updateGL();
		  }
}

void VisAtomWidget::SetVariableAtomRadiu(bool variable,bool all)
{
		VisAtomSubSample*sub = this->m_Sample->GetCurrentSubsample();

           if(!all)
		   {
			  /*sub->SetAtomHaveUnifiedSize(!variable,VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE); 
			   sub->SetAtomHaveUnifiedSize(!variable,VisAtomVisualizingPolicy::COLOR_STYLE_MAP); 
		      this->m_Sample->LoadCurSubsampleProp();*/
	          for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	          {
		          if(this->m_Sample->IsSelectedSubsample(i))
		          {
	                 sub = this->m_Sample->GetSubsample(i);
			         sub->SetAtomHaveUnifiedSize(!variable,VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE); 
			         sub->SetAtomHaveUnifiedSize(!variable,VisAtomVisualizingPolicy::COLOR_STYLE_MAP); 
                     this->m_Sample->LoadSubsampleProp(i);
		          }
	          }
		   }
		   else
		   {
			   for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
			   {
				   sub = this->m_Sample->GetSubsample(i);
				   sub->SetAtomHaveUnifiedSize(!variable, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
				   sub->SetAtomHaveUnifiedSize(!variable, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
                   this->m_Sample->LoadSubsampleProp(i);
			   }
		   }
	       this->updateGL();
        return;
}


//**************************************************************************************************
void VisAtomWidget::SetAtomShape(int shape, bool all)
{
	VisAtomVisualizingPolicy*policy;

	if(!all)
	{
	   /*VisAtomSubSample*sub = this->m_Sample->GetCurrentSubsample();
	   policy = sub->GetCurVisualPolicy(); 
	   int ageom = policy->GetGeomStyle()&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK;
	   if(ageom != shape)
	   {
			sub->SetAtomShapeStyle(shape, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
			sub->SetAtomShapeStyle(shape, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
	        this->m_Sample->LoadCurSubsampleProp();
			this->updateGL();
		}*/
      bool changed = false;
	  for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	  {
		  if(this->m_Sample->IsSelectedSubsample(i))
		  {
	         VisAtomSubSample*sub = this->m_Sample->GetSubsample(i);
		     policy = sub->GetCurVisualPolicy(); 
	         int ageom = policy->GetGeomStyle()&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK;
		     if(ageom != shape)
		     {
		    	sub->SetAtomShapeStyle(shape, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
			    sub->SetAtomShapeStyle(shape, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
                this->m_Sample->LoadSubsampleProp(i);
			    changed = true;
		     }
		  }
	  }
	  if(changed) this->updateGL();
    }
	else
	{
      bool changed = false;
	  for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	  {
	      VisAtomSubSample*sub = this->m_Sample->GetSubsample(i);
		  policy = sub->GetCurVisualPolicy(); 
	      int ageom = policy->GetGeomStyle()&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK;
		  if(ageom != shape)
		  {
			sub->SetAtomShapeStyle(shape, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
			sub->SetAtomShapeStyle(shape, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
            this->m_Sample->LoadSubsampleProp(i);
			changed = true;
		  }
	  }
	  if(changed) this->updateGL();
	}
    return;
}

//**************************************************************************************************
void VisAtomWidget::OnRotationXStep(int step)
{
	this->m_Scene->ChangeRotationStepx((float)step);
}

//**************************************************************************************************
void VisAtomWidget::OnRotationYStep(int step)
{
	this->m_Scene->ChangeRotationStepy((float)step);

}

//**************************************************************************************************
void VisAtomWidget::OnRotationZStep(int step)
{
	this->m_Scene->ChangeRotationStepz((float)step);

}

//**************************************************************************************************
void VisAtomWidget:: OnZoomStep(int step)
{
	this->m_Scene->ChangeZoomPercent(1.-(float)step/100.f);
}

//**************************************************************************************************
void VisAtomWidget::OnChangeGraphQuality(int nfai)
{
	Sphere::Set(nfai, nfai);
	Circle::Set(nfai);
	SolidCircle::Set(nfai);
	this->updateGL();
	return;
}

//**************************************************************************************************
void VisAtomWidget::ChangeGraphQuality(int nfai)
{
	Sphere::Set(nfai, nfai);
	Circle::Set(nfai);
	SolidCircle::Set(nfai);
	this->updateGL();
	return;
}

//**********************************************************************************************************
void VisAtomWidget::SetGeomStyleVectorVisible(bool show, int style, bool all)
{
	VisAtomSubSample*subp = this->m_Sample->GetCurrentSubsample();

	if(show)
	{
	  //dtermine if need creating initial lenscal
	  float vlenscal, vrad, arrowl, arrowr;
	  VisAtomVisualizingPolicy::GetVectorSize(vlenscal, vrad, arrowl, arrowr);
      if(vlenscal < 0) 
	  {
		this->m_Sample->GetDefaultVectorSize(vlenscal,vrad, arrowl, arrowr);
		VisAtomVisualizingPolicy::SetVectorSize(vlenscal, vrad);
	  }
	}

	if(!all)
	{
		/*subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyleVectorVisible(show);
		subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyleVectorVisible(show);
		for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
		        this->m_Sample->LoadSubsampleProp(i);*/
	  for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	  {
		  if(this->m_Sample->IsSelectedSubsample(i))
		  {
	         subp = this->m_Sample->GetSubsample(i);
             subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyleVectorVisible(show);
		     subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyleVectorVisible(show);		
			 this->m_Sample->LoadSubsampleProp(i);
		  }
	  }
 	  this->updateGL();
	}
    return;
}

//**********************************************************************************************************
#define VECTOR_INCREMENT 0.1
void VisAtomWidget::OnVectorLenMinus()
{
	float vlenscal, vrad, arrowl, arrowr;
	VisAtomVisualizingPolicy::GetVectorSize(vlenscal, vrad, arrowl, arrowr);
	vlenscal = vlenscal*(1.-VECTOR_INCREMENT);
	VisAtomVisualizingPolicy::SetVectorSize(vlenscal, vrad, arrowl, arrowr);
	for(int i=0; i<this->m_Sample->NumberofSubsample(); i++) this->m_Sample->LoadSubsampleProp(i);
    this->updateGL();
    return;
}
void VisAtomWidget:: OnVectorLenPlus()
{
	float vlenscal, vrad, arrowl, arrowr;
	VisAtomVisualizingPolicy::GetVectorSize(vlenscal, vrad, arrowl, arrowr);
	vlenscal = vlenscal*(1.+VECTOR_INCREMENT);
	VisAtomVisualizingPolicy::SetVectorSize(vlenscal, vrad, arrowl, arrowr);
	for(int i=0; i<this->m_Sample->NumberofSubsample(); i++) this->m_Sample->LoadSubsampleProp(i);
    this->updateGL();
    return;
}

//**********************************************************************************************************
void VisAtomWidget::OnVectorSizeMinus()
{
	float vlenscal, vrad, arrowl, arrowr;
	VisAtomVisualizingPolicy::GetVectorSize(vlenscal, vrad, arrowl, arrowr);
	vrad   = vrad*(1.-VECTOR_INCREMENT);
	arrowr = arrowr*(1.-VECTOR_INCREMENT);
	VisAtomVisualizingPolicy::SetVectorSize(vlenscal, vrad, arrowl, arrowr);
	//for(int i=0; i<this->m_Sample->NumberofSubsample(); i++) this->m_Sample->LoadSubsampleProp(i);
    this->updateGL();
    return;
}

//**********************************************************************************************************
void VisAtomWidget::OnVectorSizePlus()
{
	float vlenscal, vrad, arrowl, arrowr;
	VisAtomVisualizingPolicy::GetVectorSize(vlenscal, vrad, arrowl, arrowr);
	vrad = vrad*(1.+VECTOR_INCREMENT);
	arrowr = arrowr*(1.+VECTOR_INCREMENT);
	VisAtomVisualizingPolicy::SetVectorSize(vlenscal, vrad, arrowl, arrowr);
	//for(int i=0; i<this->m_Sample->NumberofSubsample(); i++) this->m_Sample->LoadSubsampleProp(i);
    this->updateGL();
    return;
}

//**********************************************************************************************************
void VisAtomWidget::OnSelectVectorStyleLine(bool checked)
{
	bool changed = 0;
	if(checked)
	{
	   /*VisAtomSubSample*subp = this->m_Sample->GetCurrentSubsample();
	   VisAtomVisualizingPolicy*policy = subp->GetCurVisualPolicy();
	   //reserve the shape of atoms
	   int vgeom = policy->GetGeomStyle()&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK;
	   if(vgeom != VisAtomVisualizingPolicy::VECTOR_STYLE_LINE)
	   {
		  subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_LINE, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		  subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_LINE, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
		  this->m_Sample->LoadCurSubsampleProp();
		  this->updateGL();
	   }*/

	  for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	  {
		  if(this->m_Sample->IsSelectedSubsample(i))
		  {
	         VisAtomSubSample*subp = this->m_Sample->GetSubsample(i);
      	     VisAtomVisualizingPolicy*policy = subp->GetCurVisualPolicy();
	         int vgeom = policy->GetGeomStyle()&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK;
             if(vgeom != VisAtomVisualizingPolicy::VECTOR_STYLE_LINE)
			 {
                subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_LINE, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		        subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_LINE, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);	
				this->m_Sample->LoadSubsampleProp(i);
				changed = 1;
			 }
		  }
	  }
	}
	this->updateGL();
	return;
}

//**********************************************************************************************************
void VisAtomWidget::OnSelectVectorStyleSolid(bool checked)
{
	bool changed = 0; 
	if(checked)
	{
	   /*VisAtomSubSample*subp = this->m_Sample->GetCurrentSubsample();
	   VisAtomVisualizingPolicy*policy = subp->GetCurVisualPolicy();
	   //reserve the shape of atoms
	   int vgeom = policy->GetGeomStyle()&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK;
	   if(vgeom != VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID)
	   {
		  subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		  subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
		  this->m_Sample->LoadCurSubsampleProp();
		  this->updateGL();*/
	  for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	  {
		  if(this->m_Sample->IsSelectedSubsample(i))
		  {
	         VisAtomSubSample*subp = this->m_Sample->GetSubsample(i);
      	     VisAtomVisualizingPolicy*policy = subp->GetCurVisualPolicy();
	         int vgeom = policy->GetGeomStyle()&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK;
             if(vgeom != VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID)
			 {
                subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		        subp->SetVectorStyle(VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);	
				this->m_Sample->LoadSubsampleProp(i);
				changed = 1;
			 }
		  }
	  }
	}
	if(changed) this->updateGL();

	return;
}

//**********************************************************************************************************
void VisAtomWidget::GetSceneVolume(float&xmin, float&xmax, float&ymin, float&ymax, float&zmin, float&zmax)
{
	if(!this->m_Scene) return;
	xmin = this->m_Scene->GetVolumeX0();
	xmax = this->m_Scene->GetVolumeX1();
	ymin = this->m_Scene->GetVolumeY0();
	ymax = this->m_Scene->GetVolumeY1();
	zmin = this->m_Scene->GetVolumeZ0();
	zmax = this->m_Scene->GetVolumeZ1();
	return;
}
//**********************************************************************************************************
void VisAtomWidget::SetSceneVolume(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	if(!this->m_Scene) return;
	this->m_Scene->SetVolumeX0(xmin);
	this->m_Scene->SetVolumeX1(xmax);
	this->m_Scene->SetVolumeY0(ymin);
	this->m_Scene->SetVolumeY1(ymax);
	this->m_Scene->SetVolumeZ0(zmin);
	this->m_Scene->SetVolumeZ1(zmax);
	this->m_Scene->CreateScene();


	VisAtomVisualizingPolicy::SetProjectionBox(xmin, xmax, ymin, ymax, zmin, zmax);
	return;
}
//**********************************************************************************************************
void VisAtomWidget::SetGeomStyleTrajectyoryVisible(bool show,int style,bool all)
{

	VisAtomSubSample*subp;

	if(!all)
	{
		subp = this->m_Sample->GetCurrentSubsample();
		subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyleTrajectoryVisible(show);
		subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyleTrajectoryVisible(show);
		this->m_Sample->LoadCurSubsampleProp();
	}
	else
	{
 	    for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	    {
		    subp = this->m_Sample->GetSubsample(i);
		    subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyleTrajectoryVisible(show);
		    subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyleTrajectoryVisible(show);
            this->m_Sample->LoadSubsampleProp(i);
		 }
	}
    return;

}

//**********************************************************************************************************
void VisAtomWidget::SetGeomStyleAtomVisible(bool show,int style,bool all)
{

	VisAtomSubSample*subp;

	if(!all)
	{
		subp = this->m_Sample->GetCurrentSubsample();
		subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyleAtomVisible(show);
		subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyleAtomVisible(show);
		this->m_Sample->LoadCurSubsampleProp();

	}
	else
	{
 	    for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	    {
		    subp = this->m_Sample->GetSubsample(i);
		    subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyleAtomVisible(show);
		    subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyleAtomVisible(show);
            this->m_Sample->LoadSubsampleProp(i);
		 }

	}
  	this->updateGL();
	return;
}


//**********************************************************************************************************
void VisAtomWidget::SetTrajectoryThickChanged(double thick, bool all)
{
	VisAtomSubSample*subp;
	if(!all)
	{
		subp = this->m_Sample->GetCurrentSubsample();
		subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetTrajectoryLinethick(thick);
		subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetTrajectoryLinethick(thick);
	}
	else
	{
 	    for(int i=0; i<this->m_Sample->NumberofSubsample(); i++)
	    {
		    subp = this->m_Sample->GetSubsample(i);
		    subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetTrajectoryLinethick(thick);
		    subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetTrajectoryLinethick(thick);
		 }

	}
  	this->updateGL();
	return;
}

//**********************************************************************************************************
void VisAtomWidget::OnOpenExtendedObject(VisAtomExtendObject*ptrObj, QString fname)
{
	int i;
	for(i=0; i<this->m_ExtObjs.size(); i++)
	{
		if(this->m_ExtObjs[i]->RUNTIME_CLASSNAME() == ptrObj->RUNTIME_CLASSNAME())
		{
			delete this->m_ExtObjs[i];
			if(ptrObj->HasData())	
                this->m_ExtObjs[i] = ptrObj; 
		     else
                this->m_ExtObjs.removeAt(i);

			break;
		}
	}

	if(ptrObj->HasData() && i == this->m_ExtObjs.size())
	{
         this->m_ExtObjs.append(ptrObj);
	}
	this->updateGL();
	return;
}