#include <SDL.h>

#include "file.h"

#ifndef WARP_H
#define WARP_H

class Warp {
private:
public:
	enum {LOAD_DATA_SIZE = 8};

	SDL_Rect pos;
	SDL_Rect dest;
	int teleID;

	static void loadWarps(const int& mapdID, std::vector<Warp>& w);
};

#endif