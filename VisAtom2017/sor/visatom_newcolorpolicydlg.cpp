#include "visatom_newcolorpolicydlg.h"
#include "VisAtomVisualizingPolicy.h"
#include "VisAtomDataSheet.h"
#include "VisAtomSample.h"
#include "VisAtomSubsample.h"
#include <stdlib.h>


double VisAtom_NewColorPolicyDlg::m_winmin = 0;
double VisAtom_NewColorPolicyDlg::m_winmax = 100;
double VisAtom_NewColorPolicyDlg::m_datamin = 1e8;
double VisAtom_NewColorPolicyDlg::m_datamax =-1.e8;
int    VisAtom_NewColorPolicyDlg::m_datacol = -1;

VisAtom_NewColorPolicyDlg::VisAtom_NewColorPolicyDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    
    int i;
	
	this->m_Sample = NULL;
	this->m_atype = NULL;
	this->m_px = NULL;
	this->m_py = NULL;
	this->m_pz = NULL;

	ui.scalarwindowMinSpinBox->setValue(VisAtom_NewColorPolicyDlg::m_winmin);
	ui.scalarwindowMaxSpinBox->setValue(VisAtom_NewColorPolicyDlg::m_winmax);
	if(VisAtom_NewColorPolicyDlg::m_datamax > VisAtom_NewColorPolicyDlg::m_datamin)
	{
	    QString snum;
	    snum.setNum(VisAtom_NewColorPolicyDlg::m_datamin, 'e', 4);
	    ui.rangeminLable->setText(snum);
	    snum.setNum(VisAtom_NewColorPolicyDlg::m_datamax, 'e', 4);
	    ui.rangemaxLable->setText(snum);	
	}
	QString title(tr("Edit scalar range for colormapping"));
	this->setWindowTitle(title);
	this->ui.subsampleListTreeWidget->setHeaderLabel("Scalar filter");

	return;
}

VisAtom_NewColorPolicyDlg::~VisAtom_NewColorPolicyDlg()
{
  
}

void VisAtom_NewColorPolicyDlg::SetSample(VisAtomSample*sample)
{
	this->m_Sample = sample;

	 QString sname;
     QTreeWidgetItem*item = new QTreeWidgetItem();
	 item->setText(0,tr("ALL subsamples"));
	 item->setCheckState(0,Qt::Checked);
	 this->ui.subsampleListTreeWidget->addTopLevelItem(item);

	 int i;
	 for(i=0; i<this->m_Sample->NumberofSubsample(); i++)
	 {
			 VisAtomSubSample*subp = this->m_Sample->GetSubsample(i);
			 sname = subp->GetName();
		     QTreeWidgetItem*item = new QTreeWidgetItem();
			 item->setText(0, sname);
		     item->setCheckState(0,Qt::Checked);
			 this->ui.subsampleListTreeWidget->addTopLevelItem(item);
	 }
	return;
}

void VisAtom_NewColorPolicyDlg::SetDataSheet(VisAtomDataSheet*pData)
{   
     #define MAXROW 100
	 this->m_SampleDataSheet = pData;
	 int col = this->m_SampleDataSheet->GetNumCol();
	 int row = this->m_SampleDataSheet->GetNumRow();
	 int*colprop = this->m_SampleDataSheet->GetColProp();

	 int i, j;
	 QString title, snum;

	 ui.datasheetTableWidget->setColumnCount(col);
     ui.datasheetTableWidget->setRowCount(std::min(MAXROW,row));
	 for(i=0; i<col; i++)
	 {
		 title.clear();
		 title.append(tr("Col #"));
		 snum.setNum(i+1);
		 title.append(snum);
		 switch(colprop[i])
		 {
		 case VisAtomDataSheet::COLPROP_TYPE:
			 title.append(tr(" (type)"));
			 this->m_atype = this->m_SampleDataSheet->GetData(i);
			 break;
		 case VisAtomDataSheet::COLPROP_POSX:
			 title.append(tr(" (x)"));
			 this->m_px = this->m_SampleDataSheet->GetData(i);
			 break;
		 case VisAtomDataSheet::COLPROP_POSY:
			 title.append(tr(" (y)"));
			 this->m_py = this->m_SampleDataSheet->GetData(i);
			 break;
		 case VisAtomDataSheet::COLPROP_POSZ:
			 title.append(tr(" (z)"));
			 this->m_pz = this->m_SampleDataSheet->GetData(i);
			 break;
		 }

		 QTableWidgetItem*hitem = new QTableWidgetItem(title);
		 ui.datasheetTableWidget->setHorizontalHeaderItem(i, hitem); 
	 }

	 for(i=0; i<col; i++)
	 {
	    for(j=0; j<std::min(MAXROW, row)-2; j++)
		{
          float*cdata = this->m_SampleDataSheet->GetData(i);
		  if(colprop[i] == VisAtomDataSheet::COLPROP_TYPE)
			  snum.setNum((int)(cdata[j]+0.000001));
		  else 
		     snum.setNum(cdata[j], 'e', 5);

		  QTableWidgetItem*item = new QTableWidgetItem(snum);
		  ui.datasheetTableWidget->setItem(j, i, item);
		}
	  }

	 if(row >MAXROW-2)
	 {
		snum.clear();
	   	snum.append(tr("..."));
        QTableWidgetItem*item = new QTableWidgetItem(snum);
		ui.datasheetTableWidget->setVerticalHeaderItem(MAXROW-2, item);
		for(i=0; i<col; i++)
		{
            QTableWidgetItem*item = new QTableWidgetItem(snum);
		    ui.datasheetTableWidget->setItem(MAXROW-2, i, item);
		}

	   	snum.setNum(row);
        item = new QTableWidgetItem(snum);
		ui.datasheetTableWidget->setVerticalHeaderItem(MAXROW-1, item);
		for(i=0; i<col; i++)
		{
            float*cdata = this->m_SampleDataSheet->GetData(i);
		    if(colprop[i] == VisAtomDataSheet::COLPROP_TYPE)
			  snum.setNum((int)(cdata[row-1]+0.000001));
		    else 
		       snum.setNum(cdata[row-1], 'e', 5);

            QTableWidgetItem*item = new QTableWidgetItem(snum);
		    ui.datasheetTableWidget->setItem(MAXROW-1, i, item);
		}

	 }
	 ui.datasheetTableWidget->selectColumn(this->m_datacol);
	 ui.numrowSpinBox->setValue(row);
     return;
} 

