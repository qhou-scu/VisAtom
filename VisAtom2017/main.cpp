#include "VisAtom2017.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VisAtom2017 w;
	w.show();
	return a.exec();
}
