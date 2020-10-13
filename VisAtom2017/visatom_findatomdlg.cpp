#include "visatom_findatomdlg.h"
#include "VisAtomDataSheet.h"
#include "VisAtomSample.h"
#include "VisAtomWidget.h"

static int ON_CHANGEGROUP = 0;

Visatom_FindatomDlg::Visatom_FindatomDlg(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	this->m_pVisAtomWidget = NULL;
}

Visatom_FindatomDlg::~Visatom_FindatomDlg()
{
	this->m_pVisAtomWidget = NULL;
	this->m_starta.clear();
	this->m_toa.clear();
	this->m_width.clear();
}

void Visatom_FindatomDlg::SetAtomSample(VisAtomWidget*theDisplay)
{
	
	this->m_pVisAtomWidget = theDisplay;
	if (!theDisplay)
	{
		this->setHidden(true);
		return;
	}
	VisAtomSample*theSample = theDisplay->GetSample();

	int grn=0, grind=-1, ng,i;
	grn = this->ui.groupselcomboBox->count();
	if(grn > 0) grind = this->ui.groupselcomboBox->currentIndex();
	ng = theSample->NumberofSubsample();
	//
	QList<int>starta;
	QList<int>width;
	QList<int>toa;
	for (i=0; i<ng+1; i++)
	{
		starta.append(1);
		width.append(1);
		toa.append(1);
	}


	for (i = 0; i <ng+1; i++)
	{
		if (i >= this->m_starta.size()) break;
		starta[i] = this->m_starta[i];
		width[i]  = this->m_width[i];
		toa[i] = this->m_toa[i];
	}
	this->m_starta.clear();
	this->m_width.clear();
	this->m_toa.clear();
	for(i=0; i<ng + 1; i++)
	{
	  this->m_starta.append(starta[i]);
	  this->m_width.append(width[i]);
	  this->m_toa.append(toa[i]);
	}
	starta.clear();
	width.clear();
	toa.clear();

	//update UI
	this->ui.groupselcomboBox->clear();

	QString str, snum;
	str = "in whole box ";
	this->ui.groupselcomboBox->addItem(str);

	for(i=0; i<ng; i++)
	{
		str = "in group # ";
		snum.setNum(i+1);
		str.append(snum);
		this->ui.groupselcomboBox->addItem(str);
	}
	if (grind >= 0)
	{
		this->ui.groupselcomboBox->setCurrentIndex(grind);
	}
}
void Visatom_FindatomDlg::setVisible(bool visible)
{

	if (!this->m_pVisAtomWidget)
	{
		QDialog::setVisible(false);
		return;
	}
	VisAtomSample*theSample = this->m_pVisAtomWidget->GetSample();
	if (!theSample) return;

	if (visible)
	{
		int gr;
		gr = this->ui.groupselcomboBox->currentIndex();
		this->SetGroup(gr);
		this->OnShowOnlyChanged(this->ui.showselectedonlycheckBox->isChecked());
	}
	else
	{
		
		theSample->RemoveAtomsFoundMark();
		theSample->ResetVisibility();
	}
	QDialog::setVisible(visible);
}

void Visatom_FindatomDlg::OnGroupChanged(int ind)
{
	if (ind < 0) return;
	ON_CHANGEGROUP = 1;
	this->SetGroup(ind);
	this->ui.fromatomspinBox->setValue(this->m_starta[ind]);
	this->ui.fromatomspinBox->setSingleStep(this->m_width[ind]);
	this->ui.widthspinBox->setValue(this->m_width[ind]);
	ON_CHANGEGROUP = 0;
	this->m_pVisAtomWidget->update();
}

void Visatom_FindatomDlg::SetGroup(int ind)
{
	if (!this->m_pVisAtomWidget) return;
	VisAtomSample*theSample = this->m_pVisAtomWidget->GetSample();
	if (!theSample) return;

	theSample->RemoveAtomsFoundMark();

	int from, to, step;
	from = this->m_starta[ind];
	to   = from + this->m_width[ind]-1;
	step = this->m_width[ind];
	if (ind == 0)
	{
		theSample->MarkAtomsFound(from, to, 1);
	}
	else
	{
		theSample->MarkGroupAtomsFound(ind, from, to);
	}
	//display the atom info
	this->ui.atominfotreeWidget->clear();
	int gid, type;
	float posx, posy, posz;
	from = 0;
	while (true)
	{
		theSample->GeNextAtomMarkedFound(from, gid, type, posx, posy, posz);
		if (gid <= 0) break;
		else
		{
     		QString sname, snum, snum0;
			QTreeWidgetItem*item = new QTreeWidgetItem();
			sname.clear();
			sname.append(tr("ATOM ID:"));
			snum.setNum(gid);
			item->setText(0, sname);
			item->setText(1, snum);
			this->ui.atominfotreeWidget->addTopLevelItem(item);

			//type 
			QTreeWidgetItem*itemtype = new QTreeWidgetItem();
			sname.clear();
			sname.append(tr("type: "));
			snum.setNum(type);
			itemtype->setText(0, sname);
			itemtype->setText(1, snum);
			item->addChild(itemtype);

		    //postion
			QTreeWidgetItem*itemx = new QTreeWidgetItem();
			sname.clear();
			sname.append(tr("pos-x: "));
			snum.setNum(posx, 'e', 4);
			itemx->setText(0, sname);
			itemx->setText(1, snum);
			item->addChild(itemx);
			QTreeWidgetItem*itemy = new QTreeWidgetItem();
			sname.clear();
			sname.append(tr("pos-y: "));
			snum.setNum(posy, 'e', 4);
			itemy->setText(0, sname);
			itemy->setText(1, snum);
			item->addChild(itemy);
			QTreeWidgetItem*itemz = new QTreeWidgetItem();
			sname.clear();
			sname.append(tr("pos-z: "));
			snum.setNum(posz, 'e', 4);
			itemz->setText(0, sname);
			itemz->setText(1, snum);
			item->addChild(itemz);
			item->setExpanded(true);
		}


		from++;
	}

	if(this->ui.showselectedonlycheckBox->isChecked())
	    theSample->SetShowOnlyAtomsFound(true);
}

void Visatom_FindatomDlg::OnStartatomChanged(int ind)
{
	int gr;
	gr = this->ui.groupselcomboBox->currentIndex();
	this->m_starta[gr] = ind;
	this->SetGroup(gr);
	if(!ON_CHANGEGROUP) this->m_pVisAtomWidget->updateGL();
}

void Visatom_FindatomDlg::OnStepChanged(int ind)
{
	int gr;
	gr = this->ui.groupselcomboBox->currentIndex();
	this->m_width[gr] = ind;
	this->ui.fromatomspinBox->setSingleStep(this->m_width[gr]);
	this->SetGroup(gr);
	if (!ON_CHANGEGROUP) this->m_pVisAtomWidget->updateGL();
}

void Visatom_FindatomDlg::OnToatomChanged(int ind)
{
	int gr;
	gr = this->ui.groupselcomboBox->currentIndex();
	this->m_toa[gr] = ind;
	this->SetGroup(gr);
	if (!ON_CHANGEGROUP) this->m_pVisAtomWidget->updateGL();
}

void Visatom_FindatomDlg::OnShowOnlyChanged(int stat)
{
	if (!this->m_pVisAtomWidget) return;
	VisAtomSample*theSample = this->m_pVisAtomWidget->GetSample();
	if (!theSample) return;

	theSample->SetShowOnlyAtomsFound(stat);
	if (!ON_CHANGEGROUP) this->m_pVisAtomWidget->updateGL();
}





 

