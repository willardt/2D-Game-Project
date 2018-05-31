#include "options.h"

void Options::Init() {
	File file;
	file.setPath(OPTIONS_FILE);

	isCam = file.readInt(1);
	camX = file.readInt(2);
	camY = file.readInt(3);
	camSpeed = file.readInt(4);

	fpsLimit = file.readInt(5);

	windowWidth = file.readInt(6);
	windowHeight = file.readInt(7);
	windowX = file.readInt(8);
	windowY = file.readInt(9);

	mapX = 0;
	mapY = 0;

	lang = file.readInt(10);
}