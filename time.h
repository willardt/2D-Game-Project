#include <SDL.h>
#include <iostream>

#ifndef TIME_H
#define TIME_H

class Time {
private:
	int _tick;
	int _tock;
public:
	void tock(int time);
	bool tick(const int& time);
	int getTick();
};

#endif