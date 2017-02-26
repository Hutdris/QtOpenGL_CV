#pragma once

#ifndef OPENGLWIDGET_H_
#define OPENGLWIDGET_H_

#include <QWidget>
#include <QOpenGLWidget>
//#include <QOpenGLVertexArrayObject>
#include <QKeyEvent>
#include <gl/GLU.h>
#include <gl/GL.h>

#include "stlloader.h"
#include <vector>
class OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT	
	Q_ENUMS(Position)
public:
	enum Position{Upper, Lower, Center};
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();
	class STLModel {
	public:
		STLModel();
		~STLModel();
		vector<GLfloat> vertexs, normals;
	};
	void set_stlModel(const char *model_path, Position p);
protected:
	void resizeGL(int, int);
	void paintGL();
	void initializeGL();
	void model_normalize(STLModel &model);
	void draw_model(STLModel &model); 
public slots:
	void keyPressEvent(QKeyEvent *e);
	void animate();
	signals:

private:
	STLloader stlloader;
	STLModel upper, lower, center;
	GLfloat rTri;
	GLuint  m_Pos;
};


#endif // OPENGLWIDGET_H_