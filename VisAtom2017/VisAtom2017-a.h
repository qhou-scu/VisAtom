#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisAtom2017.h"

class VisAtom2017 : public QMainWindow
{
	Q_OBJECT

public:
	VisAtom2017(QWidget *parent = Q_NULLPTR);

private:
	Ui::VisAtom2017Class ui;
};
