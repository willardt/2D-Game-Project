#include <SDL.h>

#include "file.h"

#ifndef OPTIONS_H
#define OPTIONS_H

#define ENGLISH 1
#define RUSSIAN 2

class Options {
private:
public:
	enum { MOUSE_WIDTH = 20, MOUSE_HEIGHT = 20 };
	static Options& Instance() {
		static Options o;
		return o;
	}
	bool showPaths;
	bool showWarps;
	bool showCombatRange;

	bool isCam;
	int camX;
	int camY;
	int camSpeed;

	int fpsLimit;

	int windowWidth;
	int windowHeight;
	int windowWidthHalf;
	int windowHeightHalf;
	int windowX;
	int windowY;

	int mapX;
	int mapY;

	int lang;

	SDL_Scancode up;
	SDL_Scancode down;
	SDL_Scancode right;
	SDL_Scancode left;
	SDL_Scancode interact;
	SDL_Scancode bag;
	SDL_Scancode trade;
	SDL_Scancode pickup;

	void loadKeys();

	void Init();
};

#endif
