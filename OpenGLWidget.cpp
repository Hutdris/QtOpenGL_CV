#include "OpenGLWidget.h"
#include <algorithm>
#include <QTimer>
#include <fstream>

void OpenGLWidget::SetLightSource()
{
	float light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	float light_diffuse[] = { 0.4, 0.4, 0.4, 1.0 };
	float light_specular[] = { 0.5, 0.5, 0.5, 1.0 };

	GLfloat MaterialAmbient[] = { 1.0,1.0,1.0,1.0f };
	GLfloat MaterialDiffuse[] = { 0.7,0.7,0.7,1.0f };
	GLfloat MaterialSpecular[] = { 0.5,0.5,0.5, 1.0f };
	GLfloat AmbientLightPosition[] = { 100,100,100,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, MaterialAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, MaterialDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, MaterialSpecular);

	glEnable(GL_LIGHTING);                                 //�}�O 
	glEnable(GL_LIGHT0);

														   // �]�w�o���骺�������S�� 
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);      //���ҥ�(Ambient Light) 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);      //���g��(Diffuse Light) 
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);     //�Ϯg��(Specular Light) 
	float light_position[] = { 10.0f, 10.0f, 10.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);     //�����y�� 

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);                               //�Ұʲ`�״��� 
}

void OpenGLWidget::SetMaterial()
{
	GLfloat MaterialAmbient[] = { 1.0,1.0,1.0,1.0f };
	GLfloat MaterialDiffuse[] = { 0.7,0.7,0.7,1.0f };
	GLfloat MaterialSpecular[] = { 1.2,1.2,1.2, 1.0f };
	GLfloat AmbientLightPosition[] = { 1000,1000,1000,1.0f };

	//glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
}
OpenGLWidget::STLModel::STLModel() {
}
OpenGLWidget::STLModel:: ~STLModel() { vertexs.clear(); normals.clear();}

vector <GLfloat> OpenGLWidget::getModelVertexs(Position mPos) {
	switch (mPos) {
	case Upper:
		return upper.vertexs;
	case Lower:
		return lower.vertexs;
	case Center:
		return center.vertexs;

	}
}
OpenGLWidget::OpenGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),fps(60)
{
	QTimer *timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	//timer->start(int(1000/fps));
	model_list.resize(3);
	init_pos = cv::Mat::zeros(3, 9, CV_32FC1);
	mass_point = cv::Mat::zeros(3, 1, CV_32FC1);
	tracing_points = &(init_pos);
	tracing_init_pos.setX(0);
	tracing_init_pos.setY(0);
	tracing_init_pos.setZ(30);
	tracing_pts.push_back(tracing_init_pos);
}


OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::tracing_output_reset() {
	ofstream ort("result/tracing_pts.txt");
	for (auto itt = tracing_pts.begin(); itt != tracing_pts.end(); itt++) {
		ort << itt->x()<<','<<itt->y() <<','<<itt->z()<< endl;
	}
	ort.close();
	tracing_pts.clear();
	tracing_pts.push_back(tracing_init_pos);
}
void OpenGLWidget::set_tracing_points(cv::Mat points) {
	if (points.cols != 9) {
		return;
	}
	cv::Mat _mass_point = cv::Mat::zeros(3, 1, CV_32FC1);
	for (int i = 0; i < 3; i++) {
		float _sum = 0;
		for (int j = 6 - 1; j < 9; j++) {
			_sum +=
				(points.at<float>(i, j) - init_pos.at<float>(i, j));
		}
		_sum /= 4.0f;
		_mass_point.at<float>(i, 0) = _sum;
	}
	mass_point = _mass_point.clone();
	trace.push_back(mass_point);
	QVector3D cur_pt;
	cur_pt.setX((lower.RT[0] * tracing_init_pos.x() + lower.RT[4] * tracing_init_pos.y()+ lower.RT[8] * tracing_init_pos.z() + lower.RT[12]));
	cur_pt.setY((lower.RT[1] * tracing_init_pos.x() + lower.RT[5] * tracing_init_pos.y()+ lower.RT[9] * tracing_init_pos.z() + lower.RT[13]));
	cur_pt.setZ((lower.RT[2] * tracing_init_pos.x() + lower.RT[6] * tracing_init_pos.y() + lower.RT[10] * tracing_init_pos.z() + lower.RT[14]));
	tracing_pts.push_back(cur_pt);
	
};
void OpenGLWidget::drawTrace() {
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	if (tracing_pts.size() > 5) {
	for(auto itt = tracing_pts.begin(); itt != tracing_pts.end(); itt++){
		glVertex3f(itt->x(), itt->y(), itt->z());             // startpoint
	}

		/*
	for (auto itt = trace.begin(); itt != trace.end(); itt++){
		glVertex3f(itt->at<float>(0, 0), itt->at<float>(1, 0), itt->at<float>(2, 0));             // startpoint
		//qDebug("%f, %f, %f", itt->at<float>(0, 0), itt->at<float>(1, 0), itt->at<float>(2, 0));
	}*/

	}
	glEnd();
	if (tracing_pts.size() > 5){
		auto itt = tracing_pts.end() - 1;
		qDebug("%f, %f, %f", itt->x(), itt->y(), itt->z());
	// qDebug("%f, %f, %f", last.at<float>(0, 0) - lastlast.at<float>(0, 0), last.at<float>(1, 0) - lastlast.at<float>(1, 0), last.at<float>(2, 0) - lastlast.at<float>(2, 0));
	}
}
GLuint OpenGLWidget::makeObject(STLModel *model) {
	GLuint _list;
	_list = glGenLists(1);

	glNewList(_list, GL_COMPILE);
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
	auto normal_itt = model->normals.begin();
	for (auto itt = model->vertexs.begin(); itt != model->vertexs.end(); itt += 9){
		glNormal3f(*normal_itt, *(normal_itt+1), *(normal_itt+2));
		normal_itt += 3;
		glVertex3f(*itt, *(itt + 1), *(itt + 2));
		glVertex3f(*(itt+3), *(itt + 4), *(itt + 5));
		glVertex3f(*(itt+6), *(itt + 7), *(itt + 8));
	}
	glEnd();
	glEndList();

	return _list;


}

