#include "FPS.h"

void FPS::Init() {
	_prevTicks = 0;
	_frameTimeAVG = 0;
	_frameTime = 0;
	_fps = 0;
	_currentFrame = 0;
	_counter = 0;
}

void FPS::calcFPS() {
	Options& options = options.Instance();

	_frameTime = float(SDL_GetTicks()) - _prevTicks;

	if ((1000.f / options.fpsLimit) > _frameTime) {
		SDL_Delay((1000.f / options.fpsLimit) - _frameTime);
		_frameTime = float(SDL_GetTicks()) - _prevTicks;
	}

	_prevTicks = SDL_GetTicks();
	_frameTimes[_currentFrame % FPS::_NUM_SAMPLE] = _frameTime;

	if (_currentFrame < FPS::_NUM_SAMPLE) {
		_counter = _currentFrame;
	}
	else {
		_counter = FPS::_NUM_SAMPLE;
	}

	for (int i = 0; i < _counter; i++) {
		_frameTimeAVG += _frameTimes[i];
	}
	if (_counter != 0) {
		_frameTimeAVG /= _counter;
	}
	else {
		_frameTimeAVG = (1000.f / options.fpsLimit);
	}
	
	if (_frameTimeAVG > 0) {
		_fps = (1000 / _frameTimeAVG);
	}
	else {
		_fps = options.fpsLimit;
	}

	_currentFrame++;
}

float FPS::getFPS() {
	return _fps;
}