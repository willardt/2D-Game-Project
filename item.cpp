#include "item.h"

std::vector<Texture> Item::memInit() {
	std::vector<Texture> tempVec;
	Texture temp;
	File file;
	file.read("Data/Entities/Items/items.txt");

	for (int i = 1; i <= file.getSize(); i++) {
		temp.setPath(file.getStr(i));
		tempVec.push_back(temp);
	}

	return tempVec;
}

void Item::Init(const int& nid) {
	Options& options = options.Instance();
	id = nid;
	isEquipped = false;
	type = -1;
	name = "";
	damage = 0;
	defense = 0;
	health = 0;
	hps = 0;
	mana = 0;
	mps = 0;
	leech = 0;
	drain = 0;
	luck = 0;
	speed = 0;
	dropChance = 0;
	uName = { 0x00 };

	if (id != -1) {
		File file;
		std::string itemPath = " ";
		file.read("Data/Entities/Items/itemsloc.txt");
		itemPath = file.getStr(id + 1);
		file.read("Data/Entities/Items/" + itemPath);

		type = file.getInt(2);
		name = file.getStr(3);
		damage = file.getInt(4);
		defense = file.getInt(5);
		health = file.getInt(6);
		hps = file.getInt(7);
		mana = file.getInt(8);
		mps = file.getInt(9);
		leech = file.getInt(10);
		drain = file.getInt(11);
		luck = file.getInt(12);
		speed = file.getInt(13);
		dropChance = file.getInt(14);
		price = file.getInt(15);

		if (options.lang == ENGLISH) {
			file.uread("Data/Entities/Items/En/" + itemPath);
		}
		else if (options.lang == RUSSIAN) {
			file.uread("Data/Entities/Items/Ru/" + itemPath);
		}

		uName = file.getU16(1);
	}
	
	pos = { 0, 0, 32, 32 };
}

SDL_Color Item::getRarity(int& dropChance) {
	if (dropChance > RARE_GREEN) {
		return { 150, 150, 150, 255 };
	}
	if (dropChance > RARE_BLUE) {
		return { 150, 250, 150, 255 };
	}
	if (dropChance > RARE_PURPLE) {
		return { 150, 100, 200, 255 };
	}
	if (dropChance > RARE_ORANGE) {
		return { 225, 150, 200, 255 };
	}
	if (dropChance >= 0) {
		return { 250, 200, 175, 255 };
	}
	return { 255, 255, 255, 255 };
}