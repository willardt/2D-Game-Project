#include <SDL.h>

#include "options.h"

#ifndef FPS_H
#define FPS_H

class FPS {
private:
	enum {_NUM_SAMPLE = 30};
	float _frameTimes[_NUM_SAMPLE];
	float _prevTicks;
	float _frameTimeAVG;
	float _frameTime;
	float _fps;
	int _currentFrame;
	int _counter;
public:
	void Init();
	void calcFPS();
	float getFPS();
};

#endif