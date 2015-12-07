#pragma once

#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW

#include <GL\glew.h>
#include <QtOpenGl\qglwidget>
#include <Qt\qtimer.h>
#include <string>
//class MyModel;

class MeGlWindow : public QGLWidget
{
	Q_OBJECT

	void installShaders();
	std::string readShaderCode(const char* fileName);
	bool checkProgramStatus(GLuint programID);
	bool checkShaderStatus(GLuint shaderID);
	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	void sendDataToOpenGL();
	void setupVertexArray();
	//MyModel* theModel;

protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	unsigned int loadTexture(std::string* filename);

private slots:
	void myUpdate();

public:
	//MeGlWindow(MyModel* theModel);
	MeGlWindow(){}
	QTimer myTimer;


};



#endif