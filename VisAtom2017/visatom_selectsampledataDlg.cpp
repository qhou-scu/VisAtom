#include <stdlib.h>
#include "VisAtom_SelectSampleDataDlg.h"
#include "VisAtomDataSheet.h"

int VisAtom_SelectSampleDataDlg::m_showthisdlg = 1;
int VisAtom_SelectSampleDataDlg::m_coltype = -1;
int VisAtom_SelectSampleDataDlg::m_colx = -1;
int VisAtom_SelectSampleDataDlg::m_coly = -1;
int VisAtom_SelectSampleDataDlg::m_colz = -1;

VisAtom_SelectSampleDataDlg::VisAtom_SelectSampleDataDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    
	QString title(tr("Choose data from datasheet for sample data"));
	this->setWindowTitle(title);

	return;
}

VisAtom_SelectSampleDataDlg::~VisAtom_SelectSampleDataDlg()
{
  
}

#define MAXROW   54
#define HEADROWS 4
#define BKR 220
#define BKG 220
#define BKB 220
void VisAtom_SelectSampleDataDlg::SetDataSheet(VisAtomDataSheet*pData)
{   
	 this->m_SampleDataSheet = pData;
	 int col = this->m_SampleDataSheet->GetNumCol();
	 int row = this->m_SampleDataSheet->GetNumRow();

	 int i, j;
	 QString title, snum;
	 QTableWidgetItem*hitem, *item;
	 ui.datasheetTableWidget->setColumnCount(col);
     ui.datasheetTableWidget->setRowCount(std::min(MAXROW,row+HEADROWS));

	 //reserve the previous selection
	 int oldtype = this->m_coltype;
	 int oldx    = this->m_colx;
	 int oldy    = this->m_coly;
	 int oldz    = this->m_colz;

	 for(i=0; i<col; i++)
	 {
		 title.clear();
		 title.append(tr("Col #"));
		 snum.setNum(i+1);
		 title.append(snum);

		 hitem = new QTableWidgetItem(title);
		 ui.datasheetTableWidget->setHorizontalHeaderItem(i, hitem); 
	 }

	 title.clear();
     item = new QTableWidgetItem(title);
	 ui.datasheetTableWidget->setVerticalHeaderItem(0, item);
	 title.append(tr("type"));
	 for(i=0; i<col; i++)
	 {
        item = new QTableWidgetItem(title);
		item->setCheckState(Qt::Unchecked);
		item->setBackgroundColor(QColor(BKR, BKG, BKB));
		ui.datasheetTableWidget->setItem(0, i, item);
	 }


	 title.clear();
     item = new QTableWidgetItem(title);
	 ui.datasheetTableWidget->setVerticalHeaderItem(1, item);
	 title.append(tr("X"));
	 for(i=0; i<col; i++)
	 {
        item = new QTableWidgetItem(title);
		item->setCheckState(Qt::Unchecked);
		item->setBackgroundColor(QColor(BKR, BKG, BKB));
		ui.datasheetTableWidget->setItem(1, i, item);
	 }

	 title.clear();
     item = new QTableWidgetItem(title);
	 ui.datasheetTableWidget->setVerticalHeaderItem(2, item);
	 title.append(tr("Y"));
	 for(i=0; i<col; i++)
	 {
        item = new QTableWidgetItem(title);
		item->setCheckState(Qt::Unchecked);
		item->setBackgroundColor(QColor(BKR, BKG, BKB));
		ui.datasheetTableWidget->setItem(2, i, item);
	 }

	 title.clear();
     item = new QTableWidgetItem(title);
	 ui.datasheetTableWidget->setVerticalHeaderItem(3, item);
	 title.append(tr("Z"));
	 for(i=0; i<col; i++)
	 {
        item = new QTableWidgetItem(title);
		item->setCheckState(Qt::Unchecked);
		item->setBackgroundColor(QColor(BKR, BKG, BKB));
		ui.datasheetTableWidget->setItem(3, i, item);
	 }

     //for(j=HEADROWS; j<min(MAXROW, row)-2; j++)
     for(j=HEADROWS; j<MAXROW-2; j++)
	 {
		if(j-HEADROWS+1 >row) break;

		snum.setNum(j-HEADROWS+1);
        item = new QTableWidgetItem(snum);
	    ui.datasheetTableWidget->setVerticalHeaderItem(j, item);

	    for(i=0; i<col; i++)
		{
          float cdata = this->m_SampleDataSheet->GetData(i)[j-HEADROWS];
	      snum.setNum(cdata, 'e', 5);
		  item = new QTableWidgetItem(snum);
		  ui.datasheetTableWidget->setItem(j, i, item);
		}

	  }

	 if(row >MAXROW-2)
	 {
		snum.clear();
	   	snum.append(tr("..."));
        item = new QTableWidgetItem(snum);
		ui.datasheetTableWidget->setVerticalHeaderItem(MAXROW-2, item);
		for(i=0; i<col; i++)
		{
            item = new QTableWidgetItem(snum);
		    ui.datasheetTableWidget->setItem(MAXROW-2, i, item);
		}

	   	snum.setNum(row);
        item = new QTableWidgetItem(snum);
		ui.datasheetTableWidget->setVerticalHeaderItem(MAXROW-1, item);
		for(i=0; i<col; i++)
		{
            float*cdata = this->m_SampleDataSheet->GetData(i);
	        snum.setNum(cdata[row-1], 'e', 5);
            item = new QTableWidgetItem(snum);
		    ui.datasheetTableWidget->setItem(MAXROW-1, i, item);
		}
	 }

	 this->m_coltype = oldtype;
	 this->m_colx = oldx;
	 this->m_coly = oldy;
	 this->m_colz = oldz;

	 //set the check state
	 if(this->m_coltype >=0)
	 {
        QTableWidgetItem*item = ui.datasheetTableWidget->item(0, this->m_coltype);
		item->setCheckState(Qt::Checked);
	 }
	 //set the check state
	 if(this->m_colx >=0)
	 {
        QTableWidgetItem*item = ui.datasheetTableWidget->item(1, this->m_colx);
		item->setCheckState(Qt::Checked);
	 }
	 //set the check state
	 if(this->m_coly >=0)
	 {
        QTableWidgetItem*item = ui.datasheetTableWidget->item(2, this->m_coly);
		item->setCheckState(Qt::Checked);
	 }
	 //set the check state
	 if(this->m_colz >=0)
	 {
        QTableWidgetItem*item = ui.datasheetTableWidget->item(3, this->m_colz);
		item->setCheckState(Qt::Checked);
	 }
     return;
} 

