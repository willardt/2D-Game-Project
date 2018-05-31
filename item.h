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
	enum { TOTAL_TEXTURES = 2 };
public:
	enum {LOAD_DATA_SIZE = 2};
	SDL_Rect pos;
	std::string name;
	u16string uName;

	int id;
	int type;

	int damage, defense, health, hps, mana, mps, leech, drain, luck, speed;

	bool isEquipped;

	void Init(const int& nid);

	static std::vector<Texture> memInit();
};

#endif