#include <SDL.h>
#include <iostream>

#ifndef TIME_H
#define TIME_H

class Time {
private:
	int _tick;
public:
	void tock(int time);
	bool tick(int time);
	int getTick();
};

#endif