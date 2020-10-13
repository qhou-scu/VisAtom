#ifndef VISATOM_ATOM_FILTER_H
#define VISATOM_ATOM_FILTER_H

#include <QtWidgets/QDialog>
#include "ui_visatom_atomfilterDlg.h"

//class VisAtomSample;
class Visatom_AtomfilterDlg : public QDialog
{
	Q_OBJECT

public:
	Visatom_AtomfilterDlg(QWidget *parent = Q_NULLPTR);
	~Visatom_AtomfilterDlg();


private:
	Ui::visatom_atomfilterDlg ui;
};

#endif // VISATOM_ATOM_FILTER_H
