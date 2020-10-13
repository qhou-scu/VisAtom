#ifndef VISATOM_ATOM_INFO_H
#define VISATOMABOUTDLG_H

#include <QtWidgets/QWidget>
#include "ui_visatom_atominfo.h"

class VisAtomSample;
class VisAtom_AtomInfo : public QWidget
{
	Q_OBJECT

public:
	VisAtom_AtomInfo(QWidget *parent = Q_NULLPTR);
	~VisAtom_AtomInfo();

	void StarPickAtomInfo(int posx, int posy);
	void ShowAtomInfo(int posx, int posy,VisAtomSample*pSample, int atomid);

private:
	Ui::visatom_atominfo ui;
};

#endif // VISATOMABOUTDLG_H
