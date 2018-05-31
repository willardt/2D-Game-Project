#include "input.h"
#include "texture.h"
#include "window.h"
#include "options.h"
#include "item.h"
#include "Collision.h"
#include "text.h"
#include "Player.h"

#ifndef BAG_H
#define BAG_H

class Bag {
private:
	enum { _BAG_ITEM_X = 25, _BAG_ITEM_SPACING_X = 42, _BAG_ITEM_SPACING_Y = 40, ITEMS_PER_LINE = 25, Y_OFFSET = 60, BACKGROUND_ALPHA = 5, 
		   _SELECTED_X = 800, _SELECTED_Y = 50};
	Texture _background;
	SDL_Rect _backgroundRect;
	SDL_Rect _markerRect;
	SDL_Rect _selectedRect;
	SDL_Color _markerColor;

	Window _mainWindow;

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
	void Init(Player& player, std::vector<Item>& mapItems, std::vector<Texture>& itemsMem, Window& window);
	void begin(Player& player, std::vector<Item>& mapItems, std::vector<Texture>& itemMem);
	void input(Player& player, std::vector<Item>& mapItems);
	void display(Player& player, std::vector<Texture>& itemsMem);
	int selectItem(std::vector<SDL_Rect>& area);
	void createItemPos(Player& player);
	void createEquipItemPos();
	void createTexts(Player& player);
	void createItemTexts(Player& player, const int& index);
	void createItemTextsEquip(Player& player, const int& slot);
	std::string setModifer(const int& num);
};

#endif