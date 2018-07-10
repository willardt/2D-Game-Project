#include "options.h"

void Options::Init() {
	File file;
	file.read("Data/options.txt");

	showPaths = false;
	showWarps = false;
	showCombatRange = false;
	showSolids = false;

	isCam = file.getInt(1);
	camX = file.getInt(2);
	camY = file.getInt(3);
	camSpeed = file.getInt(4);

	fpsLimit = file.getInt(5);

	windowWidth = file.getInt(6);
	windowHeight = file.getInt(7);
	windowWidthHalf = windowWidth / 2;
	windowHeightHalf = windowHeight / 2;
	windowWidthQuarter = windowWidthHalf / 2;
	windowHeightQuarter = windowHeightHalf / 2;
	windowX = file.getInt(8);
	windowY = file.getInt(9);

	mapX = 0;
	mapY = 0;

	lang = file.getInt(10);

	loadKeys();
}

void Options::loadKeys() {
	File file;
	file.read("Data/bindings.txt");
	up = SDL_Scancode(file.getInt(1));
	down = SDL_Scancode(file.getInt(2));
	right = SDL_Scancode(file.getInt(3));
	left  = SDL_Scancode(file.getInt(4));
	interact = SDL_Scancode(file.getInt(5));
	bag = SDL_Scancode(file.getInt(6));
	trade = SDL_Scancode(file.getInt(7));
	pickup = SDL_Scancode(file.getInt(8));
}