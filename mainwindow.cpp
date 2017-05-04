#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <fstream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), PGmgr(16154282u, 16154288u),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
	tri_points = cv::Mat::zeros(4, 9, CV_32FC1);
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

void MainWindow::initCameras() {
	const int fps = 60;
	tracer.initialize();

	QTimer *tracer_timer = new QTimer(this);
	connect(tracer_timer, SIGNAL(timeout()), this, SLOT(camerasDisplay()));
	tracer_timer->start(int(1000 / fps));
	//tracer_timer->stop();

}
void save_mat(vector<cv::Mat> &vec, const char* file_name) {
	ofstream od(file_name);
	for (auto itt = vec.begin(); itt != vec.end(); itt++) {
		od << *itt << endl;
	}
	od.close();
	vec.clear();
}
void save_keypoint(vector<vector<cv::KeyPoint>> &vec, const char* file_name) {
	ofstream od(file_name);
	for (auto itt = vec.begin(); itt != vec.end(); itt++) {
		for (auto jtt = itt->begin(); jtt != itt->end(); jtt++) {
			od << jtt->pt.x << ' '<<jtt->pt.y << ',';
		}
		od << endl ;
	}
	od.close();
	vec.clear();
}
void MainWindow::recording() {
	if (!rec_flag) {
		rec_flag = true;
		tri_pts_buffer.reserve(100000);
		RT_buffer.reserve(100000);
		cam1_buffer.reserve(100000);
		cam2_buffer.reserve(100000);

		append_textBrowser("Start recording.\n");
	}
	else {
		save_mat(tri_pts_buffer, "result/tri_points.txt");
		save_mat(RT_buffer, "result/RT.txt");
		save_keypoint(cam1_buffer, "result/cam1.txt");
		save_keypoint(cam2_buffer, "result/cam2.txt");
	append_textBrowser("Stop recoding.\n");
	rec_flag = false;
	}
}
void MainWindow::camerasDisplay() {
		//PGmgr.RunMultipleCamera(img1, img2);
    /*
	PGmgr.GetStereoImage(img1, img2);
	qDebug("get images");
	cv::imshow("img1", img1);
	cv::waitKey(10);
	cv::imshow("img2", img2);
	cv::waitKey(10);
*/
	// tracer.pre_frame_check();
	// tracer.image_update_from_video();
	tracer.image_update(PGmgr);
	// 
	int led_cnt = tracer.points_update();
	if (led_cnt) { 
		tracer.leds_triangulate(tri_points);


		std::vector<float> _array;
		_array.clear();

		// RT:4*4 -> 16 array openGL
		for (int i = 0; i < 4; i++) { //col
			for (int j = 0; j < 4; j++) { //row
				_array.push_back(tracer.lower_RT.at<float>(j, i));
			}
		}

		// For tracing points
		ui->openGLWidget->set_tracing_points(tri_points);
		ui->openGLWidget->updateRT(&(_array[0]));
		ui->openGLWidget->update();
		if (rec_flag){
			tri_pts_buffer.push_back(tri_points.clone());
			RT_buffer.push_back(tracer.lower_RT.clone());
			cam1_buffer.push_back(tracer.get_all_pts().first);
			cam2_buffer.push_back(tracer.get_all_pts().second);

		}

	}
}
void MainWindow::reset_init_pos() {
	tracer.reset_init_pos();
}
