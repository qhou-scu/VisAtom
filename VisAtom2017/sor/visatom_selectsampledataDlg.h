#ifndef VISATOMSELECTSAMPLEDATA_H
#define VISATOMSELECTSAMPLEDATA_H

#include <QtWidgets/QDialog>
#include "Ui_visatom_selectsampledataDlg.h"

class VisAtomDataSheet;
class VisAtom_SelectSampleDataDlg : public QDialog
{
	Q_OBJECT

protected:
    VisAtomDataSheet*m_SampleDataSheet;

	static int m_coltype, m_colx, m_coly, m_colz; 
	static int m_showthisdlg;
public:
	VisAtom_SelectSampleDataDlg(QWidget *parent = Q_NULLPTR);
	~VisAtom_SelectSampleDataDlg();

	void SetDataSheet(VisAtomDataSheet*pData);   
	static void GetDataCol(int col[4])
	{
		col[0] = m_coltype;
		col[1] = m_colx;
		col[2] = m_coly;
		col[3] = m_colz;
	}

	static bool IsShowAgain(){return m_showthisdlg;}


private slots:
	void OnDatasheetChanged(QTableWidgetItem*item);
	void OnShowAgainCheckBox(bool no);
	void WriteColLabels();

	
private:
	Ui::visatom_selectsampledataDlg ui;

};

#endif // VISATOMNEWVECTORPOLICY_H
