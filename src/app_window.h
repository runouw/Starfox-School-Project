#pragma once

#include <GL/glew.h>
#include <freeglut.h>
#include "window/window.h"
#include <cmath>
#include <ctime>
#include <rtools\keys.h>
#include <rtools\glactivity.h>

class AppWindow : GLWindow{
private:
	virtual void glutDisplay();
	virtual void glutReshape(int w, int h);
	virtual void glutIdle();
	virtual void glutKeyboard(unsigned char key, int x, int y);
	virtual void glutKeyboardUp(unsigned char key, int x, int y);
	virtual void glutSpecial(int key, int x, int y);
	virtual void glutSpecialUp(int key, int x, int y);
	virtual void glutMouse(int b, int s, int x, int y);
    virtual void glutMotion(int x, int y);
	virtual void glutPassiveMotion(int x, int y);

	GLActivity* activity;
	int framerate;

	int _mx, _my;

public:
	AppWindow(const char* label, int x, int y, int w, int h);

	void setTargetFramerate(int framerate);
	void setActivity(GLActivity* act);

	void run();
 };
