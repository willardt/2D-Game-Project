#include <iostream>
#include <SDL.h>
#include <string>

#include "window.h"
#include "input.h"
#include "texture.h"
#include "textbox.h"
#include "options.h"
#include "file.h"
#include "FPS.h"
#include "Entity.h"
#include "spell.h"
#include "player.h"
#include "npc.h"
#include "map.h"
#include "bag.h"
#include "warp.h"
#include "object.h"
#include "ui.h"

#ifndef GAME_H
#define GAME_H

#define FRESH  50
#define FRESH_ENTITIES 51
#define FRESH_ITEMS 52

class Game {

private:
	Window _mainWindow;
public:
	Player player;
	std::vector<Entity> enemies;
	std::vector<Npc> npcs;
	std::vector<Item> items;
	std::vector<Text> texts;
	std::vector<Text> uiInfo;
	std::vector<Warp> warps;
	std::vector<Object> objects;
	std::vector<Object> objectSolids;
	TextBox textBox;

	Map map;
	Bag bag;

	std::vector<Texture> tilesMem;
	std::vector<Texture> enemiesMem;
	std::vector<Texture> npcsMem;
	std::vector<Texture> spellsMem;
	std::vector<Texture> itemsMem;
	std::vector<Texture> objectsMem;

	Texture background;
	SDL_Rect backRect;

	Options& options = options.Instance();

	FPS fps;

	bool isRunning;

	int mouseX;
	int mouseY;

	void Init();
	void begin();
	void input();
	void update();
	void display();
	void save();
	void load();

	void move(Entity& e, int i, int dir, int dis, bool update);
	void move(Player& p, int dir, int dis, bool update);
	void move(Npc& n, int i, int dir, int dis, bool update);

	void interactNPC();

	int spellCollision(Spell& spell, std::vector<Entity>& objs);
	int spellCollision(Spell& spell, std::vector<Npc>& npcs);
	bool spellCollision(Spell& spell, std::vector<Tile>& tile);
	bool spellCollision(Spell& spell, std::vector<Object>& solids);
	void npcCollision(const int& dir, const int& dis, Entity& obj, std::vector<Npc>& npcs);
	void playerNpcCollision(const int& dir, const int& dis, std::vector<Npc>& npcs);
	void playerCollision(const int& dir, const int& dis, Entity& enemy);
	void playerCollision(const int& dir, const int& dis, Npc& npc);
	void objectCollision(const int& dir, const int& dis, Entity& enemy);
	void objectCollision(const int& dir, const int& dis, Npc& npc);
	void objectCollision(const int& dir, const int& dis, Player& p);
	void warpCollision();
	void itemMouseOver();

	void loadMap(const int& id, int type);
	void saveMap(const int& id);

	static void loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs, bool isFresh);
	static void loadMapItems(const int& mapID, std::vector<Item>& items, bool isFresh);

};

#endif