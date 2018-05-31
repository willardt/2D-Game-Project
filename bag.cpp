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
		Texture::drawRectNoCam(_equipItemsPos[i], Text::BLUE, _mainWindow.getRenderer());
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
		file.setPath("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.setPath("Data/UI/Bag/ru.txt");
	}

	for (size_t i = 0; i < _texts.size(); i++) {
		_texts[i].destroy();
	};
	_texts.clear();

	Text equip, unequip, drop, name, level, ilevel, exp, iexp, health, ihealth,
		hps, ihps, mana, imana, mps, imps, damage, idamage, defense, idefense,
		leech, ileech, drain, idrain, luck, iluck, speed, ispeed, shards, ishards;

	equip.color = Text::WHITE;
	equip.pos = { 950, 75, 120, 30 };
	equip.load(file.readUnicodeStr(1), _mainWindow.getRenderer());  //Equip
	_texts.push_back(equip);
	equip.destroy();

	unequip.color = Text::WHITE;
	unequip.pos = { 950, 100, 130, 30 };
	unequip.load(file.readUnicodeStr(2), _mainWindow.getRenderer());  //Unequip
	_texts.push_back(unequip);
	unequip.destroy();

	drop.color = Text::WHITE;
	drop.pos = { 950, 125, 110, 30 };
	drop.load(file.readUnicodeStr(3), _mainWindow.getRenderer());  //Drop
	_texts.push_back(drop);
	drop.destroy();

	name.color = Text::WHITE;
	name.pos = { 700, 25, NULL, 30 };
	name.load(player.uName, _mainWindow.getRenderer());  //Name
	name.setSpacing();
	_texts.push_back(name);
	name.destroy();

	level.color = Text::WHITE;
	level.pos = { 400, 50, 90, 25 };
	level.load(file.readUnicodeStr(4), _mainWindow.getRenderer());  //Level
	_texts.push_back(level);
	level.destroy();

	ilevel.type = TEXT_DAMAGE;
	ilevel.color = Text::WHITE;
	ilevel.pos = { 500, 50, NULL, 25 };
	ilevel.load(std::to_string(player.level), _mainWindow.getRenderer());
	ilevel.setSpacing();
	_texts.push_back(ilevel);
	ilevel.destroy();

	exp.color = Text::WHITE;
	exp.pos = { 400, 75, 50, 25 };
	exp.load(file.readUnicodeStr(5), _mainWindow.getRenderer());  //Exp
	_texts.push_back(exp);
	exp.destroy();

	iexp.type = TEXT_DAMAGE;
	iexp.color = Text::WHITE;
	iexp.pos = { 500, 75, NULL, 25 };
	iexp.load(std::to_string(player.exp), _mainWindow.getRenderer());
	_texts.push_back(iexp);
	iexp.destroy();

	health.color = Text::WHITE;
	health.pos = { 400, 100, 90, 25 };
	health.load(file.readUnicodeStr(6), _mainWindow.getRenderer());  //Health
	_texts.push_back(health);
	health.destroy();

	ihealth.type = TEXT_DAMAGE;
	ihealth.color = Text::WHITE;
	ihealth.pos = { 500, 100, NULL, 25 };
	ihealth.load(std::to_string(player.health) + "/" + std::to_string(player.maxHealth), _mainWindow.getRenderer());
	_texts.push_back(ihealth);
	ihealth.destroy();

	hps.color = Text::WHITE;
	hps.pos = { 400, 125, 50, 25 };
	hps.load(file.readUnicodeStr(7), _mainWindow.getRenderer());  //HPS
	_texts.push_back(hps);
	hps.destroy();

	ihps.type = TEXT_DAMAGE;
	ihps.color = Text::WHITE;
	ihps.pos = { 500, 125, NULL, 25 };
	ihps.load(std::to_string(player.hps), _mainWindow.getRenderer());
	_texts.push_back(ihps);
	ihps.destroy();

	mana.color = Text::WHITE;
	mana.pos = { 400, 150, 70, 25 };
	mana.load(file.readUnicodeStr(8), _mainWindow.getRenderer());  //Mana
	_texts.push_back(mana);
	mana.destroy();

	imana.type = TEXT_DAMAGE;
	imana.color = Text::WHITE;
	imana.pos = { 500, 150, NULL, 25 };
	imana.load(std::to_string(player.mana) + "/" + std::to_string(player.maxMana), _mainWindow.getRenderer());
	_texts.push_back(imana);
	imana.destroy();

	mps.color = Text::WHITE;
	mps.pos = { 400, 175, 50, 25 };
	mps.load(file.readUnicodeStr(9), _mainWindow.getRenderer());  //MPS
	_texts.push_back(mps);
	mps.destroy();

	imps.type = TEXT_DAMAGE;
	imps.color = Text::WHITE;
	imps.pos = { 500, 175, NULL, 25 };
	imps.load(std::to_string(player.mps), _mainWindow.getRenderer());
	_texts.push_back(imps);
	imps.destroy();

	damage.color = Text::WHITE;
	damage.pos = { 400, 200, 90, 25 };
	damage.load(file.readUnicodeStr(10), _mainWindow.getRenderer());  //Damage
	_texts.push_back(damage);
	damage.destroy();

	idamage.type = TEXT_DAMAGE;
	idamage.color = Text::WHITE;
	idamage.pos = { 500, 200, NULL, 25 };
	idamage.load(std::to_string(player.damage), _mainWindow.getRenderer());
	_texts.push_back(idamage);
	idamage.destroy();

	defense.color = Text::WHITE;
	defense.pos = { 400, 225, 100, 25 };
	defense.load(file.readUnicodeStr(11), _mainWindow.getRenderer());  //Defense
	_texts.push_back(defense);
	defense.destroy();

	idefense.type = TEXT_DAMAGE;
	idefense.color = Text::WHITE;
	idefense.pos = { 500, 225, NULL, 25 };
	idefense.load(std::to_string(player.defense), _mainWindow.getRenderer());
	_texts.push_back(idefense);
	idefense.destroy();

	leech.color = Text::WHITE;
	leech.pos = { 400, 250, 80, 25 };
	leech .load(file.readUnicodeStr(12), _mainWindow.getRenderer());  //Leech
	_texts.push_back(leech);
	leech.destroy();

	ileech.type = TEXT_DAMAGE;
	ileech.color = Text::WHITE;
	ileech.pos = { 500, 250, NULL, 25 };
	ileech.load(std::to_string(player.leech), _mainWindow.getRenderer());
	_texts.push_back(ileech);
	ileech.destroy();

	drain.color = Text::WHITE;
	drain.pos = { 400, 275, 80, 25 };
	drain.load(file.readUnicodeStr(13), _mainWindow.getRenderer());  //Drain
	_texts.push_back(drain);
	drain.destroy();

	idrain.type = TEXT_DAMAGE;
	idrain.color = Text::WHITE;
	idrain.pos = { 500, 275, NULL, 25 };
	idrain.load(std::to_string(player.drain), _mainWindow.getRenderer());
	_texts.push_back(idrain);
	idrain.destroy();

	luck.color = Text::WHITE;
	luck.pos = { 400, 300, 70, 25 };
	luck.load(file.readUnicodeStr(14), _mainWindow.getRenderer());  //Luck
	_texts.push_back(luck);
	luck.destroy();

	iluck.type = TEXT_DAMAGE;
	iluck.color = Text::WHITE;
	iluck.pos = { 500, 300, NULL, 25 };
	iluck.load(std::to_string(player.luck), _mainWindow.getRenderer());
	_texts.push_back(iluck);
	iluck.destroy();

	speed.color = Text::WHITE;
	speed.pos = { 400, 325, 80, 25 };
	speed.load(file.readUnicodeStr(15), _mainWindow.getRenderer());  //Speed
	_texts.push_back(speed);
	speed.destroy();

	ispeed.type = TEXT_DAMAGE;
	ispeed.color = Text::WHITE;
	ispeed.pos = { 500, 325, NULL, 25 };
	ispeed.load(std::to_string(player.speed), _mainWindow.getRenderer());
	_texts.push_back(ispeed);
	ispeed.destroy();

	shards.color = Text::WHITE;
	shards.pos = { 400, 350, 90, 25 };
	shards.load(file.readUnicodeStr(16), _mainWindow.getRenderer());  //Shards
	_texts.push_back(shards);
	shards.destroy();

	ishards.type = TEXT_DAMAGE;
	ishards.color = Text::WHITE;
	ishards.pos = { 500, 350, NULL, 25 };
	ishards.load(std::to_string(player.shards), _mainWindow.getRenderer());
	_texts.push_back(ishards);
	ishards.destroy();
}

