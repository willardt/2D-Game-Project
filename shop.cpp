#include "shop.h"

const SDL_Rect Shop::RECT_LEFT = { 25, 200, 500, 700 };
const SDL_Rect Shop::RECT_RIGHT = { 575, 400, 500, 700 };
const SDL_Rect Shop::RECT_SELECTED = { 602, 75, 64, 64 };
const SDL_Rect Shop::RECT_SELECTED_BACK = { 575, 43, 500, 300};
const SDL_Rect Shop::RECT_INFO_BACK = { 25, 150, 500, 30 };
const SDL_Rect Shop::RECT_INFO = { 60, 150, NULL, 30 };
const SDL_Rect Shop::RECT_INFO_SHARDS = { 30, 150, 26, 26 };
const SDL_Rect Shop::RECT_PRICE = { 330, 150, NULL, 30 };
const SDL_Rect Shop::RECT_PRICE_SHARDS = { 300, 150, 26, 26 };
const SDL_Rect Shop::RECT_PRESSE = { 600, 300, NULL, 30 };
const SDL_Color Shop::COLOR_MARKER = { 50, 100, 150, 128 };

void Shop::shop(Window* window, Player* player, Npc* npc, std::vector<Texture>& itemMem) {
	Options& options = options.Instance();
	_window = window;
	_player = player;
	_npc = npc;
	_marker = { -100, 0, 8, 8 };
	File file;
	if (options.lang == ENGLISH) {
		file.uread("Data/UI/Shop/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.uread("Data/UI/Shop/ru.txt");
	}

	_pressE = Text::printT(TEXT_DAMAGE, file.getU16(1), RECT_PRESSE, Text::WHITE);
	_isShop = true; 

	_playerItemPositions = itemPos(int(_player->items.size()), RECT_LEFT.x, RECT_LEFT.y, RECT_LEFT.w);
	_npcItemPositions = itemPos(_npc->lootTable.size(), RECT_RIGHT.x, RECT_RIGHT.y, RECT_RIGHT.w);

	while (_isShop) {
		input();
		display(itemMem);
	}

	_pressE.destroy();
}

void Shop::input() {
	_isShop = in.get(_window->getWindow());
	_isShop = !in.isKey(SDL_SCANCODE_ESCAPE);

	if (in.isMouseHeld(_mouseX, _mouseY, SDL_BUTTON_LEFT, _window->getWindow())) {
		int q = selectItem(_playerItemPositions);
		if (q != -1) {
			_isOwn = true;
			_selectedID = q;
			createItemTextPlayer();
		}
		else {
			q = selectItem(_npcItemPositions);
			if (q != -1) {
				_isOwn = false;
				_selectedID = q;
				createItemTextNpc();
			}
		}
	}

	if (in.isKey(SDL_SCANCODE_E)) {
		if (_isOwn) {
			sell();
		}
		else {
			buy();
		}
	}
}

void Shop::display(std::vector<Texture>& itemMem) {
	SDL_Rect playerItems = { 25, 200, 32, 32 };
	Texture::drawRectTransNoCam(RECT_LEFT, Text::BLACK, _window->getRenderer());
	Texture::drawRectTransNoCam(RECT_RIGHT, Text::BLACK, _window->getRenderer());
	Texture::drawRectTransNoCam(RECT_SELECTED_BACK, Text::BLACK, _window->getRenderer());
	Texture::drawRectTransNoCam(RECT_INFO_BACK, Text::BLACK, _window->getRenderer());

	_pressE.renderNoCam(_window->getRenderer());

	_shards.destroy();
	_price.destroy();
	
	itemMem[2].renderNoCam(RECT_INFO_SHARDS, _window->getRenderer());
	itemMem[2].renderNoCam(RECT_PRICE_SHARDS, _window->getRenderer());
	_shards = Text::printT(TEXT_DAMAGE, std::to_string(_player->shards), RECT_INFO, Text::WHITE);
	_shards.renderNoCam(_window->getRenderer());

	for (size_t i = 0; i < _player->items.size(); i++) {
		itemMem[_player->items[i].id].renderNoCam(_playerItemPositions[i], _window->getRenderer());
	}

	for (size_t i = 0; i < _npc->lootTable.size(); i++) {
		itemMem[_npc->lootTable[i].id].renderNoCam(_npcItemPositions[i], _window->getRenderer());
	}

	if (_isOwn && _selectedID != -1) {
		_price = Text::printT(TEXT_DAMAGE, std::to_string(_player->items[_selectedID].price), RECT_PRICE, Text::WHITE);
		itemMem[_player->items[_selectedID].id].renderNoCam(RECT_SELECTED, _window->getRenderer());
	}
	else if (_selectedID != -1) {
		_price = Text::printT(TEXT_DAMAGE, std::to_string(_npc->lootTable[_selectedID].price), RECT_PRICE, Text::WHITE);
		itemMem[_npc->lootTable[_selectedID].id].renderNoCam(RECT_SELECTED, _window->getRenderer());
	}

	_price.renderNoCam(_window->getRenderer());

	for (size_t i = 0; i < _texts.size(); i++) {
		_texts[i].renderNoCam(_window->getRenderer());
	}

	Texture::drawRectNoCam(_marker, COLOR_MARKER, _window->getRenderer());

	_window->render();
}

void Shop::sell() {
	if (_selectedID != -1) {
		_player->shards += _player->items[_selectedID].price;
		_player->items.erase(_player->items.begin() + _selectedID);
		_selectedID = -1;
		clearText();
		_playerItemPositions = itemPos(_player->items.size(), RECT_LEFT.x, RECT_LEFT.y, RECT_LEFT.w);
	}
}

void Shop::buy() {
	if (_selectedID != -1 && _player->items.size() <= Player::MAX_BAG_SIZE) {
		Item temp = _npc->lootTable[_selectedID];

		if (_player->shards >= temp.price) {
			_player->shards -= temp.price;
			_player->items.push_back(temp);
		}
	}
	_playerItemPositions = itemPos(_player->items.size(), RECT_LEFT.x, RECT_LEFT.y, RECT_LEFT.w);
}

std::vector<SDL_Rect> Shop::itemPos(int itemSize, const int& x, const int& y, const int& wBound) {
	std::vector<SDL_Rect> temp;
	SDL_Rect rect = { x, y, 32, 32 };
	for (int i = 0; i < itemSize; i++) {
		temp.push_back(rect);
		if (rect.x > x + wBound - 50) {
			rect.x = x;
			rect.y += 38;
		}
		else {
			rect.x += 38;
		}
	}
	return temp;
}

int Shop::selectItem(std::vector<SDL_Rect>& pos) {
	SDL_Rect mouse = { _mouseX, _mouseY, 10, 10 };
	for (size_t i = 0; i < pos.size(); i++) {
		if (Collision::seperateAxis(pos[i], mouse) == true) {
			_marker.x = pos[i].x;
			_marker.y = pos[i].y;
			return i;
		}
	}
	return -1;
}

void Shop::createItemTextPlayer() {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 75;
	if (options.lang == ENGLISH) {
		file.uread("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.uread("Data/UI/Bag/ru.txt");
	}

	clearText();

	Text::printT(TEXT_DAMAGE, _player->items[_selectedID].uName, { 750, height, 50, 25 }, _texts, Item::getRarity(_player->items[_selectedID].dropChance), true);
	height += 25;

	if (_player->items[_selectedID].damage != 0) {
		modifer = setModifer(_player->items[_selectedID].damage);
		Text::printT(NULL, file.getU16(10), { 775, height, 100, 25 }, _texts, { 150, 50, 100, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].damage), { 900, height, NULL, 25 }, _texts, { 150, 50, 100, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].defense != 0) {
		modifer = setModifer(_player->items[_selectedID].defense);
		Text::printT(NULL, file.getU16(11), { 775, height, 100, 25 }, _texts, { 150, 50, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].defense), { 900, height, NULL, 25 }, _texts, { 100, 50, 150, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].health != 0) {
		modifer = setModifer(_player->items[_selectedID].health);
		Text::printT(NULL, file.getU16(6), { 775, height, 100, 25 }, _texts, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].health), { 900, height, NULL, 25 }, _texts, { 200, 50, 25, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].hps != 0) {
		modifer = setModifer(_player->items[_selectedID].hps);
		Text::printT(NULL, file.getU16(7), { 775, height, 100, 25 }, _texts, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].hps), { 900, height, NULL, 25 }, _texts, { 200, 50, 25, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].mana != 0) {
		modifer = setModifer(_player->items[_selectedID].mana);
		Text::printT(NULL, file.getU16(8), { 775, height, 100, 25 }, _texts, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].mana), { 900, height, NULL, 25 }, _texts, { 25, 50, 200, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].mps != 0) {
		modifer = setModifer(_player->items[_selectedID].mps);
		Text::printT(NULL, file.getU16(9), { 775, height, 100, 25 }, _texts, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].mps), { 900, height, NULL, 25 }, _texts, { 25, 50, 200, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].leech != 0) {
		modifer = setModifer(_player->items[_selectedID].leech);
		Text::printT(NULL, file.getU16(12), { 775, height, 100, 25 }, _texts, { 50, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].leech), { 900, height, NULL, 25 }, _texts, { 50, 150, 50, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].drain != 0) {
		modifer = setModifer(_player->items[_selectedID].drain);
		Text::printT(NULL, file.getU16(13), { 775, height, 100, 25 }, _texts, { 50, 150, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].drain), { 900, height, NULL, 25 }, _texts, { 50, 150, 150, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].luck != 0) {
		modifer = setModifer(_player->items[_selectedID].luck);
		Text::printT(NULL, file.getU16(14), { 775, height, 100, 25 }, _texts, { 150, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].luck), { 900, height, NULL, 25 }, _texts, { 150, 150, 50, 0 });
		height += 25;
	}

	if (_player->items[_selectedID].speed != 0) {
		modifer = setModifer(_player->items[_selectedID].speed);
		Text::printT(NULL, file.getU16(15), { 775, height, 100, 25 }, _texts, { 150, 250, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_player->items[_selectedID].speed), { 900, height, NULL, 25 }, _texts, { 150, 250, 150, 0 });
		height += 25;
	}
}

