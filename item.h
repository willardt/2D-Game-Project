#include <string.h>

#include "file.h"
#include "texture.h"
#include "options.h"

#ifndef ITEM_H
#define ITEM_H

#define WEP 0
#define OFFHAND 1
#define PANTS 2
#define CHEST 3
#define RING 4
#define NECK 5
#define BOOTS 6
#define GLOVES 7
#define MISC 8
#define HELM 9

class Item {
private:
public:
	enum {LOAD_DATA_SIZE = 3};
	enum {RARE_GRAY = 50, RARE_GREEN = 30, RARE_BLUE = 15, RARE_PURPLE = 7, RARE_ORANGE = 1};
	SDL_Rect pos;
	std::string name;
	u16string uName;

	int id;
	int type;

	int damage, defense, health, hps, mana, mps, leech, drain, luck, speed, dropChance, price;
	bool isEquipped;

	void Init(const int& nid);

	static std::vector<Texture> memInit();
	static SDL_Color getRarity(int& dropChance);
};

#endif