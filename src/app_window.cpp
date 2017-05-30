#include <iostream>
#include "app_window.h"
#include <stdio.h>

#include <rtools\ai\textures.h>
#include <rtools\ai\material.h>
#include <rtools\ai\mesh.h>
#include <rtools\ai\shaders.h>
#include <rtools\post\deferred.h>
#include <game\models\model_laser.h>
#include <game\particle\particles.h>

#define PI_F 3.151592654f

static int getFPS(){
	static int frames = 0;
	static clock_t lastTime = clock();
	static int lastFPS = 0;

	frames++;

	clock_t currentTime = clock();
	double ms = double(currentTime - lastTime) / CLOCKS_PER_SEC * 1000;

	if (ms > 1000.0){
		lastTime = currentTime;
		lastFPS = int((frames * 1000) / ms);
		frames = 0;
	}
	return lastFPS;
}
static double frameDiff(){
	static int lastFrame = clock();

	double diff = double(clock() - lastFrame) / (CLOCKS_PER_SEC);
	lastFrame = clock();

	return diff;
}

AppWindow::AppWindow(const char* label, int x, int y, int w, int h) : GLWindow(label, x, y, w, h) {
	framerate = 60;

	glutInitWindowPosition(x, y);
	glutInitWindowSize(w, h);
	glutCreateWindow(label);

	// Init glew library (after a glut window is created!):
	GLenum res = glewInit();
	if (res!=GLEW_OK) std::cout << glewGetString(GLEW_VERSION) << ", Error: " << glewGetErrorString(res) << "\n";

	// Initialize OpenGL settings as we want
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glutIdleFunc(glutIdle_callBack);
	glutReshapeFunc(glutReshape_callBack);
	glutDisplayFunc(glutDisplay_callBack);

	glutKeyboardFunc(glutKeyboard_callBack);
	glutKeyboardUpFunc(glutKeyboardUp_callBack);
	glutSpecialFunc(glutSpecial_callBack);
	glutSpecialUpFunc(glutSpecialUp_callBack);
	glutMouseFunc(glutMouse_callBack);
	glutMotionFunc(glutMotion_callBack);
	glutPassiveMotionFunc(glutPassiveMotion_callBack);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Shaders::init();

	// default material:
	Material* material = new Material();
	material->program = Shaders::getSimpleShader();
	material->tex = Textures::get("../res/models/arwing/arwing_01.bmp");
	Mesh::setDefaultMaterial(material);

	// defualt program:
	Material::setDefaultProgram(Shaders::getSimpleShader());

	DeferredUtils::init(1.04f, 12);
	ParticleUtils::init();

	Model_Laser::init();
}
void AppWindow::glutReshape ( int w, int h) {
	// Define that OpenGL should use the whole window for rendering

	_w=w; _h=h;

	if (activity && activity->initialized){
		activity->setWH(w, h);
	}
}
void AppWindow::glutKeyboard(unsigned char key, int x, int y){
	Keys::setKeyDown(key | 32);

	switch ( key ) {
		case 27:
			exit(0);
	}
}
void AppWindow::glutKeyboardUp(unsigned char key, int x, int y){
	Keys::setKeyUp(key | 32);
}
void AppWindow::glutSpecial(int key, int x, int y){
	Keys::setKeyDown(key | 32);
}
void AppWindow::glutSpecialUp(int key, int x, int y){
	Keys::setKeyUp(key | 32);
}
void AppWindow::glutMouse(int b, int s, int x, int y){
	if (activity && activity->initialized){
		if (s == GLUT_UP){
			activity->mouseUp(b);
		}
		if (s == GLUT_DOWN){
			activity->mouseDown(b);
		}
	}
	_mx = x;
	_my = y;
}
void AppWindow::glutMotion(int x, int y){
	_mx = x;
	_my = y;
}
void AppWindow::glutPassiveMotion(int x, int y){
	_mx = x;
	_my = y;
}
void AppWindow::run(){
	glutMainLoop();
}

void AppWindow::glutDisplay() {
	if (activity){
		if (!activity->initialized){
			activity->init(_w, _h);
			activity->initialized = true;
		}

		activity->render();
	}

	// end
	glFlush();
	glutSwapBuffers();
}
void AppWindow::glutIdle(){
	Keys::frameStart();
	float diff = (float)frameDiff();

	float targetFramediff = 1.0f / (float(framerate));
	if (diff < targetFramediff){
		//printf("Sleep %d mill\n", int((targetFramediff - diff) * 1000.0f));
		Sleep(int((targetFramediff - diff) * 1000.0f));
		//diff += (targetFramediff - diff);
		diff += (float)frameDiff();
	}

	if (activity && activity->initialized){
		activity->setMousePosition(_mx, _my);
		activity->idle(diff);
	}

	Keys::frameEnd();
	glutPostRedisplay();
	
}
void AppWindow::setTargetFramerate(int framerate){
	this->framerate = framerate;
}
void AppWindow::setActivity(GLActivity* act){
	activity = act;
}