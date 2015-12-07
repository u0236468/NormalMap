
#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glm\glm.hpp>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <Primitives\Vertex.h>
#include <Primitives\ShapeGenerator.h>
#include <Camera.h>
#include <Primitives\DebugGuiManager.h>
#include <cassert>
#include <MyModel.h>
#include <Qt\qdebug.h>

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

GLuint programID;
GLuint programID2;
GLuint numIndices;
GLuint cubeNumIndices;
GLuint planeNumIndices;
GLuint sphereNumIndices;
GLuint torusNumIndices;
GLuint textureNumIndices;
GLuint vertexBufferID;
GLuint indexBufferID;
GLuint planeVertexBufferID;
GLuint planeIndexBufferID;
GLuint sphereVertexBufferID;
GLuint sphereIndexBufferID;
GLuint torusVertexBufferID;
GLuint torusIndexBufferID;
GLuint textureVertexBufferID;
GLuint textureIndexBufferID;
Camera camera;
GLuint planeVertexArrayObjectID;
GLuint sphereVertexArrayObjectID;
GLuint torusVertexArrayObjectID;
GLfloat rotationAmount;

unsigned int cubemap;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

//Sliders
/*MeGlWindow::MeGlWindow(MyModel* theModel) :theModel(theModel)
{
}*/

#define GENERATED_SHAPE
void MeGlWindow::sendDataToOpenGL()
{
#ifdef GENERATED_SHAPE
	//ShapeData shape = ShapeGenerator::makePlane(10);
	ShapeData shape = ShapeGenerator::makeCube();
	ShapeData Sshape = ShapeGenerator::makeSphere(50);
	//ShapeData shape = ShapeGenerator::makeTorus(100);
	//ShapeData shape = ShapeGenerator::makeTeapot();
#else
	ShapeDataPnut shape;

	std::ifstream input("c:\\Cube_Rock.bin", std::ios::binary);
	assert(input.good());

	input.read(reinterpret_cast<char*>(&shape.numVerts), sizeof(shape.numVerts));
	assert(input.good());
	input.read(reinterpret_cast<char*>(&shape.numIndices), sizeof(shape.numIndices));

	shape.verts = new VertexPNUT[shape.numVerts];
	shape.indices = new ushort[shape.numIndices];

	input.read(reinterpret_cast<char*>(shape.verts), shape.vertexBufferSize());
	input.read(reinterpret_cast<char*>(shape.indices), shape.indexBufferSize());
#endif
	//cube
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &indexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//ShapeData sphere = ShapeGenerator::makeSphere(50);

	//glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBufferID);
	//glBufferData(GL_ARRAY_BUFFER, Sshape.vertexBufferSize(), Sshape.verts, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBufferID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, Sshape.indexBufferSize(), Sshape.indices, GL_STATIC_DRAW);

#ifdef GENERATED_SHAPE
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_TANGENT);
#else
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNUT), (void*)VertexOffsets::VO_PNUT_POSITION);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNUT), (void*)VertexOffsets::VO_PNUT_NORMAL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VdataertexPNUT), (void*)VertexOffsets::VO_PNUT_UV);
#endif
	cubeNumIndices = shape.numIndices;
	shape.cleanUp();

	//sphere
	glGenBuffers(1, &sphereVertexBufferID);
	glGenBuffers(1, &sphereIndexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, Sshape.vertexBufferSize(), Sshape.verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Sshape.indexBufferSize(), Sshape.indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_TANGENT);
	
	sphereNumIndices = Sshape.numIndices;
	Sshape.cleanUp();
	
}

void MeGlWindow::setupVertexArray()
{
	/*//Create VertexArray for each object
	glGenVertexArrays(1, &planeVertexArrayObjectID);
	glGenVertexArrays(1, &sphereVertexArrayObjectID);
	glGenVertexArrays(1, &torusVertexArrayObjectID);

	//Bind plane to VertexArray
	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIndexBufferID);

	//Bind sphere to VertexArray
	glBindVertexArray(sphereVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBufferID);

	//Bind torus to VertexArray
	glBindVertexArray(torusVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, torusVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusIndexBufferID);

	glBindVertexArray(torusVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, torusVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusIndexBufferID);*/
}

bool MeGlWindow::checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
	    getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;

		return false;
	}

	return true;
}

bool MeGlWindow::checkShaderStatus(GLuint shaderID)
{
	/*GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
	GLint infoLogLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	GLchar* buffer = new GLchar[infoLogLength];

	GLsizei bufferSize;
	glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
	cout << buffer << endl;

	delete[] buffer;

	return false;
	}

	return true;*/

	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeGlWindow::checkProgramStatus(GLuint programID)
{
	/*GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;

		return false;
	}

	return true;*/

	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string MeGlWindow::readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File Not Found..." << fileName << endl;
		exit(1);
	}

	return std::string(std::istreambuf_iterator<char>(meInput), std::istreambuf_iterator<char>());
}

