#include<vector>

#include "texture.h"
#include "options.h"


#ifndef MAP_H
#define MAP_H

class Tile {
private:
public:
	int id;
	SDL_Rect pos;
	bool isSolid;
};

class Map {
private:
public:
	enum {
		TILE_SIZE = 32, MAX_WIDTH = 150,
		MAX_HEIGHT = 150, SOLID_TILES = 2, TILE_SHEET_WIDTH = 10,
		SOLID_DATA_SIZE = 1
	};

	std::string name;
	u16string uname;

	bool isTileLoaded = false;

	int id;
	int width;
	int height;
	int area;

	std::vector<Tile> map;
	std::vector<Tile> solids;

	void Init(const int& nid);
	static std::vector<Texture> memInit();


};

#endif