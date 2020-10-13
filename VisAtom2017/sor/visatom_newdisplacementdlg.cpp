#include "visatom_newdisplacementdlg.h"
#include "VisAtomVisualizingPolicy.h"
#include "VisAtomDataSheet.h"
#include "VisAtomSample.h"
#include "VisAtomSubsample.h"
#include <stdlib.h>

int VisAtom_NewDisplacementPolicyDlg::m_discol[3]={-1,-1,-1};
int VisAtom_NewDisplacementPolicyDlg::m_startInd=-1;
int VisAtom_NewDisplacementPolicyDlg::m_endInd=-1;
int VisAtom_NewDisplacementPolicyDlg::m_indStep=1;
int VisAtom_NewDisplacementPolicyDlg::m_IndRng[3]={-1, -1, 1};
QString VisAtom_NewDisplacementPolicyDlg::m_filename = ""; // tr("");
#define MAXROW   53
#define HEADROWS 3
#define BKR 220
#define BKG 220
#define BKB 220

#define MAXCOL  64
#define MAXSTRL 32

//****************************************************************************
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
//****************************************************************************

VisAtom_NewDisplacementPolicyDlg::VisAtom_NewDisplacementPolicyDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    
    int i;

	QString title(tr("Choose the data for displacement of atoms"));
	this->setWindowTitle(title);

	m_selcol[0] = m_selcol[1] = m_selcol[2] = -1; 
	m_fromF = m_toF = 0;
	return;
}

VisAtom_NewDisplacementPolicyDlg::~VisAtom_NewDisplacementPolicyDlg()
{
  
}

int VisAtom_NewDisplacementPolicyDlg::SetFilename(QString fname,VisAtomDataSheet*pDataSheet)
{
	if(fname.isEmpty() || !pDataSheet)
	{
		VisAtom_NewDisplacementPolicyDlg::m_filename.clear();
		VisAtom_NewDisplacementPolicyDlg::m_startInd = -1;
		VisAtom_NewDisplacementPolicyDlg::m_endInd = -1;
		VisAtom_NewDisplacementPolicyDlg::m_IndRng[0] = -1;
		VisAtom_NewDisplacementPolicyDlg::m_IndRng[1] = -1;
		VisAtom_NewDisplacementPolicyDlg::m_IndRng[2] = 1;
		return ERR_NOTINDEXEDFILE;
	}

	if(fname != this->ui.trajectfilenameEdit->text())
	{  //file name has been changed
        VisAtom_NewDisplacementPolicyDlg::m_IndRng[0] = -1;
		VisAtom_NewDisplacementPolicyDlg::m_IndRng[1] = -1;
	    this->ui.trajectfilenameEdit->setText(fname);
	}
	int from, to, step;

	if(VisAtom_NewDisplacementPolicyDlg::m_IndRng[0]<0) 
	{ // no range have been extracted
		from = 0;
		to   = 9999;
		step = 1;
	
       //*** to find the start file
	   QString fileName, ext, snum;
	   int i;
	   for(i=from; i<=to; i=i+step)
	   {
	    if(i<10)        ext = ".000";
	    else if(i<100)  ext = ".00";
	    else if(i<1000) ext = ".0";
   
	    snum.setNum(i);
	    ext.append(snum);
	    fileName = fname;
	    fileName.append(ext);
	    QFile file(fileName);
		if (file.exists()) break;
      }
      
	  if(i>to) 
	  { //no any file found
		this->m_fromF = this->m_toF = 0;
        VisAtom_NewDisplacementPolicyDlg::m_IndRng[0] = -1;
	  }
	  else 
	  {
        VisAtom_NewDisplacementPolicyDlg::m_IndRng[0] = i;
		from = i;
		//to find the end file
	    for(i=from+step; i<=to+step; i=i+step)
	    {
	     if(i<10)        ext = ".000";
	     else if(i<100)  ext = ".00";
	     else if(i<1000) ext = ".0";
   
	     snum.setNum(i);
	     ext.append(snum);
	     fileName = fname;
	     fileName.append(ext);
	     QFile file(fileName);
		 if (!file.exists()) break;
        }
	    to = i-step;
	    this->m_fromF = this->m_toF = 1;
		VisAtom_NewDisplacementPolicyDlg::m_IndRng[1] = to; 
	  }
	}
	int err = this->FillPrevewTable(pDataSheet);


	if(VisAtom_NewDisplacementPolicyDlg::m_startInd<0) from = 0;
	else from = VisAtom_NewDisplacementPolicyDlg::m_startInd;
	
	if(VisAtom_NewDisplacementPolicyDlg::m_endInd<0) to = 9999;
	else to = VisAtom_NewDisplacementPolicyDlg::m_endInd;

	step = VisAtom_NewDisplacementPolicyDlg::m_indStep; 

	this->ui.trajectfromSpinBox->setValue(from);
	this->ui.trajecttoSpinBox->setMinimum(VisAtom_NewDisplacementPolicyDlg::m_IndRng[0]);
	this->ui.trajecttoSpinBox->setMaximum(VisAtom_NewDisplacementPolicyDlg::m_IndRng[1]);
    this->ui.trajecttoSpinBox->setSingleStep(step);
	this->ui.trajecttoSpinBox->setValue(to);
	this->ui.trajectstepSpinBox->setValue(step);

	this->m_selcol[0] = VisAtom_NewDisplacementPolicyDlg::m_discol[0];
	this->m_selcol[1] = VisAtom_NewDisplacementPolicyDlg::m_discol[1];
	this->m_selcol[2] = VisAtom_NewDisplacementPolicyDlg::m_discol[2];
	this->UpdateUI();
	if(err != ERR_NOERROR) return err;
	return ERR_NOERROR;

}

