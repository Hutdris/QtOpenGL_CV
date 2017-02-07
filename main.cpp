#include "mainwindow.h"
#include "stlloader.h"
#include "pgapi.h"
#include <QApplication>
#include <iostream>
#include <string>
#include "opencv\cv.h"
#include "ICP.h"

#include "GoICP/jly_goicp.h"
int main(int argc, char *argv[])
{

	ICP myicp(1000, 0.1, 10);
	myicp.readfile("porsche.stl", "porsche.stl");
	myicp.run();
	myicp.writefile("out.obj");

	QApplication a(argc, argv);
	MainWindow w;
	/*
	unsigned int s1 = 16154282u;
	unsigned int s2 = 16154288u;
	cv::Mat img1, img2;
	PGApi PGmgr(s1, s2);
	PGmgr.PrintBuildInfo();



	while(true){
	PGmgr.GetStereoImage(img1, img2);
	cv::imshow("img1", img1);
	cv::waitKey(10);
	cv::imshow("img2", img2);
	cv::waitKey(10);}
	
	//PGmgr.RunMultipleCamera(img1, img2);
	*/
    w.show();
    return a.exec();

}
