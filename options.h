#include <SDL.h>

#include "file.h"

#ifndef OPTIONS_H
#define OPTIONS_H

#define ENGLISH 1
#define RUSSIAN 2

class Options {
private:
public:
	static Options& Instance() {
		static Options o;
		return o;
	}

	bool isCam;
	int camX;
	int camY;
	int camSpeed;

	int fpsLimit;

	int windowWidth;
	int windowHeight;
	int windowX;
	int windowY;

	int mapX;
	int mapY;

	int lang;

	void Init();
};

#endif
