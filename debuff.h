#include "texture.h"
#include "file.h"
#include "time.h"

#ifndef DEBUFF_H
#define DEBUFF_H

class Debuff {
private:
public:
	bool isStackable;
	bool isDotTick;
	bool isEnd;

	int id;
	//dotd - dot damage
	int dotd;
	//dott - dot tick
	int dott;
	int armorReduction;
	int healthReduction;
	int manaReduction;
	int slow;
	int duration;
	int element;
	int spriteTime;
	int endSpriteFrame;

	SDL_Color color;
	Sprite sprite;

	Time dot;
	Time timer;
	Time spriteTimer;

	SDL_Rect pos;

	void Init(int nid);
	static std::vector<Texture> memInit();

	void update();
};

#endif