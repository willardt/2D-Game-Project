#include "spell.h"

std::vector<Texture> Spell::memInit() {
	File file;
	file.setPath("Data/Entities/Spells/spells.txt");
	std::vector<Texture> tempVec;
	Texture temp;

	for (int i = 1; i <= Spell::TOTAL_SPELL_TEXTURES; i++) {
		temp.setPath(file.readStr(i));
		tempVec.push_back(temp);
	}

	return tempVec;
}

void Spell::Init(int nid) {
	File file;
	std::string path = "";
	isActive = true;
	isEnd = false;
	id = nid;
	dis = 0;
	sprite.frame = 0;
	frame = 0;

	switch (id) {
	case 0:
		path = "fireball";
		break;
	}

	file.setPath("Data/Entities/Spells/" + path + ".txt");
	
	name = file.readStr(1);
	pos.w = file.readInt(3);
	pos.h = file.readInt(4);
	speed = file.readFloat(5);
	maxDis = file.readInt(6);
	damage = file.readInt(7);
	endFrame = file.readInt(8);
	fireRate = file.readInt(9);
	cost = file.readInt(10);

	sprite = Texture::updateSprite(pos, sprite.frame);
}

void Spell::update() {	
	if (isActive == true) {
		if (dis >= maxDis) {
			isActive = false;
			sprite.frame = 1;
			sprite = Texture::updateSprite(pos, sprite.frame);
		}

		x += speedX;
		y += speedY;
		pos.x = int(x);
		pos.y = int(y);

		dis += disInc;
	}
	else {
		frame++;
		if (frame > endFrame) {
			isEnd = true;
		}
	}
}

void Spell::hit() {
	dis = maxDis;
	isActive = false;
	sprite.frame = 1;
	sprite = Texture::updateSprite(pos, sprite.frame);
}