void OpenGLWidget::initializeGL()
{
	/* original
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
*/
	// Web version 
	//Laggy version

	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// ref: http://blog.csdn.net/shuaihj/article/details/7230867
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 

	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	set_stlModel("Models/oriented_upper.stl", Upper);
	set_stlModel("Models/oriented_lower.stl", Lower);
	set_stlModel("Models/cube.stl", Center);

	model_list.at(Upper) = makeObject(&upper);
	model_list.at(Lower) = makeObject(&lower);
	model_list.at(Center) = makeObject(&center);
	glShadeModel(GL_SMOOTH);

	//

}

void OpenGLWidget::updateRT(float *RT) {
	for (int i = 0; i < 16; i++) {
		lower.RT[i] = RT[i];
	}
}

void OpenGLWidget::paintGL()
{
	int pointSize = 1E3;
	glClearColor(0.0,0.0,0.0,1.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_BACK, GL_LINE); //show model's back by lines 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 50.0f, 0, 0, 0, 0, 1, 0);   //���u���y�ФΤ�V
	SetLightSource();
	SetMaterial();
	//glEnable(GL_BLEND);//�ҰʲV�M�\�� �ݭn�b�z���\��ɨϥ�
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//draw_model(lower);
	//draw_model(upper);
	//draw_model(center);

	glRotatef(rotate.y(), 1.0 ,0.0, 0.0);
	glRotatef(rotate.x(), 0.0, 1.0, 0.0);
	glRotatef(rotate.z(), 0.0, 0.0, 1.0);
	//glTranslatef(-1*rTri/10.f, 0.0f, 0.0f);
	auto _zoomratio = 1.0f / Zoom_ratio;
	glScalef(_zoomratio, _zoomratio, _zoomratio);
	//glTranslated(mass_point.at<double>(0, 0), mass_point.at<double>(1, 0), mass_point.at<double>(2, 0));
	for (int model_index = Upper; model_index < EndCase; model_index++) {
		glPushMatrix();
		switch (model_index) {
		case Upper:
			glMultMatrixf(upper.RT);

			break;
		case Center:
			glMultMatrixf(center.RT);
			break;
		case Lower:
			glMultMatrixf(lower.RT);
			break;
		}

		glCallList(model_list.at(model_index));
		glPopMatrix();
	}
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);
	drawTrace();
	glEnable(GL_LIGHTING);

}


void OpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}
void OpenGLWidget::keyPressEvent(QKeyEvent* e)
{

	switch (e->key())
	{
	case Qt::Key_Q:
		rTri += 1.0f;
		update();
		break;
	case Qt::Key_W:
		if (Zoom_ratio < 100)
			Zoom_ratio += 1;
		update();
		break;
	case Qt::Key_S:
		if (Zoom_ratio > 1)
			Zoom_ratio -= 1;
		update();
		break;
	case Qt::Key_Z:
		qDebug("rotate: %f, %f, %f", rotate.x(), rotate.y(), rotate.z());
	}
	checkZoomRatio();
}
void OpenGLWidget::animate() {
	update();
	//paintGL();
}

