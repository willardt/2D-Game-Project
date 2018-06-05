#include "spell.h"

std::vector<Texture> Spell::memInit() {
	File file;
	file.read("Data/Entities/Spells/spells.txt");
	std::vector<Texture> tempVec;
	Texture temp;

	for (int i = 1; i <= file.getSize(); i++) {
		temp.setPath(file.getStr(i));
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
	sprite.frame = 0;
	frame = 0;

	switch (id) {
	case 0:
		path = "fireball";
		break;
	}

	file.read("Data/Entities/Spells/" + path + ".txt");
	
	name = file.getStr(1);
	pos.w = file.getInt(3);
	pos.h = file.getInt(4);
	speed = file.getFloat(5);
	maxDis = file.getInt(6);
	damage = file.getInt(7);
	endFrame = file.getInt(8);
	fireRate = file.getInt(9);
	cost = file.getInt(10);
	color = { Uint8(file.getInt(11)), Uint8(file.getInt(12)), Uint8(file.getInt(13)), 255 };

	speedX = 0;
	speedY = 0;
	dis = 0;
	disInc = 0;
	x = 0;
	y = 0;
	dX = 0;
	dY = 0;

	sprite = Texture::updateSprite(pos, sprite.frame);
}

void Spell::update() {	
	Options& options = options.Instance();
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
	dis = float(maxDis);
	isActive = false;
	sprite.frame = 1;
	sprite = Texture::updateSprite(pos, sprite.frame);
}
