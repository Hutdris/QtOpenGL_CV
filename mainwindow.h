#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_upperButton_clicked();
    void on_lowerButton_clicked();
    void on_centerButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
