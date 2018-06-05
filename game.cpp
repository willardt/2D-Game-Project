#include "game.h"

void Game::Init() {
	_mainWindow.Init("Title", options.windowX, options.windowY, options.windowWidth, options.windowHeight, 0);
	_mainWindow.update = true;

	isRunning = false;

	map.Init(0);

	player.Init(PLAYER, NULL, NULL, NULL);
	player.spell.Init(0);;

	backRect = { 0, 0, _mainWindow.getWidth(), _mainWindow.getHeight() };

	tilesMem = Map::memInit();
	enemiesMem = Entity::memInit(ENEMY);
	npcsMem = Entity::memInit(NPC);
	spellsMem = Spell::memInit();
	itemsMem = Item::memInit();
	objectsMem = Object::memInit();

	textBox.Init();

	fps.Init();
}

void Game::begin() {
	isRunning = true;

	//AUTO LOAD FOR DEBUGGING
	load();

	while (isRunning) {
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
	static bool keyDownV = true;
	static bool keyDownB = true;
	static bool keyDownN = true;
	static bool keyDown0 = true;
	static bool keyDownSpace = true;

	isRunning = in.getQuit(_mainWindow.getWindow());

	bool up = in.isPressed(SDL_SCANCODE_W);
	bool down = in.isPressed(SDL_SCANCODE_S);
	bool left = in.isPressed(SDL_SCANCODE_A);
	bool right = in.isPressed(SDL_SCANCODE_D);

	if (up && !left && !right && !down) {
		move(player, UP, NULL, true);
	}
	else if (!up && !left && !right && down) {
		move(player, DOWN, NULL, true);
	}
	else if (!up && left && !right && !down) {
		move(player, LEFT, NULL, true);
	}
	else if (!up && !left && right && !down) {
		move(player, RIGHT, NULL, true);
	}
	else if (up && left && right && !down) {
		move(player, UP, NULL, true);
	}
	else if (up && left && !right && !down) {
		move(player, UP, NULL, false);
		move(player, LEFT, NULL, true);
	}
	else if (up && !left && right && !down) {
		move(player, UP, NULL, false);
		move(player, RIGHT, NULL, true);
	}
	else if (up && !left && !right && !down) {
		move(player, UP, NULL, false);
	}
	else if (!up && left && right && down) {
		move(player, DOWN, NULL, true);
	}
	else if (!up && left && !right && down) {
		move(player, DOWN, NULL, false);
		move(player, LEFT, NULL, true);
	}
	else if (!up && !left && right && down) {
		move(player, DOWN, NULL, false);
		move(player, RIGHT, NULL, true);
	}
	else if (!up && left && !right && !down) {
		move(player, LEFT, NULL, true);
	}
	else if (up && left && !right && down) {
		move(player, LEFT, NULL, true);
	}
	else if (up && !left && right && down) {
		move(player, RIGHT, NULL, true);
	}

	if (!options.isCam && in.isPressed(SDL_SCANCODE_Y)) {
		options.camY -= options.camSpeed;
	}
	if (!options.isCam && in.isPressed(SDL_SCANCODE_G)) {
		options.camX -= options.camSpeed;
	}
	if (!options.isCam && in.isPressed(SDL_SCANCODE_H)) {
		options.camY += options.camSpeed;
	}
	if (!options.isCam && in.isPressed(SDL_SCANCODE_J)) {
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
		player.castSpellMouse(mouseX, mouseY);
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
		if (q != -1 && items[q].id != 2 && player.items.size() <= Player::MAX_BAG_SIZE) {
			player.items.push_back(items[q]);
			items.erase(items.begin() + q);
		}
	}

	if (in.isPressed(SDL_SCANCODE_V) == false) {
		keyDownV = false;
	}
	if (keyDownV == false && in.isPressed(SDL_SCANCODE_V) == true) {
		if (options.showPaths == true) {
			options.showPaths = false;
			std::cout << "Show Paths Off" << std::endl;
		}
		else {
			options.showPaths = true;
			std::cout << "Show Paths On" << std::endl;
		}
		keyDownV = true;
	}

	if (in.isPressed(SDL_SCANCODE_B) == false) {
		keyDownB = false;
	}
	if (keyDownB == false && in.isPressed(SDL_SCANCODE_B) == true) {
		if (options.showWarps == true) {
			options.showWarps = false;
			std::cout << "Show Warps Off" << std::endl;
		}
		else {
			options.showWarps = true;
			std::cout << "Show Warps On" << std::endl;
		}
		keyDownB = true;
	}

	if (in.isPressed(SDL_SCANCODE_N) == false) {
		keyDownN = false;
	}
	if (keyDownN == false && in.isPressed(SDL_SCANCODE_N) == true) {
		if (options.showCombatRange == true) {
			options.showCombatRange = false;
			std::cout << "Show Combat Range Off" << std::endl;
		}
		else {
			options.showCombatRange = true;
			std::cout << "Show Combat Range On" << std::endl;
		}
		keyDownN = true;
	}

	if (in.isPressed(SDL_SCANCODE_0) == false) {
		keyDown0 = false;
	}
	if (keyDown0 == false && in.isPressed(SDL_SCANCODE_0) == true) {
		loadMap(map.id, FRESH);
		keyDown0 = true;
	}
}

void Game::update() {

	warpCollision();

	// ----------PLAYER----------
	player.playerUpdate();
	player.pickupShards(items, texts);

	for (size_t i = 0; i < player.spells.size(); i++) {
		if (player.spells[i].isActive == true) {
			if (player.collision(player.spells[i], map.solids) == true) {
			}
			else {
				int q = player.collision(player.spells[i], enemies);
				if (q != -1) {
					enemies[q].applyDamage(player.calcDamage(player.spells[i].damage, enemies[q].defense, enemies[q].level), texts, player.spells[i].color);
					if (enemies[q].isDead == true) {
						int k = player.findQuestTarget(enemies[q].id);
						if (k != -1) {
							player.quests[k].addCount(enemies[q].id);
						}
					}
				}
				else {
					q = spellCollision(player.spells[i], npcs);
					if (q != -1) {

					}
					else {
						spellCollision(player.spells[i], objectSolids);
					}
				}
			}
		}
	}

	// ----------ENTITIES----------
	for (size_t i = 0; i < enemies.size(); i++) {
		move(enemies[i], int(i), enemies[i].getPathDir(), NULL, true);
		enemies[i].updateCombat(player.pos);

		for (size_t j = 0; j < enemies[i].spells.size(); j++) {
			if (enemies[i].spells[j].isActive == true) {
				if (enemies[i].collision(enemies[i].spells[j], map.solids) == true) {
				}
				else if (player.spellCollision(enemies[i].spells[j]) == true) {
					player.applyDamage(enemies[i].calcDamage(enemies[i].spells[j].damage, player.defense, player.level), texts, enemies[i].spells[j].color);
					enemies[i].isCombat = true;
				}
				else {
					int q = spellCollision(enemies[i].spells[j], npcs);
					if (q != -1) {

					}
					else {
						spellCollision(enemies[i].spells[j], objectSolids);
					}
				}
			}
		}

		enemies[i].update();

		if (enemies[i].isDead == true) {
			player.addExp(enemies[i].exp, textBox);
			Text::printT(TEXT_DAMAGE, "+" + std::to_string(enemies[i].exp), { player.pos.x + 50, player.pos.y - 50, NULL, 25 }, texts, Text::GREEN);
			enemies[i].dropLoot(items);
			enemies.erase(enemies.begin() + i);
		}
	}

	for (size_t i = 0; i < npcs.size(); i++) {
		npcs[i].update();
		move(npcs[i], i, npcs[i].getPathDir(), NULL, true);
	}


	// ----------SPELLS------------

	// ----------TEXT----------

	for (size_t i = 0; i < texts.size(); i++) {
		texts[i].update();
		if (texts[i].isEnd == true) {
			texts[i].destroy();
			texts.erase(texts.begin() + i);
		}
	}

	Text::clear(uiInfo);

	textBox.update();


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
		tilesMem[map.map[i].id].render(map.map[i].pos, _mainWindow.getRenderer());
	}

	if (options.showWarps) {
		for (size_t i = 0; i < warps.size(); i++) {
			Texture::drawRectTrans(warps[i].pos, Text::PURPLE, _mainWindow.getRenderer());
		}
	}

	for (size_t i = 0; i < items.size(); i++) {
		itemsMem[items[i].id].render(items[i].pos, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < objects.size(); i++) {
		objectsMem[objects[i].id].render(objects[i].pos, _mainWindow.getRenderer());
	}

	// ----------SPELLS----------
	for (size_t i = 0; i < player.spells.size(); i++) {
		spellsMem[player.spells[i].id].renderSprite(player.spells[i].pos, player.spells[i].sprite, _mainWindow.getRenderer());
	}

	// ----------ENTITIES----------
	enemiesMem[0].renderSprite(player.pos, player.sprite, _mainWindow.getRenderer());

	// ----------ENEMIES----------
	for (size_t i = 0; i < enemies.size(); i++) {

		if (options.showCombatRange) {
			Texture::drawRectTrans(enemies[i].combatRange, Text::GREEN, _mainWindow.getRenderer());
		}

		enemiesMem[enemies[i].id].renderSprite(enemies[i].pos, enemies[i].sprite, _mainWindow.getRenderer());

		if (options.showPaths == true) {
			for (size_t k = 0; k < enemies[i].paths.size(); k++) {
				Texture::drawRectTrans({ enemies[i].paths[k].x, enemies[i].paths[k].y, 20, 20 }, Text::RED, _mainWindow.getRenderer());
			}
		}

		for (size_t j = 0; j < enemies[i].spells.size(); j++) {
			spellsMem[enemies[i].spells[j].id].renderSprite(enemies[i].spells[j].pos, enemies[i].spells[j].sprite, _mainWindow.getRenderer());
		}

		if (enemies[i].isCombat == true) {
			UI::drawBar(enemies[i].pos, 8, enemies[i].maxHealth, enemies[i].health, Texture::healthBarFull, Texture::healthBarEmpty, _mainWindow.getRenderer());
			UI::drawBar(enemies[i].pos, 0, enemies[i].maxMana, enemies[i].mana, Texture::manaBarFull, Texture::manaBarEmpty, _mainWindow.getRenderer());
		}
	}

	// ----------NPCS----------
	for (size_t i = 0; i < npcs.size(); i++) {
		npcsMem[npcs[i].id].renderSprite(npcs[i].pos, npcs[i].sprite, _mainWindow.getRenderer());

		if (options.showPaths == true) {
			for (size_t k = 0; k < npcs[i].paths.size(); k++) {
				Texture::drawRectTrans({ npcs[i].paths[k].x, npcs[i].paths[k].y, 20, 20 }, Text::BLUE, _mainWindow.getRenderer());
			}
		}
	}

	itemMouseOver();

	// ----------TEXT----------
	for (size_t i = 0; i < texts.size(); i++) {
		texts[i].renderOutline(_mainWindow.getRenderer());
	}

	for (size_t i = 0; i < uiInfo.size(); i++) {
		uiInfo[i].render(_mainWindow.getRenderer());
	}

	// ---------TEXTBOX----------
	textBox.display(_mainWindow.getRenderer());

	_mainWindow.render();
}

void Game::move(Player& p, int dir, int dis, bool update) {
	p.move(dir, dis, update);
	p.collision(dir, dis, enemies);
	playerNpcCollision(dir, dis, npcs);
	objectCollision(dir, dis, p);
	player.collision(dir, dis, map.solids);
}

void Game::move(Entity& e, int i, int dir, int dis, bool update) {
	e.move(dir, dis, update);
	playerCollision(dir, dis, e);
	e.collision(dir, dis, enemies, i);
	npcCollision(dir, dis, e, npcs);
	objectCollision(dir, dis, e);
	e.collision(dir, dis, map.solids);
}

void Game::move(Npc& n, int i, int dir, int dis, bool update) {
	n.move(dir, dis, update);
	playerCollision(dir, dis, n);
	n.Entity::collision(dir, dis, enemies);
	n.collision(dir, dis, npcs, i);
	objectCollision(dir, dis, n);
	n.Entity::collision(dir, dis, map.solids);
}

void Game::interactNPC() {
	int nIndex = 0;
	int qIndex = 0;
	nIndex = Npc::collisionNPCSpeech(npcs, player.pos);
	if (nIndex != -1) {
		qIndex = player.findQuestNpc(npcs[nIndex].id);
		if (qIndex != -1) {
			if (npcs[nIndex].promptQuest(player.quests[qIndex], textBox) == true) {
				if (player.quests[qIndex].rewardID != 2) {
					Item reward;
					reward.Init(player.quests[nIndex].rewardID);
					player.items.push_back(reward);
				}
				else {
					player.shards += npcs[nIndex].shards;
				}
			}
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

int Game::spellCollision(Spell& spell, std::vector<Npc>& npcs) {
	for (size_t i = 0; i < npcs.size(); i++) {
		if (Collision::seperateAxis(spell.pos, npcs[i].pos) == true) {
			spell.hit();
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

bool Game::spellCollision(Spell& spell, std::vector<Object>& solids) {
	for (size_t i = 0; i < solids.size(); i++) {
		if (Collision::rectCollision(spell.pos, solids[i].pos) == true) {
			spell.hit();
			return true;
		}
	}
	return false;
}

void Game::npcCollision(const int& dir, const int& dis, Entity& obj, std::vector<Npc>& npcs) {
	for (size_t i = 0; i < npcs.size(); i++) {
		if (Collision::seperateAxis(obj.pos, npcs[i].pos) == true) {
			switch (dir) {
			case UP:		obj.move(DOWN, dis, false);				break;
			case DOWN:		obj.move(UP, dis, false);				break;
			case RIGHT:		obj.move(LEFT, dis, false);				break;
			case LEFT:		obj.move(RIGHT, dis, false);			break;
			case UPRIGHT:	obj.move(DOWNLEFT, dis, false);			break;
			case DOWNLEFT:	obj.move(UPRIGHT, dis, false);			break;
			case UPLEFT:	obj.move(DOWNRIGHT, dis, false);		break;
			case DOWNRIGHT:	obj.move(UPLEFT, dis, false);			break;
			}
			break;
		}
	}
}

void Game::playerNpcCollision(const int& dir, const int& dis, std::vector<Npc>& npcs) {
	for (size_t i = 0; i < npcs.size(); i++) {
		if (Collision::seperateAxis(player.pos, npcs[i].pos) == true) {
			switch (dir) {
			case UP:		player.move(DOWN, dis, false);			break;
			case DOWN:		player.move(UP, dis, false);			break;
			case RIGHT:		player.move(LEFT, dis, false);			break;
			case LEFT:		player.move(RIGHT, dis, false);			break;
			case UPRIGHT:	player.move(DOWNLEFT, dis, false);		break;
			case DOWNLEFT:	player.move(UPRIGHT, dis, false);		break;
			case UPLEFT:	player.move(DOWNRIGHT, dis, false);		break;
			case DOWNRIGHT:	player.move(UPLEFT, dis, false);		break;
			}
			break;
		}
	}
}

void Game::playerCollision(const int& dir, const int& dis, Entity& enemy) {
	if (Collision::seperateAxis(player.pos, enemy.pos) == true) {
		switch (dir) {
		case UP:		enemy.move(DOWN, dis, false);			break;
		case DOWN:		enemy.move(UP, dis, false);				break;
		case RIGHT:		enemy.move(LEFT, dis, false);			break;
		case LEFT:		enemy.move(RIGHT, dis, false);			break;
		case UPRIGHT:	enemy.move(DOWNLEFT, dis, false);		break;
		case DOWNLEFT:	enemy.move(UPRIGHT, dis, false);		break;
		case UPLEFT:	enemy.move(DOWNRIGHT, dis, false);		break;
		case DOWNRIGHT:	enemy.move(UPLEFT, dis, false);			break;
		}
	}
}

void Game::playerCollision(const int& dir, const int& dis, Npc& npc) {
	if (Collision::seperateAxis(player.pos, npc.pos) == true) {
		switch (dir) {
		case UP:		npc.move(DOWN, dis, false);				break;
		case DOWN:		npc.move(UP, dis, false);				break;
		case RIGHT:		npc.move(LEFT, dis, false);				break;
		case LEFT:		npc.move(RIGHT, dis, false);			break;
		case UPRIGHT:	npc.move(DOWNLEFT, dis, false);			break;
		case DOWNLEFT:	npc.move(UPRIGHT, dis, false);			break;
		case UPLEFT:	npc.move(DOWNRIGHT, dis, false);		break;
		case DOWNRIGHT:	npc.move(UPLEFT, dis, false);			break;
		}
	}
}

void Game::objectCollision(const int& dir, const int& dis, Entity& enemy) {
	for (size_t i = 0; i < objectSolids.size(); i++) {
		if (Collision::rectCollision(enemy.pos, objectSolids[i].pos) == true) {
			switch (dir) {
			case UP:		enemy.move(DOWN, dis, false);			break;
			case DOWN:		enemy.move(UP, dis, false);				break;
			case RIGHT:		enemy.move(LEFT, dis, false);			break;
			case LEFT:		enemy.move(RIGHT, dis, false);			break;
			case UPRIGHT:	enemy.move(DOWNLEFT, dis, false);		break;
			case DOWNLEFT:	enemy.move(UPRIGHT, dis, false);		break;
			case UPLEFT:	enemy.move(DOWNRIGHT, dis, false);		break;
			case DOWNRIGHT:	enemy.move(UPLEFT, dis, false);			break;
			}
			break;
		}
	}
}

void Game::objectCollision(const int& dir, const int& dis, Npc& npc) {
	for (size_t i = 0; i < objectSolids.size(); i++) {
		if (Collision::rectCollision(npc.pos, objectSolids[i].pos) == true) {
			switch (dir) {
			case UP:		npc.move(DOWN, dis, false);			break;
			case DOWN:		npc.move(UP, dis, false);			break;
			case RIGHT:		npc.move(LEFT, dis, false);			break;
			case LEFT:		npc.move(RIGHT, dis, false);			break;
			case UPRIGHT:	npc.move(DOWNLEFT, dis, false);		break;
			case DOWNLEFT:	npc.move(UPRIGHT, dis, false);		break;
			case UPLEFT:	npc.move(DOWNRIGHT, dis, false);		break;
			case DOWNRIGHT:	npc.move(UPLEFT, dis, false);		break;
			}
			break;
		}
	}
}

void Game::objectCollision(const int& dir, const int& dis, Player& p) {
	for (size_t i = 0; i < objectSolids.size(); i++) {
		if (Collision::rectCollision(p.pos, objectSolids[i].pos) == true) {
			switch (dir) {
			case UP:		p.move(DOWN, dis, false);			break;
			case DOWN:		p.move(UP, dis, false);			break;
			case RIGHT:		p.move(LEFT, dis, false);			break;
			case LEFT:		p.move(RIGHT, dis, false);			break;
			case UPRIGHT:	p.move(DOWNLEFT, dis, false);		break;
			case DOWNLEFT:	p.move(UPRIGHT, dis, false);		break;
			case UPLEFT:	p.move(DOWNRIGHT, dis, false);		break;
			case DOWNRIGHT:	p.move(UPLEFT, dis, false);		break;
			}
			break;
		}
	}
}

void Game::warpCollision() {
	for (size_t i = 0; i < warps.size(); i++) {
		if (Collision::seperateAxis(player.pos, warps[i].pos) == true) {
			int destX = warps[i].dest.x + (warps[i].dest.w / 2);
			int destY = warps[i].dest.y + (warps[i].dest.h / 2);
			saveMap(map.id);
			loadMap(warps[i].teleID, NULL);
			player.pos.x = destX;
			player.pos.y = destY;
			break;
		}
	}
}

void Game::itemMouseOver() {
	SDL_Rect mouse = { mouseX + options.camX - (Options::MOUSE_WIDTH / 2), mouseY + options.camY - (Options::MOUSE_HEIGHT / 2) , Options::MOUSE_WIDTH, Options::MOUSE_HEIGHT };
	for (size_t i = 0; i < items.size(); i++) {
		if (items[i].id != 2 && Collision::seperateAxis(mouse, items[i].pos)) {
			UI::drawItemInfo(mouse, { NULL, NULL, NULL, NULL }, items[i].uName, items[i].dropChance, uiInfo, _mainWindow.getRenderer());
			break;
		}
	}
}

void Game::loadMap(const int& id, int type) {
	player.spells.clear();
	for (size_t i = 0; i < enemies.size(); i++) {
		enemies[i].spells.clear();
	}
	map.Init(id);
	Object::loadMapObjects(id, objects);
	objectSolids.clear();
	objectSolids = Object::createObjectSolids(objects);
	switch (type) {
	case NULL:				loadMapEntities(id, enemies, npcs, false);
							loadMapItems(id, items, false);
							break;
	case FRESH:				loadMapEntities(id, enemies, npcs, true);
							loadMapItems(id, items, true);
							break;
	case FRESH_ENTITIES:	loadMapEntities(id, enemies, npcs, true);
							loadMapItems(id, items, false);
							break;
	case FRESH_ITEMS:		loadMapEntities(id, enemies, npcs, false);
							loadMapItems(id, items, true);
							break;
						
	}
	Warp::loadWarps(id, warps);
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

	saveMap(map.id);

	std::cout << "GAME SAVED" << std::endl;
}

void Game::load() {
	File file;
	int mapID = 0;
	file.uread("Data/name.txt");
	player.uName = file.getU16(1);
	file.read("Data/player.txt");
	player.pos.x = file.getInt(2);
	player.pos.y = file.getInt(3);
	player.loadItems();
	player.loadQuests();
	player.level = file.getInt(8);
	player.exp = file.getInt(9);
	player.maxHealth = file.getInt(10);
	player.health = file.getInt(11);
	player.hps = file.getInt(12);
	player.maxMana = file.getInt(13);
	player.mana = file.getInt(14);
	player.mps = file.getInt(15);
	player.damage = file.getInt(16);
	player.defense = file.getInt(17);
	player.leech = file.getInt(18);
	player.drain = file.getInt(19);
	player.luck = file.getInt(20);
	player.speed = file.getInt(21);
	player.shards = file.getInt(22);

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

	mapID = file.getInt(24);

	loadMap(mapID, NULL);

	std::cout << "GAME LOADED" << std::endl;

	//update cam
	player.move(NULL, NULL, false);
}

void Game::saveMap(const int& id) {
	std::ofstream file;
	File sfile;
	sfile.read("Data/Maps/map" + std::to_string(id) + ".txt");

	file.open("Data/Maps/map" + std::to_string(id) + ".txt");
	file << "Name - " << sfile.getStr(1) << std::endl;
	file << "Width - " << sfile.getStr(2) << std::endl;
	file << "Height - " << sfile.getStr(3) << std::endl;
	file << "Tiles - " << sfile.getStr(4) << std::endl;
	file << "Entity Size - " << std::to_string(enemies.size() + npcs.size()) << std::endl;
	file << "Entities - ";
	for (size_t i = 0; i < enemies.size(); i++) {
		file << std::to_string(enemies[i].type) + "|" + std::to_string(enemies[i].id) + "|" + std::to_string(enemies[i].pos.x) +
			"|" + std::to_string(enemies[i].pos.y) + "|" + std::to_string(enemies[i].paths.size());
		for (size_t k = 0; k < enemies[i].paths.size(); k++) {
			file << "|" + std::to_string(enemies[i].paths[k].x) + "|" + std::to_string(enemies[i].paths[k].y) + "|" + std::to_string(enemies[i].paths[k].met);
		}
		file << " ";
	}
	for (size_t i = 0; i < npcs.size(); i++) {
		file << std::to_string(npcs[i].type) + "|" + std::to_string(npcs[i].id) + "|" + std::to_string(npcs[i].pos.x) +
			"|" + std::to_string(npcs[i].pos.y) + "|" + std::to_string(npcs[i].paths.size());
		for (size_t k = 0; k < npcs[i].paths.size(); k++) {
			file << "|" + std::to_string(npcs[i].paths[k].x) + "|" + std::to_string(npcs[i].paths[k].y) + "|" + std::to_string(npcs[i].paths[k].met);
		}
		file << " ";
	}
	file << std::endl;
	file << "Item Size - " << std::to_string(items.size()) << std::endl;
	file << "Items - ";


	for (size_t i = 0; i < items.size(); i++) {
		std::string tempShards = "0";
		if (items[i].id == 2) {
			tempShards = std::to_string(items[i].damage);
		}
		file << std::to_string(items[i].id) + "|" + std::to_string(items[i].pos.x) + "|" + std::to_string(items[i].pos.y) + "|" + tempShards + " ";
	}
	file << std::endl;
	file << "Warp Size - " << sfile.getStr(9) << std::endl;
	file << "Warps - " << sfile.getStr(10) << std::endl;
	file << "Object Size - " << sfile.getStr(11) << std::endl;
	file << "Objects - " << sfile.getStr(12) << std::endl;
	file << "Entity Size - " << sfile.getStr(13) << std::endl;
	file << "Entities - " << sfile.getStr(14) << std::endl;
	file << "Item Size - " << sfile.getStr(15) << std::endl;
	file << "Items - " << sfile.getStr(16) << std::endl;
	file.close();
}













void Game::loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs, bool isFresh) {
	File file;
	Entity tempEnemy;
	Npc tempNPC;
	std::string fileData = " ";
	std::string entityData = " ";
	int fileDataLength = 0;
	int data = 0;
	int entitySize = 0;
	int type = 0;
	file.read("Data/Maps/map" + std::to_string(mapID) + ".txt");
	enemies.clear();
	npcs.clear();

	std::cout << "Loading Entities for map" << mapID << std::endl;

	if (isFresh == false) {
		entitySize = file.getInt(5);
		fileData = file.getStr(6);
	}
	else {
		entitySize = file.getInt(13);
		fileData = file.getStr(14);
	}
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;
	for (int i = 0; i < entitySize; i++) {
		dataType = 0;
		while (dataType < Entity::LOAD_DATA_SIZE) {
			if (dataType >= 0 && dataType < Entity::LOAD_DATA_SIZE - 1) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else if (dataType == Entity::LOAD_DATA_SIZE - 1) {
				while (fileData[p] != ' ' && fileData[p] != '|' && p <= fileDataLength) {
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
					case 4:		tempEnemy.loadPaths(data, fileData, fileDataLength, p, q);	break;
					}
				}
				else if (type == NPC) {
					switch (dataType) {
					case 1:		tempNPC.id = data;			break;
					case 2:		tempNPC.pos.x = data;		break;
					case 3:		tempNPC.pos.y = data;		break;
					case 4:		tempNPC.loadPaths(data, fileData, fileDataLength, p, q);	break;
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

void Game::loadMapItems(const int& mapID, std::vector<Item>& items, bool isFresh) {
	File file;
	Item tempItem;
	std::string fileData = " ";
	std::string itemData = " ";
	int fileDataLength = 0;
	int data = 0;
	int itemSize = 0;
	int type = 0;
	file.read("Data/Maps/map" + std::to_string(mapID) + ".txt");
	items.clear();

	std::cout << "Loading Items for map" << mapID << std::endl;

	if (isFresh == false) {
		itemSize = file.getInt(7);
		fileData = file.getStr(8);
	}
	else {
		itemSize = file.getInt(15);
		fileData = file.getStr(16);
	}
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
			case 0:		tempItem.Init(data);			break;
			case 1:		tempItem.pos.x = data;			break;
			case 2:		tempItem.pos.y = data;			break;
			case 3:		if (tempItem.id == 2) { tempItem.damage = data; } break;
			} 

			dataType++;
		}

		items.push_back(tempItem);
		dataType = 0;
	}
	std::cout << "Items Loaded for map" << mapID << std::endl;
}