#include "time.h"

void Time::tock(int time) {
	_tick = time;
}

bool Time::tick(const int& time) {
	_tick++;
	if (_tick == time) {
		_tick = 0;
		return true;
	}
	return false;
}

int Time::getTick() {
	return _tick;
}