void Bag::createItemTexts(Player& player, const int& index) {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 125;
	if (options.lang == ENGLISH) {
		file.setPath("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.setPath("Data/UI/Bag/ru.txt");
	}

	for (size_t i = 0; i < _itemText.size(); i++) {
		_itemText[i].destroy();
	}
	_itemText.clear();

	Text name, damage, idamage, defense, idefense, health, ihealth, hps, ihps, mana, imana, mps, imps, 
		 leech, ileech, drain, idrain, luck, iluck, speed, ispeed;

	name.type = TEXT_DAMAGE;
	name.color = Text::WHITE;
	name.pos = { 700, height, 50, 25 };
	name.load(player.items[index].uName, _mainWindow.getRenderer());
	name.setSpacing();
	_itemText.push_back(name);
	name.destroy();

	height += 25;

	if (player.items[index].damage != 0) {
		modifer = setModifer(player.items[index].damage);

		damage.color = { 150, 50, 100, 0 };
		damage.pos = { 725, height, 100, 25 };
		damage.load(file.readUnicodeStr(10), _mainWindow.getRenderer());
		_itemText.push_back(damage);
		damage.destroy();

		idamage.type = TEXT_DAMAGE;
		idamage.color = { 150, 50, 100, 0 };
		idamage.pos = { 850, height, NULL, 25 };
		idamage.load(modifer + std::to_string(player.items[index].damage), _mainWindow.getRenderer());
		idamage.setSpacing();
		_itemText.push_back(idamage);
		idamage.destroy();

		height += 25;
	}

	if (player.items[index].defense != 0) {
		modifer = setModifer(player.items[index].defense);

		defense.color = { 100, 50, 150, 0 };
		defense.pos = { 725, height, 100, 25 };
		defense.load(file.readUnicodeStr(11), _mainWindow.getRenderer());
		_itemText.push_back(defense);
		defense.destroy();

		idefense.type = TEXT_DAMAGE;
		idefense.color = { 100, 50, 150, 0 };
		idefense.pos = { 850, height, NULL, 25 };
		idefense.load(modifer + std::to_string(player.items[index].defense), _mainWindow.getRenderer());
		idefense.setSpacing();
		_itemText.push_back(idefense);
		idefense.destroy();

		height += 25;
	}

	if (player.items[index].health != 0) {
		modifer = setModifer(player.items[index].health);

		health.color = { 200, 50, 25, 0 };
		health.pos = { 725, height, 100, 25 };
		health.load(file.readUnicodeStr(6), _mainWindow.getRenderer());
		_itemText.push_back(health);
		health.destroy();

		ihealth.type = TEXT_DAMAGE;
		ihealth.color = { 200, 50, 25, 0 };
		ihealth.pos = { 850, height, NULL, 25 };
		ihealth.load(modifer + std::to_string(player.items[index].health), _mainWindow.getRenderer());
		ihealth.setSpacing();
		_itemText.push_back(ihealth);
		ihealth.destroy();

		height += 25;
	}

	if (player.items[index].hps != 0) {
		modifer = setModifer(player.items[index].hps);

		hps.color = { 200, 50, 25, 0 };
		hps.pos = { 725, height, 70, 25 };
		hps.load(file.readUnicodeStr(7), _mainWindow.getRenderer());
		_itemText.push_back(hps);
		hps.destroy();

		ihps.type = TEXT_DAMAGE;
		ihps.color = { 200, 50, 25, 0 };
		ihps.pos = { 850, height, NULL, 25 };
		ihps.load(modifer + std::to_string(player.items[index].hps), _mainWindow.getRenderer());
		ihps.setSpacing();
		_itemText.push_back(ihps);
		ihps.destroy();

		height += 25;
	}

	if (player.items[index].mana != 0) {
		modifer = setModifer(player.items[index].mana);

		mana.color = { 25, 50, 200, 0 };
		mana.pos = { 725, height, 80, 25 };
		mana.load(file.readUnicodeStr(8), _mainWindow.getRenderer());
		_itemText.push_back(mana);
		mana.destroy();

		imana.type = TEXT_DAMAGE;
		imana.color = { 25, 50, 200, 0 };
		imana.pos = { 850, height, NULL, 25 };
		imana.load(modifer + std::to_string(player.items[index].mana), _mainWindow.getRenderer());
		imana.setSpacing();
		_itemText.push_back(imana);
		imana.destroy();

		height += 25;
	}

	if (player.items[index].mps != 0) {
		modifer = setModifer(player.items[index].mps);

		mps.color = { 25, 50, 200, 0 };
		mps.pos = { 725, height, 70, 25 };
		mps.load(file.readUnicodeStr(9), _mainWindow.getRenderer());
		_itemText.push_back(mps);
		mps.destroy();

		imps.type = TEXT_DAMAGE;
		imps.color = { 25, 50, 200, 0 };
		imps.pos = { 850, height, NULL, 25 };
		imps.load(modifer + std::to_string(player.items[index].mps), _mainWindow.getRenderer());
		imps.setSpacing();
		_itemText.push_back(imps);
		imps.destroy();

		height += 25;
	}

	if (player.items[index].leech != 0) {
		modifer = setModifer(player.items[index].leech);

		leech.color = { 50, 150, 50, 0 };
		leech.pos = { 725, height, 90, 25 };
		leech.load(file.readUnicodeStr(12), _mainWindow.getRenderer());
		_itemText.push_back(leech);
		leech.destroy();

		ileech.type = TEXT_DAMAGE;
		ileech.color = { 50, 150, 50, 0 };
		ileech.pos = { 850, height, NULL, 25 };
		ileech.load(modifer + std::to_string(player.items[index].leech), _mainWindow.getRenderer());
		ileech.setSpacing();
		_itemText.push_back(ileech);
		ileech.destroy();

		height += 25;
	}

	if (player.items[index].drain != 0) {
		modifer = setModifer(player.items[index].drain);

		drain.color = { 50, 150, 150, 0 };
		drain.pos = { 725, height, 90, 25 };
		drain.load(file.readUnicodeStr(13), _mainWindow.getRenderer());
		_itemText.push_back(drain);
		drain.destroy();

		idrain.type = TEXT_DAMAGE;
		idrain.color = { 50, 150, 150, 0 };
		idrain.pos = { 850, height, NULL, 25 };
		idrain.load(modifer + std::to_string(player.items[index].drain), _mainWindow.getRenderer());
		idrain.setSpacing();
		_itemText.push_back(idrain);
		idrain.destroy();

		height += 25;
	}

	if (player.items[index].luck != 0) {
		modifer = setModifer(player.items[index].luck);

		luck.color = { 150, 150, 50, 0 };
		luck.pos = { 725, height, 80, 25 };
		luck.load(file.readUnicodeStr(14), _mainWindow.getRenderer());
		_itemText.push_back(luck);
		luck.destroy();

		iluck.type = TEXT_DAMAGE;
		iluck.color = { 150, 150, 50, 0 };
		iluck.pos = { 850, height, NULL, 25 };
		iluck.load(modifer + std::to_string(player.items[index].luck), _mainWindow.getRenderer());
		iluck.setSpacing();
		_itemText.push_back(iluck);
		iluck.destroy();

		height += 25;
	}

	if (player.items[index].speed != 0) {
		modifer = setModifer(player.items[index].speed);

		speed.color = { 150, 250, 150, 0 };
		speed.pos = { 725, height, 90, 25 };
		speed.load(file.readUnicodeStr(15), _mainWindow.getRenderer());
		_itemText.push_back(speed);
		speed.destroy();

		ispeed.type = TEXT_DAMAGE;
		ispeed.color = { 150, 250, 150, 0 };
		ispeed.pos = { 850, height, NULL, 25 };
		ispeed.load(modifer + std::to_string(player.items[index].speed), _mainWindow.getRenderer());
		ispeed.setSpacing();
		_itemText.push_back(ispeed);
		ispeed.destroy();
	}
}

void Bag::createItemTextsEquip(Player& player, const int& slot) {
	Options& options = options.Instance();
	File file;
	std::string modifer = "+";
	int height = 125;
	if (options.lang == ENGLISH) {
		file.setPath("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		file.setPath("Data/UI/Bag/ru.txt");
	}

	for (size_t i = 0; i < _itemText.size(); i++) {
		_itemText[i].destroy();
	}
	_itemText.clear();

	Text name, damage, idamage, defense, idefense, health, ihealth, hps, ihps, mana, imana, mps, imps,
		leech, ileech, drain, idrain, luck, iluck, speed, ispeed;

	name.type = TEXT_DAMAGE;
	name.color = Text::WHITE;
	name.pos = { 700, height, 50, 25 };
	name.load(player.equipped[slot].uName, _mainWindow.getRenderer());
	name.setSpacing();
	_itemText.push_back(name);
	name.destroy();

	height += 25;

	if (player.equipped[slot].damage != 0) {
		modifer = setModifer(player.equipped[slot].damage);

		damage.color = { 150, 50, 100, 0 };
		damage.pos = { 725, height, 100, 25 };
		damage.load(file.readUnicodeStr(10), _mainWindow.getRenderer());
		_itemText.push_back(damage);
		damage.destroy();

		idamage.type = TEXT_DAMAGE;
		idamage.color = { 150, 50, 100, 0 };
		idamage.pos = { 850, height, NULL, 25 };
		idamage.load(modifer + std::to_string(player.equipped[slot].damage), _mainWindow.getRenderer());
		idamage.setSpacing();
		_itemText.push_back(idamage);
		idamage.destroy();

		height += 25;
	}

	if (player.equipped[slot].defense != 0) {
		modifer = setModifer(player.equipped[slot].defense);

		defense.color = { 100, 50, 150, 0 };
		defense.pos = { 725, height, 100, 25 };
		defense.load(file.readUnicodeStr(11), _mainWindow.getRenderer());
		_itemText.push_back(defense);
		defense.destroy();

		idefense.type = TEXT_DAMAGE;
		idefense.color = { 100, 50, 150, 0 };
		idefense.pos = { 850, height, NULL, 25 };
		idefense.load(modifer + std::to_string(player.equipped[slot].defense), _mainWindow.getRenderer());
		idefense.setSpacing();
		_itemText.push_back(idefense);
		idefense.destroy();

		height += 25;
	}

	if (player.equipped[slot].health != 0) {
		modifer = setModifer(player.equipped[slot].health);

		health.color = { 200, 50, 25, 0 };
		health.pos = { 725, height, 100, 25 };
		health.load(file.readUnicodeStr(6), _mainWindow.getRenderer());
		_itemText.push_back(health);
		health.destroy();

		ihealth.type = TEXT_DAMAGE;
		ihealth.color = { 200, 50, 25, 0 };
		ihealth.pos = { 850, height, NULL, 25 };
		ihealth.load(modifer + std::to_string(player.equipped[slot].health), _mainWindow.getRenderer());
		ihealth.setSpacing();
		_itemText.push_back(ihealth);
		ihealth.destroy();

		height += 25;
	}

	if (player.equipped[slot].hps != 0) {
		modifer = setModifer(player.equipped[slot].hps);

		hps.color = { 200, 50, 25, 0 };
		hps.pos = { 725, height, 70, 25 };
		hps.load(file.readUnicodeStr(7), _mainWindow.getRenderer());
		_itemText.push_back(hps);
		hps.destroy();

		ihps.type = TEXT_DAMAGE;
		ihps.color = { 200, 50, 25, 0 };
		ihps.pos = { 850, height, NULL, 25 };
		ihps.load(modifer + std::to_string(player.equipped[slot].hps), _mainWindow.getRenderer());
		ihps.setSpacing();
		_itemText.push_back(ihps);
		ihps.destroy();

		height += 25;
	}

	if (player.equipped[slot].mana != 0) {
		modifer = setModifer(player.equipped[slot].mana);

		mana.color = { 25, 50, 200, 0 };
		mana.pos = { 725, height, 80, 25 };
		mana.load(file.readUnicodeStr(8), _mainWindow.getRenderer());
		_itemText.push_back(mana);
		mana.destroy();

		imana.type = TEXT_DAMAGE;
		imana.color = { 25, 50, 200, 0 };
		imana.pos = { 850, height, NULL, 25 };
		imana.load(modifer + std::to_string(player.equipped[slot].mana), _mainWindow.getRenderer());
		imana.setSpacing();
		_itemText.push_back(imana);
		imana.destroy();

		height += 25;
	}

	if (player.equipped[slot].mps != 0) {
		modifer = setModifer(player.equipped[slot].mps);

		mps.color = { 25, 50, 200, 0 };
		mps.pos = { 725, height, 70, 25 };
		mps.load(file.readUnicodeStr(9), _mainWindow.getRenderer());
		_itemText.push_back(mps);
		mps.destroy();

		imps.type = TEXT_DAMAGE;
		imps.color = { 25, 50, 200, 0 };
		imps.pos = { 850, height, NULL, 25 };
		imps.load(modifer + std::to_string(player.equipped[slot].mps), _mainWindow.getRenderer());
		imps.setSpacing();
		_itemText.push_back(imps);
		imps.destroy();

		height += 25;
	}

	if (player.equipped[slot].leech != 0) {
		modifer = setModifer(player.equipped[slot].leech);

		leech.color = { 50, 150, 50, 0 };
		leech.pos = { 725, height, 90, 25 };
		leech.load(file.readUnicodeStr(12), _mainWindow.getRenderer());
		_itemText.push_back(leech);
		leech.destroy();

		ileech.type = TEXT_DAMAGE;
		ileech.color = { 50, 150, 50, 0 };
		ileech.pos = { 850, height, NULL, 25 };
		ileech.load(modifer + std::to_string(player.equipped[slot].leech), _mainWindow.getRenderer());
		ileech.setSpacing();
		_itemText.push_back(ileech);
		ileech.destroy();

		height += 25;
	}

	if (player.equipped[slot].drain != 0) {
		modifer = setModifer(player.equipped[slot].drain);

		drain.color = { 50, 150, 150, 0 };
		drain.pos = { 725, height, 90, 25 };
		drain.load(file.readUnicodeStr(13), _mainWindow.getRenderer());
		_itemText.push_back(drain);
		drain.destroy();

		idrain.type = TEXT_DAMAGE;
		idrain.color = { 50, 150, 150, 0 };
		idrain.pos = { 850, height, NULL, 25 };
		idrain.load(modifer + std::to_string(player.equipped[slot].drain), _mainWindow.getRenderer());
		idrain.setSpacing();
		_itemText.push_back(idrain);
		idrain.destroy();

		height += 25;
	}

	if (player.equipped[slot].luck != 0) {
		modifer = setModifer(player.equipped[slot].luck);

		luck.color = { 150, 150, 50, 0 };
		luck.pos = { 725, height, 80, 25 };
		luck.load(file.readUnicodeStr(14), _mainWindow.getRenderer());
		_itemText.push_back(luck);
		luck.destroy();

		iluck.type = TEXT_DAMAGE;
		iluck.color = { 150, 150, 50, 0 };
		iluck.pos = { 850, height, NULL, 25 };
		iluck.load(modifer + std::to_string(player.equipped[slot].luck), _mainWindow.getRenderer());
		iluck.setSpacing();
		_itemText.push_back(iluck);
		iluck.destroy();

		height += 25;
	}

	if (player.equipped[slot].speed != 0) {
		modifer = setModifer(player.equipped[slot].speed);

		speed.color = { 150, 250, 150, 0 };
		speed.pos = { 725, height, 90, 25 };
		speed.load(file.readUnicodeStr(15), _mainWindow.getRenderer());
		_itemText.push_back(speed);
		speed.destroy();

		ispeed.type = TEXT_DAMAGE;
		ispeed.color = { 150, 250, 150, 0 };
		ispeed.pos = { 850, height, NULL, 25 };
		ispeed.load(modifer + std::to_string(player.equipped[slot].speed), _mainWindow.getRenderer());
		ispeed.setSpacing();
		_itemText.push_back(ispeed);
		ispeed.destroy();
	}
}

std::string Bag::setModifer(const int& num) {
	if (num > 0) {
		return "+";
	}
	else
		return "";
}