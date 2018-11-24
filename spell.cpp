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
	Options& options = options.Instance();
	File file;
	File locFile;
	File uFile;
	std::string path = "";
	isActive = true;
	isEnd = false;
	id = nid;
	sprite.frame = 0;

	if (id == -1) {
		return;
	}

	locFile.read("Data/Entities/Spells/spellsloc.txt");
	file.read("Data/Entities/Spells/" + locFile.getStr(id + 1));

	if (options.lang == ENGLISH) {
		uFile.uread("Data/Entities/Spells/En/" + locFile.getStr(id + 1));
	}
	else if (options.lang == RUSSIAN) {
		uFile.uread("Data/Entities/Spells/Ru/" + locFile.getStr(id + 1));
	}
	
	name = file.getStr(1);
	uname = uFile.getU16(1);
	pos.w = file.getInt(3);
	pos.h = file.getInt(4);
	speed = file.getFloat(5);
	maxDis = file.getInt(6);
	damage = file.getInt(7);
	endFrame = file.getInt(8);
	fireRate = file.getInt(9);
	cost = file.getInt(10);
	color = { Uint8(file.getInt(11)), Uint8(file.getInt(12)), Uint8(file.getInt(13)), 255 };
	type = file.getInt(14);

	speedX = 0;
	speedY = 0;
	dis = 0;
	disInc = 0;
	x = 0;
	y = 0;
	dX = 0;
	dY = 0;
	halfDis = maxDis / 2;
	quarterDis = halfDis / 2;
	endTime.tock(0);
	isHalf = false;
	isSplit = false;

	sprite = Texture::updateSprite(pos, sprite.frame);
	endSpriteFrame = file.getInt(15);
	spriteTime = file.getInt(16);
	spriteTimer.tock(0);

	if (file.getInt(17) != 0) {
		hasDebuff = true;
	}
	debuff.Init(file.getInt(17));

	element = file.getInt(18);
}

void Spell::update() {	
	Options& options = options.Instance();
	if (isActive == true) {
		if (type == TYPE_SPLIT && isSplit == false && dis > quarterDis) {
			isHalf = true;
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

		if (dis >= maxDis) {
			isActive = false;
			sprite.frame = endSpriteFrame;
			sprite = Texture::updateSprite(pos, sprite.frame);
		}

		switch (type) {
		case TYPE_MISSILE:		updateMissile();		break;
		case TYPE_SPLIT:		updateMissile();		break;
		}
	}
	else {
		if (endTime.tick(endFrame)) {
			isEnd = true;
		}
	}
}

void Spell::updateMissile() {
	x += speedX;
	y += speedY;
	pos.x = int(x);
	pos.y = int(y);

	dis += disInc;
}

void Spell::hit() {
	dis = float(maxDis);
	isActive = false;
	sprite.frame = endSpriteFrame;
	sprite = Texture::updateSprite(pos, sprite.frame);
}

void Spell::split(Spell& s1, Spell& s2) {
	if (abs(s1.speedX) < 2) {
		s1.dX += SPLIT_DISTANCE;
		s2.dX -= SPLIT_DISTANCE;
		s1.speedX = s1.dX * s1.speed;
		s2.speedX = s2.dX * s2.speed;
	}
	else if (abs(s2.speedY) < 2) {
		s1.dY += SPLIT_DISTANCE;
		s2.dY -= SPLIT_DISTANCE;
		s1.speedY = s1.dY * s1.speed;
		s2.speedY = s2.dY * s2.speed;
	}
	else {
		bool negX = Spell::isNegative(s1.speedX);
		bool negY = Spell::isNegative(s1.speedY);

		if (negX) {
			s1.dX += SPLIT_DISTANCE;
		}
		else {
			s1.dX -= SPLIT_DISTANCE;
		}
		if (negY) {
			s2.dY += SPLIT_DISTANCE;
		}
		else {
			s2.dY -= SPLIT_DISTANCE;
		}

		s1.speedX = s1.dX * s1.speed;
		s2.speedY = s2.dY * s2.speed;
	}

	s1.isSplit = true;
	s2.isSplit = true;
}

bool Spell::isNegative(const float& num) {
	if (num >= 0) {
		return false;
	}
	return true;
}

void Spell::setSpaceUI(const int& width, const int& height, const int& nx, const int& ny) {
	spaceXUI = (width - pos.w) / 2;
	spaceYUI = (height - pos.h) / 2;
	pos.x = nx + spaceXUI;
	pos.y = ny + spaceYUI;
}