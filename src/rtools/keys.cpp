#include <rtools\keys.h>

#include <iostream>

static Keys* singleton = 0;
Keys::Keys(){
	singleton = this;

	singleton->_down = new bool[KEYLIMIT];
	singleton->_downLast = new bool[KEYLIMIT];
	singleton->pressed = new keyTime[KEYLIMIT];
	singleton->released = new keyTime[KEYLIMIT];

	for (int i = 0; i < 256; i++){
		singleton->_down[i] = false;
		singleton->_downLast[i] = false;
		singleton->pressed[i] = 0;
		singleton->released[i] = 0;
	}
}

void Keys::frameStart(){
	singleton->frameMillis = Keys::getTimeMillis();
}
void Keys::frameEnd(){
	for (int i = 0; i < KEYLIMIT; i++){
		singleton->_downLast[i] = singleton->_down[i];
	}
}

void Keys::setKeyDown(char key){
	if (key > KEYLIMIT) return;
	singleton->_down[key] = true;
	singleton->pressed[key] = Keys::getTimeMillis();
}
void Keys::setKeyUp(char key){
	if (key > KEYLIMIT) return;
	singleton->_down[key] = false;
	singleton->released[key] = Keys::getTimeMillis();
}

bool Keys::justPressed(char key){
	return singleton->_down[key] && !singleton->_downLast[key];
}

bool Keys::down(char key){
	return singleton->_down[key];
}
keyTime Keys::lastPressed(char key){
	return singleton->pressed[key] - singleton->frameMillis;
}
keyTime Keys::lastReleased(char key){
	return singleton->released[key] - singleton->frameMillis;
}