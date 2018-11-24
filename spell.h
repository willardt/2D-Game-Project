#include <vector>

#include "texture.h"
#include "file.h"
#include "time.h"
#include "ui.h"
#include "debuff.h"

#ifndef SPELL_H
#define SPELL_H

#define TYPE_MISSILE 0
#define TYPE_SPLIT 1
#define TYPE_BUFF 10
#define TYPE_SHIELD 11

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UPRIGHT 5
#define UPLEFT 6
#define DOWNRIGHT 7
#define DOWNLEFT 8

#define TYPE_FIRE 20
#define TYPE_WATER 21
#define TYPE_EARTH 22
#define TYPE_LIFE 23
#define TYPE_AIR 24
#define TYPE_NORMAL 25

class Spell {
private:
public:
	enum {SPLIT_DISTANCE = 100};

	Sprite sprite;
	SDL_Color color;

	SDL_Rect pos;

	Time endTime;
	Time spriteTimer;

	Debuff debuff;

	bool isActive;
	bool isEnd;
	bool isHalf;
	bool isSplit;
	bool hasDebuff;

	std::string name;
	u16string uname;

	int id;
	int type;
	int endSpriteFrame;
	int spriteTime;

	float speed;
	float speedX;
	float speedY;
	float dis;
	float disInc;
	int maxDis;
	int halfDis;
	int quarterDis;
	float x;
	float y;
	int dX;
	int dY;
	int damage;
	int fireRate;
	int cost;
	int endFrame;
	int dir;
	int element;

	int spaceXUI;
	int spaceYUI;
	
	void Init(int nid);
	void update();
	void updateMissile();
	void hit();

	void setSpaceUI(const int& width, const int& height, const int& nx, const int& ny);

	SDL_Rect& getUIPos();

	static std::vector<Texture> memInit();
	static void split(Spell& s1, Spell& s2);
	static bool isNegative(const float& num);
};

#endif