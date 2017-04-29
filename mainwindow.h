#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <String>
#include <vector>

#include "ICP.h"
#include "pgapi.h"
#include "Tracer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	cv::Mat tri_points;
	vector <cv::Mat> tri_pts_buffer, RT_buffer;
	vector<vector<cv::KeyPoint>> cam1_buffer, cam2_buffer;
protected:
	virtual void keyPressEvent(QKeyEvent *e);
	
private slots:
    void on_upperButton_clicked();
    void on_lowerButton_clicked();
    void on_centerButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_2_pressed();
	
	void runICP();
	void initCameras();
	void camerasDisplay();

	void recording();
	void reset_init_pos();
private:
    Ui::MainWindow *ui;
	void append_textBrowser(const QString &s);
	ICP icp;
	PGApi PGmgr;
	Tracer tracer;
	cv::Mat img1, img2;
	bool rec_flag = false;
};

#endif // MAINWINDOW_H
