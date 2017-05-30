#pragma once

#include <iostream>

/*
 * An activity is a class that controls the entire gl window. Only one activity should be displayed 
 * at a time. An activity is given full keyboard and mouse controls of the application and controls
 * everything to do with rendering except utilities such as framerate, or resolution.
 */
class GLActivity{
private:
	int oldW, oldH;

protected:
	int width, height; 
	int mx, my;
public:
	bool initialized;
	GLActivity(){
		initialized = false;
	}

	virtual void init(int w, int h){
		width = w;
		height = h;
		initialized = true;
	}

	virtual void changedWH(){

	}
	void setWH(int w, int h){
		width = w;
		height = h;
	}
	void setMousePosition(int x, int y){
		mx = x;
		my = y;
	}

	virtual void mouseDown(int button){

	}
	virtual void mouseUp(int button){

	}

	// Do keyboard, mouse and timestep
	virtual void idle(float dt){

	}
	// Do render only
	virtual void render(){

		if (oldW != width || oldH != height){
			changedWH();
		}
		oldW = width;
		oldH = height;

	}

};