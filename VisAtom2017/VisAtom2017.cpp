/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#include "visatom2017.h"
#include <QFileDialog>
#include <QTextStream>
#include <stdlib.h>
#include "Bitmap.h"
#include "VisAtomDataSheet.h"
#include "VisAtomAboutDlg.h"
#include "VisAtomCliper.h"
#include "VisAtom_NewColorPolicyDlg.h"
#include "VisAtom_NewVectorPolicyDlg.h"
#include "VisAtom_NewSizePolicyDlg.h"
#include "VisAtom_SelectSampleDataDlg.h"
#include "visatom_loadtrajecoryThread.h"
#include "visatom_newdisplacementdlg.h"
#include "visatom_atomfilterdlg.h"
#include "visatom_findatomdlg.h"

#include "VisClusterEnvelope.h"


static int     CURRENT_EXTNUM  = -1;
static QString CURRENT_FNAME   = "";  

VisAtom2017::VisAtom2017(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    //extending the ui
	//fill the type of clipping style
	this->m_OnUpdateClipParamUI = 1;
    QListWidgetItem *style1 = new QListWidgetItem(ui.scenestylelistWidget);
    style1->setIcon(QIcon(":/VisAtom2017/images/scenestyle1.png"));
    style1->setText(tr("Clipping-OR"));
    style1->setTextAlignment(Qt::AlignHCenter);
    style1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *style2 = new QListWidgetItem(ui.scenestylelistWidget);
    style2->setIcon(QIcon(":/VisAtom2017/images/scenestyle2.png"));
    style2->setText(tr("Clipping-AND"));
    style2->setTextAlignment(Qt::AlignHCenter );
    style2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *style3 = new QListWidgetItem(ui.scenestylelistWidget);
    style3->setIcon(QIcon(":/VisAtom2017/images/scenestyle3.png"));
    style3->setText(tr("Clipping-ANYDIR"));
    style3->setTextAlignment(Qt::AlignHCenter );
    style3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	ui.clipDirComboBox->addItem(tr("plane #1"));
    ui.scenestylelistWidget->setCurrentRow(0);
	this->m_OnUpdateClipParamUI = 0;

	//file options of display box
	QTreeWidgetItem*itemzb = new QTreeWidgetItem();
	itemzb->setText(0, tr("X-Y back"));
	itemzb->setText(1, tr("Border"));
	itemzb->setText(2, tr("Face"));
	itemzb->setCheckState(1, Qt::Checked);
	itemzb->setCheckState(2, Qt::Checked);
	this->ui.boxDisplayTreeWidget->addTopLevelItem(itemzb);

	QTreeWidgetItem*itemxb = new QTreeWidgetItem();
	itemxb->setText(0, tr("Y-Z back"));
	itemxb->setText(1, tr("Border"));
	itemxb->setText(2, tr("Face"));
	itemxb->setCheckState(1, Qt::Checked);
	itemxb->setCheckState(2, Qt::Checked);
	this->ui.boxDisplayTreeWidget->addTopLevelItem(itemxb);

	QTreeWidgetItem*itemyb = new QTreeWidgetItem();
	itemyb->setText(0, tr("Z-X back"));
	itemyb->setText(1, tr("Border"));
	itemyb->setText(2, tr("Face"));
	itemyb->setCheckState(1, Qt::Checked);
	itemyb->setCheckState(2, Qt::Checked);
	this->ui.boxDisplayTreeWidget->addTopLevelItem(itemyb);

	QTreeWidgetItem*itemzf = new QTreeWidgetItem();
	itemzf->setText(0, tr("X-Y front"));
	itemzf->setText(1, tr("Border"));
	itemzf->setText(2, tr("Face"));
	itemzf->setCheckState(1, Qt::Checked);
	itemzf->setCheckState(2, Qt::Unchecked);
	this->ui.boxDisplayTreeWidget->addTopLevelItem(itemzf);

	QTreeWidgetItem*itemxf = new QTreeWidgetItem();
	itemxf->setText(0, tr("Y-Z front"));
	itemxf->setText(1, tr("Border"));
	itemxf->setText(2, tr("Face"));
	itemxf->setCheckState(1, Qt::Checked);
	itemxf->setCheckState(2, Qt::Unchecked);
	this->ui.boxDisplayTreeWidget->addTopLevelItem(itemxf);

	QTreeWidgetItem*itemyf = new QTreeWidgetItem();
	itemyf->setText(0, tr("Z-X front"));
	itemyf->setText(1, tr("Border"));
	itemyf->setText(2, tr("Face"));
	itemyf->setCheckState(1, Qt::Checked);
	itemyf->setCheckState(2, Qt::Unchecked);
	this->ui.boxDisplayTreeWidget->addTopLevelItem(itemyf);

	//fill group information

	//file the scalarwinlabels
	#define RGBSIZE 255
	float r, g, b;
    QColor color;

	VisAtomVisualizingPolicy::GetColorMappingBar(0, r, g, b);
	color.setRgb((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
	QPixmap fillcolor0(60,20);
	fillcolor0.fill(color);
	ui.scalarwin0Label->setPixmap(fillcolor0);

	VisAtomVisualizingPolicy::GetColorMappingBar(1, r, g, b);
	color.setRgb((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
	QPixmap fillcolor1(60,20);
	fillcolor1.fill(color);
	ui.scalarwin1Label->setPixmap(fillcolor1);

	VisAtomVisualizingPolicy::GetColorMappingBar(2, r, g, b);
	color.setRgb((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
	QPixmap fillcolor2(60,20);
	fillcolor2.fill(color);
	ui.scalarwin2Label->setPixmap(fillcolor2);

	VisAtomVisualizingPolicy::GetColorMappingBar(3, r, g, b);
	color.setRgb((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
	QPixmap fillcolor3(60,20);
	fillcolor3.fill(color);
	ui.scalarwin3Label->setPixmap(fillcolor3);

	VisAtomVisualizingPolicy::GetColorMappingBar(4, r, g, b);
	color.setRgb((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
	QPixmap fillcolor4(60,20);
	fillcolor4.fill(color);
	ui.scalarwin4Label->setPixmap(fillcolor4);

	VisAtomVisualizingPolicy::GetColorMappingBar(5, r, g, b);
	color.setRgb((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
	QPixmap fillcolor5(60,20);
	fillcolor5.fill(color);
	ui.scalarwin5Label->setPixmap(fillcolor5);


	//Reset the subsample list
	QStringList header;
	header.append(tr("Contents"));
	header.append(tr("Property"));

	ui.subsampleListTreeWidget->setHeaderLabels(header);

	//
	m_SampleDataSheet = NULL;
    //to attach the trjectory objectg to the dataloader, and start load data
    this->m_LoadDataThread = new VisAtom_LoadTrajecoryThread();
    this->m_LoadDataThread->AttachRender(this->ui.displayWidget);
	QObject::connect(this->m_LoadDataThread, SIGNAL(OnStartLoadFile(QString&,int,int)), this, SLOT(OnStartLoadTrajectory(QString&,int,int))); 
	QObject::connect(this->m_LoadDataThread, SIGNAL(OnEndLoadFile(QString&,int,int)), this, SLOT(OnEndLoadTrajectory(QString&,int,int))); 

	this->statusBar()->addWidget(&this->m_statulabel);
	this->statusBar()->addWidget(&this->m_progressbar);
	this->statusBar()->addPermanentWidget(&this->m_subsmaplelabel);
	this->m_statulabel.hide();
	this->m_progressbar.hide();


	ui.displayoptionDockWidget->hide();
	ui.subsampleSumDockWidget->hide();
	ui.actionShow_subsample_options->setEnabled(false);
	ui.actionShow_subsample_browser->setEnabled(false);
	ui.actionSubsample_filter->setEnabled(false);
	ui.actionFind_atoms->setEnabled(false);

	//ui.actionShow_scene_operation->setChecked(true);
	 float xmin, xmax, ymin, ymax, zmin, zmax; 
	 this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
	 this->ui.boxX0SpinBox->setValue((double)xmin);
	 this->ui.boxX0SpinBox->setMaximum((double)xmax);
	 //this->ui.boxX0SpinBox->setSingleStep((double)(xmax-xmin)/20);

	 this->ui.boxX1SpinBox->setValue((double)xmax);
	 this->ui.boxX1SpinBox->setMinimum((double)xmin);
	 //this->ui.boxX1SpinBox->setSingleStep((double)(xmax-xmin)/20);

	 this->ui.boxY0SpinBox->setValue((double)ymin);
	 this->ui.boxY0SpinBox->setMaximum((double)ymax);
	 //this->ui.boxY0SpinBox->setSingleStep((double)(ymax-ymin)/20);

	 this->ui.boxY1SpinBox->setValue((double)ymax);
	 this->ui.boxY1SpinBox->setMinimum((double)ymin);
	 //this->ui.boxY1SpinBox->setSingleStep((double)(ymax-ymin)/20);

	 this->ui.boxZ0SpinBox->setValue((double)zmin);
	 this->ui.boxZ0SpinBox->setMaximum((double)zmax);
	 //this->ui.boxZ0SpinBox->setSingleStep((double)(zmax-zmin)/20);

	 this->ui.boxZ1SpinBox->setValue((double)zmax);
	 this->ui.boxZ1SpinBox->setMinimum((double)zmin);
	 //this->ui.boxZ1SpinBox->setSingleStep((double)(zmax-zmin)/20);

	 if(this->ui.autoboxpos_checkBox->checkState()) 
	 {
		 this->ui.boxX0SpinBox->setEnabled(false);
		 this->ui.boxX1SpinBox->setEnabled(false);
		 this->ui.boxY0SpinBox->setEnabled(false);
		 this->ui.boxY1SpinBox->setEnabled(false);
		 this->ui.boxZ0SpinBox->setEnabled(false);
		 this->ui.boxZ1SpinBox->setEnabled(false);
	 }

	 //for change visiable atoms
	 m_Atomfilter = NULL;
	 m_FindatomDlg = NULL;

	//
	CURRENT_FNAME.clear();
	CURRENT_EXTNUM = -1;

}

VisAtom2017::~VisAtom2017()
{
   if(m_SampleDataSheet) delete m_SampleDataSheet; m_SampleDataSheet = NULL;
   if(m_LoadDataThread)  delete m_LoadDataThread; m_LoadDataThread = NULL;
   if (m_Atomfilter)     delete m_Atomfilter; m_Atomfilter = NULL;
}

void VisAtom2017::OnAboutVisatom2013()
{
	VisAtomAboutDlg aboutdlg(this);
	aboutdlg.exec();
}
int	static extract_numb( char*str, int mxCount, char**strnumb)
{
/*
!  Purpose: to extract number from a string
!  Input:   STRING a string
!  Ouput:   COUNT  the number of numbers found in the string
!           REAL_NUMB the number text founded
!  Note:    the REAL_NUMBi are character varible. These character number
!           could be transformed to real number by DRSTR or RSTRi
! Auther:   Hou Qing, Inst. of Nucl. Sci.and Tech., Sichuan Union University
!            
!
*/
	 int  i, flag, index1, count;

	 count=0;
	 index1=0;
	 i = 0;
	 flag = 0;
	 do 
	 {
	   if(str[i] == ' ' || str[i] == ',') 
	   {
	       if(index1 > 0) 
		   {
            strncpy(strnumb[count-1], &str[i-index1], index1);
            strnumb[count-1][index1] = NULL;
	        index1 = 0;
		   }
	      flag = 0;
	   }
       else 
	   {
	     if((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '+' || str[i] == '.' )
		 {
	        if(flag == 0 ) 
			{ 
			   flag = 1;
	           count ++;
	           if(count > mxCount) break;
			}
		 }
         else
		 {
	        if(flag != 1) flag = 2;
		 }
	   }
          
	  if(flag == 1) index1 ++;

	  i++;
	  if(i >= strlen(str) )
	  {
	     if(index1 > 0) 
		 {
            strncpy(strnumb[count-1], &str[i-index1], index1);
		    strnumb[count-1][index1] = NULL;
		 }
	     break;
	  }
	 } while(1);
	 return count;
}

int static adjustl( char*str)
{
/*
!  Purpose: to skip the space ahead of str
!  Input:   STR  a string
!  Ouput:   STR  the strin with the ahead space eliminited
!           REAL_NUMB the number text founded
! Auther:   Hou Qing, Inst. of Nucl. Sci.and Tech., Sichuan Union University
!            
!
*/
	 int  i;

	 do 
	 {
       if(str[0] == ' ')
	   {
   	     for(i=0; i<strlen(str); i++ ) 
		 {
			 str[i] = str[i+1];
		 }
		 str[i] = NULL;
	    }
	   else
	   {
		 break;
	   }
	 } while(1);
	 int len = strlen(str);
	 return strlen(str);
}

#define MAXCOL  64
#define MAXSTRL 32
void VisAtom2017::OnFileOpen()
{
	if (this->m_FindatomDlg) this->m_FindatomDlg->hide();

	QString fileName = QFileDialog::getOpenFileName(this);
	QString ext, snum, dot='.';
	QString currentfname;
	int index=-1;
    
	currentfname = CURRENT_FNAME;
    if (!fileName.isEmpty()) 
	{
		 this->OpenFile(fileName);

		 index = fileName.length()-fileName.lastIndexOf(dot);
		 ext = fileName.section(dot, -1);
		 CURRENT_EXTNUM = ext.toInt(); 
		 CURRENT_FNAME  = fileName;
		 CURRENT_FNAME.chop(index);

		 if(CURRENT_FNAME.isEmpty())
		 {
			 ui.actionNext->setEnabled(false);
			 ui.actionPrev->setEnabled(false);
		 }
		 else
		 {
			 ui.actionNext->setEnabled(true);
			 ui.actionPrev->setEnabled(true);
             if(CURRENT_EXTNUM<=0) ui.actionPrev->setEnabled(false);
		 }
		 //if filename changed for trajectory
		 if(!VisAtom_NewDisplacementPolicyDlg::GetFname().isEmpty())
		 {
		    if(CURRENT_FNAME != VisAtom_NewDisplacementPolicyDlg::GetFname())
			{
			  QMessageBox msgBox(this);
		      msgBox.setIcon(QMessageBox::Question);
		      msgBox.setText(tr("Filename has been changed for trajectories, reset the trajectory data?"));
		      msgBox.addButton(QMessageBox::Yes);
		      msgBox.addButton(QMessageBox::No);
		      if(msgBox.exec() == QMessageBox::Yes)
              {
                 this->OnResetTrajectoryDataPushButton();
			  }
			}
		 }
	}
    return;
}

void VisAtom2017::OnFileNext()
{
	if (this->m_FindatomDlg) this->m_FindatomDlg->hide();

	QString fileName, ext,snum;
	QFile file;
    
	int i,to=2000;
	for(i=CURRENT_EXTNUM+1; i<=CURRENT_EXTNUM+to; i++)
	{
	    if(i<10)        ext = ".000";
	    else if(i<100)  ext = ".00";
	    else if(i<1000) ext = ".0";
		else ext=".";
   
	    snum.setNum(i);
	    ext.append(snum);
	    fileName = CURRENT_FNAME;
	    fileName.append(ext);
		file.setFileName(fileName);
		if (file.exists()) break;
    }
	if (i>to) 
	{
		 QMessageBox msgBox(this);
		 msgBox.setIcon(QMessageBox::Warning);
		 
		 QString msg("Cannot find next file");
		 msgBox.setText(msg);
		 msgBox.addButton(QMessageBox::Ok);
		 msgBox.exec();
         return;
    }

	int col[4];
	VisAtom_SelectSampleDataDlg::GetDataCol(col);
	if(col[VisAtomDataSheet::COLPROP_POSX] <0 ||
	   col[VisAtomDataSheet::COLPROP_POSY] <0 ||
	   col[VisAtomDataSheet::COLPROP_POSZ] <0)
       this->OpenFile(fileName, true);
	else 
       this->OpenFile(fileName, false);
    CURRENT_EXTNUM = i;  
	if(CURRENT_EXTNUM>0) ui.actionPrev->setEnabled(true);
	else ui.actionPrev->setEnabled(false);
}

void VisAtom2017::OnFilePrev()
{
	if (this->m_FindatomDlg) this->m_FindatomDlg->hide();

	QString fileName, ext,snum;
	QFile file;
    
	int i,to=2000;
	if(CURRENT_EXTNUM <= 0) 
	{
        ui.actionPrev->setEnabled(false);
		return;
	}

	for(i=CURRENT_EXTNUM-1; i>=0; i--)
	{
	    if(i<10)        ext = ".000";
	    else if(i<100)  ext = ".00";
	    else if(i<1000) ext = ".0";
		else ext=".";
  
	    snum.setNum(i);
	    ext.append(snum);
	    fileName = CURRENT_FNAME;
	    fileName.append(ext);
		file.setFileName(fileName);
		if (file.exists()) break;
    }
	if (i<0) 
	{
		 QMessageBox msgBox(this);
		 msgBox.setIcon(QMessageBox::Warning);
		 QString msg("Cannot find previous file");
		 msgBox.setText(msg);
		 msgBox.addButton(QMessageBox::Ok);
		 msgBox.exec();
         return;
    }


	int col[4];
	VisAtom_SelectSampleDataDlg::GetDataCol(col);
	if(col[VisAtomDataSheet::COLPROP_POSX] <0 ||
	   col[VisAtomDataSheet::COLPROP_POSY] <0 ||
	   col[VisAtomDataSheet::COLPROP_POSZ] <0)
       this->OpenFile(fileName, true);
	else 
       this->OpenFile(fileName, false);

    CURRENT_EXTNUM = i;  
	if(CURRENT_EXTNUM>0) ui.actionPrev->setEnabled(true);
	else ui.actionPrev->setEnabled(false);

}

void VisAtom2017::LoadFrom_BOXCFG14(QString fileName)
{
		 QFile file(fileName);
         file.open(QIODevice::ReadOnly|QIODevice::Text);

		 //to determine how many col for each line
		 int na, nrow=0, ncol=0, i, j, len;
		 float bl[3], bs[3];

         char buf[MAXCOL*MAXSTRL+1];
		 char*strnumb[MAXCOL];
		 for(i=0; i<MAXCOL; i++) strnumb[i] = new char[MAXSTRL];

         		 
         file.open(QIODevice::ReadOnly|QIODevice::Text);

		 while (!file.atEnd()) 
		 {
				 len = file.readLine(buf, sizeof(buf))-1;
				 nrow++;  

				 if(buf[0] == '&' && buf[1] == 'N' && buf[2] == 'A' && buf[3] == 'T'&& buf[4] == 'O'&& buf[5] == 'M')
				 {
					 extract_numb( buf, MAXCOL, strnumb);
                     na = atoi(strnumb[1]); 
				 }

				 if(buf[0] == '&' && buf[1] == 'B' && buf[2] == 'O' && buf[3] == 'X'&& 
					           buf[4] == 'S'&& buf[5] == 'I'&& buf[6] == 'Z'&& buf[7] == 'E')
				 {
					 extract_numb( buf, 4, strnumb);
                     bs[0] = atof(strnumb[1]); 
                     bs[1] = atof(strnumb[2]); 
                     bs[2] = atof(strnumb[3]); 
				 }

				 if(buf[0] == '&' && buf[1] == 'B' && buf[2] == 'O' && buf[3] == 'X'&& 
					           buf[4] == 'L'&& buf[5] == 'O'&& buf[6] == 'W')
				 {
					 extract_numb( buf, 4, strnumb);
                     bl[0] = atof(strnumb[1]); 
                     bl[1] = atof(strnumb[2]); 
                     bl[2] = atof(strnumb[3]); 
				 }

				 if(buf[0] == '&' && buf[1] == 'T' && buf[2] == 'Y' && buf[3] == 'P'&& buf[4] == 'E')
				 {
					if(ncol <= 0)
				   {
					len = file.readLine(buf, sizeof(buf))-1;
					ncol = extract_numb( buf, MAXCOL, strnumb);
					}
					break;
				 }
         }
		 file.close();

		 this->m_progressbar.setMaximum(nrow);
	     this->m_progressbar.show();
		 //Add to data sheet
		 if(!m_SampleDataSheet) m_SampleDataSheet = new VisAtomDataSheet(ncol, na); 
		 else
		 {
			 m_SampleDataSheet->Clear(); 
			 m_SampleDataSheet->SetColRow(std::max(ncol,4), na);
		 }
		 m_SampleDataSheet->SetBoxSize(bl[0], bl[0]+bs[0], 
			                           bl[1], bl[1]+bs[1], 
			                           bl[2], bl[2]+bs[2]); 

		 file.open(QIODevice::ReadOnly|QIODevice::Text);
		 int progressstep = 0.1*nrow; 
		 if( progressstep == 0) progressstep = 1;

		 int ii = 0;
		 //skip the header 
         for(j=0; j<nrow; j++)
		 {
			 len = file.readLine(buf, sizeof(buf))-1;
			 ii++; 
		     if(ii%progressstep == 0) m_progressbar.setValue(ii);
		 }

         for(j=0; j<na; j++)
	      {
	         file.readLine(buf, sizeof(buf));
		     ncol = extract_numb( buf, MAXCOL, strnumb);
             this->m_SampleDataSheet->SetDataForRow(j, strnumb);
			 ii++; 
		     if(ii%progressstep == 0) m_progressbar.setValue(ii);
		 }

		 m_progressbar.setValue(nrow);
		 file.close();
         for(i=0; i<MAXCOL; i++) delete strnumb[i];

         return;
}

void VisAtom2017::LoadFrom_BOXCFG16(QString fileName)
{
		 QFile file(fileName);
         file.open(QIODevice::ReadOnly|QIODevice::Text);

		 //to determine how many col for each line
		 int na, nrow=0, ncol=0, i, j, len;
		 float bl[3], bs[3];

         char buf[MAXCOL*MAXSTRL+1];
		 char*strnumb[MAXCOL];
		 for(i=0; i<MAXCOL; i++) strnumb[i] = new char[MAXSTRL];

         		 
         file.open(QIODevice::ReadOnly|QIODevice::Text);

		 while (!file.atEnd()) 
		 {
				 len = file.readLine(buf, sizeof(buf))-1;
				 nrow++; 
				 if(adjustl(buf) <= 1) continue;
				 if(buf[0] == '!')continue;

				 if(buf[0] == '&' && buf[1] == 'N' && buf[2] == 'A' && buf[3] == 'T'&& buf[4] == 'O'&& buf[5] == 'M')
				 {
					 extract_numb( buf, MAXCOL, strnumb);
                     na = atoi(strnumb[0]); 
				 }

				 if(buf[0] == '&' && buf[1] == 'B' && buf[2] == 'O' && buf[3] == 'X'&& 
					           buf[4] == 'S'&& buf[5] == 'I'&& buf[6] == 'Z'&& buf[7] == 'E')
				 {
					 extract_numb( buf, 4, strnumb);
                     bs[0] = atof(strnumb[0]); 
                     bs[1] = atof(strnumb[1]); 
                     bs[2] = atof(strnumb[2]); 
				 }

				 if(buf[0] == '&' && buf[1] == 'B' && buf[2] == 'O' && buf[3] == 'X'&& 
					           buf[4] == 'L'&& buf[5] == 'O'&& buf[6] == 'W')
				 {
					 extract_numb( buf, 4, strnumb);
                     bl[0] = atof(strnumb[0]); 
                     bl[1] = atof(strnumb[1]); 
                     bl[2] = atof(strnumb[2]); 
				 }

				/* if(buf[0] == '&' && buf[1] == 'T' && buf[2] == 'Y' && buf[3] == 'P'&& buf[4] == 'E')
				 {
					if(ncol <= 0)
				   {
					len = file.readLine(buf, sizeof(buf))-1;
					ncol = extract_numb( buf, MAXCOL, strnumb);
					}
					break;
				 }*/
				 if(buf[0] != '&')
				 {
					 ncol = extract_numb( buf, MAXCOL, strnumb);
					 nrow--;
					 break;
				 }
         }
		 file.close();

		 this->m_progressbar.setMaximum(nrow);
	     this->m_progressbar.show();
		 //Add to data sheet
		 if(!m_SampleDataSheet) m_SampleDataSheet = new VisAtomDataSheet(ncol, na); 
		 else
		 {
			 m_SampleDataSheet->Clear(); 
			 m_SampleDataSheet->SetColRow(std::max(ncol,4), na);
		 }
		 m_SampleDataSheet->SetBoxSize(bl[0], bl[0]+bs[0], 
			                           bl[1], bl[1]+bs[1], 
			                           bl[2], bl[2]+bs[2]); 

		 file.open(QIODevice::ReadOnly|QIODevice::Text);
		 int progressstep = 0.1*nrow; 
		 if( progressstep == 0) progressstep = 1;

		 int ii = 0;
		 //skip the header 
         for(j=0; j<nrow; j++)
		 {
			 len = file.readLine(buf, sizeof(buf))-1;
			 ii++; 
		     if(ii%progressstep == 0) m_progressbar.setValue(ii);
		 }

         for(j=0; j<na; j++)
	      {
	         file.readLine(buf, sizeof(buf));
		     ncol = extract_numb( buf, MAXCOL, strnumb);
             this->m_SampleDataSheet->SetDataForRow(j, strnumb);
			 ii++; 
		     if(ii%progressstep == 0) m_progressbar.setValue(ii);
		 }

		 m_progressbar.setValue(nrow);
		 file.close();
         for(i=0; i<MAXCOL; i++) delete strnumb[i];

         return;
}

void VisAtom2017::LoadFrom_FMTUNKNOWN(QString fileName)
{
		 QFile file(fileName);
         file.open(QIODevice::ReadOnly|QIODevice::Text);

		 //to determine how many col for each line
		 int nrow=0, ncol=0, i, hlines, hasbl, hasbs;
  		 float bl[3], bs[3];
         hasbl = 0; hasbs = 0;

         char buf[MAXCOL*MAXSTRL+1];
		 char*strnumb[MAXCOL];
		 for(i=0; i<MAXCOL; i++) strnumb[i] = new char[MAXSTRL];

         file.open(QIODevice::ReadOnly|QIODevice::Text);
		 hlines = 0;
         while (!file.atEnd()) 
		 {
				 int len, lentrim, flag;
				 len = file.readLine(buf, sizeof(buf))-1;
				 if(len <= 0) {hlines++; continue;}
				 flag = 0;
	 			 lentrim = 0;
				 //skip comment lines
				 for(i=0; i<len; i++)
				 {
					 if(buf[i] == ' ') continue;
					 lentrim++;
					 if(buf[i] == '!' || buf[i] == '&') 
					 {
						flag = 1; hlines++; 
						  // check if it is box size information
				          if(buf[i] == '&' && buf[i+1] == 'B' && buf[i+2] == 'O' && buf[i+3] == 'X'&& 
					           buf[i+4] == 'S'&& buf[i+5] == 'I'&& buf[i+6] == 'Z'&& buf[i+7] == 'E')
				          {
					       extract_numb( buf, 3, strnumb);
                           bs[0] = atof(strnumb[0]); 
                           bs[1] = atof(strnumb[1]); 
                           bs[2] = atof(strnumb[2]); 
						   hasbs = 1; 
				          }

				          if(buf[i] == '&' && buf[i+1] == 'B' && buf[i+2] == 'O' && buf[i+3] == 'X'&& 
					           buf[i+4] == 'L'&& buf[i+5] == 'O'&& buf[i+6] == 'W')
				          {
					      extract_numb( buf, 3, strnumb);
                          bl[0] = atof(strnumb[0]); 
                          bl[1] = atof(strnumb[1]); 
                          bl[2] = atof(strnumb[2]); 
						  hasbl = 1;
				          }
						break; 
					 }
					 else break;
				 }
				 if(flag) continue; 
				 if(lentrim <= 0 ) { if(nrow <=0) hlines++; continue;}
				 nrow ++;
				 if(nrow == 1) ncol = extract_numb( buf, MAXCOL, strnumb);
         }
		 file.close();

		 this->m_progressbar.setMaximum(nrow);
	     this->m_progressbar.show();
		 //Add to data sheet
		 if(!m_SampleDataSheet) m_SampleDataSheet = new VisAtomDataSheet(ncol, nrow); 
		 else
		 {
			 m_SampleDataSheet->Clear(); 
			 m_SampleDataSheet->SetColRow(std::max(ncol,4), nrow);
		 }
		 // boxsize is unknown
		 if(!hasbl && !hasbs) 
		 {
		    m_SampleDataSheet->SetBoxSize(1.f, -1.f, 1.f, -1.f, 1.f, -1.f);
		 }
        
		 if(hasbs)
		 {
            if(hasbl) m_SampleDataSheet->SetBoxSize(bl[0], bl[0]+bs[0], bl[1], bl[1]+bs[1], bl[2], bl[2]+bs[2]); 
			else m_SampleDataSheet->SetBoxSize(-bs[0]*0.5f, bs[0]*0.5f, -bs[1]*0.5f, bs[1]*0.5f, -bs[2]*0.5f, bs[2]*0.5f); 
		}


		 file.open(QIODevice::ReadOnly|QIODevice::Text);
		 for(i=0; i<hlines; i++)
		    file.readLine(buf, sizeof(buf));

		 int progressstep = 0.1*nrow; if(progressstep == 0) progressstep = 1;
		 for(i=0; i<nrow; i++)
		 {
		    file.readLine(buf, sizeof(buf));
			ncol = extract_numb( buf, MAXCOL, strnumb);
            this->m_SampleDataSheet->SetDataForRow(i, strnumb);

			if(i%progressstep == 0) m_progressbar.setValue(i);
		 }
		  m_progressbar.setValue(nrow);
		 file.close();
         for(i=0; i<MAXCOL; i++) delete strnumb[i];

         return;
}
void VisAtom2017::OpenFile(QString fileName, bool selcoldlg)
{
	this->setCursor(Qt::WaitCursor);
	QString statuText = tr("Load data:");
	statuText.append(fileName);
	this->m_statulabel.setText(statuText);
	this->m_statulabel.show();
    VisAtomDataSheet*olddata = this->m_SampleDataSheet;

		 QFile file(fileName);
         file.open(QIODevice::ReadOnly|QIODevice::Text);

         char buf[MAXCOL*MAXSTRL+1];

		 //to determine the format of the file
		 int hlines = 0;
         while (!file.atEnd()) 
		 {
				 int len;
				 len = file.readLine(buf, sizeof(buf))-1;
				 //skip empty lines
				 if(len <= 0) {hlines++; continue;}  

				 //skip comment lines
				 int lentrim = -1;
				 int flag = 0;
				 for(int i=0; i<len; i++)
				 {
					 lentrim ++; 
					 if(buf[i] == ' ') {flag = -1; continue;}

					 if(buf[i] == '!') 
					  {
						flag = -1; 
						break; 
					  }
					 else 
					 {
						flag = lentrim; 
					    break;
					 }
				 }

				 //skip comment lines
				 if(flag  < 0) continue;


				 if(buf[lentrim] == '&')
				 {
				   if(buf[lentrim+1] == 'B' && buf[lentrim+2] == 'O' && buf[lentrim+3] == 'X' && 
				  	  buf[lentrim+4] == 'C' && buf[lentrim+5] == 'F' && buf[lentrim+6] == 'G' && 
					  buf[lentrim+7] == '1' && buf[lentrim+8] == '4')
				    {
                      LoadFrom_BOXCFG14(fileName);
					  break;
				    }
				   else if(buf[lentrim+1] == 'B' && buf[lentrim+2] == 'O' && buf[lentrim+3] == 'X' && 
				  	       buf[lentrim+4] == 'C' && buf[lentrim+5] == 'F' && buf[lentrim+6] == 'G' && 
					       buf[lentrim+7] == '1' && buf[lentrim+8] == '5')
				    {
                      LoadFrom_BOXCFG14(fileName);
					  break;
				    }
				   else if(buf[lentrim+1] == 'B' && buf[lentrim+2] == 'O' && buf[lentrim+3] == 'X' && 
				  	       buf[lentrim+4] == 'C' && buf[lentrim+5] == 'F' && buf[lentrim+6] == 'G' && 
					       buf[lentrim+7] == '1' && buf[lentrim+8] == '6')
				    {
                      LoadFrom_BOXCFG16(fileName);
					  break;
				    }
				   else if(buf[lentrim+1] == 'B' && buf[lentrim+2] == 'O' && buf[lentrim+3] == 'X' && 
				  	       buf[lentrim+4] == 'C' && buf[lentrim+5] == 'F' && buf[lentrim+6] == 'G' && 
					       buf[lentrim+7] == '1' && buf[lentrim+8] == '8')
				    {
                      LoadFrom_BOXCFG16(fileName);
					  break;
				    }

				   else
				   {
                      continue;
				   }
				 }
				 else
				 {
                   LoadFrom_FMTUNKNOWN(fileName);
                   break;
				 }

         }
		 file.close();

         this->unsetCursor();
		 //Choose the columes for the atom type and position of atoms
		 if((VisAtom_SelectSampleDataDlg::IsShowAgain() && selcoldlg ))
		 {
		     VisAtom_SelectSampleDataDlg dataselDlg(this);
		     dataselDlg.SetDataSheet(this->m_SampleDataSheet);
		     if(!dataselDlg.exec() == QDialog::Accepted) 
			 {
				this->m_statulabel.hide();
		        this->m_progressbar.hide();
				 return;
			 }
		 }
         
		 int col[4];
		 VisAtom_SelectSampleDataDlg::GetDataCol(col);
		 this->m_SampleDataSheet->SetTypeCol(col[VisAtomDataSheet::COLPROP_TYPE]);
		 this->m_SampleDataSheet->SetPosxCol(col[VisAtomDataSheet::COLPROP_POSX]);
		 this->m_SampleDataSheet->SetPosyCol(col[VisAtomDataSheet::COLPROP_POSY]);
		 this->m_SampleDataSheet->SetPoszCol(col[VisAtomDataSheet::COLPROP_POSZ]);

		 //Connect the datasheet to the sample
		 this->ui.displayWidget->SetSampleDataSheet(this->m_SampleDataSheet,false);
  

		 //if this is not the first file-open, using the current clipping and color style
		 if(olddata)
		 {
			 if((this->ui.displayWidget->CurrentClipper()->GetStyle() == VisAtomCliper::CLIPPING_STYLE_ANYDIR) &&
				 this->ui.clipthroughAtomRadioButton->isChecked() )
		     {
				 if( this->ui.clipthroughAtomSpinBox->value() <= this->m_SampleDataSheet->GetNumRow())
                     this->OnClipthroughAtomSpinBox(this->ui.clipthroughAtomSpinBox->value());
				 else
					 this->ui.clipthroughPosRadioButton->setChecked(true);
			 }
			 else
			    this->OnClipEnabled();
		 }
		 else if(this->ui.displayWidget->CurrentClipper()->GetStyle() == VisAtomCliper::CLIPPING_STYLE_ANYDIR)
		 {
             this->OnClipEnabled();
		 }

		 this->UpdateSubsampleLIST();
		 this->UpdateSubsampleUI();
		 this->ui.subsampleSumDockWidget->show();
		 this->ui.displayoptionDockWidget->show();
	     this->ui.actionShow_subsample_options->setEnabled(true);
	     this->ui.actionShow_subsample_browser->setEnabled(true);
		 this->ui.actionSubsample_filter->setEnabled(false);
		 this->ui.actionFind_atoms->setEnabled(true);

		 // update scene box UI
         if(this->ui.displayWidget->IfAutoNormalizingView())
		 {
		    float xmin, xmax, ymin, ymax, zmin, zmax; 
		    this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
	        this->ui.boxX0SpinBox->setValue((double)xmin);
	        this->ui.boxX0SpinBox->setMaximum((double)xmax);
	        //this->ui.boxX0SpinBox->setSingleStep((double)(xmax-xmin)/20);

            this->ui.boxX1SpinBox->setValue((double)xmax);
	        this->ui.boxX1SpinBox->setMinimum((double)xmin);
	        //this->ui.boxX1SpinBox->setSingleStep((double)(xmax-xmin)/20);

	        this->ui.boxY0SpinBox->setValue((double)ymin);
	        this->ui.boxY0SpinBox->setMaximum((double)ymax);
	        //this->ui.boxY0SpinBox->setSingleStep((double)(ymax-ymin)/20);

	        this->ui.boxY1SpinBox->setValue((double)ymax);
	        this->ui.boxY1SpinBox->setMinimum((double)ymin);
	        //this->ui.boxY1SpinBox->setSingleStep((double)(ymax-ymin)/20);

	        this->ui.boxZ0SpinBox->setValue((double)zmin);
	        this->ui.boxZ0SpinBox->setMaximum((double)zmax);
	        //this->ui.boxZ0SpinBox->setSingleStep((double)(zmax-zmin)/20);

	        this->ui.boxZ1SpinBox->setValue((double)zmax);
	        this->ui.boxZ1SpinBox->setMinimum((double)zmin);
	        //this->ui.boxZ1SpinBox->setSingleStep((double)(zmax-zmin)/20);
		 }

         //
		 QString title = tr("VisAtom2017 - ");
		 title.append(fileName);
		 this->m_statulabel.hide();
		 this->m_progressbar.hide();
		 this->setWindowTitle(title);
	     return;
}



void VisAtom2017::OnSaveImage()
{
	QString ext = tr("bmp");
	QString fileName = QFileDialog::getSaveFileName
		(this, tr("Save image to bitmap file"), NULL, tr("*.bmp"), &ext);

	   if(!fileName.isEmpty())
	   {
         //QPixmap pixmap;
	     //ui.displayWidget->RenderToPixmap(pixmap);
	     //pixmap.save ( fileName, "PNG", 100 ); 
         Bitmap bitmap;  
	     ui.displayWidget->RenderToBitmap(bitmap);
		 bitmap.SaveToFile(fileName);
	   }
}

void VisAtom2017::OnActionCopyImage()
{
    Bitmap bitmap;  
	ui.displayWidget->RenderToBitmap(bitmap);
	bitmap.SaveToClipBoard();
	return;
}

void VisAtom2017::OnActionRotateX(bool checked)
{
		//this->ui.rotationXtoolButton->setChecked(true);
		this->OnSelectRotationX();
}
void VisAtom2017::OnActionRotateY(bool checked)
{
		//this->ui.rotationYtoolButton->setChecked(true);
   	    this->OnSelectRotationY();
}

void VisAtom2017::OnActionRotateZ(bool checked)
{
		//this->ui.rotationZtoolButton->setChecked(true);
   	    this->OnSelectRotationZ();
}
void VisAtom2017::OnActionZoomview(bool checked)
{
		//this->ui.zoomScenetoolButton->setChecked(true);
   	    this->OnSelectZoomScene();
}


void VisAtom2017::OnSelectRotationX()
{
	    this->ui.actionZoom_view->setChecked(false);
		this->ui.actionRotate_X->setChecked(true);
		this->ui.actionRotate_Y->setChecked(false);
		this->ui.actionRotate_Z->setChecked(false);

		this->ui.actionViewport_center->setChecked(false);

		emit this->SetMouseAction(ui.displayWidget->ACTION_ROTATION_X);
}

void VisAtom2017::OnSelectRotationY()
{
	    this->ui.actionZoom_view->setChecked(false);
		this->ui.actionRotate_X->setChecked(false);
		this->ui.actionRotate_Y->setChecked(true);
		this->ui.actionRotate_Z->setChecked(false);
		this->ui.actionViewport_center->setChecked(false);


		emit this->SetMouseAction(ui.displayWidget->ACTION_ROTATION_Y);
}

void VisAtom2017::OnSelectRotationZ()
{
	    this->ui.actionZoom_view->setChecked(false);
		this->ui.actionRotate_X->setChecked(false);
		this->ui.actionRotate_Y->setChecked(false);
		this->ui.actionRotate_Z->setChecked(true);
		this->ui.actionViewport_center->setChecked(false);


		emit this->SetMouseAction(ui.displayWidget->ACTION_ROTATION_Z);
}

void VisAtom2017::OnSelectZoomScene()
{
	    this->ui.actionZoom_view->setChecked(true);
		this->ui.actionRotate_X->setChecked(false);
		this->ui.actionRotate_Y->setChecked(false);
		this->ui.actionRotate_Z->setChecked(false);
		this->ui.actionViewport_center->setChecked(false);

		emit this->SetMouseAction(ui.displayWidget->ACTION_ZOOM_SCENE);
}

void VisAtom2017::OnActionVeiwportCenter(bool checked)
{
	this->ui.actionZoom_view->setChecked(false);
	this->ui.actionRotate_X->setChecked(false);
	this->ui.actionRotate_Y->setChecked(false);
	this->ui.actionRotate_Z->setChecked(false);
	this->ui.actionViewport_center->setChecked(true);

	emit this->SetMouseAction(ui.displayWidget->ACTION_VIEWPORT_CENTER);
}

void VisAtom2017::OnChangeSceneDistance()
{ 
	   int range = ui.distanceSlider->maximum() - ui.distanceSlider->minimum()+1;
	   emit this->SetSceneDistance(range,ui.distanceSlider->value());
}

void VisAtom2017::OnChangeAtomRadiu()
{
   if(m_OnUpdateSubsampleUI) return;
	this->OnMultipleSelectSubsample();

	int range = ui.atomsizeSlider->maximum() - ui.atomsizeSlider->minimum()+1;
	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	emit this->SetAtomRadiu(range,ui.atomsizeSlider->value(), all);

}

int VisAtom2017::OnEditSizeDataButton()
{
    if(m_OnUpdateSubsampleUI) return 0;
    this->OnMultipleSelectSubsample();


     VisAtom_NewSizePolicyDlg sizeDlg(this);
	 sizeDlg.SetDataSheet(this->m_SampleDataSheet);

	 if(sizeDlg.exec() == QDialog::Accepted)
	 {
	    int col;
	    float min, max;
	    sizeDlg.GetData(col, min, max);
		//if data not changed, just return
		if(col == VisAtomVisualizingPolicy::GetAtomSizeCol() ) return 1; 

		VisAtomVisualizingPolicy::SetAtomSizeCol(col);
		bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
		emit this->SetVariableAtomRadiu(true, all);
		return 1;
	 }

	return 0;
}

void VisAtom2017::OnVariableAtomSize(bool checked)
{
   if(m_OnUpdateSubsampleUI) return;
    this->OnMultipleSelectSubsample();

	VisAtomSubSample*sub=ui.displayWidget->GetSample()->GetCurrentSubsample();
	if(checked) 
	{    //if alread in vairaiable size policy, return;
		 if(!sub->IsCurAtomHaveUnifiedSize())return;
		 //if no size data available, select the data
		 if(VisAtomVisualizingPolicy::GetAtomSizeCol() < 0)
		 {
            QMessageBox msgBox(this);
		    msgBox.setIcon(QMessageBox::Question);
		    msgBox.setText(tr("No scalar data available for variable radia, set the scalar data?"));
		    msgBox.addButton(QMessageBox::Yes);
		    msgBox.addButton(QMessageBox::No);
		    if(msgBox.exec() == QMessageBox::No)
            {
				ui.atomsizeSlider->setEnabled(true);
				ui.atomunitsizeRadioButton->setChecked(true);
				ui.atomvairablesizeRadioButton->setChecked(false);
				ui.atomsizedataButton->setEnabled(false);
				return;
			}

            VisAtom_NewSizePolicyDlg sizeDlg(this);
	        sizeDlg.SetDataSheet(this->m_SampleDataSheet);
	        if(sizeDlg.exec() == QDialog::Accepted)
	        {
	           int col;
	           float min, max;
	           sizeDlg.GetData(col, min, max);
	           VisAtomVisualizingPolicy::SetAtomSizeCol(col);
	         }
			else
			{
		        ui.atomsizeSlider->setEnabled(true);
		        ui.atomunitsizeRadioButton->setChecked(true);
	            ui.atomvairablesizeRadioButton->setChecked(false);
		        ui.atomsizedataButton->setEnabled(false);
				return;
			}
		 }

	     ui.atomsizeSlider->setEnabled(false);
		 ui.atomunitsizeRadioButton->setChecked(false);
		 ui.atomsizedataButton->setEnabled(true);
	     bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	     emit this->SetVariableAtomRadiu(true, all);
	}

}
void VisAtom2017::OnUnifiedAtomSize(bool checked)
{
	VisAtomSubSample*sub=ui.displayWidget->GetSample()->GetCurrentSubsample();
	if(checked && sub->IsCurAtomHaveUnifiedSize()) return;

	ui.atomsizeSlider->setEnabled(true);
    ui.atomsizedataButton->setEnabled(false);

	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	emit this->SetVariableAtomRadiu(false, all);
}


void VisAtom2017::OnChangeAtomShapeDot(bool checked)
{
	if(!checked) return;
    if(m_OnUpdateSubsampleUI) return;

	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	this->OnMultipleSelectSubsample();
	emit this->SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_DOT, all);
}
void VisAtom2017::OnChangeAtomShapeCircle(bool checked)
{
	if(!checked) return;
    if(m_OnUpdateSubsampleUI) return;


	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
    this->OnMultipleSelectSubsample();
	emit this->SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_LINE, all);
}
void VisAtom2017::OnChangeAtomShapeWireSphere(bool checked)
{
	if(!checked) return;
    if(m_OnUpdateSubsampleUI) return;

	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	this->OnMultipleSelectSubsample();
	emit SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_WIRE, all);
}
void VisAtom2017::OnChangeAtomShapeSolidSphere(bool checked)
{
	if(!checked) return;
    if(m_OnUpdateSubsampleUI) return;

	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	this->OnMultipleSelectSubsample();
	emit SetAtomShape(VisAtomVisualizingPolicy::SHAPE_STYLE_SOLID, all);
}

//**************************************************************************************************
void VisAtom2017::OnChangeGraphQuality(int nfai)
{
    if(m_OnUpdateSubsampleUI) return;
	this->OnMultipleSelectSubsample();
	emit ChangeGraphQuality(nfai);
	return;
}


void VisAtom2017::OnChangeAtomColor()
{
	 int i;

	 VisAtomSample*sample = this->ui.displayWidget->GetSample();
	 VisAtomSubSample*subsample;
	 VisAtomVisualizingPolicy*policy;

	 subsample = sample->GetCurrentSubsample();
	 policy    = subsample->GetCurVisualPolicy(); 

	//Update color UI
	float r, g, b, a;
	#define RGBSIZE 255
	policy->GetColor(r, g, b,a );
	QColor color((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
    QColorDialog colorsel(color, this);

	 if(colorsel.exec())
	  {
		  int R, G, B, A;
		  color = colorsel.currentColor(); 
	      color.getRgb(&R, &G, &B, &A);
		  this->OnMultipleSelectSubsample();
	      
		  /*VisAtomSubSample*p = sample->GetCurrentSubsample(); 
		  p->SetColor(R/float(RGBSIZE), G/float(RGBSIZE), B/float(RGBSIZE), 1.f, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		  p->SetColor(R/float(RGBSIZE), G/float(RGBSIZE), B/float(RGBSIZE), 1.f, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
		  sample->LoadCurSubsampleProp();*/
     	  for(int i=0; i<sample->NumberofSubsample(); i++)
	      {
		    if(sample->IsSelectedSubsample(i))
		    {
	         VisAtomSubSample*p = sample->GetSubsample(i);
   		     p->SetColor(R/float(RGBSIZE), G/float(RGBSIZE), B/float(RGBSIZE), 1.f, VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		     p->SetColor(R/float(RGBSIZE), G/float(RGBSIZE), B/float(RGBSIZE), 1.f, VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
             sample->LoadSubsampleProp(i);
		    }
		  }

		  QPixmap colicon(QSize(32,16));
	      colicon.fill(color);
	      ui.changeatomColorButton->setIcon(colicon);
		  ui.displayWidget->updateGL();
	   }
}

void VisAtom2017::OnSelectSceneStyle(QListWidgetItem*current,QListWidgetItem*previous)
{
	if(!current)
		current = previous;

	    int row = this->ui.scenestylelistWidget->row(current);

		this->ui.scenestylestackedWidget->setCurrentIndex(row);
		this->ui.displayWidget->SetCurrentClipperStyle(row, false);
  	    this->OnClipEnabled();
		return;
}


void VisAtom2017:: OnClipEnabled()
{
	if (this->m_OnUpdateClipParamUI) return;
	VisAtomCliper clip; 
	clip.SetParameters(ui.displayWidget->CurrentClipper());
	this->GetClipParamFromUI(&clip);
	this->ui.displayWidget->SetCurrentCliperParameters(&clip);
	return;
}

void VisAtom2017::OnNewClipDirPushButton()
{
	VisAtomCliper*clip0 = this->ui.displayWidget->CurrentClipper();
	VisAtomCliper*clip = this->ui.displayWidget->NewCliper();
	if (!clip) return;
	clip->SetParameters(clip0);

	int i = ui.clipDirComboBox->count();
	QString sname, sindex;
	sindex.setNum(i+1);
	sname.clear();
	sname.append(tr("Plane #"));
	sname.append(sindex);
	ui.clipDirComboBox->addItem(sname);
	ui.clipDirComboBox->setCurrentIndex(i);
}
void VisAtom2017::OnClipDirComboBoxIndexChanged(int index)
{
	if (this->m_OnUpdateClipParamUI) return;

	VisAtomCliper*clip = ui.displayWidget->SetCurrentClipper(index);
	
	this->m_OnUpdateClipParamUI = 1;
	double axsis[3], pos[3];
	clip->GetClipAxsis(axsis);
	this->ui.clipDirXSpinBox->setValue(axsis[0]);
	this->ui.clipDirYSpinBox->setValue(axsis[1]);
	this->ui.clipDirZSpinBox->setValue(axsis[2]);
	
	this->ui.clipAnyDirEnablecheckBox->setChecked(clip->IsEnabledClip());
	this->ui.scenestyleDClipDCheckBox->setChecked(clip->IsEnabledDClip());

	clip->GetClipPos(pos);
	this->ui.clipPosXSpinBox->setValue(pos[0]); 
	this->ui.clipPosYSpinBox->setValue(pos[1]); 
	this->ui.clipPosZSpinBox->setValue(pos[2]); 

	double thick;
	thick = clip->GetClipThickness();
	this->ui.clipstepDSpinBox->setValue(thick);
	
	this->ui.clipDFrontfaceShowCheckBox->setChecked(clip->IsShowFronFace());
	this->ui.clipDBackfaceShowCheckBox->setChecked(clip->IsShowBackFace());

	double opacity = clip->GetFaceOpacity()*(double)(this->ui.clipOpacitySlider->maximum() - this->ui.clipOpacitySlider->minimum() + 1);
	this->ui.clipOpacitySlider->setValue((int)opacity);

	this->m_OnUpdateClipParamUI = 0;

}

void VisAtom2017::OnClipDirCheckBoxChanged(bool enable)
{
	VisAtomCliper*clip = ui.displayWidget->CurrentClipper();
	clip->EnableClip(enable);
	this->ui.displayWidget->SetCurrentCliperParameters(clip);
}

void VisAtom2017::GetClipParamFromUI(VisAtomCliper*clip)
{
	int proppage =  this->ui.scenestylestackedWidget->currentIndex();

	if(proppage == 0)
	{
	 clip->SetStyle(this->ui.scenestylelistWidget->currentRow());
	 clip->EnableClipX(this->ui.scenestyleClipXGroupBox->isChecked());
     clip->EnableClipY(this->ui.scenestyleClipYGroupBox->isChecked());
     clip->EnableClipZ(this->ui.scenestyleClipZGroupBox->isChecked());

	 clip->EnableDClipX(this->ui.scenestyleDClipXCheckBox->isChecked());
	 clip->EnableDClipY(this->ui.scenestyleDClipYCheckBox->isChecked());
	 clip->EnableDClipZ(this->ui.scenestyleDClipZCheckBox->isChecked());

	 clip->SetInverseClipX(this->ui.scenestyleclipInvXCheckBox->isChecked());
	 clip->SetInverseClipY(this->ui.scenestyleclipInvYCheckBox->isChecked());
	 clip->SetInverseClipZ(this->ui.scenestyleclipInvZCheckBox->isChecked());

	 double step[3];
	 step[0] = (double)this->ui.clipstepXSpinBox->value(); ///(double)this->ui.clipstepXSpinBox->maximum();
	 step[1] = (double)this->ui.clipstepYSpinBox->value(); ///(double)this->ui.clipstepYSpinBox->maximum();
	 step[2] = (double)this->ui.clipstepZSpinBox->value(); ///(double)this->ui.clipstepZSpinBox->maximum();
	 clip->SetClipStep(step);
	}

	else if(proppage == 1)
	{
	 clip->SetStyle(this->ui.scenestylelistWidget->currentRow());
	 clip->EnableClipX(this->ui.scenestyleClipXGroupBox1->isChecked());
     clip->EnableClipY(this->ui.scenestyleClipYGroupBox1->isChecked());
     clip->EnableClipZ(this->ui.scenestyleClipZGroupBox1->isChecked());

	 clip->EnableDClipX(this->ui.scenestyleDClipXCheckBox1->isChecked());
	 clip->EnableDClipY(this->ui.scenestyleDClipYCheckBox1->isChecked());
	 clip->EnableDClipZ(this->ui.scenestyleDClipZCheckBox1->isChecked());

	 clip->SetInverseClipX(this->ui.scenestyleclipInvXCheckBox1->isChecked());
	 clip->SetInverseClipY(this->ui.scenestyleclipInvYCheckBox1->isChecked());
	 clip->SetInverseClipZ(this->ui.scenestyleclipInvZCheckBox1->isChecked());

	 double step[3];
	 step[0] = (double)this->ui.clipstepXSpinBox1->value(); ///(double)this->ui.clipstepXSpinBox1->maximum();
	 step[1] = (double)this->ui.clipstepYSpinBox1->value(); ///(double)this->ui.clipstepYSpinBox1->maximum();
	 step[2] = (double)this->ui.clipstepZSpinBox1->value(); ///(double)this->ui.clipstepZSpinBox1->maximum();
	 clip->SetClipStep(step);
	}

	else if(proppage == 2)
	{
	 clip->SetStyle(this->ui.scenestylelistWidget->currentRow());

	 double axsis[3], pos[3];
	 axsis[0] =  (double)this->ui.clipDirXSpinBox->value();
	 axsis[1] =  (double)this->ui.clipDirYSpinBox->value();
	 axsis[2] =  (double)this->ui.clipDirZSpinBox->value();
	 clip->SetClipAxsis(axsis);
	 clip->EnableClip(this->ui.clipAnyDirEnablecheckBox->isChecked());
	 clip->EnableDClip(this->ui.scenestyleDClipDCheckBox->isChecked());

	 pos[0] = (double)this->ui.clipPosXSpinBox->value(); /*/
		      (double)(this->ui.clipPosXSpinBox->maximum() - this->ui.clipPosXSpinBox->minimum());*/
	 pos[1] = (double)this->ui.clipPosYSpinBox->value(); /*/
		      (double)(this->ui.clipPosYSpinBox->maximum() - this->ui.clipPosYSpinBox->minimum());*/
	 pos[2] = (double)this->ui.clipPosZSpinBox->value(); /*/
		      (double)(this->ui.clipPosZSpinBox->maximum() - this->ui.clipPosZSpinBox->minimum());*/

     clip->SetClipPos(pos);

	 double step[3];
	 step[0] = step[1] = step[2] =this->ui.clipstepDSpinBox->value();
		// (double)(this->ui.clipstepDSpinBox->value())/(double)this->ui.clipstepXSpinBox1->maximum();
	 clip->SetClipStep(step);

	 double opacity;
	 opacity = (double)this->ui.clipOpacitySlider->value()/
		       (double)(this->ui.clipOpacitySlider->maximum() - this->ui.clipOpacitySlider->minimum()+1);
	 clip->SetFaceOpacity(opacity);
	 
	}
}

//********************************************************************************
void VisAtom2017::UpdateSubsampleUI()
{
	//on update UI, we do not need to reddraw the scene
	m_OnUpdateSubsampleUI = 1;

	//to update ui
	VisAtomSample*sample = this->ui.displayWidget->GetSample();
	VisAtomSubSample*subsample;
	VisAtomVisualizingPolicy*policy;

	subsample = sample->GetCurrentSubsample();
	policy    = subsample->GetCurVisualPolicy(); 

	//atom UI
	int geomstyle = policy->GetAtomShapeStyle();
	switch(geomstyle&VisAtomVisualizingPolicy::SHAPE_STYLE_MASK)
	{
	   case VisAtomVisualizingPolicy::SHAPE_STYLE_DOT:
			this->ui.dotRadioButton->setChecked(true);
			break;
	   case VisAtomVisualizingPolicy::SHAPE_STYLE_LINE:
			this->ui.circleRadioButton->setChecked(true);
			break;
	   case VisAtomVisualizingPolicy::SHAPE_STYLE_WIRE:
			this->ui.wireRadioButton->setChecked(true);
			break;
	   case VisAtomVisualizingPolicy::SHAPE_STYLE_SOLID:
			this->ui.sphereRadioButton->setChecked(true);
			break;
	}
	this->ui.shapeoptionGroupBox->setChecked(policy->IsGeomStyleAtomVisible());

    //vector style
    geomstyle = policy->GetVectorStyle();
	switch(geomstyle&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK)
	{
	   case VisAtomVisualizingPolicy::VECTOR_STYLE_LINE:
			this->ui.vectorlineRadioButton->setChecked(true);
			break;
	   case VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID:
			this->ui.vectorcylinderRadioButton->setChecked(true);
			break;
	}
	this->ui.vectoroptionGroupBox->setChecked(policy->IsGeomStyleVectorVisible());

    //trajectory style
    geomstyle = policy->GetTrajectoryStyle();
	this->ui.trajectoryOptionGroupBox->setChecked(policy->IsGeomStyleTrajectoryVisible());
	this->ui.trjaectoryThickSpinBox->setValue(policy->GetTrajectoryLinethick());
	//

	//Update size UI
    float x0, x1, y0, y1, z0, z1,ds;
	sample->GetSampleBoxRangeX(x0, x1);
	sample->GetSampleBoxRangeX(y0, y1);
	sample->GetSampleBoxRangeX(z0, z1);

	ds = x1 - x0;
    if(ds > y1-y0) ds = y1 - y0;
    if(ds > z1-z0) ds = z1 - z0;
	ds = (ds/4.0); 

	float rad = policy->GetAtomSize();
    double ratio = rad/ds;
	double range = ui.atomsizeSlider->maximum() - ui.atomsizeSlider->minimum() + 1;
	ui.atomsizeSlider->setValue((int)(ratio*range));

	if(policy->IsAtomHaveUnifiedSize())
	{
		ui.atomunitsizeRadioButton->setChecked(true);
		ui.atomsizeSlider->setEnabled(true);
		ui.atomsizedataButton->setEnabled(false);
	}
	else
	{
		ui.atomvairablesizeRadioButton->setChecked(true);
		ui.atomsizedataButton->setEnabled(true);
		ui.atomsizeSlider->setEnabled(false);
	}

	//Update color UI
	if(policy->GetColorStyle() == VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)
	{
	   float r, g, b, a;
	   #define RGBSIZE 255
	   policy->GetColor(r, g, b,a );
	   QColor color((int)(r*RGBSIZE), (int)(g*RGBSIZE), (int)(b*RGBSIZE));
	   QPixmap colicon(QSize(32,16));
	   colicon.fill(color);
	   ui.changeatomColorButton->setIcon(colicon);
	   ui.changeatomColorButton->setEnabled(true);
	   ui.singlecolorRadioButton->setChecked(true);
	   ui.colorstyleStackedWidget->setCurrentIndex(0);
	}
	else if(policy->GetColorStyle() == VisAtomVisualizingPolicy::COLOR_STYLE_MAP)
	{
		ui.mappingcolorRadioButton->setChecked(true);
	    ui.colorstyleStackedWidget->setCurrentIndex(1);
	}

	//
	QString title(subsample->GetName());
	title.append(tr(" options"));
	ui.displayoptionDockWidget->setWindowTitle(title);

	//
	m_OnUpdateSubsampleUI = 0;

	return;
}

//********************************************************************************
void VisAtom2017::UpdateSubsampleLIST()
{
		 //Update the subsample list
		 this->ui.subsampleListTreeWidget->clear();

		 QString sname, sindex;
		 int i,gstyle;
		 QTreeWidgetItem*item0 = NULL;
		 for (i = 0; i < this->ui.displayWidget->GetSample()->NumberofSubsample(); i++)
		 {
			 VisAtomSubSample*subp = this->ui.displayWidget->GetSample()->GetSubsample(i);
			 sname = subp->GetName();
			 if (sname.isEmpty())
			 {
				 sindex.setNum(i + 1);
				 sname.clear();
				 sname.append(tr("Subsample"));
				 sname.append(sindex);
				 subp->SetName(sname);
			 }
			 QTreeWidgetItem*item = new QTreeWidgetItem(); if (!item0) item0 = item;
			 item->setText(0, sname);
			 item->setText(1, tr("Visible"));
			 if (subp->IsVisible())
				 item->setCheckState(1, Qt::Checked);
			 else
				 item->setCheckState(1, Qt::Unchecked);
			 //fill group information
			 QTreeWidgetItem*typeitem = new QTreeWidgetItem();
			 typeitem->setText(0, tr("Atom type"));
			 int j;
			 sname.clear();
			 sname.append(tr("type"));
			 for (j = 0; j < subp->GetNumberofGroup(); j++)
			 {
				 //QTreeWidgetItem*aTypeitem = new QTreeWidgetItem();
				 sindex.setNum(subp->GetGroupID(j));
				 sname.append(tr(" #"));
				 sname.append(sindex);
				 //aTypeitem->setText(0, sname);
				 //typeitem->addChild(aTypeitem);
				 //typeitem->setText(0, tr("Atom"));
			 }
			 typeitem->setText(1, sname);

			 item->addChild(typeitem);

              //fill number of atom
			  QTreeWidgetItem*numatomitem =  new QTreeWidgetItem();
			  numatomitem->setText(0, QString(tr("Number of atoms")));
			  int num = subp->GetNumberofAtoms();
			  QString snum;
			  snum.setNum(num);
			  numatomitem->setText(1, snum);  
			  item->addChild(numatomitem);

			  //fill 3D diaplay information
			  QTreeWidgetItem*D3item = new QTreeWidgetItem();
			  D3item->setText(0, tr("3D"));
			  if (subp->GetCurVisualPolicy()->IsGeomStyle3DVisible())
			  {
				  D3item->setCheckState(1, Qt::Checked);
			  }
			  else
			  {
				  D3item->setCheckState(1, Qt::Unchecked);
			  }
			  item->addChild(D3item);

			  /*if (subp->GetNumberofRegions() <= 0)
			  {
			  regionitem->setCheckState(1, Qt::Checked);
			  regionitem->setText(1, tr("Visible for all"));
			  }
			  else
			  for(int j=0; j<subp->GetNumberofRegions(); j++)
			  {
			  }*/

			  //fill scalar range information
			  QTreeWidgetItem*datarangeitem = new QTreeWidgetItem();
			  datarangeitem->setText(0, tr("Data range"));
			  if (subp->GetNumberofScalarRegions() <= 0)
				  datarangeitem->setText(1, tr("No constraint"));
			  /*else
			  for(int j=0; j<subp->GetNumberofScalarRegions(); j++)
			  {
			  }*/

			  //item->addChild(datarangeitem);


			  //fill projection visibility
              QTreeWidgetItem*projectionitem =  new QTreeWidgetItem();
			  projectionitem->setText(0, QString(tr("Projection")));
			  if(subp->GetCurVisualPolicy()->IsGeomStyleProjectionVisible())
			     projectionitem->setCheckState(1,Qt::Checked);
			  else
			     projectionitem->setCheckState(1,Qt::Unchecked);

			  gstyle = subp->GetCurVisualPolicy()->GetProjectionStyle();
			  item->addChild(projectionitem);

              QTreeWidgetItem*pX0item =  new QTreeWidgetItem();
			  pX0item->setText(0, QString(tr("X0-face")));
			  pX0item->setText(1, QString(tr("Visible")));
			  if(gstyle&VisAtomVisualizingPolicy::PROJECTION_STYLE_X0)
                 pX0item->setCheckState(1,Qt::Checked);
			  else
                 pX0item->setCheckState(1,Qt::Unchecked);
			  projectionitem->addChild(pX0item);

              QTreeWidgetItem*pX1item =  new QTreeWidgetItem();
			  pX1item->setText(0, QString(tr("X1-face")));
			  pX1item->setText(1, QString(tr("Visible")));
			  if(gstyle&VisAtomVisualizingPolicy::PROJECTION_STYLE_X1)
                 pX1item->setCheckState(1,Qt::Checked);
			  else
                 pX1item->setCheckState(1,Qt::Unchecked);
			  projectionitem->addChild(pX1item);

              QTreeWidgetItem*pY0item =  new QTreeWidgetItem();
			  pY0item->setText(0, QString(tr("Y0-face")));
			  pY0item->setText(1, QString(tr("Visible")));
			  if(gstyle&VisAtomVisualizingPolicy::PROJECTION_STYLE_Y0)
                 pY0item->setCheckState(1,Qt::Checked);
			  else 
                 pY0item->setCheckState(1,Qt::Unchecked);
			  projectionitem->addChild(pY0item);

              QTreeWidgetItem*pY1item =  new QTreeWidgetItem();
			  pY1item->setText(0, QString(tr("Y1-face")));
			  pY1item->setText(1, QString(tr("Visible")));
			  if(gstyle&VisAtomVisualizingPolicy::PROJECTION_STYLE_Y1)
                 pY1item->setCheckState(1,Qt::Checked);
			  else
                 pY1item->setCheckState(1,Qt::Unchecked);
			  projectionitem->addChild(pY1item);

              QTreeWidgetItem*pZ0item =  new QTreeWidgetItem();
			  pZ0item->setText(0, QString(tr("Z0-face")));
			  pZ0item->setText(1, QString(tr("Visible")));
			  if(gstyle&VisAtomVisualizingPolicy::PROJECTION_STYLE_Z0)
                 pZ0item->setCheckState(1,Qt::Checked);
			  else
                 pZ0item->setCheckState(1,Qt::Unchecked);
			  projectionitem->addChild(pZ0item);

              QTreeWidgetItem*pZ1item =  new QTreeWidgetItem();
			  pZ1item->setText(0, QString(tr("Z1-face")));
			  pZ1item->setText(1, QString(tr("Visible")));
			  if(gstyle&VisAtomVisualizingPolicy::PROJECTION_STYLE_Z1)
                 pZ1item->setCheckState(1,Qt::Checked);
			  else
                 pZ1item->setCheckState(1,Qt::Unchecked);
			  projectionitem->addChild(pZ1item);


	  		  this->ui.subsampleListTreeWidget->addTopLevelItem(item);
		 }
		 this->ui.subsampleListTreeWidget->expandToDepth(0);
		 this->ui.subsampleListTreeWidget->setCurrentItem(item0);
		 //end of updating subsample list
         return;
}

//********************************************************************************
void VisAtom2017::OnSubsampleListItemChanged(QTreeWidgetItem*item)
{
	if(!item) return; 

/*	QString name;

	if(!item->parent())
	{
       name =  item->text(0);
	   
       VisAtomSample*sample = this->ui.displayWidget->GetSample();
	   VisAtomSubSample*subsample = sample->GetSubsample(name);
       subsample->SetVisible(checked);
	   sample->ResetVisibility();
	   ui.displayWidget->updateGL();
   	}*/

		 QString tname1, tname2;
		 VisAtomSample*sample = this->ui.displayWidget->GetSample();
		 QTreeWidgetItem*item0 = NULL;
		 QTreeWidgetItem*item1 = NULL;
		 QTreeWidgetItem*item2 = NULL;

		 int i, j, k; 
		 bool checked, checked1, checked2;
		 for(i=0; i<sample->NumberofSubsample(); i++)
		 {
			 VisAtomSubSample*subp = sample->GetSubsample(i);
			 item0 = this->ui.subsampleListTreeWidget->topLevelItem(i);
			 checked = item0->checkState(1);
			 subp->SetVisible(checked);

			 //check the projection item
			 for(j=0; j<item0->childCount(); j++)
			 {
				 item1  = item0->child(j);
				 tname1 = item1->text(0);
				 //check projection state
				 if(tname1 == QString(tr("Projection")))
				 {
                  checked1 = item1->checkState(1);
                  subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyleProjectionVisible(checked1);
		          subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyleProjectionVisible(checked1);	
				  int pstyle =0;
					 for(k=0; k<item1->childCount(); k++)
					 {
                         item2  = item1->child(k);
                         tname2 = item2->text(0);
						 checked2 = item2->checkState(1);
						 if(tname2 == QString(tr("X0-face")) )
							 if(checked2) pstyle = pstyle|VisAtomVisualizingPolicy::PROJECTION_STYLE_X0; 
						 if(tname2 == QString(tr("X1-face")) )
							 if(checked2) pstyle = pstyle|VisAtomVisualizingPolicy::PROJECTION_STYLE_X1; 
						 if(tname2 == QString(tr("Y0-face")) ) 
							 if(checked2) pstyle = pstyle|VisAtomVisualizingPolicy::PROJECTION_STYLE_Y0; 
						 if(tname2 == QString(tr("Y1-face")) )
							 if(checked2) pstyle = pstyle|VisAtomVisualizingPolicy::PROJECTION_STYLE_Y1; 
						 if(tname2 == QString(tr("Z0-face")) )
							 if(checked2) pstyle = pstyle|VisAtomVisualizingPolicy::PROJECTION_STYLE_Z0; 
						 if(tname2 == QString(tr("Z1-face")) )
							 if(checked2) pstyle = pstyle|VisAtomVisualizingPolicy::PROJECTION_STYLE_Z1; 
					 }
				  subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetProjectionStyle(pstyle);
		          subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetProjectionStyle(pstyle);
 				  sample->LoadSubsampleProp(i);
				 }
				 else if(tname1 == QString(tr("3D")) )
				 {
				  checked1 = item1->checkState(1);
				  subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->SetGeomStyle3DVisible(checked1);
				  subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->SetGeomStyle3DVisible(checked1);

				  int pstyle = 0;
				  pstyle = pstyle | VisAtomVisualizingPolicy::D3_STYLE_INBOX;
				  pstyle = pstyle | VisAtomVisualizingPolicy::D3_STYLE_OUTBOX;
				  //subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)->Set3DStyle(pstyle);
				  //subp->GetVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP)->Set3DStyle(pstyle);
				  sample->LoadSubsampleProp(i);
				 }
			 }
		 }
	     sample->ResetVisibility();
	     this->ui.displayWidget->updateGL();
         return;

	return;
}


//********************************************************************************
void VisAtom2017::OnSubsampleListTreeItemDBClick(QTreeWidgetItem*item, int ind)
{
	if (!item) return;

	QString tname1, tname2;
	VisAtomSample*sample = this->ui.displayWidget->GetSample();
	QTreeWidgetItem*item0 = NULL;
	QTreeWidgetItem*item1 = NULL;
	QTreeWidgetItem*item2 = NULL;

	int i, j, k;
	bool checked, checked1, checked2;
	//to check which item has been double clicked
	for (i = 0; i<sample->NumberofSubsample(); i++)
	{
		VisAtomSubSample*subp = sample->GetSubsample(i);
		item0 = this->ui.subsampleListTreeWidget->topLevelItem(i);
		checked = item0->checkState(1);
		subp->SetVisible(checked);

		//check the projection item
		if(checked)
		{
		   for (j = 0; j < item0->childCount(); j++)
		   {
			item1 = item0->child(j);
			tname1 = item1->text(0);
			//check projection state
			  if(tname1 == QString(tr("Region")) && item1==item)
			  {

				  int num = subp->GetNumberofRegions();
				  QString sname, sindex;
				  QTreeWidgetItem*regionitem = new QTreeWidgetItem;
				  item1->addChild(regionitem);
				  sindex.setNum(num + 1);
				  sname.append(tr("#"));
				  sname.append(sindex);
				  regionitem->setText(0,sname);
			  }

		   }
		}
	}
	sample->ResetVisibility();
	this->ui.displayWidget->updateGL();

	return;
}
//********************************************************************************
void VisAtom2017::OnSelectSubsample(QTreeWidgetItem*item)
{
	if(!item) return; 

	QString subname, curname;
	QTreeWidgetItem*present= item;
	QTreeWidgetItem*parent = present->parent();

	  while(parent)
	 { 
       present = parent;
	   parent  = present->parent();
	  }
  	
	  subname = present->text(0);
	  curname = subname;
	  //to update ui
	  VisAtomSample*sample = this->ui.displayWidget->GetSample();
	  sample->SetCurrentSubsample(subname);

	  subname = "Ed:";
	  subname.append(curname);
	  this->m_subsmaplelabel.setText(subname);
	  this->UpdateSubsampleUI();

	return;
}

//********************************************************************************
void VisAtom2017::OnMultipleSelectSubsample()
{
	  //if multiple selection enabled we need to update the list of selected subsample
  	  QString subname;
	  QTreeWidgetItem*present;
	  VisAtomSample*sample = this->ui.displayWidget->GetSample();
	  int ns = sample->NumberofSubsample();
	  int i;
	  for(i=0; i<ns; i++)
	  {
		  present = this->ui.subsampleListTreeWidget->topLevelItem (i);
  	      subname = present->text(0);
		  if(present->isSelected()) 
			 sample->SetSelSubsample(subname);
		  else
			 sample->SetUnselSubsample(subname);
	  }
      return;
}

//********************************************************************************
void VisAtom2017::OnSingleColorRadioButton(bool checked)
{
	if(checked)
	{
		ui.colorstyleStackedWidget->setCurrentIndex(0);
        VisAtomSample*sample = this->ui.displayWidget->GetSample();
		VisAtomSubSample*subsample = sample->GetCurrentSubsample();
		subsample->SetCurVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
		sample->LoadCurSubsampleProp();
		ui.displayWidget->updateGL();
		this->UpdateSubsampleUI();
	}
}

//********************************************************************************
void VisAtom2017::OnMappingColorRadioButton(bool checked)
{
	if(checked)
	{
		ui.colorstyleStackedWidget->setCurrentIndex(1);
		if(VisAtomVisualizingPolicy::GetScalarCol() < 0)
		{
           QMessageBox msgBox(this);
		   msgBox.setIcon(QMessageBox::Question);
		   msgBox.setText(tr("No scalar data available, set the scalar data?"));
		   msgBox.addButton(QMessageBox::Yes);
		   msgBox.addButton(QMessageBox::No);
		   if(msgBox.exec() == QMessageBox::Yes)
		   {
	          VisAtom_NewColorPolicyDlg newcolorDlg(this);

	          newcolorDlg.SetDataSheet(this->m_SampleDataSheet);
	          newcolorDlg.SetSample(ui.displayWidget->GetSample());
	          if(newcolorDlg.exec() == QDialog::Accepted)
	          {
		         int col;
		         float dmin, dmax;
		         newcolorDlg.GetData(col, dmin, dmax);
				 if(col < 0)
				 {
					 QMessageBox msgBox(this);
					 msgBox.setIcon(QMessageBox::Warning);
                     msgBox.setText(tr("No scalar data selected."));
					 msgBox.addButton(QMessageBox::Ok);
					 msgBox.exec();
					 ui.singlecolorRadioButton->setChecked(true);
					 return;
				 }
		         VisAtomVisualizingPolicy::SetScalarCol(col);
		         VisAtomVisualizingPolicy::SetScalarRange(dmin, dmax);
		         VisAtomVisualizingPolicy::SetScalarMappingTab(dmin, dmax);
	             QString snum;
	             snum.setNum(dmin, 'e', 2);
	             ui.scalarminLabel->setText(snum);
	             snum.setNum(dmax, 'e', 2);
	             ui.scalarmaxLabel->setText(snum);

				 dmin = -1.5*abs(dmin);
				 dmax = 1.5*abs(dmax);
				 ui.scalarwin0SpinBox->setMinimum(dmin);
				 ui.scalarwin0SpinBox->setMaximum(dmax);
				 ui.scalarwin0SpinBox->setSingleStep((dmax - dmin)/10.f);
				 ui.scalarwin0SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(0));

				 ui.scalarwin1SpinBox->setMinimum(dmin);
				 ui.scalarwin1SpinBox->setMaximum(dmax);
				 ui.scalarwin1SpinBox->setSingleStep((dmax - dmin)/10.f);
				 ui.scalarwin1SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(1));

				 ui.scalarwin2SpinBox->setMinimum(dmin);
				 ui.scalarwin2SpinBox->setMaximum(dmax);
				 ui.scalarwin2SpinBox->setSingleStep((dmax - dmin)/10.f);
				 ui.scalarwin2SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(2));

				 ui.scalarwin3SpinBox->setMinimum(dmin);
				 ui.scalarwin3SpinBox->setMaximum(dmax);
				 ui.scalarwin3SpinBox->setSingleStep((dmax - dmin)/10.f);
				 ui.scalarwin3SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(3));

				 ui.scalarwin4SpinBox->setMinimum(dmin);
				 ui.scalarwin4SpinBox->setMaximum(dmax);
				 ui.scalarwin4SpinBox->setSingleStep((dmax - dmin)/10.f);
				 ui.scalarwin4SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(4));

				 ui.scalarwin5SpinBox->setMinimum(dmin);
				 ui.scalarwin5SpinBox->setMaximum(dmax);
				 ui.scalarwin5SpinBox->setSingleStep((dmax - dmin)/10.f);
				 ui.scalarwin5SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(5));
	          }
			  else
			 {
                 ui.singlecolorRadioButton->setChecked(true);
			     return;
			  }
		    }
           //still having no scalar table
		   if(VisAtomVisualizingPolicy::GetScalarCol() < 0) 
		   {
			   ui.singlecolorRadioButton->setChecked(true);
			   return;
		   }
		}

        VisAtomSample*sample = this->ui.displayWidget->GetSample();
		VisAtomSubSample*subsample = sample->GetCurrentSubsample();
		int style = subsample->GetCurVisualPolicy()->GetGeomStyle();
		style = style&VisAtomVisualizingPolicy::VECTOR_STYLE_MASK;
		style = style|VisAtomVisualizingPolicy::COLOR_STYLE_MAP;
		subsample->SetCurVisualPolicy(VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
		sample->LoadCurSubsampleProp();
        ui.displayWidget->updateGL();
	    this->UpdateSubsampleUI();
	}
}


//********************************************************************************
void VisAtom2017::OnNewColorPolicy()
{
	VisAtom_NewColorPolicyDlg newcolorDlg(this);

	newcolorDlg.SetDataSheet(this->m_SampleDataSheet);
	newcolorDlg.SetSample(ui.displayWidget->GetSample());
	if(newcolorDlg.exec() == QDialog::Accepted)
	{
		int col;
		float dmin, dmax;
		newcolorDlg.GetData(col, dmin, dmax);
		//if(col == VisAtomVisualizingPolicy::GetScalarCol()) return;

		if(col < 0)
		{
		   QMessageBox msgBox(this);
		   msgBox.setIcon(QMessageBox::Warning);
           msgBox.setText(tr("No scalar data selected. Present scalar data to be used."));
		   msgBox.addButton(QMessageBox::Ok);
		   msgBox.exec();
		   return;
		}

		if(dmin == dmax) dmax = dmin+1;
		VisAtomVisualizingPolicy::SetScalarCol(col);
		VisAtomVisualizingPolicy::SetScalarRange(dmin, dmax);
		VisAtomVisualizingPolicy::SetScalarMappingTab(dmin, dmax);

        VisAtomSample*sample = this->ui.displayWidget->GetSample();
		for(int i=0; i<sample->NumberofSubsample(); i++) sample->LoadSubsampleProp(i);

	    QString snum;
	    snum.setNum(dmin, 'e', 4);
	    ui.scalarminLabel->setText(snum);
	    snum.setNum(dmax, 'e', 4);
	    ui.scalarmaxLabel->setText(snum);

        dmin = -1.5*abs(dmin);
		dmax = 1.5*abs(dmax);
		ui.scalarwin0SpinBox->setMinimum(dmin);
		ui.scalarwin0SpinBox->setMaximum(dmax);
		ui.scalarwin0SpinBox->setSingleStep((dmax - dmin)/10.f);
		ui.scalarwin0SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(0));

		ui.scalarwin1SpinBox->setMinimum(dmin);
		ui.scalarwin1SpinBox->setMaximum(dmax);
		ui.scalarwin1SpinBox->setSingleStep((dmax - dmin)/10.f);
		ui.scalarwin1SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(1));

		ui.scalarwin2SpinBox->setMinimum(dmin);
		ui.scalarwin2SpinBox->setMaximum(dmax);
		ui.scalarwin2SpinBox->setSingleStep((dmax - dmin)/10.f);
		ui.scalarwin2SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(2));

		ui.scalarwin3SpinBox->setMinimum(dmin);
		ui.scalarwin3SpinBox->setMaximum(dmax);
		ui.scalarwin3SpinBox->setSingleStep((dmax - dmin)/10.f);
		ui.scalarwin3SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(3));

		ui.scalarwin4SpinBox->setMinimum(dmin);
		ui.scalarwin4SpinBox->setMaximum(dmax);
		ui.scalarwin4SpinBox->setSingleStep((dmax - dmin)/10.f);
		ui.scalarwin4SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(4));

		ui.scalarwin5SpinBox->setMinimum(dmin);
		ui.scalarwin5SpinBox->setMaximum(dmax);
		ui.scalarwin5SpinBox->setSingleStep((dmax - dmin)/10.f);
		ui.scalarwin5SpinBox->setValue(VisAtomVisualizingPolicy::GetScalarMappingTabValue(5));

        ui.displayWidget->updateGL();
	}
}

//********************************************************************************
void VisAtom2017::OnScalarwin0SpinBox(double value)
{

   VisAtomVisualizingPolicy::SetScalarMappingTab(0, value);
   VisAtomSample*sample = this->ui.displayWidget->GetSample();
   for(int i=0; i<sample->NumberofSubsample(); i++) sample->LoadSubsampleProp(i);
   this->ui.displayWidget->updateGL();
   return;
}
//********************************************************************************
void VisAtom2017::OnScalarwin1SpinBox(double value)
{

   VisAtomVisualizingPolicy::SetScalarMappingTab(1, value);
   VisAtomSample*sample = this->ui.displayWidget->GetSample();
   for(int i=0; i<sample->NumberofSubsample(); i++) sample->LoadSubsampleProp(i);
   this->ui.displayWidget->updateGL();
   return;
}

//********************************************************************************
void VisAtom2017::OnScalarwin2SpinBox(double value)
{

   VisAtomVisualizingPolicy::SetScalarMappingTab(2, value);
   VisAtomSample*sample = this->ui.displayWidget->GetSample();
   for(int i=0; i<sample->NumberofSubsample(); i++) sample->LoadSubsampleProp(i);
   this->ui.displayWidget->updateGL();
   return;
}

//********************************************************************************
void VisAtom2017::OnScalarwin3SpinBox(double value)
{

   VisAtomVisualizingPolicy::SetScalarMappingTab(3, value);
   VisAtomSample*sample = this->ui.displayWidget->GetSample();
   for(int i=0; i<sample->NumberofSubsample(); i++) sample->LoadSubsampleProp(i);
   this->ui.displayWidget->updateGL();
   return;
}

//********************************************************************************
void VisAtom2017::OnScalarwin4SpinBox(double value)
{

   VisAtomVisualizingPolicy::SetScalarMappingTab(4, value);
   VisAtomSample*sample = this->ui.displayWidget->GetSample();
   for(int i=0; i<sample->NumberofSubsample(); i++) sample->LoadSubsampleProp(i);
   this->ui.displayWidget->updateGL();
   return;
}

//********************************************************************************
void VisAtom2017::OnScalarwin5SpinBox(double value)
{

   VisAtomVisualizingPolicy::SetScalarMappingTab(5, value);
   VisAtomSample*sample = this->ui.displayWidget->GetSample();
   for(int i=0; i<sample->NumberofSubsample(); i++) sample->LoadSubsampleProp(i);
   this->ui.displayWidget->updateGL();
   return;
}




//********************************************************************************
void VisAtom2017::OnNewVectorPolicy()
{
	VisAtom_NewVectorPolicyDlg newvectorDlg(this);

	newvectorDlg.SetDataSheet(this->m_SampleDataSheet);
	if(newvectorDlg.exec() == QDialog::Accepted)
	{
		int vcol[3];
		newvectorDlg.GetVectorColume(vcol);

		if(vcol[0] == vcol[1])
		{
		   QMessageBox msgBox(this);
		   msgBox.setIcon(QMessageBox::Warning);
           msgBox.setText(tr("Vx and Vy are assigned the same data."));
		   msgBox.addButton(QMessageBox::Ok);
		   msgBox.exec();

		}
		else if(vcol[0] == vcol[2])
		{
		   QMessageBox msgBox(this);
		   msgBox.setIcon(QMessageBox::Warning);
           msgBox.setText(tr("Vx and Vz are assigned the same data."));
		   msgBox.addButton(QMessageBox::Ok);
		   msgBox.exec();

		}

		if(vcol[1] == vcol[2])
		{
		   QMessageBox msgBox(this);
		   msgBox.setIcon(QMessageBox::Warning);
           msgBox.setText(tr("Vy and Vz are assigned the same data."));
		   msgBox.addButton(QMessageBox::Ok);
		   msgBox.exec();

		}
		//if the data has been changed, reset the default vector len scal
		int oldcol[3];
		VisAtomVisualizingPolicy::GetVectorCol(oldcol); 
		//
		if(oldcol[0] != vcol[0] || oldcol[1] != vcol[1] || oldcol[2] != vcol[2])
		{
			QString slabel, snum;
			slabel.append(tr("col. #"));
			snum.setNum(vcol[0]+1);
			slabel.append(snum);
			ui.currenVxdataLabel->setText(slabel);

			slabel.clear();
			slabel.append(tr("col. #"));
			snum.setNum(vcol[1]+1);
			slabel.append(snum);
			ui.currenVydataLabel->setText(slabel);

			slabel.clear();
			slabel.append(tr("col. #"));
			snum.setNum(vcol[2]+1);
			slabel.append(snum);
			ui.currenVzdataLabel->setText(slabel);
           //
		   VisAtomVisualizingPolicy::SetVectorCol(vcol); 
		   float vlenscal, vrad, arrowl, arrowr;
	       ui.displayWidget->GetSample()->GetDefaultVectorSize(vlenscal,vrad, arrowl, arrowr);
		   VisAtomVisualizingPolicy::SetVectorSize(vlenscal,vrad, arrowl, arrowr);
		   this->OnVectorOptionGroupBox(ui.vectoroptionGroupBox->isChecked());
	     }

	}
	//ui.vectoroptionGroupBox->setEnabled(VisAtomVisualizingPolicy::HasVectorData());

    return;
}

//********************************************************************************
void VisAtom2017::OnVectorOptionGroupBox(bool checked)
{
    if(m_OnUpdateSubsampleUI) return;

    if(checked)
	{
       if(!VisAtomVisualizingPolicy::HasVectorData())
	   {
           QMessageBox msgBox(this);
		   msgBox.setIcon(QMessageBox::Question);
		   msgBox.setText(tr("No vector data available, set the vector data?"));
		   msgBox.addButton(QMessageBox::Yes);
		   msgBox.addButton(QMessageBox::No);
		   if(msgBox.exec() == QMessageBox::Yes)
		   {
			   this->OnNewVectorPolicy();
               if(!VisAtomVisualizingPolicy::HasVectorData())
			   {
                 ui.vectoroptionGroupBox->setChecked(false);
				 return;
			   }
		   }
		   else
		   {
                 ui.vectoroptionGroupBox->setChecked(false);
				 return;
		   }
	   }
	}
	ui.vectorlineRadioButton->setEnabled(checked);
	ui.vectorcylinderRadioButton->setEnabled(checked);

	ui.vectorarrowlenLabel->setEnabled(checked);
	ui.vectorlenMpushButton->setEnabled(checked);
	ui.vectorlenPpushButton->setEnabled(checked);
	ui.vectorsizeMpushButton->setEnabled(checked);
	ui.vectorsizePpushButton->setEnabled(checked);
	ui.vectorarrowsizeLabel->setEnabled(checked);
	
 	this->OnMultipleSelectSubsample();
	int style = 0;
	if(ui.vectorlineRadioButton->isChecked()) 
		style = style|VisAtomVisualizingPolicy::VECTOR_STYLE_LINE;
	else
		style = style|VisAtomVisualizingPolicy::VECTOR_STYLE_SOLID;

	emit this->SetGeomStyleVectorVisible(checked, style, false);
}

//********************************************************************************
void VisAtom2017::OnSelectVectorStyleLine(bool checked)
{
	if(!checked) return;
    if(m_OnUpdateSubsampleUI) return;

	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	this->OnMultipleSelectSubsample();
	emit this->SelectVectorStyleLine(checked);
}

//********************************************************************************
void VisAtom2017::OnSelectVectorStyleSolid(bool checked)
{
	if(!checked) return;
    if(m_OnUpdateSubsampleUI) return;

	bool all = ui.atomsphaeApplyforAllCheckBox->isChecked();
	this->OnMultipleSelectSubsample();
	emit this->SelectVectorStyleSolid(checked);
}


//********************************************************************************
void VisAtom2017::OnShowSceneOperation(bool checked)
{
    if(checked)
	{
		if(!this->ui.sceneDockWidget->isVisible())
		   this->ui.sceneDockWidget->show();
	}
	else
	{
		if(this->ui.sceneDockWidget->isVisible())
		   this->ui.sceneDockWidget->hide();
	}
}

//********************************************************************************
void VisAtom2017::OnShowSubsampleOption(bool checked)
{
	if(checked)
	{
		if(!this->ui.displayoptionDockWidget->isVisible())
		this->ui.displayoptionDockWidget->show();
	}
	else
	{
		if(this->ui.displayoptionDockWidget->isVisible())
		this->ui.displayoptionDockWidget->hide();
	}
}

//********************************************************************************
void VisAtom2017::OnShowSubsampleBrowser(bool checked)
{
	if(checked)
	{
		if(!this->ui.subsampleSumDockWidget->isVisible())
		this->ui.subsampleSumDockWidget->show();
	}
	else
	{
		if(this->ui.subsampleSumDockWidget->isVisible())
		this->ui.subsampleSumDockWidget->hide();
	}
}

//********************************************************************************
void VisAtom2017::OnCloseSceneOperation(bool checked)
{
	ui.actionShow_scene_operation->setChecked(checked);
}

//********************************************************************************
void VisAtom2017::OnCloseSubsampleOption(bool checked)
{
	ui.actionShow_subsample_options->setChecked(checked);
}
//********************************************************************************
void VisAtom2017::OnCloseSubsampleBrowser(bool checked)
{
	ui.actionShow_subsample_browser->setChecked(checked);
}

//********************************************************************************
void VisAtom2017::OnNormalizeView()
{
	this->ui.displayWidget->FitGraphView();
}

//********************************************************************************
void VisAtom2017::OnTrajectoryOptionGroupBox(bool checked)
{

    if(checked)
	{
		if(this->ui.trajectoryDataMRadioButton->isChecked())
		{
         if(!VisAtom_NewDisplacementPolicyDlg::HasTrajectData())
	     {
           QMessageBox msgBox(this);
		   msgBox.setIcon(QMessageBox::Question);
		   msgBox.setText(tr("No trajectory data available, set the data sources?"));
		   msgBox.addButton(QMessageBox::Yes);
		   msgBox.addButton(QMessageBox::No);
		   if(msgBox.exec() == QMessageBox::Yes)
          {
			   //popup the dialog
		       VisAtom_NewDisplacementPolicyDlg newvtrajectDlg(this);
			   //initializr the dialog
			   newvtrajectDlg.SetFilename(CURRENT_FNAME, this->m_SampleDataSheet);

			   if(newvtrajectDlg.exec() != QDialog::Accepted)
			   {
                 this->ui.trajectoryOptionGroupBox->setChecked(false);
				 return;
			   }
		   }
		   else
		   {
                 this->ui.trajectoryOptionGroupBox->setChecked(false);
				 return;
		   }
          //update the UI
		   int colx, coly, colz;
           VisAtom_NewDisplacementPolicyDlg::GetDisplacementColume(colx, coly, colz);
		   this->m_LoadDataThread->SetFileSerial(CURRENT_FNAME, VisAtom_NewDisplacementPolicyDlg::GetStartInd(),
		                                                        VisAtom_NewDisplacementPolicyDlg::GetEndInd(),
														  	    VisAtom_NewDisplacementPolicyDlg::GetIndexStep(),
																colx, coly, colz);

	      this->ui.trajectfromSpinBox->setValue(VisAtom_NewDisplacementPolicyDlg::GetStartInd());
	      this->ui.trajecttoSpinBox->setValue(VisAtom_NewDisplacementPolicyDlg::GetEndInd());
	      this->ui.trajectfilenameLineEdit->setText(CURRENT_FNAME);
	     }
		 else //load from a single trajectory file
		 {
		 }
		}
	   // reload the data if necessary
	}
    int style;
	emit this->SetGeomStyleTrajectyoryVisible(checked, style, false);

	if(!this->m_LoadDataThread->IsDataLoaded())
	    this->m_LoadDataThread->StartLoadData();
	else
        this->ui.displayWidget->updateGL();

	return;
}

//********************************************************************************
void VisAtom2017::OnShapeOptionGroupBox(bool checked)
{
	int style;
	emit this->SetGeomStyleAtomVisible(checked, style, false);
	return;

}

//********************************************************************************
void VisAtom2017::OnStartLoadTrajectory(QString& filename, int index, int total)
{
	this->setCursor(Qt::WaitCursor);
	     QString title;
		 title = tr("Load trajectory data...");
		 title.append(filename);
	     this->m_statulabel.setText(title);
		 this->m_progressbar.setMaximum(total);
		 this->m_statulabel.show();
         this->m_progressbar.show(); 
  	  return;
}

//********************************************************************************
void VisAtom2017::OnEndLoadTrajectory(QString& filename, int index, int total)
{
	     QString title;
		 title = tr("Data loaded from:");
		 title.append(filename);
		 this->m_statulabel.setText(title);
 		 this->m_statulabel.show();

		 this->m_progressbar.setValue(index+1);
		 this->m_progressbar.show();
		 this->ui.displayWidget->updateGL();
         this->unsetCursor();

		 if(index+1 == total)
		 {
	        this->m_statulabel.hide();
            this->m_progressbar.hide(); 
		 }
		 return;
}



//********************************************************************************
void VisAtom2017:: OnTrajectoryThickChanged(double thick)
{
     
	emit SetTrajectoryThickChanged(thick,false);
    return;
}

//********************************************************************************
void VisAtom2017::OnClipthroughPosRadioButton(bool checked)
{
	this->ui.clipPosXSpinBox->setReadOnly(false);
	this->ui.clipPosYSpinBox->setReadOnly(false);
	this->ui.clipPosZSpinBox->setReadOnly(false);
	this->ui.clipthroughAtomSpinBox->setReadOnly(true);
}

//********************************************************************************
void VisAtom2017::OnClipthroughAtomRadioButton(bool checked)
{
	this->ui.clipPosXSpinBox->setReadOnly(true);
	this->ui.clipPosYSpinBox->setReadOnly(true);
	this->ui.clipPosZSpinBox->setReadOnly(true);
	this->ui.clipthroughAtomSpinBox->setReadOnly(false);
	int atomid = this->ui.clipthroughAtomSpinBox->value();

	if(this->m_SampleDataSheet)
	{
		this->ui.clipthroughAtomSpinBox->setMaximum(this->m_SampleDataSheet->GetNumRow());
		float*x = this->m_SampleDataSheet->GetData(this->m_SampleDataSheet->GetPosXCol());
		float*y = this->m_SampleDataSheet->GetData(this->m_SampleDataSheet->GetPosYCol());
		float*z = this->m_SampleDataSheet->GetData(this->m_SampleDataSheet->GetPosZCol());

		if(x && y && z)
		{
			this->ui.clipPosXSpinBox->setValue(x[atomid-1]);
			this->ui.clipPosYSpinBox->setValue(y[atomid-1]);
			this->ui.clipPosZSpinBox->setValue(z[atomid-1]);
		}
	}
	return;
}

//********************************************************************************
void VisAtom2017::OnClipthroughAtomSpinBox(int atomid)
{
	if(this->m_SampleDataSheet)
	{
		float*x = this->m_SampleDataSheet->GetData(this->m_SampleDataSheet->GetPosXCol());
		float*y = this->m_SampleDataSheet->GetData(this->m_SampleDataSheet->GetPosYCol());
		float*z = this->m_SampleDataSheet->GetData(this->m_SampleDataSheet->GetPosZCol());

		if(x && y && z)
		{
			this->ui.clipPosXSpinBox->setValue(x[atomid-1]);
			this->ui.clipPosYSpinBox->setValue(y[atomid-1]);
			this->ui.clipPosZSpinBox->setValue(z[atomid-1]);
		}
	}   
	return;
}


//********************************************************************************
void VisAtom2017::OnResetTrajectoryDataPushButton()
{
	if(this->ui.trajectoryDataMRadioButton->isChecked())
	{
	  //reserve old set 
      int startind =  VisAtom_NewDisplacementPolicyDlg::GetStartInd();
      int endind   =  VisAtom_NewDisplacementPolicyDlg::GetEndInd();
      int indstep  =  VisAtom_NewDisplacementPolicyDlg::GetIndexStep();

	  //popup the dialog
	   VisAtom_NewDisplacementPolicyDlg newvtrajectDlg(this);
	  //initializr the dialog
	  newvtrajectDlg.SetFilename(CURRENT_FNAME, this->m_SampleDataSheet);
      if(newvtrajectDlg.exec() != QDialog::Accepted) return;

      //update the UI
	  int colx, coly, colz;
      VisAtom_NewDisplacementPolicyDlg::GetDisplacementColume(colx, coly, colz);

	  //check if data files have been changed
      if(startind != VisAtom_NewDisplacementPolicyDlg::GetStartInd() ||
         endind   != VisAtom_NewDisplacementPolicyDlg::GetEndInd()   ||
         indstep  != VisAtom_NewDisplacementPolicyDlg::GetIndexStep()||
		 CURRENT_FNAME != this->ui.trajectfilenameLineEdit->text()    )
	  {
   	     this->m_LoadDataThread->SetFileSerial(CURRENT_FNAME, VisAtom_NewDisplacementPolicyDlg::GetStartInd(),
	                                                          VisAtom_NewDisplacementPolicyDlg::GetEndInd(),
			  										  	      VisAtom_NewDisplacementPolicyDlg::GetIndexStep(),
															  colx, coly, colz);
	    this->ui.trajectfromSpinBox->setValue(VisAtom_NewDisplacementPolicyDlg::GetStartInd());
	    this->ui.trajecttoSpinBox->setValue(VisAtom_NewDisplacementPolicyDlg::GetEndInd());
	    this->ui.trajectfilenameLineEdit->setText(CURRENT_FNAME);

	     int style;
	     if(this->ui.trajectoryOptionGroupBox->isChecked())
	        emit this->SetGeomStyleTrajectyoryVisible(true, style, false);
		 this->m_LoadDataThread->StartLoadData();
	  }
	  else
	  {
        this->m_LoadDataThread->SetDataCol(colx, coly, colz);
	    if(!this->m_LoadDataThread->IsDataLoaded())
	       this->m_LoadDataThread->StartLoadData();
		else
        this->ui.displayWidget->updateGL();
	  }
	}
	else // loads from a single trajectory fiel
	{
	}
    
    return;
}


//********************************************************************************
void VisAtom2017::OnTrajectoryDataSRadioButton(bool checked)
{
	if(!checked) return;

       
    this->ui.trajectfromSpinBox->setEnabled(false);
    this->ui.trajecttoSpinBox->setEnabled(false);
    this->ui.trajectstepSpinBox->setEnabled(false);

      
    return;
}

//********************************************************************************
void VisAtom2017::OnTrajectoryDataMRadioButton(bool checked)
{
	if(!checked) return;
    this->ui.trajectfromSpinBox->setEnabled(true);
    this->ui.trajecttoSpinBox->setEnabled(true);
    this->ui.trajectstepSpinBox->setEnabled(true);
    return;
}


//********************************************************************************
void VisAtom2017::OnBoxX0Changed(double newX0)
{
	 float xmin, xmax, ymin, ymax, zmin, zmax; 
	 this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);

	 if(abs(newX0 - (double)xmin) > 0.1)
	 {
		 xmin =  newX0;
	     this->ui.displayWidget->SetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
		 this->ui.displayWidget->updateGL();
	 }
}

//********************************************************************************
void VisAtom2017::OnBoxX1Changed(double newX1)
{
	 float xmin, xmax, ymin, ymax, zmin, zmax; 
	 this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);

	 if(abs(newX1 - (double)xmax) > 0.1)
	 {
		 xmax =  newX1;
	     this->ui.displayWidget->SetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
		 this->ui.displayWidget->updateGL();
	 }
}

//********************************************************************************
void VisAtom2017::OnBoxY0Changed(double newY0)
{
	 float xmin, xmax, ymin, ymax, zmin, zmax; 
	 this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);

	 if(abs(newY0 - (double)ymin) > 0.1)
	 {
		 ymin =  newY0;
	     this->ui.displayWidget->SetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
		 this->ui.displayWidget->updateGL();
	 }
}

//********************************************************************************
void VisAtom2017::OnBoxY1Changed(double newY1)
{
	 float xmin, xmax, ymin, ymax, zmin, zmax; 
	 this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);

	 if(abs(newY1 - (double)ymax) > 0.1)
	 {
		 ymax =  newY1;
	     this->ui.displayWidget->SetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
		 this->ui.displayWidget->updateGL();
	 }
}

