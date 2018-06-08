#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "texture.h"
#include "text.h"
#include "file.h"
#include "Collision.h"
#include "animation.h"
#include "map.h"
#include "textbox.h"
#include "quest.h"
#include "item.h"
#include "time.h"
#include "spell.h"

#ifndef ENTITY_H
#define ENTITY_H

#define PLAYER 0
#define ENEMY 1
#define NPC 2

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UPRIGHT 5
#define UPLEFT 6
#define DOWNRIGHT 7
#define DOWNLEFT 8

#define MOVE 9
#define CAST 10
#define SPECIAL 11

struct SpellAni {
	int totalFrames;
	int up;
	int down;
	int left;
	int right;
	int uright;
	int uleft;
	int dright;
	int dleft;
};

struct Path {
	bool met;
	int x;
	int y;
};

class Entity {
private:
public:
	// _NPCSRC - NPC SPEECH REGION SCALE SIZE
	enum { LOAD_DATA_SIZE = 5, PATH_LOAD_DATA_SIZE = 3, NPCSRC = 40 };
	enum { HEALTH_BAR_MAX_FRAME = 500 };
	enum { RAND_DAMAGE_TEXT_X = 40, RAND_DAMAGE_TEXT_Y = 16, DAMAGE_TEXT_SPACEING_X = 10, DAMAGE_TEXT_SPACEING_Y = 10 };
	enum { CASTING_ANI_TIME = 25, REGEN_TIME = 300};

	Sprite sprite;
	SDL_Rect pos;
	int posWidthHalf;
	int posHeightHalf;
	SDL_Rect combatRange;
	int combatRangeWidthHalf;
	int combatRangeHeightHalf;

	Animation ani;
	SpellAni spellAni;

	std::vector<Spell> spells;
	Spell spell;

	std::vector<Path> paths;

	Time combat;
	Time regen;
	Time casting;
	Time castingAni;

	std::string name;
	u16string uName;

	bool isDead;
	bool isCombat;
	bool isCasting;
	bool isCastable;

	int id;
	int type;

	int speed, level, exp, health, maxHealth, hps, mana, maxMana, mps, damage, defense, leech, drain, luck, shards;

	int combatFrame;
	int castDir;
	int activePath;
	int fireRate;

	std::vector<Item> lootTable;

	void Init(const int& ntype, const int& nid, const int& x, const int& y);
	
	void update();
	void updateCombat();
	void updateRegen();
	void updateCasting();
	void updateCastingAni();
	void updateSpells();
	void updatePath();
	void updateCombat(SDL_Rect& player);

	void move(const int& dir, int dis, const bool& update);
	void checkCam(const int& type, const int& dis);
	void centerCamera(const int& mapWidth, const int& mapHeight);
	void updateSprite(const int& type, const int& dir);
	void collision(const int& dir, const int& dis, std::vector<Entity>& obj, const int& selfIndex);
	void collision(const int& dir, const int& dis, std::vector<Entity>& obj);
	void collision(const int& dir, const int& dis, std::vector<Tile>& tile);
	int collision(std::vector<Entity>& obj);
	bool collision(std::vector<Tile>& tile);
	int collision(std::vector<Item>& items);
	void collision(const int& dir, const int& dis, Entity& obj);
	int collision(Spell& s, std::vector<Entity>& obj);
	bool collision(Spell& s, std::vector<Tile>& tile);

	void applyDamage(const int& damage);
	void applyDamage(const int& damage, std::vector<Text>& t, SDL_Color& color);
	int calcDamage(int d, int def, int lvl);

	void castSpellMouse(int mX, int mY);
	void castSpell(int nX, int nY);
	int calcCastDir(int mX, int mY);

	void addExp(int xp, TextBox& t);
	void dropLoot(std::vector<Item>& items);

	void loadPaths(int& size, std::string& fileData, const int& fileDataLength, int& p, int& q);

	int getPathDir();
	void reversePath();

	static std::vector<Item> loadLootTable(const int& id, const int& type);
	static std::vector<Texture> memInit(const int& type);
	static std::string findName(const int& type, const int& id);
};

#endif