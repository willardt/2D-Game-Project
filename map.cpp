#include "map.h"

std::vector<Texture> Map::memInit() {
	File file;

	file.read("Data/Maps/Tiles/tiles.txt");
	
	std::vector<Texture> tempVect;
	Texture temp;

	for (int i = 1; i <= file.getSize(); i++) {

		temp.setPath(file.getStr(i));

		tempVect.push_back(temp);
	}

	return tempVect;
}

void Map::Init(const int& nid) {
	id = nid;
	map.clear();
	solids.clear();
	Options& options = options.Instance();
	File mapSolidsFile;
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

	std::string path = "Data/Maps/map" + std::to_string(id) + ".txt";

	mapSolidsFile.read("Data/Maps/Tiles/solids.txt");

	file.read(path);

	name = file.getStr(1);
	width = file.getInt(2);
	height = file.getInt(3);
	tileData = file.getStr(4);

	tileDataLength = tileData.length();

	area = height * width;
	options.mapX = width;
	options.mapY = height;

	std::cout << "Loading Map " + path << std::endl;

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
		tempTile.isSolid = bool(mapSolidsFile.getInt(tempTile.id + 1));

		if (tempTile.isSolid) {
			solids.push_back(tempTile);
		}

		map.push_back(tempTile);

		m++;
	}

	std::cout << "Map Loaded " + path << std::endl;
}