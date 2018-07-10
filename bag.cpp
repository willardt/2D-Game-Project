#include "bag.h"

const SDL_Color Bag::LEVEL = {138, 240, 157, 255};
const SDL_Color Bag::EXP = { 138, 210, 157, 255 };
const SDL_Color Bag::HEALTH = { 240, 80, 80, 255 };
const SDL_Color Bag::HEALTH_REGEN = { 210, 80, 80, 255 };
const SDL_Color Bag::MANA = { 80, 194, 240, 255 };
const SDL_Color Bag::MANA_REGEN = { 80, 194, 210, 255 };
const SDL_Color Bag::DAMAGE = { 225, 168, 130, 255 };
const SDL_Color Bag::DEFENSE = { 190, 135, 220, 255 };
const SDL_Color Bag::LEECH = { 250, 140, 140, 255 };
const SDL_Color Bag::DRAIN = { 170, 210, 240, 255 };
const SDL_Color Bag::LUCK = { 225, 180, 230, 255 };
const SDL_Color Bag::SPEED = { 170, 240, 190, 255 };
const SDL_Color Bag::SHARDS = { 180, 230, 230, 255 };
const SDL_Color Bag::DURATION = { 100, 165, 180, 255 };

void Bag::Init(Player* nplayer, std::vector<Item>* nmapItems, std::vector<Texture>& itemsMem, Window* window) {
	Options& options = options.Instance();
	_isRunning = true;
	_mainWindow = window;
	player = nplayer;
	mapItems = nmapItems;
	_playerBack = { 50, 50, 400, 400 };
	_playerRect = { 200, 150, 71, 145 };
	_playerTexture.setPath("Data/Textures/UI/bagPlayer.png");
	_itemBack = { 50, 475, options.windowWidth - 100, 400};
	_infoBack = { 500, 50, 1050, 400 };
	_markerRect = { -100, 0, 8, 8 };
	_markerColor = { 50, 100, 150, 128 };
	_selectedRect = { _SELECTED_X, _SELECTED_Y, 64, 64 };
	createItemPos();
	createEquipItemPos();
	createTexts();
	_selected = 0;
	_isSelected = false;
	_isSelectedEquip = false;
	fps.Init();
	begin(itemsMem);
}

void Bag::begin(std::vector<Texture>& itemsMem) {
	_isRunning = true;
	createItemPos();
	_selected = 0;
	_isSelected = false;

	while (_isRunning != false) {
		input();
		display(itemsMem);
		fps.calcFPS();
	}
}

void Bag::input() {

	_isRunning = in.get(_mainWindow->getWindow());
	if (_isRunning) {
		_isRunning = !in.isKey(SDL_SCANCODE_ESCAPE);
	}


	if (in.isMouseHeld(_mouseX, _mouseY, SDL_BUTTON_LEFT, _mainWindow->getWindow())) {
		_selected = selectItem(_itemsPos);
		if (_selected != -1) {
			_isSelected = true;
			_isSelectedEquip = false;
			createItemTexts(_selected);
		}
		else {
			_isSelected = false;
			_selectedEquip = selectItem(_equipItemsPos);
			if (_selectedEquip != -1 && player->equipped[_selectedEquip].id != -1) {
				_isSelectedEquip = true;
				createItemTextsEquip(_selectedEquip);
			}
			else {
				_isSelectedEquip = false;
			}
		}
	}

	if (in.isKey(SDL_SCANCODE_E)) {
		if (_isSelected == true) {
			if (player->items[_selected].type == USE) {
				player->useItem(_selected);
				createItemPos();
				createTexts();
				_isSelected = false;
			}
			else {
				if (player->items.size() <= Player::MAX_BAG_SIZE) {
					player->equipItem(_selected);
					createItemPos();
					createTexts();
					_isSelected = false;
				}
			}
		}
	}

	if (in.isKey(SDL_SCANCODE_R)) {
		if (_isSelectedEquip == true) {
			if (player->items.size() <= Player::MAX_BAG_SIZE) {
				player->unequipItem(_selectedEquip);
				createItemPos();
				createTexts();
				_isSelectedEquip = false;
			}
		}
	}

	if (in.isKey(SDL_SCANCODE_Q)) {
		if (_isSelectedEquip == true) {
			if (player->equipped[_selectedEquip].isEquipped == true) {
				player->maxHealth -= player->equipped[_selectedEquip].health;
				if (player->health > player->maxHealth) {
					player->health = player->maxHealth;
				}
				player->hps -= player->equipped[_selectedEquip].hps;
				player->maxMana -= player->equipped[_selectedEquip].mana;
				if (player->mana > player->maxMana) {
					player->mana = player->maxMana;
				}
				player->mps -= player->equipped[_selectedEquip].mps;
				player->damage -= player->equipped[_selectedEquip].damage;
				player->defense -= player->equipped[_selectedEquip].defense;
				player->leech -= player->equipped[_selectedEquip].leech;
				player->drain -= player->equipped[_selectedEquip].drain;
				player->luck -= player->equipped[_selectedEquip].luck;
				player->speed -= player->equipped[_selectedEquip].speed;

				player->equipped[_selectedEquip].pos.x = player->pos.x;
				player->equipped[_selectedEquip].pos.y = player->pos.y;
				mapItems->push_back(player->equipped[_selectedEquip]);

				player->equipped[_selectedEquip].isEquipped = false;
				player->equipped[_selectedEquip].id = -1;
				_isSelectedEquip = false;
				createItemPos();
				createTexts();
			}
		}
		else if (_isSelected == true) {
			player->items[_selected].pos.x = player->pos.x;
			player->items[_selected].pos.y = player->pos.y;
			mapItems->push_back(player->items[_selected]);
			player->items.erase(player->items.begin() + _selected);
			_isSelected = false;
			createItemPos();
			createTexts();
		}
	}
}

