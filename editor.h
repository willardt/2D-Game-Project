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
#include "warp.h"
#include "object.h"

#ifndef EDITOR_H
#define EDITOR_H

#define NEW 1
#define OLD 2

#define CHOOSE_TILES 50
#define CHOOSE_ENEMIES 51
#define CHOOSE_NPCS 52
#define CHOOSE_ITEMS 53
#define CHOOSE_WARPS 54
#define CHOOSE_OBJECTS 55
#define CHOOSE_NONE 56

#define SELECT_ENEMY 60
#define SELECT_NPC 61
#define SELECT_ITEM 62
#define SELECT_WARP 63
#define SELECT_OBJECT 64

#define FRESH  40
#define FRESH_ENTITIES 41
#define FRESH_ITEMS 42

class Editor {
private:
	Window _mainWindow;
	Window _tileWindow;
public:
	enum { MAIN_WINDOW_WIDTH = 1100, MAIN_WINDOW_HEIGHT = 800, MAIN_WINDOW_X = 400, MAIN_WINDOW_Y = 200 };
	enum {TILE_WINDOW_WIDTH = 400, TILE_WINDOW_HEIGHT = 800, TILE_WINDOW_X = 1500, TILE_WINDOW_Y = 200};
	enum {TILE_LAYOUT_Y = 400, ROW_SIZE = 12};

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
	Texture selectedTextureObject;

	FPS fps;
	Map map;
	std::vector<Texture> mainMap;
	std::vector<Texture> tileMap;
	std::vector<Texture> tileEnemyMem;
	std::vector<Texture> mainEnemyMem;
	std::vector<Texture> tileNpcMem;
	std::vector<Texture> mainNpcMem;
	std::vector<Texture> tileItemsMem;
	std::vector<Texture> mainItemsMem;
	std::vector<Texture> tileObjectMem;
	std::vector<Texture> mainObjectMem;

	std::vector<Entity> enemies;
	std::vector<Npc> npcs;
	std::vector<Item> items;
	std::vector<Path> paths;
	std::vector<Warp> warps;
	std::vector<Object> objects;

	Text selectedName;

	bool isRunning;
	bool isWarpSelecting;
	bool isWarpPos;
	bool isWarpDest;
	bool isPathSelecting;

	int mouseX;
	int mouseY;
	int selected;
	int currentT;
	int chooseType;
	int selectType;
	Entity currentE;
	Npc currentN;
	Item currentI;
	Warp currentW;
	Object currentO;

	void Init();
	void begin();
	void input();
	void update();
	void display();

	int setCurrentTile();
	Entity setCurrentEntity();
	Npc setCurrentNpc();
	Item setCurrentItem();
	void setCurrentWarp(SDL_Rect& rect);
	Object setCurrentObject();

	void placeTile();
	void placeEntity();
	void placeItem();
	void placePath(Entity& e, std::vector<Path>& p);
	void placePath(Npc& n, std::vector<Path>& p);
	void placeWarp(SDL_Rect& rect);
	void placeObject();
	void select();

	int collision(const int& x, const int& y, std::vector<Entity>& e);
	int collision(const int& x, const int& y, std::vector<Npc>& n);
	int collision(const int& x, const int& y, std::vector<Item>& it);
	int collision(const int& x, const int& y, std::vector<Warp>& w);
	int collision(const int& x, const int& y, std::vector<Object>& o);

	void save();
	void load();
	void create();

	void loadMap(const int& id);
	void Editor::loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs, bool isFresh);
	void Editor::loadMapItems(const int& mapID, std::vector<Item>& items, bool isFresh);

	std::string getFileIDPath(const int& id);
	int getMapIDInput(const int& type);
};

#endif