//*****************************************************************************
void VisAtom_NewDisplacementPolicyDlg::UpdateUI()
{
	int from = this->ui.trajectfromSpinBox->value();
	int to  = this->ui.trajecttoSpinBox->value();
	QString fileName, snum, ext;

	 if(!this->m_fromF)
	 {
		this->ui.trajectfilefromLineEdit->setText(tr("not exist"));

	 }
	 else
	 {
		 fileName = this->ui.trajectfilenameEdit->text();
	     if(from<10)        ext = ".000";
	     else if(from<100)  ext = ".00";
	     else if(from<1000) ext = ".0";
   	     snum.setNum(from);
	     ext.append(snum);
	     fileName.append(ext);
		 this->ui.trajectfromSpinBox->setValue(from);
		 this->ui.trajectfilefromLineEdit->setText(fileName);

	 }

	 if(!this->m_toF)
	 {
		this->ui.trajectfiletoLineEdit->setText(tr("not exist"));
	 }
	 else
	 {
		 fileName = this->ui.trajectfilenameEdit->text();
	     if(to<10)        ext = ".000";
	     else if(to<100)  ext = ".00";
	     else if(to<1000) ext = ".0";
   	     snum.setNum(to);
	     ext.append(snum);
	     fileName.append(ext);
		 this->ui.trajecttoSpinBox->setValue(to);
		 this->ui.trajectfiletoLineEdit->setText(fileName);
	 }
    //write the stat of the table
	for(int i=0; i<HEADROWS; i++)  if(this->m_selcol[i] >=0)
	{
         QTableWidgetItem*item = ui.datasheetTableWidget->item(i, this->m_selcol[i]);
	     item->setCheckState(Qt::Checked);
	}
    //WriteColLabels
	QString title;
	if(this->m_selcol[0] >= 0) {title = tr("Col.#"); snum.setNum(this->m_selcol[0]+1); title.append(snum);}
	else title = tr("N/A");
	ui.vxcolLabel->setText(title);

	if(this->m_selcol[1] >= 0) {title = tr("Col.#"); snum.setNum(this->m_selcol[1]+1); title.append(snum);}
	else title = tr("N/A");
	ui.vycolLabel->setText(title);

	if(this->m_selcol[2] >= 0) {title = tr("Col.#"); snum.setNum(this->m_selcol[2]+1); title.append(snum);}
	else title = tr("N/A");
	ui.vzcolLabel->setText(title);

    //enable or disable the okButton
	 if(this->m_fromF && this->m_toF && this->m_selcol[0] >=0 && this->m_selcol[1] >= 0 &&this->m_selcol[2]>= 0) 
		this->ui.okButton->setEnabled(true);
	 else 
		 this->ui.okButton->setEnabled(false);


}
//*******************************************************************************
int VisAtom_NewDisplacementPolicyDlg::FillPrevewTable(VisAtomDataSheet*pDataSheet)
{

	  //to determine how many col for each line
	  int ncol=0, i,j;

		 ncol = pDataSheet->GetNumCol();
	     this->ui.datasheetTableWidget->clear();
        //set the header
		 QString title, snum;
	     QTableWidgetItem*hitem, *item;
	     this->ui.datasheetTableWidget->setColumnCount(ncol);
         this->ui.datasheetTableWidget->setRowCount(MAXROW);

	     for(i=0; i<ncol; i++)
	     {
           title.clear();
           title.append(tr("Col #"));
	       snum.setNum(i+1);
           title.append(snum);
		   if(i == pDataSheet->GetTypeCol())
              title.append(tr("(atom type)"));
		    else if(i == pDataSheet->GetPosXCol())
		      title.append(tr("(pos x)"));
		    else if(i == pDataSheet->GetPosYCol())
		      title.append(tr("(pos y)"));
		    else if(i == pDataSheet->GetPosZCol())
			title.append(tr("(pos z)"));

		    hitem = new QTableWidgetItem(title);
		    ui.datasheetTableWidget->setHorizontalHeaderItem(i, hitem); 
	     }


	    title.clear();
        item = new QTableWidgetItem(title);
	    this->ui.datasheetTableWidget->setVerticalHeaderItem(0, item);
	    title.append(tr("Dx"));
	    for(i=0; i<ncol; i++)
	    {
            item = new QTableWidgetItem(title);
		    item->setCheckState(Qt::Unchecked);
		    item->setBackgroundColor(QColor(BKR, BKG, BKB));
		    this->ui.datasheetTableWidget->setItem(0, i, item);
		}

	     title.clear();
         item = new QTableWidgetItem(title);
	     ui.datasheetTableWidget->setVerticalHeaderItem(1, item);
	     title.append(tr("Dy"));
	     for(i=0; i<ncol; i++)
	     {
             item = new QTableWidgetItem(title);
		     item->setCheckState(Qt::Unchecked);
		     item->setBackgroundColor(QColor(BKR, BKG, BKB));
		     this->ui.datasheetTableWidget->setItem(1, i, item);
	     }

	     title.clear();
         item = new QTableWidgetItem(title);
	     ui.datasheetTableWidget->setVerticalHeaderItem(2, item);
	     title.append(tr("Dz"));
	     for(i=0; i<ncol; i++)
	     {
             item = new QTableWidgetItem(title);
		     item->setCheckState(Qt::Unchecked);
		     item->setBackgroundColor(QColor(BKR, BKG, BKB));
		     this->ui.datasheetTableWidget->setItem(2, i, item);
	    }


        for(j=HEADROWS; j<std::min(MAXROW, pDataSheet->GetNumRow())-2; j++)
	    {
		   snum.setNum(j-HEADROWS+1);
           item = new QTableWidgetItem(snum);
	       ui.datasheetTableWidget->setVerticalHeaderItem(j, item);

	       for(i=0; i<ncol; i++)
		  {
             float cdata = pDataSheet->GetData(i)[j-HEADROWS];
	          snum.setNum(cdata, 'e', 5);
		      item = new QTableWidgetItem(snum);
		      this->ui.datasheetTableWidget->setItem(j, i, item);
		  }
	    }

	    if(pDataSheet->GetNumRow() >MAXROW-2)
	    {
	 	   snum.clear();
	   	   snum.append(tr("..."));
           item = new QTableWidgetItem(snum);
		   ui.datasheetTableWidget->setVerticalHeaderItem(MAXROW-2, item);
		   for(i=0; i<ncol; i++)
		   {
            item = new QTableWidgetItem(snum);
		    ui.datasheetTableWidget->setItem(MAXROW-2, i, item);
		   }

	   	   snum.setNum(pDataSheet->GetNumRow());
           item = new QTableWidgetItem(snum);
		   ui.datasheetTableWidget->setVerticalHeaderItem(MAXROW-1, item);
		   for(i=0; i<ncol; i++)
		   {
            float*cdata = pDataSheet->GetData(i);
			snum.setNum(cdata[pDataSheet->GetNumRow()-1], 'e', 5);
            item = new QTableWidgetItem(snum);
		    ui.datasheetTableWidget->setItem(MAXROW-1, i, item);
		   }
	    }
        return 0;
}