void Bag::display(std::vector<Texture>& itemsMem) {
	Options& options = options.Instance();
	size_t j = 0;
	size_t bagSize = player->items.size();

	//-----BACKGROUND------
	Texture::drawRectTransNoCam(_playerBack, Text::CLEAR, _mainWindow->getRenderer());
	Texture::drawRectTransNoCam(_itemBack, Text::BLACK, _mainWindow->getRenderer());
	Texture::drawRectTransNoCam(_infoBack, Text::BLACK, _mainWindow->getRenderer());
	_playerTexture.renderNoCam(_playerRect, _mainWindow->getRenderer());

	//-----Bag-----
	for (size_t i = 0; i < player->items.size(); i++) {
		itemsMem[player->items[i].id].renderNoCam(_itemsPos[i], _mainWindow->getRenderer());
	}

	for (size_t i = 0; i < _equipItemsPos.size(); i++) {
		Texture::drawRectNoCam(_equipItemsPos[i], Text::BLACK , _mainWindow->getRenderer());
	}

	for (int i = 0; i < Player::EQUIPPED_SIZE; i++) {
		if (player->equipped[i].id != -1) {
			itemsMem[player->equipped[i].id].renderNoCam(_equipItemsPos[i], _mainWindow->getRenderer());
		}
	}

	if (_isSelected == true) {
		Texture::drawRectNoCam(_markerRect, _markerColor, _mainWindow->getRenderer());
		//SDL_SetRenderDrawColor(_mainWindow->getRenderer(), _markerColor.r, _markerColor.g, _markerColor.b, _markerColor.a);
		//SDL_RenderDrawRect(_mainWindow->getRenderer(), &_markerRect);

		itemsMem[player->items[_selected].id].renderNoCam(_selectedRect, _mainWindow->getRenderer());

		for (size_t i = 0; i < _itemText.size(); i++) {
			_itemText[i].renderNoCam(_mainWindow->getRenderer());
		}
	}

	if (_isSelectedEquip == true) {
		Texture::drawRectNoCam(_markerRect, _markerColor, _mainWindow->getRenderer());

		itemsMem[player->equipped[_selectedEquip].id].renderNoCam(_selectedRect, _mainWindow->getRenderer());

		for (size_t i = 0; i < _itemText.size(); i++) {
			_itemText[i].renderNoCam(_mainWindow->getRenderer());
		}
	}

	//-----Text-----
	for (size_t i = 0; i < _texts.size(); i++) {
		_texts[i].renderNoCam(_mainWindow->getRenderer());
	}

	_mainWindow->render();

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

void Bag::createItemPos() {
	Options& options = options.Instance();
	size_t bagSize = player->items.size();
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

	temp = { 100, 200, 48, 48 };		//Weapon
	_equipItemsPos.push_back(temp);
	temp = { 350, 200, 48, 48 };	//OffHand
	_equipItemsPos.push_back(temp);
	temp = { 100, 250, 48, 48 };		//Pants
	_equipItemsPos.push_back(temp);
	temp = { 100, 150, 48, 48 };		//Chest
	_equipItemsPos.push_back(temp);
	temp = { 100, 300, 48, 48 };		//Ring
	_equipItemsPos.push_back(temp);
	temp = { 100, 100, 48, 48 };		//Neck
	_equipItemsPos.push_back(temp);
	temp = { 350, 250, 48, 48 };	//Boots
	_equipItemsPos.push_back(temp);
	temp = { 350, 150, 48, 48 };	//Gloves
	_equipItemsPos.push_back(temp);
	temp = { 350, 300, 48, 48 };	//Misc
	_equipItemsPos.push_back(temp);
	temp = { 350, 100, 48, 48 };	//Helm
	_equipItemsPos.push_back(temp);
}

void Bag::createTexts() {
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


	Text::printT(NULL, file.getU16(1), { 1400, 375, 120, 30 }, _texts, Text::WHITE, false);		//EQUIP
	Text::printT(NULL, file.getU16(2), { 1400, 400, 130, 30 }, _texts, Text::WHITE, false);		//UNEQUIP
	Text::printT(NULL, file.getU16(3), { 1400, 425, 110, 30 }, _texts, Text::WHITE, false);		//DROP
	Text::printT(NULL, player->uName, { 600, 50, NULL, 30 }, _texts, Text::WHITE, true);					//NAME
	Text::printT(NULL, file.getU16(4), { 500, 75, 90, 25 }, _texts, Bag::LEVEL, true);		//Level
	Text::printT(TEXT_DAMAGE, std::to_string(player->level), { 700, 75, NULL, 25 }, _texts, Bag::LEVEL);
	Text::printT(NULL, file.getU16(5), { 500, 100, 50, 25 }, _texts, Bag::EXP, true);		//EXP
	Text::printT(TEXT_DAMAGE, std::to_string(player->exp), { 700, 100, NULL, 25 }, _texts, Bag::EXP);
	Text::printT(NULL, file.getU16(6), { 500, 125, 90, 25 }, _texts, Bag::HEALTH, true);		//HEALTH
	Text::printT(TEXT_DAMAGE, std::to_string(player->health) + "/" + std::to_string(player->maxHealth), { 700, 125, NULL, 25 }, _texts, Bag::HEALTH);
	Text::printT(NULL, file.getU16(7), { 500, 150, 50, 25 }, _texts, Bag::HEALTH_REGEN, true);		//HPS
	Text::printT(TEXT_DAMAGE, std::to_string(player->hps), { 700, 150, NULL, 25 }, _texts, Bag::HEALTH_REGEN);
	Text::printT(NULL, file.getU16(8), { 500, 175, 70, 25 }, _texts, Bag::MANA, true);		//MANA
	Text::printT(TEXT_DAMAGE, std::to_string(player->mana) + "/" + std::to_string(player->maxMana), { 700, 175, NULL, 25 }, _texts, Bag::MANA);
	Text::printT(NULL, file.getU16(9), { 500, 200, 50, 25 }, _texts, Bag::MANA_REGEN, true);		//MPS
	Text::printT(TEXT_DAMAGE, std::to_string(player->mps), { 700, 200, NULL, 25 }, _texts, Bag::MANA_REGEN);
	Text::printT(NULL, file.getU16(10), { 500, 225, 90, 25 }, _texts, Bag::DAMAGE, true);		//DAMAGE
	Text::printT(TEXT_DAMAGE, std::to_string(player->damage), { 700, 225, NULL, 25 }, _texts, Bag::DAMAGE);
	Text::printT(NULL, file.getU16(11), { 500, 250, 100, 25 }, _texts, Bag::DEFENSE, true);		//DEFENSE
	Text::printT(TEXT_DAMAGE, std::to_string(player->defense), { 700, 250, NULL, 25 }, _texts, Bag::DEFENSE);
	Text::printT(NULL, file.getU16(12), { 500, 275, 80, 25 }, _texts, Bag::LEECH, true);		//LEECH
	Text::printT(TEXT_DAMAGE, std::to_string(player->leech), { 700, 275, NULL, 25 }, _texts, Bag::LEECH);
	Text::printT(NULL, file.getU16(13), { 500, 300, 80, 25 }, _texts, Bag::DRAIN, true);		//DRAIN
	Text::printT(TEXT_DAMAGE, std::to_string(player->drain), { 700, 300, NULL, 25 }, _texts, Bag::DRAIN);
	Text::printT(NULL, file.getU16(14), { 500, 325, 70, 25 }, _texts, Bag::LUCK, true);		//LUCK
	Text::printT(TEXT_DAMAGE, std::to_string(player->luck), { 700, 325, NULL, 25 }, _texts, Bag::LUCK);
	Text::printT(NULL, file.getU16(15), { 500, 350, 80, 25 }, _texts, Bag::SPEED, true);		//SPEED
	Text::printT(TEXT_DAMAGE, std::to_string(player->speed), { 700, 350, NULL, 25 }, _texts, Bag::SPEED);
	Text::printT(NULL, file.getU16(16), { 500, 375, 90, 25 }, _texts, Bag::SHARDS, true);		//SHARDS
	Text::printT(TEXT_DAMAGE, std::to_string(player->shards), { 700, 375, NULL, 25 }, _texts, Bag::SHARDS);
}

void Bag::createItemTexts(const int& index) {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 150;
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

	Text::printT(TEXT_DAMAGE, player->items[index].uName, { 1000, 50, 50, 25 }, _itemText, Item::getRarity(player->items[index].dropChance), true);
	height += 25;

	if (player->items[index].damage != 0) {
		modifer = setModifer(player->items[index].damage);
		Text::printT(NULL, file.getU16(10), { 1025, height, NULL, 25 }, _itemText, Bag::DAMAGE, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].damage), { 1250, height, NULL, 25 }, _itemText, Bag::DAMAGE);
		height += 25;
	}

	if (player->items[index].defense != 0) {
		modifer = setModifer(player->items[index].defense);
		Text::printT(NULL, file.getU16(11), { 1025, height, 100, 25 }, _itemText, Bag::DEFENSE, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].defense), { 1250, height, NULL, 25 }, _itemText, Bag::DEFENSE);
		height += 25;
	}

	if (player->items[index].health != 0) {
		modifer = setModifer(player->items[index].health);
		Text::printT(NULL, file.getU16(6), { 1025, height, 100, 25 }, _itemText, Bag::HEALTH, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].health), { 1250, height, NULL, 25 }, _itemText, Bag::HEALTH);
		height += 25;
	}

	if (player->items[index].hps != 0) {
		modifer = setModifer(player->items[index].hps);
		Text::printT(NULL, file.getU16(7), { 1025, height, 100, 25 }, _itemText, Bag::HEALTH_REGEN, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].hps), { 1250, height, NULL, 25 }, _itemText, Bag::HEALTH_REGEN);
		height += 25;
	}

	if (player->items[index].mana != 0) {
		modifer = setModifer(player->items[index].mana);
		Text::printT(NULL, file.getU16(8), { 1025, height, 100, 25 }, _itemText, Bag::MANA, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].mana), { 1250, height, NULL, 25 }, _itemText, Bag::MANA);
		height += 25;
	}

	if (player->items[index].mps != 0) {
		modifer = setModifer(player->items[index].mps);
		Text::printT(NULL, file.getU16(9), { 1025, height, 100, 25 }, _itemText, Bag::MANA_REGEN, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].mps), { 1250, height, NULL, 25 }, _itemText, Bag::MANA_REGEN);
		height += 25;
	}

	if (player->items[index].leech != 0) {
		modifer = setModifer(player->items[index].leech);
		Text::printT(NULL, file.getU16(12), { 1025, height, 100, 25 }, _itemText, Bag::LEECH, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].leech), { 1250, height, NULL, 25 }, _itemText, Bag::LEECH);
		height += 25;
	}

	if (player->items[index].drain != 0) {
		modifer = setModifer(player->items[index].drain);
		Text::printT(NULL, file.getU16(13), { 1025, height, 100, 25 }, _itemText, Bag::DRAIN, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].drain), { 1250, height, NULL, 25 }, _itemText, Bag::DRAIN);
		height += 25;
	}

	if (player->items[index].luck != 0) {
		modifer = setModifer(player->items[index].luck);
		Text::printT(NULL, file.getU16(14), { 1025, height, 100, 25 }, _itemText, Bag::LUCK, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].luck), { 1250, height, NULL, 25 }, _itemText, Bag::LUCK);
		height += 25;
	}

	if (player->items[index].speed != 0) {
		modifer = setModifer(player->items[index].speed);
		Text::printT(NULL, file.getU16(15), { 1025, height, 100, 25 }, _itemText, Bag::SPEED, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->items[index].speed), { 1250, height, NULL, 25 }, _itemText, Bag::SPEED);
		height += 25;
	}

	if (player->items[index].duration != 0) {
		modifer = setModifer(player->items[index].duration);
		Text::printT(NULL, file.getU16(17), { 1025, height, 100, 25 }, _itemText, Bag::DURATION, true);
		int duration = player->items[index].duration / 60;
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(duration), { 1250, height, NULL, 25 }, _itemText, Bag::DURATION);
		height += 25;
	}
}

