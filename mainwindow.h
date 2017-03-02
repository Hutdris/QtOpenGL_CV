#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
//#include "OpenGLWidget.h"
#include <String>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
	virtual void keyPressEvent(QKeyEvent *e);
	
private slots:
    void on_upperButton_clicked();
    void on_lowerButton_clicked();
    void on_centerButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_2_pressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
