#include "item.h"

std::vector<Texture> Item::memInit() {
	std::vector<Texture> tempVec;
	Texture temp;
	File file;
	file.setPath("Data/Entities/Items/items.txt");

	for (int i = 1; i <= Item::TOTAL_TEXTURES; i++) {
		temp.setPath(file.readStr(i));
		tempVec.push_back(temp);
	}

	return tempVec;
}

void Item::Init(const int& nid) {
	Options& options = options.Instance();
	id = nid;
	isEquipped = false;
	File file;
	std::string itemPath = " ";
	file.setPath("Data/Entities/Items/itemsloc.txt");
	itemPath = file.readStr(id + 1);
	file.setPath("Data/Entities/Items/" + itemPath);

	type = file.readInt(2);
	name = file.readStr(3);
	damage = file.readInt(4);
	defense = file.readInt(5);
	health = file.readInt(6);
	hps = file.readInt(7);
	mana = file.readInt(8);
	mps = file.readInt(9);
	leech = file.readInt(10);
	drain = file.readInt(11);
	luck = file.readInt(12);
	speed = file.readInt(13);

	if (options.lang == ENGLISH) {
		file.setPath("Data/Entities/Items/En/" + itemPath);
	}
	else if (options.lang == RUSSIAN) {
		file.setPath("Data/Entities/Items/Ru/" + itemPath);
	}

	uName = file.readUnicodeStr(1);
	
	pos = { 0, 0, 32, 32 };
}