void Bag::createItemTextsEquip(const int& slot) {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 150;
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

	Text::printT(TEXT_DAMAGE, player->equipped[slot].uName, { 1000, 50, 50, 25 }, _itemText, Item::getRarity(player->equipped[slot].dropChance), true);
	height += 25;

	if (player->equipped[slot].damage != 0) {
		modifer = setModifer(player->equipped[slot].damage);
		Text::printT(NULL, file.getU16(10), { 1025, height, 100, 25 }, _itemText, Bag::DAMAGE, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].damage), { 1250, height, NULL, 25 }, _itemText, Bag::DAMAGE);
		height += 25;
	}

	if (player->equipped[slot].defense != 0) {
		modifer = setModifer(player->equipped[slot].defense);
		Text::printT(NULL, file.getU16(11), { 1025, height, 100, 25 }, _itemText, Bag::DEFENSE, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].defense), { 1250, height, NULL, 25 }, _itemText, Bag::DEFENSE);
		height += 25;
	}

	if (player->equipped[slot].health != 0) {
		modifer = setModifer(player->equipped[slot].health);
		Text::printT(NULL, file.getU16(6), { 1025, height, 100, 25 }, _itemText, Bag::HEALTH, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].health), { 1250, height, NULL, 25 }, _itemText, Bag::HEALTH);
		height += 25;
	}

	if (player->equipped[slot].hps != 0) {
		modifer = setModifer(player->equipped[slot].hps);
		Text::printT(NULL, file.getU16(7), { 1025, height, 100, 25 }, _itemText, Bag::HEALTH_REGEN, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].hps), { 1250, height, NULL, 25 }, _itemText, Bag::HEALTH_REGEN);
		height += 25;
	}

	if (player->equipped[slot].mana != 0) {
		modifer = setModifer(player->equipped[slot].mana);
		Text::printT(NULL, file.getU16(8), { 1025, height, 100, 25 }, _itemText, Bag::MANA, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].mana), { 1250, height, NULL, 25 }, _itemText, Bag::MANA);
		height += 25;
	}

	if (player->equipped[slot].mps != 0) {
		modifer = setModifer(player->equipped[slot].mps);
		Text::printT(NULL, file.getU16(9), { 1025, height, 100, 25 }, _itemText, Bag::MANA_REGEN, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].mps), { 1250, height, NULL, 25 }, _itemText, Bag::MANA_REGEN);
		height += 25;
	}

	if (player->equipped[slot].leech != 0) {
		modifer = setModifer(player->equipped[slot].leech);
		Text::printT(NULL, file.getU16(12), { 1025, height, 100, 25 }, _itemText, Bag::LEECH, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].leech), { 1250, height, NULL, 25 }, _itemText, Bag::LEECH);
		height += 25;
	}

	if (player->equipped[slot].drain != 0) {
		modifer = setModifer(player->equipped[slot].drain);
		Text::printT(NULL, file.getU16(13), { 1025, height, 100, 25 }, _itemText, Bag::DRAIN, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].drain), { 1250, height, NULL, 25 }, _itemText, Bag::DRAIN);
		height += 25;
	}

	if (player->equipped[slot].luck != 0) {
		modifer = setModifer(player->equipped[slot].luck);
		Text::printT(NULL, file.getU16(14), { 1025, height, 100, 25 }, _itemText, Bag::LUCK, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].luck), { 1250, height, NULL, 25 }, _itemText, Bag::LUCK);
		height += 25;
	}

	if (player->equipped[slot].speed != 0) {
		modifer = setModifer(player->equipped[slot].speed);
		Text::printT(NULL, file.getU16(15), { 1025, height, 100, 25 }, _itemText, Bag::SPEED, true);
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(player->equipped[slot].speed), { 1250, height, NULL, 25 }, _itemText, Bag::SPEED);
		height += 25;
	}

	if (player->equipped[slot].duration != 0) {
		modifer = setModifer(player->items[slot].duration);
		Text::printT(NULL, file.getU16(17), { 1025, height, 100, 25 }, _itemText, Bag::DURATION, true);
		int duration = player->items[slot].duration / 60;
		Text::printT(TEXT_DAMAGE, modifer + std::to_string(duration), { 1250, height, NULL, 25 }, _itemText, Bag::DURATION);
		height += 25;
	}
}

std::string Bag::setModifer(const int& num) {
	if (num > 0) {
		return "+";
	}
	return "";
}