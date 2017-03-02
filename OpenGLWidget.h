#pragma once

#ifndef OPENGLWIDGET_H_
#define OPENGLWIDGET_H_

#include <QWidget>
#include <QOpenGLWidget>
//#include <QOpenGLVertexArrayObject>
#include <QtOpenGL/qgl.h>
#include <QtOpenGL>

#include <gl/GLUT.h>
//#include <gl/GL.h>

#include "stlloader.h"
#include <vector>
class OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT	
	Q_ENUMS(Position)
public:
	enum Position{Upper, Lower, Center, EndCase};
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();
	class STLModel {
	public:
		STLModel();
		~STLModel();
		vector<GLfloat> vertexs, normals;
	};
	void set_stlModel(const char *model_path, Position p);
	GLuint makeObject(STLModel model);
protected:
	void resizeGL(int, int);
	void paintGL();
	void initializeGL();
	void model_normalize(STLModel &model);
	void model_resize(STLModel &model, int old_ratio);
	void draw_model(STLModel &model); 
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *e);
	void whellEvent(QWheelEvent *whell_event);
public slots:
	void animate();
	void setZoomRatio(int _zoom_ratio);
	signals:

private:
	STLloader stlloader;
	STLModel upper, lower, center;
	GLfloat rTri;
	QVector3D rotate;
	GLuint  m_Pos;
	int Zoom_ratio = 50;
	GLuint  fps;
	vector <GLuint> model_list;
	QPoint m_start_pos;
	
};


#endif // OPENGLWIDGET_H_