void VisAtom_NewDisplacementPolicyDlg::OnDatasheetChanged(QTableWidgetItem*item)
{
	int col = item->column();
	int row = item->row();
	int checked = item->checkState();
	if(row >= HEADROWS) return;

	if(checked)
	{
		int i;
		for(i=0; i<ui.datasheetTableWidget->columnCount(); i++)
		{
            QTableWidgetItem*item = ui.datasheetTableWidget->item(row, i);
			if(i != col) item->setCheckState(Qt::Unchecked);
		}
		//in this case, we permit the same colume for are chosen
		/*for(i=0; i<HEADROWS; i++)
		{
            QTableWidgetItem*item = ui.datasheetTableWidget->item(i, col);
			if(i != row) item->setCheckState(Qt::Unchecked);
		}*/

        //////////////////////////////////////
		m_selcol[row] = col;
	}
	else //unchecked
	{
		m_selcol[row] = -1;
	}
	this->UpdateUI();

    return;	
}

bool VisAtom_NewDisplacementPolicyDlg::HasTrajectData()
{
	if(VisAtom_NewDisplacementPolicyDlg::m_filename.isEmpty() ||
	   VisAtom_NewDisplacementPolicyDlg::m_startInd < 0      ||
	   VisAtom_NewDisplacementPolicyDlg::m_endInd < 0 )
	   return false;
	else return true;
}

