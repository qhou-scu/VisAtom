#ifndef VISATOMNEWSIZEPOLICY_H
#define VISATOMNEWSIZEPOLICY_H

#include <QtWidgets/QDialog>
#include "ui_visatom_newsizepolicyDlg.h"

class VisAtomDataSheet;
class VisAtomVisualizingPolicy;
class VisAtomSample;
class VisAtom_NewSizePolicyDlg : public QDialog
{
	Q_OBJECT

protected:
    VisAtomDataSheet*m_SampleDataSheet;
	static double m_datamin, m_datamax;
	static int   m_datacol;

public:
	VisAtom_NewSizePolicyDlg(QWidget *parent = Q_NULLPTR);
	~VisAtom_NewSizePolicyDlg();

	void SetDataSheet(VisAtomDataSheet*pData);   

	void GetData(int&col, float&min, float&max)
	{
		col = m_datacol; min = m_datamin; max = m_datamax;
	}

private slots:
	
	void OnTableSelected();

private:
	Ui::visatom_newsizepolicyForm ui;
	
};

#endif // VISATOMNEWSIZEPOLICY_H
