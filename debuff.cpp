#include "debuff.h"

std::vector<Texture> Debuff::memInit() {
	std::vector<Texture> tempVec;
	Texture temp;
	File file;
	file.read("Data/Entities/Debuffs/debuffs.txt");

	for (int i = 1; i <= file.getSize(); i++) {
		temp.setPath(file.getStr(i));
		tempVec.push_back(temp);
	}

	return tempVec;
}

void Debuff::Init(int nid) {
	id = nid;

	File file;
	File locFile;
	locFile.read("Data/Entities/Debuffs/debuffsloc.txt");
	file.read("Data/Entities/Debuffs/" + locFile.getStr(id + 1));

	isStackable = (bool)file.getInt(1);
	duration = file.getInt(2);
	dotd = file.getInt(3);
	dott = file.getInt(4);
	slow = file.getInt(5);
	armorReduction = file.getInt(6);
	healthReduction = file.getInt(7);
	manaReduction = file.getInt(8);
	pos = { NULL, NULL, file.getInt(9), file.getInt(10) };
	spriteTime = file.getInt(11);
	endSpriteFrame = file.getInt(12);

	color = { Uint8(file.getInt(13)), Uint8(file.getInt(14)), Uint8(file.getInt(15)), 255 };

	timer.tock(0);
	spriteTimer.tock(0);
	dot.tock(0);

	isEnd = false;
	isDotTick = true;

	element = file.getInt(16);
}

void Debuff::update() {
	if (timer.tick(duration)) {
		isEnd = true;
	}

	if (dot.tick(dott)) {
		isDotTick = true;
	}

	if (spriteTimer.tick(spriteTime)) {
		if (sprite.frame < endSpriteFrame - 1) {
			sprite.frame++;
			sprite = Texture::updateSprite(pos, sprite.frame);
		}
		else {
			sprite.frame = 0;
			sprite = Texture::updateSprite(pos, sprite.frame);
		}
	}
}