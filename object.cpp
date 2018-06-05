#include "object.h"

std::vector<Texture> Object::memInit() {
	File file;

	file.read("Data/Entities/Objects/objects.txt");

	std::vector<Texture> tempVec;
	Texture temp;

	for (int i = 1; i <= file.getSize(); i++) {
		temp.setPath(file.getStr(i));
		tempVec.push_back(temp);
	}

	return tempVec;
}

void Object::Init(int nid) {
	id = nid;

	File file;
	file.read("Data/Entities/Objects/objectsloc.txt");
	std::string path = file.getStr(id + 1);
	file.read("Data/Entities/Objects/" + path);

	name = file.getStr(2);
	pos.w = file.getInt(3);
	pos.h = file.getInt(4);
	isSolid = file.getInt(5);
}

std::vector<Object> Object::createObjectSolids(std::vector<Object>& o) {
	std::vector<Object> temp;
	for (size_t i = 0; i < o.size(); i++) {
		if (o[i].isSolid) {
			temp.push_back(o[i]);
		}
	}
	return temp;
}

void Object::loadMapObjects(const int& id, std::vector<Object>& o) {
	File file;
	Object tempObject;
	std::string fileData = " ";
	std::string objectData = " ";
	int fileDataLength = 0;
	int data = 0;
	int objectSize = 0;
	int type = 0;
	file.read("Data/Maps/map" + std::to_string(id) + ".txt");
	o.clear();

	std::cout << "Loading Objects for map" << id << std::endl;

	objectSize = file.getInt(11);
	fileData = file.getStr(12);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;

	for (int i = 0; i < objectSize; i++) {
		while (dataType <= Object::LOAD_DATA_SIZE) {
			if (dataType < Object::LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			objectData = fileData.substr(q, p - q);
			data = std::stoi(objectData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		tempObject.Init(data);			break;
			case 1:		tempObject.pos.x = data;			break;
			case 2:		tempObject.pos.y = data;			break;
			}

			dataType++;
		}

		o.push_back(tempObject);
		dataType = 0;
	}
	std::cout << "Objects Loaded for map" << id << std::endl;
}