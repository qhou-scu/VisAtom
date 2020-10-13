#ifndef VISATOMNEWCOLORPOLICY_H
#define VISATOMNEWCOLORPOLICY_H

#include <QtWidgets/QDialog>
#include "ui_visatom_newcolorpolicyDlg.h"

class VisAtomDataSheet;
class VisAtomVisualizingPolicy;
class VisAtomSample;
class VisAtom_NewColorPolicyDlg : public QDialog
{
	Q_OBJECT

protected:
    VisAtomDataSheet*m_SampleDataSheet;
	VisAtomSample*m_Sample;

	static double m_winmin, m_winmax;
	static double m_datamin, m_datamax;
	static int   m_datacol;

public:
	VisAtom_NewColorPolicyDlg(QWidget *parent = Q_NULLPTR);
	~VisAtom_NewColorPolicyDlg();

	void SetDataSheet(VisAtomDataSheet*pData);   
	void SetSample(VisAtomSample*sample); 

	void GetData(int&col, float&min, float&max)
	{
		col = m_datacol; min = m_datamin; max = m_datamax;
	}

private slots:
	
	void OnTableSelected();
	void OnSubsampleListItemChanged(QTreeWidgetItem*item);

private:
	Ui::visatom_newcolorpolicyForm ui;
	void GetFilteredScalarRange(int col, double&fmin, double&fmax);
	float*m_atype;
	float*m_px;
	float*m_py;
	float*m_pz;


    //
	
};

#endif // VISATOMNEWCOLORPOLICY_H
