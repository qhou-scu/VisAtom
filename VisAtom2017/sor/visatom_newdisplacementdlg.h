#ifndef VISATOMNEWDISPLACEMENTPOLICY_H
#define VISATOMNEWDISPLACEMENTPOLICY_H

#include <QtWidgets/QDialog>
#include <QString.h>

#include "Ui_visatom_newdisplacementDlg.h"

class VisAtomDataSheet;
class VisAtomVisualizingPolicy;
class VisAtomSample;
class VisAtom_NewDisplacementPolicyDlg : public QDialog
{
	Q_OBJECT

protected:

	static int m_discol[3]; 
	static QString m_filename;
	static int m_startInd;
	static int m_endInd;
	static int m_indStep;
	static int m_IndRng[3]; //available file index

public:
	VisAtom_NewDisplacementPolicyDlg(QWidget *parent = Q_NULLPTR);
	~VisAtom_NewDisplacementPolicyDlg();

	static void GetDisplacementColume(int dcol[3])
	{
        dcol[0] = m_discol[0];
        dcol[1] = m_discol[1];
        dcol[2] = m_discol[2];
	}
	static void GetDisplacementColume(int& dx, int& dy, int& dz)
	{
        dx = m_discol[0];
        dy = m_discol[1];
        dz = m_discol[2];
	}

	static int GetStartInd() {return m_startInd;};
	static int GetEndInd() {return m_endInd;};
	static int GetIndexStep() {return m_indStep;};
	static QString GetFname() {return m_filename;};

    enum ERRORCODE
	{
	      ERR_NOERROR     = 0,
		  ERR_NOTINDEXEDFILE       = 1,
		  ERR_INCONSISTENTFOMAT    = 2,
    };


	int SetFilename(QString fname,VisAtomDataSheet*pDataSheet);
	static bool HasTrajectData();

private slots:
	void UpdateUI();
	int FillPrevewTable(VisAtomDataSheet*pDataSheet);
	void OnDatasheetChanged(QTableWidgetItem*);
	void OnFromChanged(int from);
	void OnStepChanged(int step);
	void accept();

private:
	Ui::visatom_newdisplacementpolicyForm ui;
	int m_selcol[3];
	int m_fromF, m_toF;
	
};

#endif // VISATOMNEWVECTORPOLICY_H
