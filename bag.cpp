#include "bag.h"

void Bag::Init(Player& player, std::vector<Item>& mapItems, std::vector<Texture>& itemsMem, Window& window) {
	Options& options = options.Instance();
	_isRunning = true;
	_mainWindow = window;
	_background.setPath("Data/Textures/UI/bagbackground.png");
	_backgroundRect = { 0, 0, int(options.windowWidth), int(options.windowHeight) };
	_markerRect = { -100, 0, 8, 8 };
	_markerColor = { 50, 100, 150, 128 };
	_selectedRect = { _SELECTED_X, _SELECTED_Y, 64, 64 };
	createItemPos(player);
	createEquipItemPos();
	createTexts(player);
	_selected = 0;
	_isSelected = false;
	_isSelectedEquip = false;
	begin(player, mapItems, itemsMem);
}

void Bag::begin(Player& player, std::vector<Item>& mapItems, std::vector<Texture>& itemsMem) {
	_isRunning = true;
	createItemPos(player);
	_selected = 0;
	_isSelected = false;

	while (_isRunning != false) {
		input(player, mapItems);
		display(player, itemsMem);
	}
}

void Bag::input(Player& player, std::vector<Item>& mapItems) {
	static Input in;

	static bool keyDownC = true;
	static bool keyDownE = true;
	static bool keyDownR = true;
	static bool keyDownQ = true;

	_isRunning = in.getQuit(_mainWindow.getWindow());


	if (in.isPressed(SDL_SCANCODE_C) == false) {
		keyDownC = false;
	}

	if (keyDownC == false && in.isPressed(SDL_SCANCODE_C) == true) {
		keyDownC = true;
		_isRunning = false;
	}

	if (in.leftClick(_mouseX, _mouseY, _mainWindow.getWindow())) {
		_selected = selectItem(_itemsPos);
		if (_selected != -1) {
			_isSelected = true;
			_isSelectedEquip = false;
			createItemTexts(player, _selected);
		}
		else {
			_isSelected = false;
			_selectedEquip = selectItem(_equipItemsPos);
			if (_selectedEquip != -1 && player.equipped[_selectedEquip].id != -1) {
				_isSelectedEquip = true;
				createItemTextsEquip(player, _selectedEquip);
			}
			else {
				_isSelectedEquip = false;
			}
		}
	}

	if (in.isPressed(SDL_SCANCODE_E) == false) {
		keyDownE = false;
	}

	if (keyDownE == false && in.isPressed(SDL_SCANCODE_E) == true) {
		keyDownE = true;
		if (_isSelected == true) {
			player.equipItem(_selected);
			createItemPos(player);
			createTexts(player);
			_isSelected = false;
		}
	}

	if (in.isPressed(SDL_SCANCODE_R) == false) {
		keyDownR = false;
	}

	if (keyDownR == false && in.isPressed(SDL_SCANCODE_R) == true) {
		keyDownR = true;
		if (_isSelectedEquip == true) {
			player.unequipItem(_selectedEquip);
			createItemPos(player);
			createTexts(player);
			_isSelectedEquip = false;
		}	
	}

	if (in.isPressed(SDL_SCANCODE_Q) == false) {
		keyDownQ = false;
	}

	if (keyDownQ == false && in.isPressed(SDL_SCANCODE_Q) == true) {
		keyDownQ = true;
		if (_isSelectedEquip == true) {
			if (player.equipped[_selectedEquip].isEquipped == true) {
				player.maxHealth -= player.equipped[_selectedEquip].health;
				if (player.health > player.maxHealth) {
					player.health = player.maxHealth;
				}
				player.hps -= player.equipped[_selectedEquip].hps;
				player.maxMana -= player.equipped[_selectedEquip].mana;
				if (player.mana > player.maxMana) {
					player.mana = player.maxMana;
				}
				player.mps -= player.equipped[_selectedEquip].mps;
				player.damage -= player.equipped[_selectedEquip].damage;
				player.defense -= player.equipped[_selectedEquip].defense;
				player.leech -= player.equipped[_selectedEquip].leech;
				player.drain -= player.equipped[_selectedEquip].drain;
				player.luck -= player.equipped[_selectedEquip].luck;
				player.speed -= player.equipped[_selectedEquip].speed;

				player.equipped[_selectedEquip].pos.x = player.pos.x;
				player.equipped[_selectedEquip].pos.y = player.pos.y;
				mapItems.push_back(player.equipped[_selectedEquip]);

				player.equipped[_selectedEquip].isEquipped = false;
				player.equipped[_selectedEquip].id = -1;
				_isSelectedEquip = false;
				createItemPos(player);
				createTexts(player);
			}
		}
		else if (_isSelected == true) {
			player.items[_selected].pos.x = player.pos.x;
			player.items[_selected].pos.y = player.pos.y;
			mapItems.push_back(player.items[_selected]);
			player.items.erase(player.items.begin() + _selected);
			_isSelected = false;
			createItemPos(player);
			createTexts(player);
		}
	}

}

