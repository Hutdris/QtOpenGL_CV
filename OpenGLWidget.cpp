#include "OpenGLWidget.h"
#include <algorithm>

OpenGLWidget::STLModel::STLModel() {

}
OpenGLWidget::STLModel:: ~STLModel() { vertexs.clear(); normals.clear();}

OpenGLWidget::OpenGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
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

	set_stlModel("cube.stl", Lower);
	set_stlModel("cube.stl", Upper);
	set_stlModel("cube.stl", Center);

	rTri = 0.0;
	//

}

void OpenGLWidget::draw_model(STLModel &model) {

	glRotatef(rTri, 0.5, 1.0, 0.3);

	glColor3f(0.5, 0.5, 0.0);
	glBegin(GL_TRIANGLES);
for (auto itt = model.vertexs.begin(); itt != model.vertexs.end(); itt += 9){
	glVertex3f(*itt, *(itt + 1), *(itt + 2));
	glVertex3f(*(itt+3), *(itt + 4), *(itt + 5));
	glVertex3f(*(itt+6), *(itt + 7), *(itt + 8));
}
	glEnd();
}
void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_model(lower);
	draw_model(upper);
	draw_model(center);
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
	rTri += 3.0f;
	update();
	//paintGL();
}

void OpenGLWidget::model_normalize(STLModel &model) {
	auto max_corr = *(std::max_element(model.vertexs.begin(), model.vertexs.end()));
	for (auto i = 0; i < model.vertexs.size(); i++) {
		model.vertexs[i] /= max_corr;
	}
}

void OpenGLWidget::set_stlModel(const char *model_path, Position p) {

		stlloader.set_path(model_path);
		stlloader.load();
		//STLModel *ptm;
	switch (p) {
	case Upper:
		//ptm = &upper;
		upper.vertexs = stlloader.get_vertexs();
		upper.normals = stlloader.get_normals();
		model_normalize(upper);
	case Lower:
		//ptm = &lower;
		lower.vertexs = stlloader.get_vertexs();
		lower.normals = stlloader.get_normals();
		model_normalize(lower);
	case Center:
		center.vertexs = stlloader.get_vertexs();
		center.normals = stlloader.get_normals();
		model_normalize(center);
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

