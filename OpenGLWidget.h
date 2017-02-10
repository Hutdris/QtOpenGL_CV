#pragma once

#ifndef OPENGLWIDGET_H_
#define OPENGLWIDGET_H_

#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>

class OpenGLWidget : public QOpenGLWidget
{
public:
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();

protected:
	void initializeGL();
	void resizeGL(int, int);
	void paintGL();

};


#endif // OPENGLWIDGET_H_