#include "options.h"

void Options::Init() {
	File file;
	file.read("Data/options.txt");

	showPaths = false;
	showWarps = false;
	showCombatRange = false;

	isCam = file.getInt(1);
	camX = file.getInt(2);
	camY = file.getInt(3);
	camSpeed = file.getInt(4);

	fpsLimit = file.getInt(5);

	windowWidth = file.getInt(6);
	windowHeight = file.getInt(7);
	windowWidthHalf = windowWidth / 2;
	windowHeightHalf = windowHeight / 2;
	windowX = file.getInt(8);
	windowY = file.getInt(9);

	mapX = 0;
	mapY = 0;

	lang = file.getInt(10);
}