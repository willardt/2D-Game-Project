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

#ifndef GAME_H
#define GAME_H

class Game {

private:
	Window _mainWindow;
public:
	Player player;
	std::vector<Entity> enemies;
	std::vector<Npc> npcs;
	std::vector<Item> items;
	std::vector<Text> texts;
	TextBox textBox;

	Map map;
	Bag bag;

	std::vector<Texture> enemiesMem;
	std::vector<Texture> npcsMem;
	std::vector<Texture> spellsMem;
	std::vector<Texture> itemsMem;

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

	void interactNPC();

	int spellCollision(Spell& spell, std::vector<Entity>& objs);
	bool spellCollision(Spell& spell, std::vector<Tile>& tile);

	void loadMap(const int& id);
	void saveMapItems(const int& id);

	void createMap();

	static void loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs);
	static void loadMapItems(const int& mapID, std::vector<Item>& items);

};

#endif