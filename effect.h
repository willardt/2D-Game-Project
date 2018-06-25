#include "texture.h"
#include "file.h"
#include "time.h"

#ifndef EFFECT_H
#define EFFECT_H

class Effect {
private:
	enum { LOAD_DATA_SIZE = 2};
public:
	int id;
	SDL_Rect pos;
	Sprite sprite;
	int maxFrames;
	Time spriteTimer;
	int spriteTime;
	std::string name;

	void Init(const int& nid);
	void update();
	static std::vector<Texture> memInit();
	static void loadEffects(const int& mapID, std::vector<Effect>& e);
};

#endif