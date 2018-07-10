#include "warp.h"

void Warp::loadWarps(const int& mapID, std::vector<Warp>& w) {
	File file;
	Warp tempWarp;
	std::string fileData = " ";
	std::string warpData = " ";
	int fileDataLength = 0;
	int data = 0;
	int warpSize = 0;
	int type = 0;
	file.read("Data/Maps/map" + std::to_string(mapID) + ".txt");
	w.clear();

	std::cout << "Loading Warps for map" << mapID << std::endl;

	warpSize = file.getInt(11);
	fileData = file.getStr(12);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;

	for (int i = 0; i < warpSize; i++) {
		while (dataType <= Warp::LOAD_DATA_SIZE) {
			if (dataType < Warp::LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			warpData = fileData.substr(q, p - q);
			data = std::stoi(warpData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		tempWarp.teleID = data;			break;
			case 1:		tempWarp.pos.x = data;			break;
			case 2:		tempWarp.pos.y = data;			break;
			case 3:		tempWarp.pos.w = data;			break;
			case 4:		tempWarp.pos.h = data;			break;
			case 5:		tempWarp.dest.x = data;			break;
			case 6:		tempWarp.dest.y = data;			break;
			case 7:		tempWarp.dest.w = data;			break;
			case 8:		tempWarp.dest.h = data;			break;
			}

			dataType++;
		}

		w.push_back(tempWarp);
		dataType = 0;
	}
	std::cout << "Warps Loaded for map" << mapID << std::endl;
}