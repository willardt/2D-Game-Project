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
	File file;
	File uFile;
	Tile tempTile;
	std::string tileData = "";
	std::string dataStr = "";
	int tileDataLength = 0;
	int data = 0;

	int j = 0;
	int m = 0;
	int q = 0;
	int p = 0;

	std::string path = "Data/Maps/map" + std::to_string(id) + ".txt";

	file.read(path);

	if (options.lang == ENGLISH) {
		uFile.uread("Data/Maps/En/maps.txt");
	}
	else if (options.lang == RUSSIAN) {
		uFile.uread("Data/Maps/Ru/maps.txt");
	}

	uname = uFile.getU16(id);

	name = file.getStr(1);
	width = file.getInt(2);
	height = file.getInt(3);
	tileData = file.getStr(4);

	tileDataLength = int(tileData.length());

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

		map.push_back(tempTile);

		m++;
	}

	Tile tempSolid;
	std::string fileData = " ";
	std::string solidData = " ";
	int fileDataLength = 0;
	int solidSize = 0;
	p = 0;
	q = 0;

	std::cout << "Loading Solids for map" << id << std::endl;

	solidSize = file.getInt(5);
	fileData = file.getStr(6);
	fileDataLength = int(fileData.length());

	int dataType = 0;

	for (int i = 0; i < solidSize; i++) {
		while (dataType <= SOLID_DATA_SIZE) {
			if (dataType < SOLID_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			solidData = fileData.substr(q, p - q);
			data = std::stoi(solidData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		tempSolid.pos.x = data;			break;
			case 1:		tempSolid.pos.y = data;			break;
			}

			dataType++;
		}

		tempSolid.pos.w = 32;
		tempSolid.pos.h = 32;
		solids.push_back(tempSolid);
		dataType = 0;
	}
	std::cout << "Solids Loaded for map" << id << std::endl;


	std::cout << "Map Loaded " + path << std::endl;
}