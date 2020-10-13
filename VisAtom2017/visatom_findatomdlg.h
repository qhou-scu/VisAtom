#ifndef VISATOM_FINDATOM_H
#define VISATOM_FINDATOM_H

#include <QtWidgets/QDialog>
#include "ui_visatom_findatomDlg.h"

class VisAtomWidget;
class Visatom_FindatomDlg : public QDialog
{
	Q_OBJECT

public:
	Visatom_FindatomDlg(QWidget *parent = Q_NULLPTR);
	~Visatom_FindatomDlg();
	void setVisible(bool);

	void SetAtomSample(VisAtomWidget*theSample);
protected:
	VisAtomWidget * m_pVisAtomWidget;
	QList<int> m_starta;
	QList<int> m_toa;
	QList<int> m_width;

	void SetGroup(int);

private slots:

	void OnGroupChanged(int);
	void OnStartatomChanged(int);
	void OnStepChanged(int);
	void OnToatomChanged(int);
	void OnShowOnlyChanged(int);
     

private:
	Ui::visatom_findatomDlg ui;

signals:
	
};

#endif // VISATOM_FINDATOM_H
