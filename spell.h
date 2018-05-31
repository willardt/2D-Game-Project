#include <vector>

#include "texture.h"
#include "file.h"

#ifndef SPELL_H
#define SPELL_H

class Spell {
private:
public:
	enum {TOTAL_SPELL_TEXTURES = 1};

	Sprite sprite;

	SDL_Rect pos;

	bool isActive;
	bool isEnd;

	std::string name;

	int id;

	float speed;
	float speedX;
	float speedY;
	float dis;
	float disInc;
	int maxDis;
	float x;
	float y;
	int dX;
	int dY;
	int damage;
	int fireRate;
	int cost;
	int frame;
	int endFrame;
	
	void Init(int nid);
	void update();
	void hit();

	static std::vector<Texture> memInit();
};

#endif