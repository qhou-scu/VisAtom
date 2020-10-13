#ifndef VISATOMNEWVECTORPOLICY_H
#define VISATOMNEWVECTORPOLICY_H

#include <QtWidgets/QDialog>
#include "Ui_visatom_newvectorpolicyDlg.h"

class VisAtomDataSheet;
class VisAtomVisualizingPolicy;
class VisAtomSample;
class VisAtom_NewVectorPolicyDlg : public QDialog
{
	Q_OBJECT

protected:
    VisAtomDataSheet*m_SampleDataSheet;

	static int m_vcol[3]; 
public:
	VisAtom_NewVectorPolicyDlg(QWidget *parent = Q_NULLPTR);
	~VisAtom_NewVectorPolicyDlg();

	void SetDataSheet(VisAtomDataSheet*pData);   
	static void GetVectorColume(int vcol[3])
	{
        vcol[0] = m_vcol[0];
        vcol[1] = m_vcol[1];
        vcol[2] = m_vcol[2];
	}
	static void GetVectorColume(int& vx, int& vy, int& vz)
	{
        vx = m_vcol[0];
        vy = m_vcol[1];
        vz = m_vcol[2];
	}

private slots:
	void OnDatasheetChanged(QTableWidgetItem*);
	void WriteColLabels();

private:
	Ui::visatom_newvectorpolicyForm ui;
	
};

#endif // VISATOMNEWVECTORPOLICY_H
