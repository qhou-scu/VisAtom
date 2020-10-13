#include "visatom_newvectorpolicydlg.h"
#include "VisAtomVisualizingPolicy.h"
#include "VisAtomDataSheet.h"
#include "VisAtomSubsample.h"
#include <stdlib.h>

int VisAtom_NewVectorPolicyDlg::m_vcol[3]={-1,-1,-1};
#define MAXROW   53
#define HEADROWS 3
#define BKR 220
#define BKG 220
#define BKB 220

VisAtom_NewVectorPolicyDlg::VisAtom_NewVectorPolicyDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    
    int i;

	QString title(tr("Choose the data for vector for visualizing"));
	this->setWindowTitle(title);

	return;
}

VisAtom_NewVectorPolicyDlg::~VisAtom_NewVectorPolicyDlg()
{
  
}

void VisAtom_NewVectorPolicyDlg::SetDataSheet(VisAtomDataSheet*pData)
{   
	 this->m_SampleDataSheet = pData;
	 int col = this->m_SampleDataSheet->GetNumCol();
	 int row = this->m_SampleDataSheet->GetNumRow();

	 int i, j;
	 QString title, snum;
	 QTableWidgetItem*hitem, *item;
	 ui.datasheetTableWidget->setColumnCount(col);
     ui.datasheetTableWidget->setRowCount(std::min(MAXROW,row));

	 //reserve the previous selection
	 int oldcol[HEADROWS];
	 for(i=0; i<HEADROWS; i++) oldcol[i] = m_vcol[i];

	 for(i=0; i<col; i++)
	 {
        title.clear();
        title.append(tr("Col #"));
	    snum.setNum(i+1);
        title.append(snum);
		if(i == pData->GetTypeCol())
           title.append(tr("(atom type)"));
		else if(i == pData->GetPosXCol())
		   title.append(tr("(pos x)"));
		else if(i == pData->GetPosYCol())
		   title.append(tr("(pos y)"));
		else if(i == pData->GetPosZCol())
			title.append(tr("(pos z)"));

		 hitem = new QTableWidgetItem(title);
		 ui.datasheetTableWidget->setHorizontalHeaderItem(i, hitem); 
	 }

	 title.clear();
     item = new QTableWidgetItem(title);
	 ui.datasheetTableWidget->setVerticalHeaderItem(0, item);
	 title.append(tr("Vx"));
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
	 title.append(tr("Vy"));
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
	 title.append(tr("Vz"));
	 for(i=0; i<col; i++)
	 {
        item = new QTableWidgetItem(title);
		item->setCheckState(Qt::Unchecked);
		item->setBackgroundColor(QColor(BKR, BKG, BKB));
		ui.datasheetTableWidget->setItem(2, i, item);
	 }

     for(j=HEADROWS; j<std::min(MAXROW, row)-2; j++)
	 {
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

	 for(i=0; i<HEADROWS; i++) 
	 {
		 this->m_vcol[i] = oldcol[i];
		 if(this->m_vcol[i] >=0)
		 {
            QTableWidgetItem*item = ui.datasheetTableWidget->item(i, this->m_vcol[i]);
		   item->setCheckState(Qt::Checked);
		 }
	 }
     return;
} 

void VisAtom_NewVectorPolicyDlg::OnDatasheetChanged(QTableWidgetItem*item)
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
		m_vcol[row] = col;
	}
	else //unchecked
	{
		m_vcol[row] = -1;
	}
	//write the col labels
	WriteColLabels();
	if( this->m_vcol[0]>=0 && this->m_vcol[1]>=0 && this->m_vcol[2]>= 0)
		ui.okButton->setEnabled(true);
	else
		ui.okButton->setEnabled(false);

    return;	
}

void VisAtom_NewVectorPolicyDlg::WriteColLabels()
{
	//write the col labels
	QString title, snum;
	
	if(this->m_vcol[0] >= 0) {title = tr("Col.#"); snum.setNum(this->m_vcol[0]+1); title.append(snum);}
	else title = tr("N/A");
	ui.vxcolLabel->setText(title);

	if(this->m_vcol[1] >= 0) {title = tr("Col.#"); snum.setNum(this->m_vcol[1]+1); title.append(snum);}
	else title = tr("N/A");
	ui.vycolLabel->setText(title);

	if(this->m_vcol[2] >= 0) {title = tr("Col.#"); snum.setNum(this->m_vcol[2]+1); title.append(snum);}
	else title = tr("N/A");
	ui.vzcolLabel->setText(title);

	return;
}




