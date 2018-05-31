#include "game.h"

void Game::Init() {
	_mainWindow.Init("Title", options.windowX, options.windowY, options.windowWidth, options.windowHeight, 0);
	_mainWindow.update = true;

	isRunning = false;

	map.Init(0);

	player.Init(PLAYER, NULL, NULL, NULL);
	player.spell.Init(0);;

	backRect = { 0, 0, _mainWindow.getWidth(), _mainWindow.getHeight() };

	enemiesMem = Entity::memInit(ENEMY);
	npcsMem = Entity::memInit(NPC);
	spellsMem = Spell::memInit();
	itemsMem = Item::memInit();

	textBox.isActive = false;

	fps.Init();
}

void Game::begin() {
	isRunning = true;

	//AUTO LOAD FOR DEBUGGING
	load();

	while (isRunning != false) {
		input();
		update();
		display();
		fps.calcFPS();
	}
}

void Game::input() {
	static Input in;

	static bool keyDownO = true;
	static bool keyDownP = true;
	static bool keyDownI = true;
	static bool keyDownE = true;
	static bool keyDownC = true;
	static bool keyDownQ = true;
	static bool keyDownSpace = true;

	isRunning = in.getQuit(_mainWindow.getWindow());

	if (in.isPressed(SDL_SCANCODE_W) == true && in.isPressed(SDL_SCANCODE_D) == true) {
		player.move(UP, NULL, false);
		//player.collision(UP, NULL, objects);
		player.collision(UP, NULL, map.solids);
	}
	else if (in.isPressed(SDL_SCANCODE_W) == true && in.isPressed(SDL_SCANCODE_A) == true) {
		player.move(UP, NULL, false);
		//player.collision(UP, NULL, objects);
		player.collision(UP, NULL, map.solids);
	}
	else if (in.isPressed(SDL_SCANCODE_W) == true) {
		player.move(UP, NULL, true);
		//player.collision(UP, NULL, objects);
		player.collision(UP, NULL, map.solids);
	}
	if (in.isPressed(SDL_SCANCODE_S) == true && in.isPressed(SDL_SCANCODE_A) == true) {
		player.move(DOWN, NULL, false);
		//player.collision(DOWN, NULL, objects);
		player.collision(DOWN, NULL, map.solids);
	}
	else if (in.isPressed(SDL_SCANCODE_S) == true && in.isPressed(SDL_SCANCODE_D) == true) {
		player.move(DOWN, NULL, false);
		//player.collision(DOWN, NULL, objects);
		player.collision(DOWN, NULL, map.solids);
	}
	else if (in.isPressed(SDL_SCANCODE_S) == true) {
		player.move(DOWN, NULL, true);
		//player.collision(DOWN, NULL, objects);
		player.collision(DOWN, NULL, map.solids);
	}
	if (in.isPressed(SDL_SCANCODE_A) == true) {
		player.move(LEFT, NULL, true);
		//player.collision(LEFT, NULL, objects);
		player.collision(LEFT, NULL, map.solids);
	}
	if (in.isPressed(SDL_SCANCODE_D) == true) {
		player.move(RIGHT, NULL, true);
		//player.collision(RIGHT, NULL, objects);
		player.collision(RIGHT, NULL, map.solids);
	}
	if (options.isCam == false && in.isPressed(SDL_SCANCODE_Y) == true) {
		options.camY -= options.camSpeed;
	}
	if (options.isCam == false && in.isPressed(SDL_SCANCODE_G) == true) {
		options.camX -= options.camSpeed;
	}
	if (options.isCam == false && in.isPressed(SDL_SCANCODE_H) == true) {
		options.camY += options.camSpeed;
	}
	if (options.isCam == false && in.isPressed(SDL_SCANCODE_J) == true) {
		options.camX += options.camSpeed;
	}
	//REMOVE THIS LATER SO YOU DONT ACCIDENTALY HIT IT
	if (in.isPressed(SDL_SCANCODE_ESCAPE) == true) {
		isRunning = false;
	}
	if (in.isPressed(SDL_SCANCODE_SPACE) == false) {
		keyDownSpace = false;
	}
	if (keyDownSpace == false && in.isPressed(SDL_SCANCODE_SPACE) == true) {
		if (textBox.isActive == true) {
			textBox.isActive = false;
		}
		else {
			textBox.isActive = true;
		}
		keyDownSpace = true;
	}
	if (in.isPressed(SDL_SCANCODE_O) == false) {
		keyDownO = false;
	}
	if (keyDownO == false && in.isPressed(SDL_SCANCODE_O) == true) {
		save();
		keyDownO = true;
	}
	if (in.isPressed(SDL_SCANCODE_P) == false) {
keyDownP = false;
	}
	if (keyDownP == false && in.isPressed(SDL_SCANCODE_P) == true) {
		load();
		keyDownP = true;
	}
	if (in.isPressed(SDL_SCANCODE_I) == false) {
		keyDownI = false;
	}
	if (keyDownI == false && in.isPressed(SDL_SCANCODE_I) == true) {
		if (options.isCam == true) {
			options.isCam = false;
			std::cout << "Cam Lock Off" << std::endl;
		}
		else {
			options.isCam = true;
			player.move(NULL, NULL, false);
			std::cout << "Cam Lock On" << std::endl;
		}
		keyDownI = true;
	}
	if (in.isPressed(SDL_SCANCODE_E) == false) {
		keyDownE = false;
	}
	if (keyDownE == false && in.isPressed(SDL_SCANCODE_E) == true) {
		interactNPC();
		keyDownE = true;
	}

	if (in.leftClick(mouseX, mouseY, _mainWindow.getWindow()) == true) {
		player.castSpell(mouseX, mouseY);
	}

	if (in.isPressed(SDL_SCANCODE_C) == false) {
		keyDownC = false;
	}
	if (keyDownC == false && in.isPressed(SDL_SCANCODE_C) == true) {
		bag.Init(player, items, itemsMem, _mainWindow);
		keyDownC = true;
	}

	if (in.isPressed(SDL_SCANCODE_Q) == false) {
		keyDownQ = false;
	}
	if (keyDownQ == false && in.isPressed(SDL_SCANCODE_Q) == true) {
		keyDownQ = true;
		int q = player.collision(items);
		if (q != -1 && player.items.size() <= Player::MAX_BAG_SIZE) {
			player.items.push_back(items[q]);
			items.erase(items.begin() + q);
		}
	}
}

