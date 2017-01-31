#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_upperButton_clicked()
{
    QString upperPath;
    upperPath = QFileDialog::getOpenFileName(this, "Open upper stl file.");
    if(!upperPath.isEmpty()){
        ui->upperLabel->setText(upperPath);
    }
}

void MainWindow::on_lowerButton_clicked()
{
    QString lowerPath;
    lowerPath = QFileDialog::getOpenFileName(this, "Open lower stl file.");
    if(!lowerPath.isEmpty()){
        ui->lowerLabel->setText(lowerPath);
    }

}

void MainWindow::on_centerButton_clicked()
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Open center stl file.");
    if(!filePath.isEmpty()){
        ui->centerLabel->setText(filePath);
    }

}
