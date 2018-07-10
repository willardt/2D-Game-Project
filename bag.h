#include "input.h"
#include "texture.h"
#include "window.h"
#include "options.h"
#include "item.h"
#include "Collision.h"
#include "text.h"
#include "Player.h"
#include "FPS.h"
#ifndef BAG_H
#define BAG_H

class Bag {
private:
	enum { _BAG_ITEM_X = 50, _BAG_ITEM_SPACING_X = 42, _BAG_ITEM_SPACING_Y = 40, ITEMS_PER_LINE = 36, Y_OFFSET = 30, BACKGROUND_ALPHA = 5, 
		   _SELECTED_X = 1100, _SELECTED_Y = 100};

	SDL_Rect _playerBack;
	SDL_Rect _itemBack;
	SDL_Rect _infoBack;
	SDL_Rect _markerRect;
	SDL_Rect _selectedRect;
	SDL_Rect _playerRect;
	SDL_Color _markerColor;

	Texture _playerTexture;

	Window* _mainWindow;
	Input in;

	FPS fps;

	Player* player;

	std::vector<Item>* mapItems;

	std::vector<SDL_Rect> _itemsPos;
	std::vector<SDL_Rect> _equipItemsPos;
	std::vector<Text> _texts;
	std::vector<Text> _itemText;

	bool _isRunning;
	bool _isSelected;
	bool _isSelectedEquip;

	int _bagItemX;
	int _bagItemY;

	int _mouseX;
	int _mouseY;

	int _selected;
	int _selectedEquip;
public:

	const static SDL_Color LEVEL;
	const static SDL_Color EXP;
	const static SDL_Color HEALTH;
	const static SDL_Color HEALTH_REGEN;
	const static SDL_Color MANA;
	const static SDL_Color MANA_REGEN;
	const static SDL_Color DAMAGE;
	const static SDL_Color DEFENSE;
	const static SDL_Color LEECH;
	const static SDL_Color DRAIN;
	const static SDL_Color LUCK;
	const static SDL_Color SPEED;
	const static SDL_Color SHARDS;
	const static SDL_Color DURATION;

	void Init(Player* nplayer, std::vector<Item>* nmapItems, std::vector<Texture>& itemsMem, Window* window);
	void begin(std::vector<Texture>& itemsMem);
	void input();
	void display(std::vector<Texture>& itemsMem);
	int selectItem(std::vector<SDL_Rect>& area);
	void createItemPos();
	void createEquipItemPos();
	void createTexts();
	void createItemTexts(const int& index);
	void createItemTextsEquip(const int& slot);
	std::string setModifer(const int& num);
};

#endif