void Shop::createItemTextNpc() {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 75;
	if (options.lang == ENGLISH) {
		file.uread("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.uread("Data/UI/Bag/ru.txt");
	}

	clearText();

	Text::printT(TEXT_DAMAGE, _npc->lootTable[_selectedID].uName, { 750, height, 50, 25 }, _texts, Item::getRarity(_npc->lootTable[_selectedID].dropChance), true);
	height += 25;

	if (_npc->lootTable[_selectedID].damage != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].damage);
		Text::printT(NULL, file.getU16(10), { 775, height, 100, 25 }, _texts, { 150, 50, 100, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].damage), { 900, height, NULL, 25 }, _texts, { 150, 50, 100, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].defense != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].defense);
		Text::printT(NULL, file.getU16(11), { 775, height, 100, 25 }, _texts, { 150, 50, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].defense), { 900, height, NULL, 25 }, _texts, { 100, 50, 150, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].health != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].health);
		Text::printT(NULL, file.getU16(6), { 775, height, 100, 25 }, _texts, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].health), { 900, height, NULL, 25 }, _texts, { 200, 50, 25, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].hps != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].hps);
		Text::printT(NULL, file.getU16(7), { 775, height, 100, 25 }, _texts, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].hps), { 900, height, NULL, 25 }, _texts, { 200, 50, 25, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].mana != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].mana);
		Text::printT(NULL, file.getU16(8), { 775, height, 100, 25 }, _texts, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].mana), { 900, height, NULL, 25 }, _texts, { 25, 50, 200, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].mps != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].mps);
		Text::printT(NULL, file.getU16(9), { 775, height, 100, 25 }, _texts, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].mps), { 900, height, NULL, 25 }, _texts, { 25, 50, 200, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].leech != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].leech);
		Text::printT(NULL, file.getU16(12), { 775, height, 100, 25 }, _texts, { 50, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].leech), { 900, height, NULL, 25 }, _texts, { 50, 150, 50, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].drain != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].drain);
		Text::printT(NULL, file.getU16(13), { 775, height, 100, 25 }, _texts, { 50, 150, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].drain), { 900, height, NULL, 25 }, _texts, { 50, 150, 150, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].luck != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].luck);
		Text::printT(NULL, file.getU16(14), { 775, height, 100, 25 }, _texts, { 150, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].luck), { 900, height, NULL, 25 }, _texts, { 150, 150, 50, 0 });
		height += 25;
	}

	if (_npc->lootTable[_selectedID].speed != 0) {
		modifer = setModifer(_npc->lootTable[_selectedID].speed);
		Text::printT(NULL, file.getU16(15), { 775, height, 100, 25 }, _texts, { 150, 250, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(_npc->lootTable[_selectedID].speed), { 900, height, NULL, 25 }, _texts, { 150, 250, 150, 0 });
		height += 25;
	}
}

std::string Shop::setModifer(const int& num) {
	if (num >= 0) {
		return "";
	}
	return "-";
}

void Shop::clearText() {
	for (size_t i = 0; i < _texts.size(); i++) {
		_texts[i].destroy();
	}
	_texts.clear();
}