void Game::update() {
	static int frame = 0;
	if (frame > 1000) { frame = 0; }
	frame++;

	bool collideSolid = false;

	// ----------PLAYER----------
	player.playerUpdate();

	// ----------ENTITIES----------

	for (size_t i = 0; i < enemies.size(); i++) {
		if (frame < 500) {
			enemies[i].move(RIGHT, NULL, true);
			enemies[i].castSpell(400, 400);
		}
		else {
			enemies[i].move(LEFT, NULL, true);
		}
		enemies[i].update();
		if (enemies[i].isDead == true) {
			enemies.erase(enemies.begin() + i);
		}
	}


	// ----------SPELLS------------

	for (size_t i = 0; i < player.spells.size(); i++) {
		if (player.spells[i].isActive == true) {
			if (player.collision(player.spells[i], map.solids) == true) {
			}
			else {
				int q = player.collision(player.spells[i], enemies);
				if (q != -1) {
					enemies[q].applyDamage(player.calcDamage(player.spells[i].damage), texts);
					if (enemies[q].isDead == true) {
						int k = player.findQuestTarget(enemies[q].id);
						if (k != -1) {
							player.quests[k].addCount(enemies[q].id);
						}
					}
				}
			}
		}
	}

	for (size_t i = 0; i < enemies.size(); i++) {
		for (size_t j = 0; j < enemies[i].spells.size(); j++) {
			if (enemies[i].spells[j].isActive == true) {
				if (enemies[i].collision(enemies[i].spells[j], map.solids) == true) {
				}
				else if (player.spellCollision(enemies[i].spells[j]) == true) {
					player.applyDamage(enemies[i].calcDamage(enemies[i].spells[j].damage), texts);
					enemies[i].isCombat = true;
				}
			}
		}
	}

	// ----------TEXT----------

	for (size_t i = 0; i < texts.size(); i++) {
		texts[i].update();
		if (texts[i].isEnd == true) {
			texts.erase(texts.begin() + i);
		}
	}


	// ----------OTHER----------
	std::string windowTitle = "FPS: " + std::to_string(fps.getFPS()) + " Cam - x: " + std::to_string(options.camX) + " y: " + std::to_string(options.camY) + " Player - x: " + std::to_string(player.pos.x) + " y: " + std::to_string(player.pos.y);

	SDL_SetWindowTitle(_mainWindow.getWindow(), windowTitle.c_str());

}