//********************************************************************************
void VisAtom2017::OnBoxZ0Changed(double newZ0)
{
	 float xmin, xmax, ymin, ymax, zmin, zmax; 
	 this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);

	 if(abs(newZ0 - (double)zmin) > 0.1)
	 {
		 zmin =  newZ0;
	     this->ui.displayWidget->SetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
		 this->ui.displayWidget->updateGL();
	 }
}

//********************************************************************************
void VisAtom2017::OnBoxZ1Changed(double newZ1)
{
	 float xmin, xmax, ymin, ymax, zmin, zmax; 
	 this->ui.displayWidget->GetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);

	 if(abs(newZ1 - (double)zmax) > 0.1)
	 {
		 zmax =  newZ1;
	     this->ui.displayWidget->SetSceneVolume(xmin, xmax, ymin, ymax, zmin, zmax);
		 this->ui.displayWidget->updateGL();
	 }
}

//********************************************************************************
void VisAtom2017::OnBoxAutoPostion(bool checked)
{
		 this->ui.boxX0SpinBox->setEnabled(!checked);
		 this->ui.boxX1SpinBox->setEnabled(!checked);
		 this->ui.boxY0SpinBox->setEnabled(!checked);
		 this->ui.boxY1SpinBox->setEnabled(!checked);
		 this->ui.boxZ0SpinBox->setEnabled(!checked);
		 this->ui.boxZ1SpinBox->setEnabled(!checked);

		 if(checked)  
		    this->ui.displayWidget->SetAutoNormalizingView();
		 else  
		    this->ui.displayWidget->SetNoAutoNormalizingView();

	     VisAtomVisualizingPolicy::SetAutoProjectionBox(checked);

}

