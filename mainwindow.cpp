#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>

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

void MainWindow::append_textBrowser(const QString &s) {
	ui->textBrowser->insertPlainText(s);
	ui->textBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::runICP() {
	QMessageBox msgBox;
	msgBox.setWindowTitle("Run ICP");
	msgBox.setText("run icp?");
	msgBox.setStandardButtons(QMessageBox::Yes);
	msgBox.addButton(QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	if (msgBox.exec() == QMessageBox::Yes) {
		icp.load_points(ui->openGLWidget->getModelVertexs(OpenGLWidget::Position::Upper),
			ui->openGLWidget->getModelVertexs(OpenGLWidget::Position::Center));
		append_textBrowser("Load upper as model; center as data");
		icp.readConfig("config.txt");
		icp.run();
		append_textBrowser("complete, output to ");

	}
	else {

	}
}
void MainWindow::keyPressEvent(QKeyEvent *e) {
	ui->openGLWidget->keyPressEvent(e);
}
void MainWindow::on_upperButton_clicked()
{
    QString upperPath = "upper.stl";
    upperPath = QFileDialog::getOpenFileName(this, "Open upper stl file.");
    if(!upperPath.isEmpty()){
        ui->upperLabel->setText(upperPath);
    }
	ui->openGLWidget->set_stlModel(upperPath.toStdString().c_str(),OpenGLWidget::Position::Upper);
	upperPath.prepend("Upper model path: ");
	upperPath.append('\n');
	append_textBrowser(upperPath);
}

void MainWindow::on_lowerButton_clicked()
{
    QString lowerPath = "lower.stl";
    lowerPath = QFileDialog::getOpenFileName(this, "Open lower stl file.");
    if(!lowerPath.isEmpty()){
        ui->lowerLabel->setText(lowerPath);
    }

	ui->openGLWidget->set_stlModel(lowerPath.toStdString().c_str(),OpenGLWidget::Position::Lower);
	lowerPath.prepend("lower model path: ");
	lowerPath.append('\n');
	append_textBrowser(lowerPath);
}


void MainWindow::on_centerButton_clicked()
{
    QString filePath = "center.stl";
    filePath = QFileDialog::getOpenFileName(this, "Open center stl file.");
    if(!filePath.isEmpty()){
        ui->centerLabel->setText(filePath);
    }

	ui->openGLWidget->set_stlModel(filePath.toStdString().c_str(),OpenGLWidget::Position::Center);
	filePath.prepend("center model path: ");
	filePath.append('\n');
	append_textBrowser(filePath);

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->openGLWidget->animate();

}

void MainWindow::on_pushButton_2_pressed()
{

}