void Game::display() {
	background.drawRectNoCam(backRect, { 5, 255, 255, 0 }, _mainWindow.getRenderer());
	background.drawRectNoCam({ (_mainWindow.getWidth() / 2) - 3, 0, 3, _mainWindow.getHeight() }, { 255, 255, 255, 0 }, _mainWindow.getRenderer());
	background.drawRectNoCam({ 0, (_mainWindow.getHeight() / 2) - 3, _mainWindow.getWidth(), 3 }, { 255, 255, 255, 0 }, _mainWindow.getRenderer());

	// ----------MAP----------
	for (int i = 0; i < map.area; i++) {
		map.tiles[map.map[i].id].render(map.map[i].pos, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < items.size(); i++) {
		itemsMem[items[i].id].render(items[i].pos, _mainWindow.getRenderer());
	}

	// ----------SPELLS----------
	for (size_t i = 0; i < player.spells.size(); i++) {
		spellsMem[player.spells[i].id].renderSprite(player.spells[i].pos, player.spells[i].sprite, _mainWindow.getRenderer());
	}

	// ----------ENTITIES----------
	enemiesMem[0].renderSprite(player.pos, player.sprite, _mainWindow.getRenderer());

	// ----------ENEMIES----------
	for (size_t i = 0; i < enemies.size(); i++) {
		enemiesMem[enemies[i].id].renderSprite(enemies[i].pos, enemies[i].sprite, _mainWindow.getRenderer());

		for (size_t j = 0; j < enemies[i].spells.size(); j++) {
			spellsMem[enemies[i].spells[j].id].renderSprite(enemies[i].spells[j].pos, enemies[i].spells[j].sprite, _mainWindow.getRenderer());
		}

		if (enemies[i].isCombat == true) {
			Texture::drawBar(enemies[i].pos, 8, enemies[i].maxHealth, enemies[i].health, Texture::healthBarFull, Texture::healthBarEmpty, _mainWindow.getRenderer());
			Texture::drawBar(enemies[i].pos, 0, enemies[i].maxMana, enemies[i].mana, Texture::manaBarFull, Texture::manaBarEmpty, _mainWindow.getRenderer());
		}
	}

	// ----------NPCS----------
	for (size_t i = 0; i < npcs.size(); i++) {
		npcsMem[npcs[i].id].renderSprite(npcs[i].pos, npcs[i].sprite, _mainWindow.getRenderer());
	}

	// ----------TEXT----------
	for (size_t i = 0; i < texts.size(); i++) {
		texts[i].renderOutline(_mainWindow.getRenderer());
	}

	// ---------TEXTBOX----------
	textBox.display(_mainWindow.getRenderer());

	_mainWindow.render();
}

void Game::interactNPC() {
	int nIndex = 0;
	int qIndex = 0;
	nIndex = Npc::collisionNPCSpeech(npcs, player.pos);
	if (nIndex != -1) {
		qIndex = player.findQuestNpc(npcs[nIndex].id);
		if (qIndex != -1) {
			npcs[nIndex].promptQuest(player.quests[qIndex], textBox);
		}
		else if (qIndex == -1 && npcs[nIndex].hasQuest == true) {
			player.quests.push_back(npcs[nIndex].quest);
			qIndex = player.quests.size() - 1;
			npcs[nIndex].promptQuest(player.quests[qIndex], textBox);
		}
	}
}

int Game::spellCollision(Spell& spell, std::vector<Entity>& entity) {
	for (size_t i = 0; i < entity.size(); i++) {
		if (Collision::seperateAxis(spell.pos, entity[i].pos) == true) {
			return int(i);
		}
	}
	return -1;
}

bool Game::spellCollision(Spell& spell, std::vector<Tile>& tile) {
	for (size_t i = 0; i < tile.size(); i++) {
		if (Collision::rectCollision(spell.pos, tile[i].pos) == true) {
			return true;
		}
	}
	return false;
}




void Game::loadMap(const int& id) {
	map.Init(id);
	loadMapEntities(id, enemies, npcs);
	loadMapItems(id, items);
}





/*********************************************
Save & Load Save & Load Save & Load Save & Load
Save & Load Save & Load Save & Load Save & Load
Save & Load Save & Load Save & Load Save & Load
*********************************************/

void Game::save() {

	// ----- Player -----
	player.save(map.id);

	std::ofstream file;

	// ----- Options -----
	file.open("Data/options.txt");
	file << "Cam Lock Enabled - " << options.isCam << std::endl;
	file << "Cam.x - " << options.camX << std::endl;
	file << "Cam.y - " << options.camY << std::endl;
	file << "Cam Speed - " << options.camSpeed << std::endl;
	file << "FPS Limit - " << options.fpsLimit << std::endl;
	file << "Window Width - " << options.windowWidth << std::endl;
	file << "Window Height - " << options.windowHeight << std::endl;
	file << "Window.x - " << options.windowX << std::endl;
	file << "Window.y - " << options.windowY << std::endl;
	file << "Language - " << options.lang << std::endl;
	file.close();

	saveMapItems(map.id);

	std::cout << "GAME SAVED" << std::endl;
}

void Game::load() {
	File file;
	int mapID = 0;
	file.setPath("Data/name.txt");
	player.uName = file.readUnicodeStr(1);
	file.setPath(PLAYER_FILE);
	player.pos.x = file.readInt(2);
	player.pos.y = file.readInt(3);
	player.loadItems();
	player.loadQuests();
	player.level = file.readInt(8);
	player.exp = file.readInt(9);
	player.maxHealth = file.readInt(10);
	player.health = file.readInt(11);
	player.hps = file.readInt(12);
	player.maxMana = file.readInt(13);
	player.mana = file.readInt(14);
	player.mps = file.readInt(15);
	player.damage = file.readInt(16);
	player.defense = file.readInt(17);
	player.leech = file.readInt(18);
	player.drain = file.readInt(19);
	player.luck = file.readInt(20);
	player.speed = file.readInt(21);
	player.shards = file.readInt(22);

	for (int i = 0; i < Player::EQUIPPED_SIZE; i++) {
		if (player.equipped[i].isEquipped == true && player.equipped[i].id != -1) {
			player.maxHealth += player.equipped[i].health;
			player.hps += player.equipped[i].hps;
			player.maxMana += player.equipped[i].mana;
			player.mps += player.equipped[i].mps;
			player.damage += player.equipped[i].damage;
			player.defense += player.equipped[i].defense;
			player.leech += player.equipped[i].leech;
			player.drain += player.equipped[i].drain;
			player.luck += player.equipped[i].luck;
			player.speed += player.equipped[i].speed;
		}
	}

	if (player.health > player.maxHealth) {
		player.health = player.maxHealth;
	}
	if (player.mana > player.maxMana) {
		player.mana = player.maxMana;
	}

	mapID = file.readInt(24);

	loadMap(mapID);

	std::cout << "GAME LOADED" << std::endl;

	//update cam
	player.move(NULL, NULL, false);
}

void Game::saveMapItems(const int& id) {
	std::ofstream file;
	File sfile;
	sfile.setPath("Data/Maps/map" + std::to_string(id) + ".txt");
	std::string temp;
	std::vector<std::string> tempStrings;
	for (int i = 1; i < 7; i++) {
		tempStrings.push_back(sfile.readStr(i));
	}

	file.open("Data/Maps/map" + std::to_string(id) + ".txt");
	file << "Name - " << tempStrings[0] << std::endl;
	file << "Width - " << tempStrings[1] << std::endl;
	file << "Height - " << tempStrings[2] << std::endl;
	file << "Tiles - " << tempStrings[3] << std::endl;
	file << "Entity Size - " << tempStrings[4] << std::endl;
	file << "Entities - " << tempStrings[5] << std::endl;
	file << "Item Size - " << items.size() << std::endl;
	file << "Items - ";
	for (size_t i = 0; i < items.size(); i++) {
		file << std::to_string(items[i].id) + "|" + std::to_string(items[i].pos.x) + "|" + std::to_string(items[i].pos.y) + " ";
	}
	file << std::endl;
}













void Game::createMap() {
	std::ofstream file;

	std::string name = "Map 0";
	int width = 50;
	int height = 50;
	int area = width * height;

	file.open("Data/Maps/map0.txt");

	file << "Name - " << name << std::endl;
	file << "Width - " << width << std::endl;
	file << "Height - " << height << std::endl;
	file << "Tiles - ";

	for (int i = 0; i < area; i++) {
		//file << std::to_string((i % 2) + 1) + " ";
		file << "0 ";
	}

	file << std::endl << "Entity Size - " << 1 << std::endl;
	file << "Entities - " << " " << std::endl;

	file.close();
}

void Game::loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs) {
	File file;
	Entity tempEnemy;
	Npc tempNPC;
	std::string fileData = " ";
	std::string entityData = " ";
	int fileDataLength = 0;
	int data = 0;
	int entitySize = 0;
	int type = 0;
	file.setPath("Data/Maps/map" + std::to_string(mapID) + ".txt");
	enemies.clear();
	npcs.clear();

	std::cout << "Loading Entities for map" << mapID << std::endl;

	entitySize = file.readInt(5);
	fileData = file.readStr(6);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;
	for (int i = 0; i < entitySize; i++) {
		dataType = 0;
		while (dataType <= Entity::LOAD_DATA_SIZE) {
			if (dataType < Entity::LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else  {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			entityData = fileData.substr(q, p - q);
			data = std::stoi(entityData);

			q = p + 1;
			p = q;

			if (dataType == 0) {
				switch (data) {
				case ENEMY:		tempEnemy.type = ENEMY;		 break;
				case NPC:		tempNPC.type = NPC;	     break;
				}
				type = data;
			}

			if (dataType > 0) {
				if (type == ENEMY) {
					switch (dataType) {
					case 1:		tempEnemy.id = data;		break;
					case 2:		tempEnemy.pos.x = data;		break;
					case 3:		tempEnemy.pos.y = data;		break;
					}
				}
				else if (type == NPC) {
					switch (dataType) {
					case 1:		tempNPC.id = data;			break;
					case 2:		tempNPC.pos.x = data;		break;
					case 3:		tempNPC.pos.y = data;		break;
					}
				}
			}

			dataType++;
		}

		switch (type) {
		case ENEMY:		tempEnemy.Init(tempEnemy.type, tempEnemy.id, tempEnemy.pos.x, tempEnemy.pos.y);
						enemies.push_back(tempEnemy);													
						break;
		case NPC:		tempNPC.Init(tempNPC.type, tempNPC.id, tempNPC.pos.x, tempNPC.pos.y);
						tempNPC.NPCInit();
						npcs.push_back(tempNPC);													
						break;
		}
	}
	std::cout << "Entities Loaded for map" << mapID << std::endl;
}

void Game::loadMapItems(const int& mapID, std::vector<Item>& items) {
	File file;
	Item tempItem;
	std::string fileData = " ";
	std::string itemData = " ";
	int fileDataLength = 0;
	int data = 0;
	int itemSize = 0;
	int type = 0;
	file.setPath("Data/Maps/map" + std::to_string(mapID) + ".txt");
	items.clear();

	std::cout << "Loading Items for map" << mapID << std::endl;

	itemSize = file.readInt(7);
	fileData = file.readStr(8);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;

	for (int i = 0; i < itemSize; i++) {
		while (dataType <= Item::LOAD_DATA_SIZE) {
			if (dataType < Item::LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			itemData = fileData.substr(q, p - q);
			data = std::stoi(itemData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		tempItem.Init(data);		break;
			case 1:		tempItem.pos.x = data;		break;
			case 2:		tempItem.pos.y = data;		break;
			}

			dataType++;
		}

		items.push_back(tempItem);
		dataType = 0;
	}
	std::cout << "Items Loaded for map" << mapID << std::endl;
}