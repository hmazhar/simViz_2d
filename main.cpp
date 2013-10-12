#include "visualizer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	visualizer w;
	w.show();
	return a.exec();
}