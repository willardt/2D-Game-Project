#include "map.h"

const int Map::solidTiles[Map::SOLID_TILES] = { 3, 4 };
const int Map::enviromentTiles[Map::ENVIROMENT_TILES] = { -1 };

std::vector<Texture> Map::memInit() {
	File file;

	file.setPath("Data/Maps/Tiles/tiles.txt");
	
	std::vector<Texture> tempVect;
	Texture temp;

	for (int i = 1; i <= Map::TOTAL_TEXTURES; i++) {

		temp.setPath(file.readStr(i));

		tempVect.push_back(temp);
	}

	return tempVect;
}

void Map::Init(const int& nid) {
	id = nid;
	map.clear();
	Options& options = options.Instance();
	File file;
	Tile tempTile;
	std::string tileData = "";
	std::string dataStr = "";
	size_t tileDataLength = 0;
	int data;

	int j = 0;
	int m = 0;
	int q = 0;
	int p = 0;

	if (isTileLoaded != true) {
		tiles = Map::memInit();
		isTileLoaded = true;
	}

	std::string path = "Data/Maps/map" + std::to_string(id) + ".txt";

	file.setPath(path);

	name = file.readStr(1);
	width = file.readInt(2);
	height = file.readInt(3);
	tileData = file.readStr(4);

	tileDataLength = tileData.length();

	area = height * width;
	options.mapX = width;
	options.mapY = height;

	std::cout << "Loading " + path << std::endl;

	for (int i = 0; i < area; i++) {
		//SETTING TILE ID
		while (tileData[p] != ' ' && p <= tileDataLength) {
			p++;
		}
		dataStr = tileData.substr(q, p - q);
		data = std::stoi(dataStr);
		tempTile.id = data;

		q = p + 1;
		p = q;

		//SETTING TILE POSITION
		if (i % width == 0 && i != 0) {
			j++;
			m = 0;
		}

		tempTile.pos.y = j * Map::TILE_SIZE;
		tempTile.pos.x = m * Map::TILE_SIZE;
		tempTile.pos.w = Map::TILE_SIZE;
		tempTile.pos.h = Map::TILE_SIZE;

		map.push_back(tempTile);

		for (int i = 0; i < Map::SOLID_TILES; i++) {
			if (tempTile.id == solidTiles[i]) {
				solids.push_back(tempTile);
				break;
			}
		}
		for (int i = 0; i < Map::ENVIROMENT_TILES; i++) {
			if (tempTile.id == enviromentTiles[i]) {
				enviroments.push_back(tempTile);
				break;
			}
		}

		m++;
	}

	std::cout << "Loaded " + path << std::endl;
}

bool Map::isPassable(const int& nid) {
	bool found = false;

	for (int i = 0; i < Map::SOLID_TILES; i++) {
		if (solidTiles[i] == nid) {
			return true;
		}
	}
	return false;

}