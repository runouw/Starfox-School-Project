#include "window.h"

static GLWindow* singleton = 0;
GLWindow::GLWindow(const char* label, int x, int y, int w, int h){
	singleton = this;

	_x = x;
	_y = y;
	_w = w;
	_h = h;
}
void GLWindow::glutDisplay_callBack(){
	singleton->glutDisplay();
}
void GLWindow::glutReshape_callBack(int w, int h){
	singleton->glutReshape(w, h);
}
void GLWindow::glutIdle_callBack(){
	singleton->glutIdle();
}
void GLWindow::glutKeyboard_callBack(unsigned char key, int x, int y){
	singleton->glutKeyboard(key, x, y);
}
void GLWindow::glutKeyboardUp_callBack(unsigned char key, int x, int y){
	singleton->glutKeyboardUp(key, x, y);
}
void GLWindow::glutSpecial_callBack(int key, int x, int y){
	singleton->glutSpecial(key, x, y);
}
void GLWindow::glutSpecialUp_callBack(int key, int x, int y){
	singleton->glutSpecialUp(key, x, y);
}
void GLWindow::glutMouse_callBack(int b, int s, int x, int y){
	singleton->glutMouse(b, s, x, y);
}
void GLWindow::glutMotion_callBack(int x, int y){
	singleton->glutMotion(x, y);
}
void GLWindow::glutPassiveMotion_callBack(int x, int y){
	singleton->glutPassiveMotion(x, y);
}