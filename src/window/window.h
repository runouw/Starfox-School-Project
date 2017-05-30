#pragma once

class GLWindow{
private:

	virtual void glutDisplay() = 0;
	virtual void glutReshape(int w, int h){};
	virtual void glutIdle(){};
	virtual void glutKeyboard(unsigned char key, int x, int y){};
	virtual void glutKeyboardUp(unsigned char key, int x, int y){};
    virtual void glutSpecial(int key, int x, int y){};
	virtual void glutSpecialUp(int key, int x, int y){};
	virtual void glutMouse(int b, int s, int x, int y){};
    virtual void glutMotion(int x, int y){};
	virtual void glutPassiveMotion(int x, int y){};
public:
	GLWindow(const char* label, int x, int y, int w, int h);

	static void glutDisplay_callBack();
	static void glutReshape_callBack(int w, int h);
	static void glutIdle_callBack();
	static void glutKeyboard_callBack(unsigned char key, int x, int y);
	static void glutKeyboardUp_callBack(unsigned char key, int x, int y);
	static void glutSpecial_callBack(int key, int x, int y);
	static void glutSpecialUp_callBack(int key, int x, int y);
	static void glutMouse_callBack(int b, int s, int x, int y);
    static void glutMotion_callBack(int x, int y);
	static void glutPassiveMotion_callBack(int x, int y);

	int _x;
	int _y;
	int _w;
	int _h;
 };
