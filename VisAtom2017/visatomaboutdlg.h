#ifndef VISATOMABOUTDLG_H
#define VISATOMABOUTDLG_H

#include <QtWidgets/QDialog>
#include "ui_visatomaboutdlg.h"

class VisAtomAboutDlg : public QDialog
{
	Q_OBJECT

public:
	VisAtomAboutDlg(QWidget *parent = 0);
	~VisAtomAboutDlg();

private:
	Ui::visatomaboutForm ui;
};

#endif // VISATOMABOUTDLG_H
