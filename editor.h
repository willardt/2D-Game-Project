#include <iostream>
#include <string>
#include <SDL.h>

#include "window.h"
#include "texture.h"
#include "input.h"
#include "FPS.h"
#include "map.h"
#include "file.h"
#include "entity.h"
#include "npc.h"
#include "item.h"

#ifndef EDITOR_H
#define EDITOR_H

#define NEW 1

class Editor {
private:
	Window _mainWindow;
	Window _tileWindow;
public:
	enum { MAIN_WINDOW_WIDTH = 1100, MAIN_WINDOW_HEIGHT = 800, MAIN_WINDOW_X = 400, MAIN_WINDOW_Y = 200 };
	enum {TILE_WINDOW_WIDTH = 400, TILE_WINDOW_HEIGHT = 800, TILE_WINDOW_X = 1500, TILE_WINDOW_Y = 200};
	enum {TILE_LAYOUT_Y = 400};

	std::string mainTitle = "Level Editor - Blank";
	std::string tileTitle = "Tiles";

	Texture background;
	SDL_Rect backRect;
	SDL_Rect tileRect;
	SDL_Rect selectedRect;
	Texture selectedTextureTile;
	Texture selectedTextureEnemy;
	Texture selectedTextureNpc;
	Texture selectedTextureItem;

	FPS fps;
	Map map;
	std::vector<Texture> tileMap;
	std::vector<Texture> tileEnemyMem;
	std::vector<Texture> mainEnemyMem;
	std::vector<Texture> tileNpcMem;
	std::vector<Texture> mainNpcMem;
	std::vector<Texture> tileItemsMem;
	std::vector<Texture> mainItemsMem;

	std::vector<Entity> enemies;
	std::vector<Npc> npcs;
	std::vector<Item> items;

	bool isRunning;
	bool isTiles;
	bool isEnemies;
	bool isNpcs;
	bool isItems;

	int mouseX;
	int mouseY;
	int currentT;
	Entity currentE;
	Npc currentN;
	Item currentI;

	void Init();
	void begin();
	void input();
	void update();
	void display();

	int setCurrentTile();
	Entity setCurrentEntity();
	Npc setCurrentNpc();
	Item setCurrentItem();
	void placeTile();
	void placeEntity();
	void placeItem();

	void save();
	void load();
	void create();

	void loadMap(const int& id);
	void Editor::loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs);
	void Editor::loadMapItems(const int& mapID, std::vector<Item>& items);

	std::string getFileIDPath(const int& id);
	int getMapIDInput(const int& type);
};

#endif