void OpenGLWidget::model_normalize(STLModel &model) {
	//Normalize vertexs from (min, max) to (-1, 1)
	//TODO: Different model should divide by the same ratio, not max(vertexs)!!

	//auto max_corr = *(std::max_element(model.vertexs.begin(), model.vertexs.end()));
	for (auto i = 0; i < model.vertexs.size(); i++) {
		model.vertexs[i] /= (GLfloat)Zoom_ratio;
	}
}


void OpenGLWidget::set_stlModel(const char *model_path, Position p) {

		stlloader.set_path(model_path);
		stlloader.load();
	STLModel *ptm;
	switch (p) {
	case (Upper):
		ptm = &upper;
		break;
	case (Lower):
		ptm = &lower;
		break;
	case(Center):
		ptm = &center;
		break;
	}
	ptm->vertexs = stlloader.get_vertexs();
	ptm->normals = stlloader.get_normals();
	glDeleteLists(model_list.at(p), 1);
	model_list.at(p) = makeObject(ptm);
}

void OpenGLWidget::model_resize(STLModel &model, int old_ratio) {

	GLfloat muti = ((GLfloat)old_ratio / Zoom_ratio);
	for (auto i = 0; i < model.vertexs.size(); i++) {
		model.vertexs[i] *= muti;
	}
}
void OpenGLWidget::setZoomRatio(int _zoom_ratio) {
	GLfloat old_ratio = Zoom_ratio;
	Zoom_ratio = _zoom_ratio; 
	update();

};

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
	Q_UNUSED(event);
	qDebug("Start pos: %d, %d", event->x(), event->y());
	// Double click -> reset rotation
	if ((event->x() == m_start_pos.x()) & (event->y() == m_start_pos.y())) {
		rotate.setX(0);
		rotate.setY(0);
		rotate.setZ(0);
	}
	m_start_pos = event->pos();
	update();
}
void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
	Q_UNUSED(event);

	auto step = 50.f;
	auto x_delta = event->x() - m_start_pos.x();
	auto y_delta = event->y() - m_start_pos.y();
	qDebug("Move!!: %d, %d", x_delta, y_delta);
	if (((abs(x_delta) + abs(y_delta)) > 10)){
		//event->button() didn't work!??
		//event->buttons() !!!!
		if (event->buttons() & Qt::LeftButton) {
			rotate.setX(qNormalizeAngle(rotate.x() + x_delta / step));
			rotate.setY(qNormalizeAngle(rotate.y() + y_delta / step));
		}
		else if (event->buttons() & Qt::RightButton) {
			rotate.setX(qNormalizeAngle(rotate.x() + x_delta / step));
			rotate.setZ(qNormalizeAngle(rotate.z() + y_delta / step));
		}
		else if (event->buttons() & Qt::MiddleButton) {
			Zoom_ratio = (Zoom_ratio + x_delta/(int)step);
			checkZoomRatio();
		}
	}
	update();
}
void OpenGLWidget::wheelEvent(QWheelEvent *whell_event) {
	Q_UNUSED(whell_event);
	qDebug("Wheel move: %d", whell_event->delta());
	Zoom_ratio += whell_event->delta() / 40;
	checkZoomRatio();

}
void OpenGLWidget::draw_model(STLModel &model) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5.0f, 0, 0, 0, 0, 1, 0);   //���u���y�ФΤ�V
	glRotatef(rTri, 0.5, 1.0, 0.3);
	auto _zoomratio = 1.0f / Zoom_ratio;

	glScalef(_zoomratio, _zoomratio, _zoomratio);
	glColor3f(1.0, 0.5, 0.0);
	glBegin(GL_TRIANGLES);
	auto normal_itt = model.normals.begin();
for (auto itt = model.vertexs.begin(); itt != model.vertexs.end(); itt += 9){

	glNormal3f(*normal_itt, *(normal_itt+1), *(normal_itt+2));
	normal_itt += 3;
	glVertex3f(*itt, *(itt + 1), *(itt + 2));
	glVertex3f(*(itt+3), *(itt + 4), *(itt + 5));
	glVertex3f(*(itt+6), *(itt + 7), *(itt + 8));
}
	glEnd();
	glScalef(Zoom_ratio, Zoom_ratio, Zoom_ratio);
	//glutSwapBuffers();
	//glLoadIdentity();
}
void OpenGLWidget::checkZoomRatio() {
	if (Zoom_ratio < 1) Zoom_ratio = 1;
	if (Zoom_ratio > 100) Zoom_ratio = 100;
	emit getZoomRatio(Zoom_ratio);
	update();
}
