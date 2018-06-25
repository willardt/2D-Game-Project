#include "npc.h"
#include "item.h"
#include "file.h"
#include "texture.h"
#include "input.h"
#include "player.h"
#include "window.h"
#include "text.h"

#ifndef SHOP_H
#define SHOP_H

class Shop {
private:
	static const SDL_Rect RECT_LEFT;
	static const SDL_Rect RECT_RIGHT;
	static const SDL_Rect RECT_SELECTED;
	static const SDL_Rect RECT_SELECTED_BACK;
	static const SDL_Rect RECT_INFO_BACK;
	static const SDL_Rect RECT_INFO;
	static const SDL_Rect RECT_INFO_SHARDS;
	static const SDL_Rect RECT_PRICE;
	static const SDL_Rect RECT_PRICE_SHARDS;
	static const SDL_Rect RECT_PRESSE;
	static const SDL_Color COLOR_MARKER;

	int _selectedID;
	int _mouseX;
	int _mouseY;

	bool _isOwn;		// Is in the player's inventory
	bool _isShop;

	Input in;

	Window* _window;
	Player* _player;
	Npc* _npc;

	Text _shards;
	Text _price;
	Text _pressE;

	SDL_Rect _marker;

	std::vector<SDL_Rect> _playerItemPositions;
	std::vector<SDL_Rect> _npcItemPositions;
	std::vector<Text> _texts;
public:
	void shop(Window* window, Player* player, Npc* npc, std::vector<Texture>& itemMem);
	void input();
	void display(std::vector<Texture>& itemMem);
	void sell();
	void buy();
	int selectItem(std::vector<SDL_Rect>& area);
	std::vector<SDL_Rect> itemPos(int size, const int& x, const int& y, const int& wBound);
	void createItemTextPlayer();
	void createItemTextNpc();
	std::string setModifer(const int& num);
	void clearText();
};

#endif