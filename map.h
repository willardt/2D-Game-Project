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
};

class Map {
private:
public:
	enum {
		TILE_SIZE = 32, TOTAL_TEXTURES = 5, MAX_WIDTH = 150,
		MAX_HEIGHT = 150, SOLID_TILES = 2, ENVIROMENT_TILES = 1, TILE_SHEET_WIDTH = 10
	};

	static const int solidTiles[Map::SOLID_TILES];
	static const int enviromentTiles[Map::ENVIROMENT_TILES];

	std::string name;

	bool isTileLoaded = false;

	int id;
	int width;
	int height;
	int area;

	std::vector<Tile> map;
	std::vector<Texture> tiles;
	std::vector<Tile> solids;
	std::vector<Tile> enviroments;

	void Init(const int& nid);
	static std::vector<Texture> memInit();

	bool isPassable(const int& nid);
};

#endif