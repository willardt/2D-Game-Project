#include "effect.h"

std::vector<Texture> Effect::memInit() {
	std::vector<Texture> tempVec;
	Texture temp;
	File file;
	file.read("Data/Entities/Effects/effects.txt");

	for (int i = 1; i <= file.getSize(); i++) {
		temp.setPath(file.getStr(i));
		tempVec.push_back(temp);
	}

	return tempVec;
}

void Effect::Init(const int& nid) {
	id = nid;

	File file;
	File locFile;
	locFile.read("Data/Entities/Effects/effectsloc.txt");
	file.read("Data/Entities/Effects/" + locFile.getStr(id + 1));

	name = file.getStr(2);
	pos.w = file.getInt(3);
	pos.h = file.getInt(4);
	maxFrames = file.getInt(5);
	spriteTime = file.getInt(6);
	sprite.x = 0;
	sprite.y = 0;
	sprite.frame = 0;
	spriteTimer.tock(0);
}

void Effect::update() {
	if (spriteTimer.tick(spriteTime)) {
		sprite.frame++;
		if (sprite.frame > maxFrames) {
			sprite.frame = 0;
		}
		sprite = Texture::updateSprite(pos, sprite.frame);
	}
}

void Effect::loadEffects(const int& mapID, std::vector<Effect>& e) {
	File file;
	Effect tempEffect;
	std::string fileData = " ";
	std::string effectData = " ";
	int fileDataLength = 0;
	int data = 0;
	int effectSize = 0;
	int type = 0;
	file.read("Data/Maps/map" + std::to_string(mapID) + ".txt");
	e.clear();

	std::cout << "Loading Effects for map" << mapID << std::endl;

	effectSize = file.getInt(15);
	fileData = file.getStr(16);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;

	for (int i = 0; i < effectSize; i++) {
		while (dataType <= Effect::LOAD_DATA_SIZE) {
			if (dataType < Effect::LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			effectData = fileData.substr(q, p - q);
			data = std::stoi(effectData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		tempEffect.id = data;				break;
			case 1:		tempEffect.pos.x = data;			break;
			case 2:		tempEffect.pos.y = data;			break;
			}

			dataType++;
		}

		tempEffect.Init(tempEffect.id);

		e.push_back(tempEffect);
		dataType = 0;
	}
	std::cout << "Effects Loaded for map" << mapID << std::endl;
}