void VisAtom_NewColorPolicyDlg::OnTableSelected()
{
	int col = ui.datasheetTableWidget->currentColumn();
	if(col < 0) return;
	if(!this->m_Sample) return;
	int i;
	double fmin, fmax;

	this->m_datacol = col;
	this->GetFilteredScalarRange(col, fmin, fmax);

	QString snum;
	snum.setNum(fmin, 'e', 4);
	ui.dataminLabel->setText(snum);
	snum.setNum(fmax, 'e', 4);
	ui.datamaxLabel->setText(snum);

	double range = ui.scalarwindowMinSpinBox->maximum() - ui.scalarwindowMinSpinBox->minimum();
	this->m_datamin =  ui.scalarwindowMinSpinBox->value()*(fmax-fmin)/range + fmin;
	this->m_datamax =  ui.scalarwindowMaxSpinBox->value()*(fmax-fmin)/range + fmin;

	//to make data in 10 based data
	fmin = this->m_datamin;
	fmax = this->m_datamax;
	int scal, num;
	scal = log10(abs(fmin));
    num = floor(fmin*pow(10.f, -scal));
	this->m_datamin = (float)num*pow(10.f,scal);

	scal = log10(abs(fmax));
    num  = ceil(fmax*pow(10.f, -scal));
	this->m_datamax = (float)num*pow(10.f,scal);

	snum.setNum(this->m_datamin, 'e', 4);
    ui.rangeminLable->setText(snum);
	snum.setNum(this->m_datamax, 'e', 4);
    ui.rangemaxLable->setText(snum);
    return;
}

void VisAtom_NewColorPolicyDlg::OnSubsampleListItemChanged(QTreeWidgetItem*item)
{
	if(item->text(0) == tr("ALL subsamples") && item->checkState(0) == Qt::Checked )
	{
		int i;
		for(i=0; i<this->m_Sample->NumberofSubsample(); i++)
		{
			QTreeWidgetItem*p = this->ui.subsampleListTreeWidget->topLevelItem(i+1);
	        p->setCheckState(0,Qt::Checked);
		}
	}
	else if(item->checkState(0) == Qt::Unchecked) 
	{
         this->ui.subsampleListTreeWidget->topLevelItem(0)->setCheckState(0,Qt::Unchecked);   
	}
   this->OnTableSelected();
}

void VisAtom_NewColorPolicyDlg::GetFilteredScalarRange(int col, double&fmin, double&fmax)
{
 	float*pdat = this->m_SampleDataSheet->GetData(col);

	fmin = 1.e10;
	fmax =-1.e10;
    int i,j, nsub, ingroup;

	QTreeWidgetItem*item;
	QList<VisAtomSubSample*> subp;
	for(i=0; i<this->m_Sample->NumberofSubsample(); i++)
	{
		item = this->ui.subsampleListTreeWidget->topLevelItem(i+1);
		if(item->checkState(0) == Qt::Checked)
		{
			subp.append(this->m_Sample->GetSubsample(i));
		}
	}

	nsub = subp.count();
	for(i=0; i<this->m_SampleDataSheet->GetNumRow(); i++)
	{
          ingroup = false;
		  for(j=0; j<nsub;j++)
		  {
			  if(m_atype)
			  {
			    if(subp[j]->CheckMembership((int)(m_atype[i]+0.00001), m_px[i], m_py[i], m_pz[i]))
			    {
				  ingroup = true; break;
			    }
			  }
			  else
			  {
			    if(subp[j]->CheckMembership(1,m_px[i], m_py[i], m_pz[i]))
			    {
				  ingroup = true; break;
			    }
			  }
		  }
		  
		  if(ingroup)
		  {
		    if(pdat[i] > fmax) fmax = pdat[i];
		    if(pdat[i] < fmin) fmin = pdat[i];
		  }
	  }
}