void Bag::display(Player& player, std::vector<Texture>& itemsMem) {
	Options& options = options.Instance();
	size_t j = 0;
	size_t bagSize = player.items.size();

	//-----BACKGROUND------
	_background.renderNoCamTrans(_backgroundRect, _mainWindow.getRenderer(), BACKGROUND_ALPHA);

	//-----Bag-----
	for (size_t i = 0; i < player.items.size(); i++) {
		itemsMem[player.items[i].id].renderNoCam(_itemsPos[i], _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < _equipItemsPos.size(); i++) {
		Texture::drawRectNoCam(_equipItemsPos[i], { 150, 150, 150, 150 } , _mainWindow.getRenderer());
	}

	for (int i = 0; i < Player::EQUIPPED_SIZE; i++) {
		if (player.equipped[i].id != -1) {
			itemsMem[player.equipped[i].id].renderNoCam(_equipItemsPos[i], _mainWindow.getRenderer());
		}
	}

	if (_isSelected == true) {
		Texture::drawRectNoCam(_markerRect, _markerColor, _mainWindow.getRenderer());
		//SDL_SetRenderDrawColor(_mainWindow.getRenderer(), _markerColor.r, _markerColor.g, _markerColor.b, _markerColor.a);
		//SDL_RenderDrawRect(_mainWindow.getRenderer(), &_markerRect);

		itemsMem[player.items[_selected].id].renderNoCam(_selectedRect, _mainWindow.getRenderer());

		for (size_t i = 0; i < _itemText.size(); i++) {
			_itemText[i].renderNoCam(_mainWindow.getRenderer());
		}
	}

	if (_isSelectedEquip == true) {
		Texture::drawRectNoCam(_markerRect, _markerColor, _mainWindow.getRenderer());

		itemsMem[player.equipped[_selectedEquip].id].renderNoCam(_selectedRect, _mainWindow.getRenderer());

		for (size_t i = 0; i < _itemText.size(); i++) {
			_itemText[i].renderNoCam(_mainWindow.getRenderer());
		}
	}

	//-----Text-----
	for (size_t i = 0; i < _texts.size(); i++) {
		_texts[i].renderNoCam(_mainWindow.getRenderer());
	}

	_mainWindow.render();

}

int Bag::selectItem(std::vector<SDL_Rect>& area) {
	SDL_Rect mouse = { _mouseX, _mouseY, 10, 10};
	size_t bagSize = area.size();
	for (size_t i = 0; i < bagSize; i++) {
		if (Collision::seperateAxis(area[i], mouse) == true) {
			_markerRect.x = area[i].x;
			_markerRect.y = area[i].y;
			return i;
		}
	}
	return -1;

}

void Bag::createItemPos(Player& player) {
	Options& options = options.Instance();
	size_t bagSize = player.items.size();
	SDL_Rect position = { _BAG_ITEM_X, (options.windowHeight / 2) + Y_OFFSET, 32, 32 };

	_itemsPos.clear();

	for (size_t i = 0; i < bagSize;) {
		int j = 0;
		position.x = _BAG_ITEM_X;
		while (j < ITEMS_PER_LINE && i < int(bagSize)) {
			position.x = j * _BAG_ITEM_SPACING_X + _BAG_ITEM_X;
			_itemsPos.push_back(position);
			i++;
			j++;
		}
		position.y += _BAG_ITEM_SPACING_Y;
	}
}

void Bag::createEquipItemPos() {
	SDL_Rect temp = { 0, 0, 48, 48 };
	_equipItemsPos.clear();

	temp = { 75, 200, 48, 48 };		//Weapon
	_equipItemsPos.push_back(temp);
	temp = { 250, 200, 48, 48 };	//OffHand
	_equipItemsPos.push_back(temp);
	temp = { 75, 250, 48, 48 };		//Pants
	_equipItemsPos.push_back(temp);
	temp = { 75, 150, 48, 48 };		//Chest
	_equipItemsPos.push_back(temp);
	temp = { 75, 300, 48, 48 };		//Ring
	_equipItemsPos.push_back(temp);
	temp = { 75, 100, 48, 48 };		//Neck
	_equipItemsPos.push_back(temp);
	temp = { 250, 250, 48, 48 };	//Boots
	_equipItemsPos.push_back(temp);
	temp = { 250, 150, 48, 48 };	//Gloves
	_equipItemsPos.push_back(temp);
	temp = { 250, 300, 48, 48 };	//Misc
	_equipItemsPos.push_back(temp);
	temp = { 250, 100, 48, 48 };	//Helm
	_equipItemsPos.push_back(temp);
}

void Bag::createTexts(Player& player) {
	Options& options = options.Instance();
	File file;
	if (options.lang == ENGLISH) {
		file.uread("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.uread("Data/UI/Bag/ru.txt");
	}

	for (size_t i = 0; i < _texts.size(); i++) {
		_texts[i].destroy();
	};
	_texts.clear();


	Text::printT(NULL, file.getU16(1), { 950, 75, 120, 30 }, _texts, Text::WHITE, false);		//EQUIP
	Text::printT(NULL, file.getU16(2), { 950, 100, 130, 30 }, _texts, Text::WHITE, false);		//UNEQUIP
	Text::printT(NULL, file.getU16(3), { 950, 125, 110, 30 }, _texts, Text::WHITE, false);		//DROP
	Text::printT(NULL, player.uName, { 700, 25, NULL, 30 }, _texts, Text::WHITE, true);					//NAME
	Text::printT(NULL, file.getU16(4), { 400, 50, 90, 25 }, _texts, Text::WHITE, false);		//Level
	Text::printT(TEXT_DAMAGE, std::to_string(player.level), { 500, 50, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(5), { 400, 75, 50, 25 }, _texts, Text::WHITE, false);		//EXP
	Text::printT(TEXT_DAMAGE, std::to_string(player.exp), { 500, 75, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(6), { 400, 100, 90, 25 }, _texts, Text::WHITE, false);		//HEALTH
	Text::printT(TEXT_DAMAGE, std::to_string(player.health) + "/" + std::to_string(player.maxHealth), { 500, 100, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(7), { 400, 125, 50, 25 }, _texts, Text::WHITE, false);		//HPS
	Text::printT(TEXT_DAMAGE, std::to_string(player.hps), { 500, 125, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(8), { 400, 150, 70, 25 }, _texts, Text::WHITE, false);		//MANA
	Text::printT(TEXT_DAMAGE, std::to_string(player.mana) + "/" + std::to_string(player.maxMana), { 500, 150, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(9), { 400, 175, 50, 25 }, _texts, Text::WHITE, false);		//MPS
	Text::printT(TEXT_DAMAGE, std::to_string(player.mps), { 500, 175, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(10), { 400, 200, 90, 25 }, _texts, Text::WHITE, false);		//DAMAGE
	Text::printT(TEXT_DAMAGE, std::to_string(player.damage), { 500, 200, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(11), { 400, 225, 100, 25 }, _texts, Text::WHITE, false);		//DEFENSE
	Text::printT(TEXT_DAMAGE, std::to_string(player.defense), { 500, 225, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(12), { 400, 250, 80, 25 }, _texts, Text::WHITE, false);		//LEECH
	Text::printT(TEXT_DAMAGE, std::to_string(player.leech), { 500, 250, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(13), { 400, 275, 80, 25 }, _texts, Text::WHITE, false);		//DRAIN
	Text::printT(TEXT_DAMAGE, std::to_string(player.drain), { 500, 275, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(14), { 400, 300, 70, 25 }, _texts, Text::WHITE, false);		//LUCK
	Text::printT(TEXT_DAMAGE, std::to_string(player.luck), { 500, 300, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(15), { 400, 325, 80, 25 }, _texts, Text::WHITE, false);		//SPEED
	Text::printT(TEXT_DAMAGE, std::to_string(player.speed), { 500, 325, NULL, 25 }, _texts, Text::WHITE);
	Text::printT(NULL, file.getU16(16), { 400, 350, 90, 25 }, _texts, Text::WHITE, false);		//SHARDS
	Text::printT(TEXT_DAMAGE, std::to_string(player.shards), { 500, 350, NULL, 25 }, _texts, Text::WHITE);
}

void Bag::createItemTexts(Player& player, const int& index) {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 125;
	if (options.lang == ENGLISH) {
		file.uread("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.uread("Data/UI/Bag/ru.txt");
	}

	for (size_t i = 0; i < _itemText.size(); i++) {
		_itemText[i].destroy();
	}
	_itemText.clear();

	Text::printT(TEXT_DAMAGE, player.items[index].uName, { 700, height, 50, 25 }, _itemText, Item::getRarity(player.items[index].dropChance), true);
	height += 25;

	if (player.items[index].damage != 0) {
		modifer = setModifer(player.items[index].damage);
		Text::printT(NULL, file.getU16(10), { 725, height, 100, 25 }, _itemText, { 150, 50, 100, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].damage), { 850, height, NULL, 25 }, _itemText, { 150, 50, 100, 0 });
		height += 25;
	}

	if (player.items[index].defense != 0) {
		modifer = setModifer(player.items[index].defense);
		Text::printT(NULL, file.getU16(11), { 725, height, 100, 25 }, _itemText, { 150, 50, 100, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].defense), { 850, height, NULL, 25 }, _itemText, { 100, 50, 150, 0 });
		height += 25;
	}

	if (player.items[index].health != 0) {
		modifer = setModifer(player.items[index].health);
		Text::printT(NULL, file.getU16(6), { 725, height, 100, 25 }, _itemText, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].health), { 850, height, NULL, 25 }, _itemText, { 200, 50, 25, 0 });
		height += 25;
	}

	if (player.items[index].hps != 0) {
		modifer = setModifer(player.items[index].hps);
		Text::printT(NULL, file.getU16(7), { 725, height, 100, 25 }, _itemText, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].hps), { 850, height, NULL, 25 }, _itemText, { 200, 50, 25, 0 });
		height += 25;
	}

	if (player.items[index].mana != 0) {
		modifer = setModifer(player.items[index].mana);
		Text::printT(NULL, file.getU16(8), { 725, height, 100, 25 }, _itemText, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].mana), { 850, height, NULL, 25 }, _itemText, { 25, 50, 200, 0 });
		height += 25;
	}

	if (player.items[index].mps != 0) {
		modifer = setModifer(player.items[index].mps);
		Text::printT(NULL, file.getU16(9), { 725, height, 100, 25 }, _itemText, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].mps), { 850, height, NULL, 25 }, _itemText, { 25, 50, 200, 0 });
		height += 25;
	}

	if (player.items[index].leech != 0) {
		modifer = setModifer(player.items[index].leech);
		Text::printT(NULL, file.getU16(12), { 725, height, 100, 25 }, _itemText, { 50, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].leech), { 850, height, NULL, 25 }, _itemText, { 50, 150, 50, 0 });
		height += 25;
	}

	if (player.items[index].drain != 0) {
		modifer = setModifer(player.items[index].drain);
		Text::printT(NULL, file.getU16(13), { 725, height, 100, 25 }, _itemText, { 50, 150, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].drain), { 850, height, NULL, 25 }, _itemText, { 50, 150, 150, 0 });
		height += 25;
	}

	if (player.items[index].luck != 0) {
		modifer = setModifer(player.items[index].luck);
		Text::printT(NULL, file.getU16(14), { 725, height, 100, 25 }, _itemText, { 150, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].luck), { 850, height, NULL, 25 }, _itemText, { 150, 150, 50, 0 });
		height += 25;
	}

	if (player.items[index].speed != 0) {
		modifer = setModifer(player.items[index].speed);
		Text::printT(NULL, file.getU16(15), { 725, height, 100, 25 }, _itemText, { 150, 250, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.items[index].speed), { 850, height, NULL, 25 }, _itemText, { 150, 250, 150, 0 });
		height += 25;
	}
}

void Bag::createItemTextsEquip(Player& player, const int& slot) {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 125;
	if (options.lang == ENGLISH) {
		file.uread("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.uread("Data/UI/Bag/ru.txt");
	}

	for (size_t i = 0; i < _itemText.size(); i++) {
		_itemText[i].destroy();
	}
	_itemText.clear();

	Text::printT(TEXT_DAMAGE, player.equipped[slot].uName, { 700, height, 50, 25 }, _itemText, Item::getRarity(player.equipped[slot].dropChance), true);
	height += 25;

	if (player.equipped[slot].damage != 0) {
		modifer = setModifer(player.equipped[slot].damage);
		Text::printT(NULL, file.getU16(10), { 725, height, 100, 25 }, _itemText, { 150, 50, 100, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].damage), { 850, height, NULL, 25 }, _itemText, { 150, 50, 100, 0 });
		height += 25;
	}

	if (player.equipped[slot].defense != 0) {
		modifer = setModifer(player.equipped[slot].defense);
		Text::printT(NULL, file.getU16(11), { 725, height, 100, 25 }, _itemText, { 150, 50, 100, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].defense), { 850, height, NULL, 25 }, _itemText, { 100, 50, 150, 0 });
		height += 25;
	}

	if (player.equipped[slot].health != 0) {
		modifer = setModifer(player.equipped[slot].health);
		Text::printT(NULL, file.getU16(6), { 725, height, 100, 25 }, _itemText, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].health), { 850, height, NULL, 25 }, _itemText, { 200, 50, 25, 0 });
		height += 25;
	}

	if (player.equipped[slot].hps != 0) {
		modifer = setModifer(player.equipped[slot].hps);
		Text::printT(NULL, file.getU16(7), { 725, height, 100, 25 }, _itemText, { 200, 50, 25, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].hps), { 850, height, NULL, 25 }, _itemText, { 200, 50, 25, 0 });
		height += 25;
	}

	if (player.equipped[slot].mana != 0) {
		modifer = setModifer(player.equipped[slot].mana);
		Text::printT(NULL, file.getU16(8), { 725, height, 100, 25 }, _itemText, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].mana), { 850, height, NULL, 25 }, _itemText, { 25, 50, 200, 0 });
		height += 25;
	}

	if (player.equipped[slot].mps != 0) {
		modifer = setModifer(player.equipped[slot].mps);
		Text::printT(NULL, file.getU16(9), { 725, height, 100, 25 }, _itemText, { 25, 50, 200, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].mps), { 850, height, NULL, 25 }, _itemText, { 25, 50, 200, 0 });
		height += 25;
	}

	if (player.equipped[slot].leech != 0) {
		modifer = setModifer(player.equipped[slot].leech);
		Text::printT(NULL, file.getU16(12), { 725, height, 100, 25 }, _itemText, { 50, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].leech), { 850, height, NULL, 25 }, _itemText, { 50, 150, 50, 0 });
		height += 25;
	}

	if (player.equipped[slot].drain != 0) {
		modifer = setModifer(player.equipped[slot].drain);
		Text::printT(NULL, file.getU16(13), { 725, height, 100, 25 }, _itemText, { 50, 150, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].drain), { 850, height, NULL, 25 }, _itemText, { 50, 150, 150, 0 });
		height += 25;
	}

	if (player.equipped[slot].luck != 0) {
		modifer = setModifer(player.equipped[slot].luck);
		Text::printT(NULL, file.getU16(14), { 725, height, 100, 25 }, _itemText, { 150, 150, 50, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].luck), { 850, height, NULL, 25 }, _itemText, { 150, 150, 50, 0 });
		height += 25;
	}

	if (player.equipped[slot].speed != 0) {
		modifer = setModifer(player.equipped[slot].speed);
		Text::printT(NULL, file.getU16(15), { 725, height, 100, 25 }, _itemText, { 150, 250, 150, 0 }, false);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player.equipped[slot].speed), { 850, height, NULL, 25 }, _itemText, { 150, 250, 150, 0 });
		height += 25;
	}
}

std::string Bag::setModifer(const int& num) {
	if (num > 0) {
		return "+";
	}
	return "";
}