#pragma once

#include <ctime>

#define KEYLIMIT 256

typedef int keyTime;

class Keys{
	bool* _down;
	bool* _downLast;
	keyTime* pressed;
	keyTime* released;

	int frameMillis;

	static int getTimeMillis(){
		clock_t currentTime = clock();
		double ms = double(currentTime) / CLOCKS_PER_SEC * 1000;
		return (int)ms;
	}
public:
	Keys();

	static void frameStart();
	static void frameEnd();

	static void setKeyDown(char key);
	static void setKeyUp(char key);

	static bool justPressed(char key);
	static bool down(char key);
	static keyTime lastPressed(char key);
	static keyTime lastReleased(char key);
};