void VisAtom_SelectSampleDataDlg::OnDatasheetChanged(QTableWidgetItem*item)
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
		for(i=0; i<HEADROWS; i++)
		{
            QTableWidgetItem*item = ui.datasheetTableWidget->item(i, col);
			if(i != row) item->setCheckState(Qt::Unchecked);
		}

		if(row == 0) //type is selected
		{
          QString snum;
          int j;
		  //restore the float format if type colume select before;
		  if(this->m_coltype  >=0)
		  {
             QString snum;
             int j;
             //for(j=HEADROWS; j<min(MAXROW, m_SampleDataSheet->GetNumRow())-2; j++)
             for(j=HEADROWS; j<MAXROW-2; j++)
	         {
		        if(j-HEADROWS+1 > m_SampleDataSheet->GetNumRow()) break;

		        QTableWidgetItem*item = ui.datasheetTableWidget->item(j, this->m_coltype);
                float cdata = this->m_SampleDataSheet->GetData(this->m_coltype)[j-HEADROWS];
			    snum.setNum(cdata, 'e', 5);
			    item->setText(snum);
		      }
		     if(m_SampleDataSheet->GetNumRow() >MAXROW-2)
		     {
			    QTableWidgetItem*item = ui.datasheetTableWidget->item(MAXROW-1, this->m_coltype);
                float cdata = this->m_SampleDataSheet->GetData(this->m_coltype)[m_SampleDataSheet->GetNumRow()-1];
			    snum.setNum(cdata, 'e', 5);
			    item->setText(snum);
		     }
		  }

          //for(j=HEADROWS; j<min(MAXROW, m_SampleDataSheet->GetNumRow())-2; j++)
          for(j=HEADROWS; j<MAXROW-2; j++)
	      {
	         if(j-HEADROWS+1 > m_SampleDataSheet->GetNumRow()) break;
		     QTableWidgetItem*item = ui.datasheetTableWidget->item(j, col);
             int ityp = this->m_SampleDataSheet->GetData(col)[j-HEADROWS] + 0.000001;
			 snum.setNum(ityp);
			 item->setText(snum);
		  }
		  if(m_SampleDataSheet->GetNumRow() >MAXROW-2)
		  {
			  QTableWidgetItem*item = ui.datasheetTableWidget->item(MAXROW-1, col);
              int ityp = this->m_SampleDataSheet->GetData(col)[m_SampleDataSheet->GetNumRow()-1] + 0.000001;
			  snum.setNum(ityp);
			  item->setText(snum);
		  }
		  //
		}
        //////////////////////////////////////
		switch(row)
		{
		case VisAtomDataSheet::COLPROP_TYPE: 
			 this->m_coltype = col;
			 break;
		case VisAtomDataSheet::COLPROP_POSX: 
			 this->m_colx = col;
			 break;
		case VisAtomDataSheet::COLPROP_POSY: 
			 this->m_coly = col;
			 break;
		case VisAtomDataSheet::COLPROP_POSZ: 
			 this->m_colz = col;
			 break;
		}
	}
	else //unchecked
	{
		switch(row)
		{
		case VisAtomDataSheet::COLPROP_TYPE: 
			 this->m_coltype = -1;
			 break;
		case VisAtomDataSheet::COLPROP_POSX: 
			 this->m_colx = -1;
			 break;
		case VisAtomDataSheet::COLPROP_POSY: 
			 this->m_coly = -1;
			 break;
		case VisAtomDataSheet::COLPROP_POSZ: 
			 this->m_colz = -1;
			 break;
		}        
	}
	//write the col labels
	WriteColLabels();
	//if( this->m_coltype>=0 && this->m_colx>=0 && this->m_coly>= 0 && this->m_colz >= 0)
	if( this->m_colx>=0 && this->m_coly>= 0 && this->m_colz >= 0)
		ui.okButton->setEnabled(true);
	else
		ui.okButton->setEnabled(false);

    return;	
}

void VisAtom_SelectSampleDataDlg::WriteColLabels()
{
	//write the col labels
	QString title, snum;
	
	if(this->m_coltype >= 0) {title = tr("Col.#"); snum.setNum(this->m_coltype+1); title.append(snum);}
	else title = tr("N/A");
	ui.coltypeLabel->setText(title);

	if(this->m_colx >= 0) {title = tr("Col.#"); snum.setNum(this->m_colx+1); title.append(snum);}
	else title = tr("N/A");
	ui.colxLabel->setText(title);

	if(this->m_coly >= 0) {title = tr("Col.#"); snum.setNum(this->m_coly+1); title.append(snum);}
	else title = tr("N/A");
	ui.colyLabel->setText(title);

	if(this->m_colz >= 0) {title = tr("Col.#"); snum.setNum(this->m_colz+1); title.append(snum);}
	else title = tr("N/A");
	ui.colzLabel->setText(title);

	return;
}

void VisAtom_SelectSampleDataDlg::OnShowAgainCheckBox(bool no)
{
     m_showthisdlg = !no;
}