void VisAtom_NewDisplacementPolicyDlg::OnFromChanged(int from)
{

	    QString snum, ext, fileName;
		int i, to, step;
	    if(from<10)        ext = ".000";
	    else if(from<100)  ext = ".00";
	    else if(from<1000) ext = ".0";
   
	    snum.setNum(from);
	    ext.append(snum);
	    fileName = this->ui.trajectfilenameEdit->text();
	    fileName.append(ext);
	    QFile file(fileName);
        if(!file.exists())
		{
			this->m_fromF = 0;
		}
		else 
		{
		    this->ui.trajectfilefromLineEdit->setText(fileName);
			step = this->ui.trajectstepSpinBox->value();
		    //to find the end file
			to = 9999;
	        for(i=from+step; i<=to+step; i=i+step)
	        {
	           if(i<10)        ext = ".000";
	           else if(i<100)  ext = ".00";
	           else if(i<1000) ext = ".0";
   
	           snum.setNum(i);
	           ext.append(snum);
	           fileName = this->ui.trajectfilenameEdit->text();
	           fileName.append(ext);
			   file.setFileName(fileName);
		       if (!file.exists()) break;
            }
	        to = i-step;
	        this->ui.trajecttoSpinBox->setValue(to);
			this->ui.trajecttoSpinBox->setMinimum(from);
	        this->ui.trajecttoSpinBox->setMaximum(to);

			this->m_fromF = this->m_toF = 1;
		}
	    this->UpdateUI();
		return;
}

void VisAtom_NewDisplacementPolicyDlg::OnStepChanged(int step)
{
	  QString fileName, ext, snum;
	  QFile file;
	   //to find the end file
	   int i, from, to = 9999;
	   from = this->ui.trajectfromSpinBox->value();
	   for(i=from+step; i<=to+step; i=i+step)
	    {
	         if(i<10)        ext = ".000";
	         else if(i<100)  ext = ".00";
	         else if(i<1000) ext = ".0";
   
	         snum.setNum(i);
	         ext.append(snum);
	         fileName = this->ui.trajectfilenameEdit->text();
	         fileName.append(ext);
		     file.setFileName(fileName);
		    if (!file.exists()) break;
         }
	     to = i-step;
	     this->ui.trajecttoSpinBox->setMaximum(to);
	     this->ui.trajecttoSpinBox->setValue(to);
	     this->ui.trajecttoSpinBox->setSingleStep(step);
		 this->UpdateUI();
		 return;
}


void VisAtom_NewDisplacementPolicyDlg::accept()
{
	VisAtom_NewDisplacementPolicyDlg::m_filename = this->ui.trajectfilenameEdit->text();
	VisAtom_NewDisplacementPolicyDlg::m_startInd = this->ui.trajectfromSpinBox->value();
	VisAtom_NewDisplacementPolicyDlg::m_endInd   = this->ui.trajecttoSpinBox->value();
	VisAtom_NewDisplacementPolicyDlg::m_discol[0] = this->m_selcol[0];
	VisAtom_NewDisplacementPolicyDlg::m_discol[1] = this->m_selcol[1];
	VisAtom_NewDisplacementPolicyDlg::m_discol[2] = this->m_selcol[2];
	VisAtom_NewDisplacementPolicyDlg::m_indStep   = this->ui.trajectstepSpinBox->value();
	QDialog::accept();
}


