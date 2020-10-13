#include "visatom_atominfo.h"
#include "VisAtomDataSheet.h"
#include "VisAtomSample.h"
#include "VisAtomTrajectory.h"

VisAtom_AtomInfo::VisAtom_AtomInfo(QWidget *parent)
	: QWidget(parent, Qt::FramelessWindowHint)
{
	ui.setupUi(this);
	
}

VisAtom_AtomInfo::~VisAtom_AtomInfo()
{

}

void VisAtom_AtomInfo::ShowAtomInfo(int posx, int posy, VisAtomSample*pSample, int atomid)
{
   		 this->ui.treeWidget->clear();

		 int col;
		 if(atomid >0)
		 {

		   QString snum, snum0;
		   QTreeWidgetItem*item = new QTreeWidgetItem();
		   item->setText(0, tr("ATOM ID:"));
		   snum.setNum(atomid);
		   item->setText(1, snum);
           this->ui.treeWidget->addTopLevelItem(item);
           
		   //following the index should minus 1
		   VisAtomDataSheet*pDataSheet = pSample->GetDataSheet();
           atomid = atomid-1;
		   //type 
		   QTreeWidgetItem*item1 = new QTreeWidgetItem();
		   item1->setText(0, tr("TYPE ID:"));
		   col = pDataSheet->GetTypeCol();
		   if(col >= 0)
		     snum.setNum((int)(pDataSheet->GetData(col)[atomid]+0.00001));
		   else
			   snum.setNum(1);
		   item1->setText(1, snum);
           this->ui.treeWidget->addTopLevelItem(item1);

		   //postion
		   QTreeWidgetItem*item2 = new QTreeWidgetItem();
		   item2->setText(0, tr("POS:"));
		   snum = tr("(");
		   col = pDataSheet->GetPosXCol();
		   snum0.setNum(pDataSheet->GetData(col)[atomid]);
		   snum.append(snum0);
		   snum.append(tr(", "));
		   col = pDataSheet->GetPosYCol();
		   snum0.setNum(pDataSheet->GetData(col)[atomid]);
		   snum.append(snum0);
		   snum.append(tr(", "));
		   col = pDataSheet->GetPosZCol();
		   snum0.setNum(pDataSheet->GetData(col)[atomid]);
		   snum.append(snum0);
		   snum.append(tr(")"));
		   item2->setText(1, snum);
           this->ui.treeWidget->addTopLevelItem(item2);

		   //scalar
		   QTreeWidgetItem*item3 = new QTreeWidgetItem();
		   item3->setText(0, tr("SCARLAR:"));
		   col = VisAtomVisualizingPolicy::GetScalarCol();
		   if(col >=0)
		     snum.setNum(pDataSheet->GetData(col)[atomid], 'e', 6);
		   else 
             snum = tr("undefined"); 
		   item3->setText(1, snum);
           this->ui.treeWidget->addTopLevelItem(item3);

		   //vector
		   QTreeWidgetItem*item4 = new QTreeWidgetItem();
		   item4->setText(0, tr("VECTOR:"));
		   int vcol[3];
		   VisAtomVisualizingPolicy::GetVectorCol(vcol);
		   if(vcol[0]>=0 && vcol[1]>=0 && vcol[2]>=0)
		   {
		      snum = tr("(");
		      snum0.setNum(pDataSheet->GetData(vcol[0])[atomid], 'e',4);
		      snum.append(snum0);
		      snum.append(tr(", "));
		      snum0.setNum(pDataSheet->GetData(vcol[1])[atomid], 'e',4);
		      snum.append(snum0);
		      snum.append(tr(", "));
		      snum0.setNum(pDataSheet->GetData(vcol[2])[atomid], 'e',4);
		      snum.append(snum0);
		      snum.append(tr(")"));
		   }
		   else
              snum = tr("undefined");
		   item4->setText(1, snum);
           this->ui.treeWidget->addTopLevelItem(item4);

           //trajectory displacement
		   VisAtomTrajectory*trajectoryDate = pSample->GetTrajectoryData();
		   QTreeWidgetItem*item5 = new QTreeWidgetItem();
		   item5->setText(0, tr("DISPLACEMENT:"));
		   float dx, dy, dz, len;
		   if(trajectoryDate)
		   {
			   if(trajectoryDate->GetDisplacement(atomid,dx,dy,dz))
		       {
		          snum = tr("(");
		          snum0.setNum(dx, 'e',4);
		          snum.append(snum0);
		          snum.append(tr(", "));
		          snum0.setNum(dy, 'e',4);
		          snum.append(snum0);
		          snum.append(tr(", "));
		          snum0.setNum(dz, 'e',4);
		          snum.append(snum0);
		          snum.append(tr("), len="));
		          snum0.setNum(pow(dx*dx+dy*dy+dz*dz,0.5f), 'e',4);
		          snum.append(snum0);
		       }
			   else 
                  snum = tr("undefined");
		   }
		    else
               snum = tr("undefined");
		   item5->setText(1, snum);
           this->ui.treeWidget->addTopLevelItem(item5);

          //trajectory length
		   QTreeWidgetItem*item6 = new QTreeWidgetItem();
		   item6->setText(0, tr("PATH LEN:"));
		   if(trajectoryDate)
		   {
			   if(trajectoryDate->GetPathlength(atomid,len))
		          snum.setNum(len, 'e',4);
			   else 
                  snum = tr("undefined");
		   }
		    else
               snum = tr("undefined");
		   item6->setText(1, snum);
           this->ui.treeWidget->addTopLevelItem(item6);

		 }
		 else
		 {
		   QTreeWidgetItem*item = new QTreeWidgetItem();
		   item->setText(0, tr("ID"));
		   item->setText(1, tr("No atom picked"));
           this->ui.treeWidget->addTopLevelItem(item);
		 }

		 QRect rect = this->rect();
		 QRect rect0 = QApplication::desktop()->screenGeometry();
		 if(posx + rect.width() > rect0.width()) posx = posx - rect.width();
		 if(posy + rect.height() > rect0.height()) posy = posy - rect.height();
		 rect.moveTo(posx, posy);
		 this->setGeometry(rect);
		 this->show();

}

void VisAtom_AtomInfo::StarPickAtomInfo(int posx, int posy)
{
     this->ui.treeWidget->clear();

	 QTreeWidgetItem*item = new QTreeWidgetItem();
     item->setText(0, tr("Pick atom,...."));
     this->ui.treeWidget->addTopLevelItem(item);

	 QRect rect = this->rect();
	 rect.moveTo(posx, posy);
	 this->setGeometry(rect);
	 this->show();
}
