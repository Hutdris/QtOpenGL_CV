#include "mainwindow.h"
#include "stlloader.h"
#include <QApplication>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    
	STLloader stlmodel("porsche.stl");
	//stlmodel.set_path("porsche.stl");
	stlmodel.load();
	
    w.show();

    return a.exec();
}
