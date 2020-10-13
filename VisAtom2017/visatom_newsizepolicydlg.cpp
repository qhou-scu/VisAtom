#include "VisAtom_NewSizePolicyDlg.h"
#include "VisAtomVisualizingPolicy.h"
#include "VisAtomDataSheet.h"
#include "VisAtomSample.h"
#include "VisAtomSubsample.h"
#include "math.h"


double VisAtom_NewSizePolicyDlg::m_datamin = 1e8;
double VisAtom_NewSizePolicyDlg::m_datamax = 0;
int    VisAtom_NewSizePolicyDlg::m_datacol = -1;

VisAtom_NewSizePolicyDlg::VisAtom_NewSizePolicyDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    
    int i;
	if(VisAtom_NewSizePolicyDlg::m_datamax > VisAtom_NewSizePolicyDlg::m_datamin)
	{
	    QString snum;
	    snum.setNum(VisAtom_NewSizePolicyDlg::m_datamin, 'e', 4);
	    ui.dataminLabel->setText(snum);
	    snum.setNum(VisAtom_NewSizePolicyDlg::m_datamax, 'e', 4);
	    ui.datamaxLabel->setText(snum);	
	}
	QString title(tr("Edit scalar range for atom radia"));
	this->setWindowTitle(title);

	return;
}

VisAtom_NewSizePolicyDlg::~VisAtom_NewSizePolicyDlg()
{
  
}


void VisAtom_NewSizePolicyDlg::SetDataSheet(VisAtomDataSheet*pData)
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
			 break;
		 case VisAtomDataSheet::COLPROP_POSX:
			 title.append(tr(" (x)"));
			 break;
		 case VisAtomDataSheet::COLPROP_POSY:
			 title.append(tr(" (y)"));
			 break;
		 case VisAtomDataSheet::COLPROP_POSZ:
			 title.append(tr(" (z)"));
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

	 if(this->m_datacol<0 ||this->m_datamin <0) ui.okButton->setEnabled(false);

     return;
} 

void VisAtom_NewSizePolicyDlg::OnTableSelected()
{
	int col = ui.datasheetTableWidget->currentColumn();
	if(col < 0) return;

	int i,j;
	double fmin, fmax;
    float*pdat = this->m_SampleDataSheet->GetData(col);

	fmin = 1.e10;
	fmax =-1.e10;

	for(i=0; i<this->m_SampleDataSheet->GetNumRow(); i++)
	{
	    if(pdat[i] > fmax) fmax = pdat[i];
	    if(pdat[i] < fmin) fmin = pdat[i];
	}

	QString snum;
	snum.setNum(fmin, 'e', 4);
	ui.dataminLabel->setText(snum);
	snum.setNum(fmax, 'e', 4);
	ui.datamaxLabel->setText(snum);

	this->m_datacol = col;
	this->m_datamin =  fmin;
	this->m_datamax =  fmin;
	if(fmin < 0) 
	{
		 QMessageBox msgBox(this);
		 msgBox.setIcon(QMessageBox::Warning);
		 
		 QString msg("Warnning");
         msgBox.setText("Radius cannot be negative");
		 msgBox.addButton(QMessageBox::Ok);
		 msgBox.exec();
		 ui.okButton->setEnabled(false);
	}
	else
	{
	    ui.okButton->setEnabled(true);
	}

    return;
}