void MeGlWindow::installShaders()
{
	//cube
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	{
		return;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
	{
		return;
	}

	glUseProgram(programID);


	//sphere
	/*GLuint vertexShaderID2 = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID2 = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode2.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID2, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode2.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID2, 1, adapter, 0);

	glCompileShader(vertexShaderID2);
	glCompileShader(fragmentShaderID2);

	if (!checkShaderStatus(vertexShaderID2) || !checkShaderStatus(fragmentShaderID2))
	{
		return;
	}

	programID2 = glCreateProgram();
	glAttachShader(programID2, vertexShaderID2);
	glAttachShader(programID2, fragmentShaderID2);
	glLinkProgram(programID2);

	if (!checkProgramStatus(programID2))
	{
		return;
	}

	glUseProgram(programID2);*/

	GL_TEXTURE_CUBE_MAP;
}

void MeGlWindow::initializeGL()
{
	QGLWidget::initializeGL();
	setMinimumSize(1200, 600);
	setMouseTracking(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	setupVertexArray();
	//loadTexture();
	installShaders();

	std::string filename[6];
	filename[0] = "posx.png";
	filename[1] = "negx.png";
	filename[2] = "posy.png";
	filename[3] = "negy.png";
	filename[4] = "posz.png";
	filename[5] = "negz.png";

	cubemap = loadTexture(filename);

	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(0);
}

unsigned int MeGlWindow::loadTexture(std::string* filename)
{
	/*GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	QImage myTexture = QGLWidget::convertToGLFormat(QImage("NormalMap.png", "PNG"));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myTexture.width(), myTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myTexture.bits());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint textureUniformLocation = glGetUniformLocation(programID, "meTexture");
	glUniform1i(textureUniformLocation, 0);*/

	uint TARGETS[6] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};


	GLuint cubemapID;
	glGenTextures(1, &cubemapID);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	for (int i = 0; i < 6; i++)
	{
		QImage myTexture = QGLWidget::convertToGLFormat(QImage(filename[i].c_str(), "PNG"));

		qDebug() << myTexture.width();
		glTexImage2D(TARGETS[i], 0, GL_RGBA, myTexture.width(), myTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myTexture.bits());

		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	}
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	GLint textureUniformLocation = glGetUniformLocation(programID, "meTexture");
	glUniform1i(textureUniformLocation, 0);

	return cubemapID;

	/*uint TARGETS[6] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	string EXTS[6] =
	{
		"posx",
		"negx",
		"posy",
		"negy",
		"posz",
		"negz"
	};

	// Create & bind cubemap texture
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	for (int i = 0; i < 6; i++)
	{
		string file = "textures/cubemap_" + EXTS[i] + ".png";
		uint image = ilGenImage();

		// Load with DevIL
		ilBindImage(image);
		if (!ilLoadImage(file.c_str()))
		{
			cout << "ERROR: Failed to load image " << endl;
			return false;
		}

		// Fetch info from DevIL
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);
		uint format = ilGetInteger(IL_IMAGE_FORMAT);
		uint type = ilGetInteger(IL_IMAGE_TYPE);

		// Send data to OpenGL  
		glTexImage2D(
			TARGETS[i],
			0,
			GL_RGBA,
			width,
			height,
			0,
			format,
			type,
			ilGetData());

		// Error check
		if (!ErrorCheck("Failed to bind a side of the cubemap!"))
			return false;

		// Get rid of DevIL data
		ilDeleteImage(image);*/
}

void MeGlWindow::paintGL()
{
	debugGuiManager.update();
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	rotationAmount += 0.00f;

	mat4 modelToProjectionMatrix;     //fullTransformMatrix
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.001f, 100.0f); //projectionMatrix
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix() * glm::lookAt(
		glm::vec3(0.0f, 1.0f, 0.0f), //eyePosition
		glm::vec3(0.0f, 1.0f, -10.0f), //Center
		glm::vec3(0.0f, 1.0f, 0.0f)); //Up direction;
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	glm::mat3 normalMatrix;

	GLint fullTransformUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix"); 

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.02f, 0.02f, 0.02f, 1.0f);
	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	GLint eyePositionWorldUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	vec3 eyePosition = camera.position;
	glUniform3fv(eyePositionWorldUniformLocation, 1, &eyePosition[0]);

	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	vec3 lightPositionWorld = /*theModel->lightPosition;*/ vec3(0.0f, 0.0f, 0.1f);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);

	GLint modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");
	GLuint normalUniformLocation = glGetUniformLocation(programID, "normalMatrix");

	GLint skyboxModelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "skyboxModelToWorldMatrix");
	GLint sphereModelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "sphereModelToWorldMatrix");
	GLint worldToProjectionMatrixUniformLocation = glGetUniformLocation(programID, "worldToProjectionMatrix");

	//skybox
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_TANGENT);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	
	//skybox1
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 skyboxModelToWorldMatrix =
		glm::scale(+10.0f, +10.0f, +10.0f) *
		glm::translate(vec3(0.0f, 0.0f, 0.5f)) *
		glm::rotate(180.0f, vec3(1.0f, 0.0f, 0.0f));// *
		//glm::rotate(180.0f, vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rotationAmount, vec3(1.0f, 0.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * skyboxModelToWorldMatrix; //*translationMatrix * rotationMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(skyboxModelToWorldMatrixUniformLocation, 1, GL_FALSE, &skyboxModelToWorldMatrix[0][0]);
	glUniformMatrix3fv(normalUniformLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix3fv(worldToProjectionMatrixUniformLocation, 1, GL_FALSE, &worldToProjectionMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, 0);

	//Sphere
	/*glBindVertexArray(sphereVertexArrayObjectID);
	mat4 sphereModelToWorldMatrix =
		glm::scale(+1.0f, +1.0f, +1.0f) *
		glm::translate(vec3(0.0f, 0.0f, 0.5f)) *
		glm::rotate(180.0f, vec3(1.0f, 0.0f, 0.0f));// *
		//glm::rotate(180.0f, vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rotationAmount, vec3(1.0f, 0.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * sphereModelToWorldMatrix; //*translationMatrix * rotationMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(4, 1, GL_FALSE, &sphereModelToWorldMatrix[0][0]);*/

	//glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, 0);
	
	//Sphere
	glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_POSITION);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_UV);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_NORMAL);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCNUT), (void*)VertexOffsets::VO_PCNUT_TANGENT);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBufferID);

	//sphere1
	mat4 sphereModelToWorld = glm::scale(vec3(1.0f, 1.0f, 1.0f)) *
		glm::translate(vec3(0.0f, 0.0f, 0.5f)) *
		glm::rotate(180.0f, vec3(1.0f, 0.0f, 0.0f)); /*glm::translate(0.0f, 0.0f, -5.0f) * glm::rotate(90.0f, 1.0f, 0.0f, 02.0f);*/
	modelToProjectionMatrix = worldToProjectionMatrix * sphereModelToWorld;
	normalMatrix = glm::mat3(sphereModelToWorld);
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(skyboxModelToWorldMatrixUniformLocation, 1, GL_FALSE, &sphereModelToWorld[0][0]);
	glUniformMatrix3fv(normalUniformLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix3fv(worldToProjectionMatrixUniformLocation, 1, GL_FALSE, &worldToProjectionMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, 0);

	/*
	// Update
	glfwPollEvents();
	UpdateTime();

	// Clear back buffer for new frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Bind shader
	shader->Bind();

	// Cubemap
	shader->SetUniform("cubemapTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	// Bind model
	if (model->Bind())
	{
		static float angle = 0;
		angle += 25.0f * deltaTime;

		// Matrices
		mat4 world =
			translate(vec3(0.0f, 0.0f, 0.0f)) *
			rotateZ(angle * PI / 180) *
			rotateX(angle * PI / 180) *
			scale(vec3(1.0f, 1.0f, 1.0f));
		mat4 view = ViewMatrix(
			cameraPosition,
			cameraTarget,
			vec3(0.0f, 0.0f, 1.0f));
		mat4 proj = ProjectionMatrix(
			fov,
			(float)windowX,
			(float)windowY,
			nearPlane,
			farPlane);

		// Uniforms
		shader->SetUniform("uWorld", world);
		shader->SetUniform("uView", view);
		shader->SetUniform("uProj", proj);

		shader->SetUniform("materialColor", vec3(0.5f, 0.5f, 0.5f));

		shader->SetUniform("drawSkybox", false);
		shader->SetUniform("world_cameraPosition", cameraPosition);
		shader->SetUniform("reflectFactor", 0.5f);

		// Draw
		glDrawElements(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_SHORT, NULL);
	}

	// Put the new image on the screen
	glfwSwapBuffers(window);*/
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	setFocus();
	repaint();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();
}

void MeGlWindow::myUpdate()
{
	paintGL();
	//myUpdate();
	repaint();
}