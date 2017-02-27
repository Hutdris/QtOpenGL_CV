#include "OpenGLWidget.h"
#include <algorithm>

#include <QTimer>
OpenGLWidget::STLModel::STLModel() {

}
OpenGLWidget::STLModel:: ~STLModel() { vertexs.clear(); normals.clear();}

OpenGLWidget::OpenGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(int(1000/24));
}


OpenGLWidget::~OpenGLWidget()
{

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
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	set_stlModel("Models/cube.stl", Lower);
	set_stlModel("Models/cube.stl", Upper);
	set_stlModel("Models/cube.stl", Center);

	rTri = 0.0;
	//

}

void OpenGLWidget::draw_model(STLModel &model) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5.0f, 0, 0, 0, 0, 1, 0);   //視線的座標及方向
	glRotatef(rTri, 0.5, 1.0, 0.3);
	auto _zoomratio = 1.0f / Zoom_ratio;

	glScalef(_zoomratio, _zoomratio, _zoomratio);
	glBegin(GL_TRIANGLES);
for (auto itt = model.vertexs.begin(); itt != model.vertexs.end(); itt += 9){
	glColor3f(0.5, 0.5, 0.0);
	glVertex3f(*itt, *(itt + 1), *(itt + 2));
	glVertex3f(*(itt+3), *(itt + 4), *(itt + 5));
	glVertex3f(*(itt+6), *(itt + 7), *(itt + 8));
}
	glEnd();
	glScalef(Zoom_ratio, Zoom_ratio, Zoom_ratio);
	//glLoadIdentity();
}
void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_model(lower);
	draw_model(upper);
	draw_model(center);
	rTri += Zoom_ratio/10.0f;
	//glLoadIdentity();
	//glTranslatef(-1.5, 0.0, 0.0);
	/*
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-0.5, -0.5, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, -0.5, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.5, 0);
	*/
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
void OpenGLWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_Q:
		rTri += 1.0f;
		update();
		break;
	case Qt::Key_Escape:
		close();
	}
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
		//STLModel *ptm;
		//TODO: using pointer instead of copy&paste = =
	switch (p) {
	case Upper:
		//ptm = &upper;
		upper.vertexs = stlloader.get_vertexs();
		upper.normals = stlloader.get_normals();
		//model_normalize(upper);
	case Lower:
		//ptm = &lower;
		lower.vertexs = stlloader.get_vertexs();
		lower.normals = stlloader.get_normals();
		//model_normalize(lower);
	case Center:
		center.vertexs = stlloader.get_vertexs();
		center.normals = stlloader.get_normals();
		//model_normalize(center);
		//ptm = &center;
		}
	/* ISSUE:Can't access pointer???

	ptm->vertexs.resize(stlloader.get_vertexs().size());
	ptm->vertexs = stlloader.get_vertexs();
	ptm->normals.clear();
	ptm->normals = stlloader.get_normals();
	model_normalize(*ptm);
	*/
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
	/*
	model_resize(upper, old_ratio);
	model_resize(lower, old_ratio);
	model_resize(center, old_ratio);
	*/
	update();

};