//********************************************************************************
void VisAtom2017::OnClusterEvelope()
{
	QString ext, snum, dot='.', caption, dir, selfilter;
	QString currentfname;
	if(CURRENT_EXTNUM>= 0) 
	{
		ext = tr("*.");
        snum.setNum(CURRENT_EXTNUM);
		if(CURRENT_EXTNUM < 10)
 		   ext.append(tr("000"));
		else if(CURRENT_EXTNUM < 100)
		   ext.append(tr("00"));
		else if(CURRENT_EXTNUM < 1000)
		   ext.append(tr("0"));

	   ext.append(snum);
	}
    selfilter = tr("(");
    selfilter.append(ext);
    selfilter.append(tr(");;(*,*)"));


	QString fileName = QFileDialog::getOpenFileName(this, caption, dir, ext, &selfilter);

    if (!fileName.isEmpty()) 
	{
		VisClusterEnvelope*pObj = new VisClusterEnvelope();
		if(pObj->LoadData(fileName))
  	       emit this->OpenExtendedObject(pObj, fileName);
	}

	return;
}

void VisAtom2017::OnActionSubsample_filter(bool checked)
{
	if (!this->m_Atomfilter)
	{
		this->m_Atomfilter = new Visatom_AtomfilterDlg(this);
	}
	this->m_Atomfilter->setVisible(checked);
}

void VisAtom2017::OnActionFindatom()
{
	if (!this->m_FindatomDlg)
	{
		this->m_FindatomDlg = new Visatom_FindatomDlg(this);
	}
	this->m_FindatomDlg->SetAtomSample(this->ui.displayWidget);

	this->m_FindatomDlg->setVisible(true);
	this->ui.displayWidget->OnActionFindatom();
}



