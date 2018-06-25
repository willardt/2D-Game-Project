#include "editor.h"

void Editor::Init() {
	Options& options = options.Instance();
	_mainWindow.Init(mainTitle.c_str(), Editor::MAIN_WINDOW_X, Editor::MAIN_WINDOW_Y, Editor::MAIN_WINDOW_WIDTH, Editor::MAIN_WINDOW_HEIGHT, 0);
	_mainWindow.update = true;
	_tileWindow.Init(tileTitle.c_str(), Editor::TILE_WINDOW_X, Editor::TILE_WINDOW_Y, Editor::TILE_WINDOW_WIDTH, Editor::TILE_WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
	_tileWindow.update = true;

	map.Init(0);
	mainMap = Map::memInit();
	tileMap = Map::memInit();
	tileEnemyMem = Entity::memInit(ENEMY);
	mainEnemyMem = Entity::memInit(ENEMY);
	tileNpcMem = Entity::memInit(NPC);
	mainNpcMem = Entity::memInit(NPC);
	tileItemsMem = Item::memInit();
	mainItemsMem = Item::memInit();
	tileObjectMem = Object::memInit();
	mainObjectMem = Object::memInit();
	tileEffectMem = Effect::memInit();
	mainEffectMem = Effect::memInit();

	backRect = { 0, 0, _mainWindow.getWidth(), _mainWindow.getHeight() };
	tileRect = { 0, 0, _tileWindow.getWidth(), _tileWindow.getHeight() };
	selectedRect = { (_tileWindow.getWidth() / 2) - (Map::TILE_SIZE / 2), 200, Map::TILE_SIZE, Map::TILE_SIZE };
	selectedTextureTile.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureEnemy.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureNpc.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureItem.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureObject.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureEffect.setPath("Data/Maps/Tiles/empty.png");

	options.camX = 0;
	options.camY = 0;
	options.isCam = false;

	isRunning = true;
	
	chooseType = CHOOSE_TILES;
	selectType = NULL;
	
	isWarpSelecting = false;
	isWarpPos = false;
	isWarpDest = false;
	isPathSelecting = false;

	selected = -1;

	mouseX = 0;
	mouseY = 0;
	currentT = 0;
}

void Editor::begin() {

	while (isRunning != false) {
		input();
		update();
		display();
		fps.calcFPS();
	}
}

void Editor::input() {
	Options& options = options.Instance();

	isRunning = in.get(_mainWindow.getWindow());
	if (isRunning) {
		isRunning = !in.isKey(SDL_SCANCODE_ESCAPE);
	}

	if (options.isCam == false && in.isHeld(SDL_SCANCODE_W) == true) {
		options.camY -= options.camSpeed;
	}
	if (options.isCam == false && in.isHeld(SDL_SCANCODE_A) == true) {
		options.camX -= options.camSpeed;
	}
	if (options.isCam == false && in.isHeld(SDL_SCANCODE_S) == true) {
		options.camY += options.camSpeed;
	}
	if (options.isCam == false && in.isHeld(SDL_SCANCODE_D) == true) {
		options.camX += options.camSpeed;
	}

	//SELECT TILE ON TILE WINDOW

	if (inTile.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _tileWindow.getWindow())) {
		if (chooseType == CHOOSE_TILES) {
			currentT = setCurrentTile();
		}
		else if (chooseType == CHOOSE_ENEMIES) {
			currentE = setCurrentEntity();
		}
		else if (chooseType == CHOOSE_NPCS) {
			currentN = setCurrentNpc();
		}
		else if (chooseType == CHOOSE_ITEMS) {
			currentI = setCurrentItem();
		}
		else if (chooseType == CHOOSE_OBJECTS) {
			currentO = setCurrentObject();
		}
		else if (chooseType == CHOOSE_EFFECTS) {
			currentEff = setCurrentEffect();
		}
	}

	//PLACE TILE ON MAIN WINDOW

	if (in.isMouseHeld(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow())) {
		if (chooseType == CHOOSE_TILES) {
			placeTile();
		}
	}

	if (in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow())) {
		if (chooseType == CHOOSE_ENEMIES || chooseType == CHOOSE_NPCS) {
			placeEntity();
		}
		else if (chooseType == CHOOSE_ITEMS) {
			placeItem();
		}
		else if (chooseType == CHOOSE_NONE) {
			select();
		}
		else if (chooseType == CHOOSE_WARPS) {
			bool isSelecting = true;
			isWarpPos = true;
			isWarpDest = false;
			placeWarp(currentW.pos);
			currentW.teleID = getMapIDInput(OLD);
			int tempMapID = map.id;
			int tempCamX = options.camX;
			int tempCamY = options.camY;
			save();
			loadMap(currentW.teleID);
			isWarpPos = false;
			isWarpDest = true;
			//std::cout << in.leftClick(mouseX, mouseY, _mainWindow.getWindow()) << " " << in.getQuit(_mainWindow.getWindow()) << std::endl;
			while (isSelecting == true) {
				isSelecting = in.get(_mainWindow.getWindow());

				if (in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow())) {
					isSelecting = false;
				}

				if (options.isCam == false && in.isHeld(SDL_SCANCODE_W) == true) {
					options.camY -= options.camSpeed;
				}
				if (options.isCam == false && in.isHeld(SDL_SCANCODE_A) == true) {
					options.camX -= options.camSpeed;
				}
				if (options.isCam == false && in.isHeld(SDL_SCANCODE_S) == true) {
					options.camY += options.camSpeed;
				}
				if (options.isCam == false && in.isHeld(SDL_SCANCODE_D) == true) {
					options.camX += options.camSpeed;
				}

				display();
				fps.calcFPS();
			}
			placeWarp(currentW.dest);
			loadMap(tempMapID);
			options.camX = tempCamX;
			options.camY = tempCamY;
			warps.push_back(currentW);
		}
		else if (chooseType == CHOOSE_OBJECTS) {
			placeObject();
		}
		else if (chooseType == CHOOSE_EFFECTS) {
			placeEffect();
		}
	}

	//Create or Load Map
	if (in.isKey(SDL_SCANCODE_L)) {
		create();
	}

	//Save Map
	if (in.isKey(SDL_SCANCODE_O)) {
		save();
	}

	//Switch Tiles / Entities

	if (in.isKey(SDL_SCANCODE_1)) {
		chooseType = CHOOSE_TILES;
		selectedRect.w = 64;
		selectedRect.h = 64;
	}

	if (in.isKey(SDL_SCANCODE_2)) {
		chooseType = CHOOSE_NPCS;
		selectedRect.w = currentN.pos.w;
		selectedRect.h = currentN.pos.h;
	}

	if (in.isKey(SDL_SCANCODE_3)) {
		chooseType = CHOOSE_ENEMIES;
		selectedRect.w = currentE.pos.w;
		selectedRect.h = currentE.pos.h;
	}

	if (in.isKey(SDL_SCANCODE_4)) {
		chooseType = CHOOSE_ITEMS;
		selectedRect.w = 64;
		selectedRect.h = 64;
	}

	if (in.isKey(SDL_SCANCODE_5)) {
		chooseType = CHOOSE_OBJECTS;
		selectedRect.w = 86;
		selectedRect.h = 86;
	}

	if (in.isKey(SDL_SCANCODE_6)) {
		chooseType = CHOOSE_EFFECTS;
		selectedRect.w = 50;
		selectedRect.h = 50;
	}

	if (in.isKey(SDL_SCANCODE_7)) {
		chooseType = CHOOSE_WARPS;
	}

	if (in.isKey(SDL_SCANCODE_E)) {
		if (chooseType == CHOOSE_ENEMIES) {
			selectedRect.w = 32;
			selectedRect.h = 64;
		}
		else if (chooseType == CHOOSE_NPCS) {
			selectedRect.w = 32;
			selectedRect.h = 64;
		}
		else if (chooseType == CHOOSE_ITEMS) {
			selectedRect.w = 64;
			selectedRect.h = 64;
		}
		else if (chooseType == CHOOSE_OBJECTS) {
			selectedRect.w = 86;
			selectedRect.h = 86;
		}
		else if (chooseType == CHOOSE_EFFECTS) {
			selectedRect.w = 50;
			selectedRect.h = 50;
		}
		chooseType = CHOOSE_NONE;
	}

	if (in.isKey(SDL_SCANCODE_Q)) {
		if (chooseType == CHOOSE_NONE) {
			if (selectType == SELECT_ENEMY) {
				enemies.erase(enemies.begin() + selected);
				selectType = NULL;
			}
			else if (selectType == SELECT_NPC) {
				npcs.erase(npcs.begin() + selected);
				selectType = NULL;
			}
			else if (selectType == SELECT_ITEM) {
				items.erase(items.begin() + selected);
				selectType = NULL;
			}
			else if (selectType == SELECT_WARP) {
				warps.erase(warps.begin() + selected);
				selectType = NULL;
			}
			else if (selectType == SELECT_OBJECT) {
				objects.erase(objects.begin() + selected);
				selectType = NULL;
			}
			else if (selectType == SELECT_EFFECT) {
				effects.erase(effects.begin() + selected);
				selectType = NULL;
			}
		}
	}
}

void Editor::update() {
	Options& options = options.Instance();

	std::string windowTitle = "Level: " + map.name + " id: " + std::to_string(map.id) + " x: " + std::to_string((mouseX + options.camX) / Map::TILE_SIZE) +
		" y: " + std::to_string((mouseY + options.camY) / Map::TILE_SIZE);

	SDL_SetWindowTitle(_mainWindow.getWindow(), windowTitle.c_str());
}

void Editor::display() {
	Options& options = options.Instance();

	// ----------MAIN WINDOW----------
	background.drawRectNoCam(backRect, { 255, 0, 255, 255 }, _mainWindow.getRenderer());
	background.drawRectNoCam({ (_mainWindow.getWidth() / 2) - 3, 0, 3, _mainWindow.getHeight() }, { 255, 255, 255, 0 }, _mainWindow.getRenderer());
	background.drawRectNoCam({ 0, (_mainWindow.getHeight() / 2) - 3, _mainWindow.getWidth(), 3 }, { 255, 255, 255, 0 }, _mainWindow.getRenderer());
	SDL_Rect mouse = { mouseX + options.camX - 43, mouseY + options.camY - 43, 86, 86 };

	// ----------MAP----------
	for (int i = 0; i < map.area; i++) {
		mainMap[map.map[i].id].render(map.map[i].pos, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < warps.size(); i++) {
		Texture::drawRectTrans(warps[i].pos, Text::PURPLE, _mainWindow.getRenderer());
	}

	if (isWarpSelecting) {
		if (isWarpDest) {
			Texture::drawRectTrans(currentW.dest, Text::RED, _mainWindow.getRenderer());
		}
		else if(isWarpPos) {
			Texture::drawRectTrans(currentW.pos, Text::RED, _mainWindow.getRenderer());
		}
	}

	for (size_t i = 0; i < paths.size(); i++) {
		Texture::drawRectTrans({ paths[i].x, paths[i].y, 20, 20 }, Text::RED, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < objects.size(); i++) {
		mainObjectMem[objects[i].id].render(objects[i].pos, _mainWindow.getRenderer());
	}
	if (chooseType == CHOOSE_OBJECTS) {
		mouse = { mouseX + options.camX - (currentO.pos.w / 2), mouseY + options.camY - (currentO.pos.h / 2), currentO.pos.w, currentO.pos.h};
		mainObjectMem[currentO.id].render(mouse, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < effects.size(); i++) {
		mainEffectMem[effects[i].id].renderSprite(effects[i].pos, effects[i].sprite, _mainWindow.getRenderer());
		effects[i].update();
	}
	if (chooseType == CHOOSE_EFFECTS) {
		mouse = { mouseX + options.camX - (currentEff.pos.w / 2), mouseY + options.camY - (currentEff.pos.h / 2), currentEff.pos.w, currentEff.pos.h };
		Sprite effectSprite;
		effectSprite = { 0, 0, NULL };
		mainEffectMem[currentEff.id].renderSprite(mouse, effectSprite , _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < items.size(); i++) {
		mainItemsMem[items[i].id].render(items[i].pos, _mainWindow.getRenderer());
	}
	if (chooseType == CHOOSE_ITEMS) {
		mouse = { mouseX + options.camX - 16, mouseY + options.camY - 16, 32, 32 };
		mainItemsMem[currentI.id].render(mouse, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < enemies.size(); i++) {
		mainEnemyMem[enemies[i].id].renderSprite(enemies[i].pos, enemies[i].sprite, _mainWindow.getRenderer());
	}
	if (chooseType == CHOOSE_ENEMIES && !isPathSelecting) {
		Sprite s = { 0, 0, NULL };
		mouse = { mouseX + options.camX - (currentE.pos.w / 2), mouseY + options.camY - (currentE.pos.h / 2), currentE.pos.w, currentE.pos.h };
		mainEnemyMem[currentE.id].renderSprite(mouse, s, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < npcs.size(); i++) {
		mainNpcMem[npcs[i].id].renderSprite(npcs[i].pos, npcs[i].sprite, _mainWindow.getRenderer());
	}
	if (chooseType == CHOOSE_NPCS && !isPathSelecting) {
		Sprite s = { 0, 0, NULL };
		mouse = { mouseX + options.camX - (currentN.pos.w / 2), mouseY + options.camY - (currentN.pos.h / 2), currentN.pos.w, currentN.pos.h };
		mainNpcMem[currentN.id].renderSprite(mouse, s, _mainWindow.getRenderer());
	}

	if (chooseType == CHOOSE_TILES) {
		Texture::drawRectOutline({ mouseX - (Map::TILE_SIZE / 2), mouseY - (Map::TILE_SIZE / 2), Map::TILE_SIZE, Map::TILE_SIZE }, Text::RED, _mainWindow.getRenderer());
	}

	// ----------TILE WINDOW----------
	background.drawRectNoCam(tileRect, { 55, 55, 55, 255 }, _tileWindow.getRenderer());

	if (chooseType == CHOOSE_TILES) {
		selectedTextureTile.renderNoCam(selectedRect, _tileWindow.getRenderer());

		int k = 0;
		int j = 0;
		for (int i = 0; i < int(mainMap.size()); i++) {
			if (i % Editor::ROW_SIZE == 0 && i != 0) {
				k++;
				j = 0;
			}
			if (i < Editor::ROW_SIZE) {
				tileMap[i].renderNoCam({ j * (Map::TILE_SIZE + 1), Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, _tileWindow.getRenderer());
			}
			else {
				tileMap[i].renderNoCam({ j * (Map::TILE_SIZE + 1), (k * Map::TILE_SIZE + 1) + Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, _tileWindow.getRenderer());
			}
			j++;
		}
	}

	else if (chooseType == CHOOSE_ENEMIES) {
		selectedTextureEnemy.renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileEnemyMem.size()); i++) {
			tileEnemyMem[i].renderNoCamSprite({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
	}
	else if (chooseType == CHOOSE_NPCS) {
		selectedTextureNpc.renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileNpcMem.size()); i++) {
			tileNpcMem[i].renderNoCamSprite({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
	}
	else if (chooseType == CHOOSE_ITEMS) {
		selectedTextureItem.renderNoCam(selectedRect, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileItemsMem.size()); i++) {
			tileItemsMem[i].renderNoCam({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, _tileWindow.getRenderer());
		}
	}
	else if (chooseType == CHOOSE_OBJECTS) {
		selectedTextureObject.renderNoCam(selectedRect, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileObjectMem.size()); i++) {
			tileObjectMem[i].renderNoCam({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, _tileWindow.getRenderer());
		}
	}
	else if (chooseType == CHOOSE_EFFECTS) {
		selectedTextureEffect.renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileEffectMem.size()); i++) {
			tileEffectMem[i].renderNoCamSprite({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
	}
	else if (chooseType == CHOOSE_NONE) {
		if (selectType == SELECT_ENEMY) {
			tileEnemyMem[enemies[selected].id].renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
		else if (selectType == SELECT_NPC) {
			tileNpcMem[npcs[selected].id].renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
		else if (selectType == SELECT_ITEM) {
			tileItemsMem[items[selected].id].renderNoCam(selectedRect, _tileWindow.getRenderer());
		}
		else if (selectType == SELECT_OBJECT) {
			tileObjectMem[objects[selected].id].renderNoCam(selectedRect, _tileWindow.getRenderer());
		}
		else if (selectType == SELECT_EFFECT) {
			tileEffectMem[effects[selected].id].renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
	}

	selectedName.renderNoCam(_tileWindow.getRenderer());

	_mainWindow.render();
	_tileWindow.render();
}

int Editor::setCurrentTile() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int width = Editor::ROW_SIZE;
	int n = 0;
	int length = Map::TILE_SIZE;

	widthPos = (mouseX - 6) / length;
	heightPos = ((mouseY - 8) - Editor::TILE_LAYOUT_Y) / length;

	if (widthPos > width * length || widthPos < 0) {
		return 0;
	}
	if (heightPos > (length * (int(tileMap.size()) / width)) || mouseY < Editor::TILE_LAYOUT_Y) {
		return 0;
	}

	if (heightPos == 0) {
		n = widthPos;
	}
	else {
		n = (heightPos * width) + widthPos;
	}

	if (n < int(mainMap.size())) {
		selectedTextureTile.setPath(tileMap[n].getPath());
	}

	selectedRect.w = 64;
	selectedRect.h = 64;
	selectedName = Text::printT(TEXT_NORMAL, "Tile", { 50, 50, NULL, 35 }, Text::WHITE);

	return n;
}

Entity Editor::setCurrentEntity() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int width = 12;
	int length = Map::TILE_SIZE;
	int id = 0;
	Entity temp;

	widthPos = mouseX / length;
	heightPos = (mouseY - Editor::TILE_LAYOUT_Y) / length;

	if (widthPos > width * length || widthPos < 0) {
		temp.Init(ENEMY, id, NULL, NULL);
	}
	if (heightPos > (length * (int(tileMap.size()) / width)) || mouseY < Editor::TILE_LAYOUT_Y) {
		temp.Init(ENEMY, id, NULL, NULL);
	}

	if (heightPos == 0) {
		id = widthPos;
		temp.Init(ENEMY, id, NULL, NULL);
	}
	else {
		id = (heightPos * map.width) + widthPos;
		temp.Init(ENEMY, id, NULL, NULL);
	}

	if (id < int(mainEnemyMem.size()) && id >= 0) {
		selectedTextureEnemy.setPath(mainEnemyMem[id].getPath());
	}
	else {
		temp.Init(ENEMY, 0, NULL, NULL);
	}

	selectedRect.w = temp.pos.w;
	selectedRect.h = temp.pos.h;
	selectedName = Text::printT(TEXT_NORMAL, temp.name, { 50, 50, NULL, 35 }, Text::WHITE);

	return temp;
}

Npc Editor::setCurrentNpc() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int width = 12;
	int length = Map::TILE_SIZE;
	int id = 0;
	Npc temp;

	widthPos = mouseX / length;
	heightPos = (mouseY - Editor::TILE_LAYOUT_Y) / length;

	if (widthPos > width * length || widthPos < 0) {
		temp.Init(NPC, id, NULL, NULL);
	}
	if (heightPos > (length * (int(tileMap.size()) / width)) || mouseY < Editor::TILE_LAYOUT_Y) {
		temp.Init(NPC, id, NULL, NULL);
	}

	if (heightPos == 0) {
		id = widthPos;
		temp.Init(NPC, id, NULL, NULL);
	}
	else {
		id = (heightPos * map.width) + widthPos;
		temp.Init(NPC, id, NULL, NULL);
	}

	if (id < int(mainNpcMem.size()) && id >= 0) {
		selectedTextureNpc.setPath(mainNpcMem[id].getPath());
	}
	else {
		temp.Init(NPC, 0, NULL, NULL);
	}

	selectedRect.w = temp.pos.w;
	selectedRect.h = temp.pos.h;
	selectedName = Text::printT(TEXT_NORMAL, temp.name, { 50, 50, NULL, 35 }, Text::WHITE);

	return temp;
}

Item Editor::setCurrentItem() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int width = 12;
	int length = Map::TILE_SIZE;
	int id = 0;
	Item temp;

	widthPos = mouseX / length;
	heightPos = (mouseY - Editor::TILE_LAYOUT_Y) / length;

	if (widthPos > width * length || widthPos < 0) {
		temp.Init(id);
	}
	if (heightPos >(length * (int(tileMap.size()) / width)) || mouseY < Editor::TILE_LAYOUT_Y) {
		temp.Init(id);
	}

	if (heightPos == 0) {
		id = widthPos;
		temp.Init(id);
	}
	else {
		id = (heightPos * map.width) + widthPos;
		temp.Init(id);
	}

	if (id < int(mainItemsMem.size()) && id >= 0) {
		selectedTextureItem.setPath(mainItemsMem[id].getPath());
	}
	else {
		temp.Init(0);
	}

	selectedRect.w = 64;
	selectedRect.h = 64;
	selectedName = Text::printT(TEXT_NORMAL, temp.name, { 50, 50, NULL, 35 }, Text::WHITE);

	return temp;
}

void Editor::setCurrentWarp(SDL_Rect& rect) {
	Options& options = options.Instance();
	Input in;
	bool isSetting = true;
	bool isLeft = false;

	rect.x = mouseX + options.camX;
	rect.y = mouseY + options.camY;
	rect.w = 0;
	rect.h = 0;

	std::cout << "----------Selecting----------" << std::endl;
	isWarpSelecting = true;

	while (isSetting == true) {
		isSetting = in.get(_mainWindow.getWindow());
		if (!isLeft) {
			isLeft = in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow());
		}
		else {
			isSetting = !in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow());
		}

		if (options.isCam == false && in.isHeld(SDL_SCANCODE_W) == true) {
			options.camY -= options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_A) == true) {
			options.camX -= options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_S) == true) {
			options.camY += options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_D) == true) {
			options.camX += options.camSpeed;
		}

		rect.w = (mouseX + options.camX) - rect.x;
		rect.h = (mouseY + options.camY) - rect.y;

		display();
		fps.calcFPS();
	}

	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w = abs(rect.w);
	}

	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h = abs(rect.h);
	}


	std::cout << "x: " << rect.x << std::endl;
	std::cout << "y: " << rect.y << std::endl;
	std::cout << "w: " << rect.w << std::endl;
	std::cout << "h: " << rect.h << std::endl;

	isWarpSelecting = false;
}

Object Editor::setCurrentObject() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int width = 12;
	int length = Map::TILE_SIZE;
	int id = 0;
	Object temp;

	widthPos = mouseX / length;
	heightPos = (mouseY - Editor::TILE_LAYOUT_Y) / length;

	if (widthPos > width * length || widthPos < 0) {
		temp.Init(id);
	}
	if (heightPos > (length * (int(tileMap.size()) / width)) || mouseY < Editor::TILE_LAYOUT_Y) {
		temp.Init(id);
	}

	if (heightPos == 0) {
		id = widthPos;
		temp.Init(id);
	}
	else {
		id = (heightPos * map.width) + widthPos;
		temp.Init(id);
	}

	if (id < int(mainObjectMem.size()) && id >= 0) {
		selectedTextureObject.setPath(mainObjectMem[id].getPath());
	}
	else {
		temp.Init(0);
	}

	selectedRect.w = 86;
	selectedRect.h = 86;
	selectedName = Text::printT(TEXT_NORMAL, temp.name, { 50, 50, NULL, 35 }, Text::WHITE);

	return temp;
}

Effect Editor::setCurrentEffect() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int width = 12;
	int length = Map::TILE_SIZE;
	int id = 0;
	Effect temp;

	widthPos = mouseX / length;
	heightPos = (mouseY - Editor::TILE_LAYOUT_Y) / length;

	if (widthPos > width * length || widthPos < 0) {
		temp.Init(id);
	}
	if (heightPos >(length * (int(tileMap.size()) / width)) || mouseY < Editor::TILE_LAYOUT_Y) {
		temp.Init(id);
	}

	if (heightPos == 0) {
		id = widthPos;
		temp.Init(id);
	}
	else {
		id = (heightPos * map.width) + widthPos;
		temp.Init(id);
	}

	if (id < int(mainEffectMem.size()) && id >= 0) {
		selectedTextureEffect.setPath(mainEffectMem[id].getPath());
	}
	else {
		temp.Init(0);
	}

	selectedRect.w = 50;
	selectedRect.h = 50;
	selectedName = Text::printT(TEXT_NORMAL, temp.name, { 50, 50, NULL, 35 }, Text::WHITE);

	return temp;
}

void Editor::placeTile() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int n = 0;

	widthPos = (mouseX + options.camX) / Map::TILE_SIZE;
	heightPos = (mouseY + options.camY) / Map::TILE_SIZE;

	if (widthPos > map.width - 1) {
		widthPos = map.width - 1;
	}
	else if (widthPos < 0) {
		widthPos = 0;
	}

	if (heightPos > map.height - 1) {
		heightPos = map.height - 1;
	}
	else if (heightPos < 0) {
		heightPos = 0;
	}

	if (heightPos == 0) {
		n = widthPos;
	}
	else {
		n = (heightPos * map.width) + widthPos;
	}

	map.map[n].id = currentT;
}


void Editor::placeEntity() {
	Options& options = options.Instance();

	bool isValid = true;
	int widthPos = 0;
	int heightPos = 0;
	int n = 0;
	int x = 0;
	int y = 0;

	widthPos = (mouseX + options.camX) / Map::TILE_SIZE;
	heightPos = (mouseY + options.camY) / Map::TILE_SIZE;

	x = mouseX;
	y = mouseY;

	if (chooseType == CHOOSE_ENEMIES) {
		if (widthPos > map.width - (currentE.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
		if (heightPos > map.height - (currentE.pos.h / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	else if (chooseType == CHOOSE_NPCS) {
		if (widthPos > map.width - (currentN.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
		if (heightPos > map.height - (currentN.pos.h / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	if (mouseX + options.camX < 0) {
		isValid = false;
	}
	if (mouseY + options.camY < 0) {
		isValid = false;
	}

	if (isValid == true) {
		if (chooseType == CHOOSE_NPCS) {
			currentN.pos.x = x + options.camX - (currentN.pos.w / 2);
			currentN.pos.y = y + options.camY - (currentN.pos.h / 2);
			npcs.push_back(currentN);
			placePath(currentN, paths);
			npcs.pop_back();
			npcs.push_back(currentN);
		}
		else if (chooseType == CHOOSE_ENEMIES) {
			currentE.pos.x = x + options.camX - (currentE.pos.w / 2);
			currentE.pos.y = y + options.camY - (currentE.pos.h / 2);
			enemies.push_back(currentE);
			placePath(currentE, paths);
			enemies.pop_back();
			enemies.push_back(currentE);
		}
	}

}

void Editor::placeItem() {
	Options& options = options.Instance();

	bool isValid = true;
	int widthPos = 0;
	int heightPos = 0;
	int n = 0;
	int x = 0;
	int y = 0;

	widthPos = (mouseX + options.camX) / Map::TILE_SIZE;
	heightPos = (mouseY + options.camY) / Map::TILE_SIZE;

	x = mouseX;
	y = mouseY;

	if (widthPos > map.width - (32 / Map::TILE_SIZE) - 1) {
		isValid = false;
	}
	else if (mouseX + options.camX < 0) {
		isValid = false;
	}

	if (heightPos > map.height - (32/ Map::TILE_SIZE) - 1) {
			isValid = false;	
	}
	else if (mouseY + options.camY < 0) {
		isValid = false;
	}

	if (isValid == true) {
		currentI.pos.x = x + options.camX - 16;
		currentI.pos.y = y + options.camY - 16;
		if (currentI.id == 2) {
			int shards = 0;
			std::cout << "Shards: ";
			std::cin >> shards;
			currentI.damage = shards;
		}
		items.push_back(currentI);
	}

}

void Editor::placePath(Entity& e, std::vector<Path>& p) {
	Input in;
	Options& options = options.Instance();
	bool selecting = true;
	bool isLeft = false;
	isPathSelecting = true;

	e.paths.clear();
	p.clear();

	while (selecting == true) {

		if (in.get(_mainWindow.getWindow()) == false) {
			selecting = false;
			isRunning = false;
		}

		if (options.isCam == false && in.isHeld(SDL_SCANCODE_W) == true) {
			options.camY -= options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_A) == true) {
			options.camX -= options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_S) == true) {
			options.camY += options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_D) == true) {
			options.camX += options.camSpeed;
		}

		if (in.isKey(SDL_SCANCODE_BACKSPACE)) {
			if (p.size() > 0) {
				p.pop_back();
				e.paths.pop_back();
			}
		}

		if (in.isKey(SDL_SCANCODE_SPACE) || in.isKey(SDL_SCANCODE_ESCAPE)) {
			selecting = false;
		}

		if (!isLeft) {
			isLeft = in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow());
		}
		else if (in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow())) {
			Path temp;
			temp.x = (mouseX - 10) + options.camX;
			temp.y = (mouseY - 10) + options.camY;
			temp.met = false;
			p.push_back(temp);
			e.paths.push_back(temp);
		}

		display();

		fps.calcFPS();
	}
	p.clear();

	isPathSelecting = false;
}

void Editor::placePath(Npc& n, std::vector<Path>& p) {
	Input in;
	Options& options = options.Instance();
	bool selecting = true;
	bool isLeft = false;
	isPathSelecting = true;

	n.paths.clear();
	p.clear();

	while (selecting == true) {

		if (in.get(_mainWindow.getWindow()) == false) {
			selecting = false;
			isRunning = false;
		}

		if (options.isCam == false && in.isHeld(SDL_SCANCODE_W) == true) {
			options.camY -= options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_A) == true) {
			options.camX -= options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_S) == true) {
			options.camY += options.camSpeed;
		}
		if (options.isCam == false && in.isHeld(SDL_SCANCODE_D) == true) {
			options.camX += options.camSpeed;
		}

		if (in.isKey(SDL_SCANCODE_BACKSPACE)) {
			if (p.size() > 0) {
				p.pop_back();
				n.paths.pop_back();
			}
		}

		if (in.isKey(SDL_SCANCODE_SPACE) || in.isKey(SDL_SCANCODE_ESCAPE)) {
			selecting = false;
		}

		if (!isLeft) {
			isLeft = in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow());
		}
		else if (in.isMouse(mouseX, mouseY, SDL_BUTTON_LEFT, _mainWindow.getWindow())) {
			Path temp;
			temp.x = (mouseX - 10) + options.camX;
			temp.y = (mouseY - 10) + options.camY;
			temp.met = false;
			p.push_back(temp);
			n.paths.push_back(temp);
		}

		display();

		fps.calcFPS();
	}
	p.clear();

	isPathSelecting = false;
}

void Editor::placeWarp(SDL_Rect& rect) {
	Options& options = options.Instance();

	bool isValid = true;
	int widthPos = 0;
	int heightPos = 0;
	int n = 0;
	int x = 0;
	int y = 0;

	widthPos = (mouseX + options.camX) / Map::TILE_SIZE;
	heightPos = (mouseY + options.camY) / Map::TILE_SIZE;

	x = mouseX;
	y = mouseY;

	if (widthPos > map.width - (rect.w / Map::TILE_SIZE) - 1) {
		isValid = false;
	}
	else if (mouseX + options.camX < 0) {
		isValid = false;
	}

	if (heightPos > map.height - (rect.h / Map::TILE_SIZE) - 1) {
		isValid = false;
	}
	else if (mouseY + options.camY < 0) {
		isValid = false;
	}

	if (isValid == true) {
		setCurrentWarp(rect);
	}
}

void Editor::placeObject() {
	Options& options = options.Instance();

	bool isValid = true;
	int widthPos = 0;
	int heightPos = 0;
	int n = 0;
	int x = 0;
	int y = 0;

	widthPos = (mouseX + options.camX) / Map::TILE_SIZE;
	heightPos = (mouseY + options.camY) / Map::TILE_SIZE;

	x = mouseX;
	y = mouseY;

	if (widthPos > map.width - (currentO.pos.w / Map::TILE_SIZE) - 1) {
		isValid = false;
	}
	else if (mouseX + options.camX < 0) {
		isValid = false;
	}

	if (heightPos > map.height - (currentO.pos.h / Map::TILE_SIZE) - 1) {
		isValid = false;
	}
	else if (mouseY + options.camY < 0) {
		isValid = false;
	}

	if (isValid == true) {
		currentO.pos.x = mouseX + options.camX - (currentO.pos.w / 2);
		currentO.pos.y = mouseY + options.camY - (currentO.pos.h / 2);
		objects.push_back(currentO);
	}
}

void Editor::placeEffect() {
	Options& options = options.Instance();

	bool isValid = true;
	int widthPos = 0;
	int heightPos = 0;
	int n = 0;
	int x = 0;
	int y = 0;

	widthPos = (mouseX + options.camX) / Map::TILE_SIZE;
	heightPos = (mouseY + options.camY) / Map::TILE_SIZE;

	x = mouseX;
	y = mouseY;

	if (widthPos > map.width - (currentEff.pos.w / Map::TILE_SIZE) - 1) {
		isValid = false;
	}
	else if (mouseX + options.camX < 0) {
		isValid = false;
	}

	if (heightPos > map.height - (currentEff.pos.h / Map::TILE_SIZE) - 1) {
		isValid = false;
	}
	else if (mouseY + options.camY < 0) {
		isValid = false;
	}

	if (isValid == true) {
		currentEff.pos.x = mouseX + options.camX - (currentEff.pos.w / 2);
		currentEff.pos.y = mouseY + options.camY - (currentEff.pos.h / 2);
		effects.push_back(currentEff);
	}
}

void Editor::select() {
	selected = collision(mouseX, mouseY, enemies);
	if (selected != -1) {
		selectType = SELECT_ENEMY;
		selectedRect.w = enemies[selected].pos.w;
		selectedRect.h = enemies[selected].pos.h; 
		std::cout << "Enemy name: " << enemies[selected].name << std::endl;
	}
	else {
		selected = collision(mouseX, mouseY, npcs);
		if (selected != -1) {
			selectType = SELECT_NPC;
			selectedRect.w = npcs[selected].pos.w;
			selectedRect.h = npcs[selected].pos.h;
			std::cout << "Npc name: " << npcs[selected].name << std::endl;
		}
		else {
			selected = collision(mouseX, mouseY, items);
			if (selected != -1) {
				selectType = SELECT_ITEM;
				selectedRect.w = 64;
				selectedRect.h = 64;
				std::cout << "Item name: " << items[selected].name << std::endl;
			}
			else {
				selected = collision(mouseX, mouseY, warps);
				if (selected != -1) {
					selectType = SELECT_WARP;
					std::cout << "Warp teleID: " << warps[selected].teleID << std::endl;
				}
				else{
					selected = collision(mouseX, mouseY, objects);
					if (selected != -1) {
						selectType = SELECT_OBJECT;
						std::cout << "Object id: " << objects[selected].id << std::endl;
					}
					else {
						selected = collision(mouseX, mouseY, effects);
						if (selected != -1) {
							selectType = SELECT_EFFECT;
							selectedRect.w = 50;
							selectedRect.h = 50;
							std::cout << "Effect name: " << effects[selected].name << std::endl;
						}
						else {
							selectType = NULL;
						}
					}
				}
			}
		}
	}
}

int Editor::collision(const int& x, const int& y, std::vector<Entity>& e) {
	Options& options = options.Instance();
	SDL_Rect mouse = { x - 15 + options.camX, y - 15 + options.camY, 30, 30 };
	for (size_t i = 0; i < e.size(); i++) {
		if (Collision::seperateAxis(mouse, e[i].pos) == true) {
			return i;
		}
	}
	return -1;
}

int Editor::collision(const int& x, const int& y, std::vector<Npc>& n) {
	Options& options = options.Instance();
	SDL_Rect mouse = { x - 15 + options.camX, y - 15 + options.camY, 30, 30 };
	for (size_t i = 0; i < n.size(); i++) {
		if (Collision::seperateAxis(mouse, n[i].pos) == true) {
			return i;
		}
	}
	return -1;
}

int Editor::collision(const int& x, const int& y, std::vector<Item>& it) {
	Options& options = options.Instance();
	SDL_Rect mouse = { x - 15 + options.camX, y - 15 + options.camY, 30, 30 };
	for (size_t i = 0; i < it.size(); i++) {
		if (Collision::seperateAxis(mouse, it[i].pos) == true) {
			return i;
		}
	}
	return -1;
}

int Editor::collision(const int& x, const int& y, std::vector<Warp>& w) {
	Options& options = options.Instance();
	SDL_Rect mouse = { x - 15 + options.camX, y - 15 + options.camY, 30, 30 };
	for (size_t i = 0; i < w.size(); i++) {
		if (Collision::seperateAxis(mouse, w[i].pos) == true) {
			return i;
		}
	}
	return -1;
}

int Editor::collision(const int& x, const int& y, std::vector<Object>& o) {
	Options& options = options.Instance();
	SDL_Rect mouse = { x - 15 + options.camX, y - 15 + options.camY, 30, 30 };
	for (size_t i = 0; i < o.size(); i++) {
		if (Collision::seperateAxis(mouse, o[i].pos) == true) {
			return i;
		}
	}
	return -1;
}

int Editor::collision(const int& x, const int& y, std::vector<Effect>& e) {
	Options& options = options.Instance();
	SDL_Rect mouse = { x - 15 + options.camX, y - 15 + options.camY, 30, 30 };
	for (size_t i = 0; i < e.size(); i++) {
		if (Collision::seperateAxis(mouse, e[i].pos)) {
			return i;
		}
	}
	return -1;
}

void Editor::save() {
	std::ofstream file;
	std::string tempESize = "";
	std::string tempEntities = "";
	std::string tempISize = "";
	std::string tempItems = "";

	if (map.id == 0) {
		std::cout << "Enter Map Name" << std::endl;
		std::cin >> map.name;

		map.id = getMapIDInput(NEW);
	}

	std::string path = getFileIDPath(map.id);

	file.open(path);

	file << "Name - " << map.name << std::endl;
	file << "Width - " << map.width << std::endl;
	file << "Height - " << map.height << std::endl;
	file << "Tiles - ";

	for (int i = 0; i < map.area; i++) {
		file << std::to_string(map.map[i].id) + " ";
	}

	file << std::endl;

	tempESize = "Entity Size - " + std::to_string(enemies.size() + npcs.size());
	file << tempESize << std::endl;
	tempEntities = "Entities - ";
	for (size_t i = 0; i < enemies.size(); i++) {
		tempEntities += std::to_string(enemies[i].type) + "|" + std::to_string(enemies[i].id) + "|" + std::to_string(enemies[i].pos.x) +
			"|" + std::to_string(enemies[i].pos.y) + "|" + std::to_string(enemies[i].paths.size());
		for (size_t k = 0; k < enemies[i].paths.size(); k++) {
			tempEntities += "|" + std::to_string(enemies[i].paths[k].x) + "|" + std::to_string(enemies[i].paths[k].y) + "|" + std::to_string(enemies[i].paths[k].met);
		}
		tempEntities += " ";
	}
	for (size_t i = 0; i < npcs.size(); i++) {
		tempEntities += std::to_string(npcs[i].type) + "|" + std::to_string(npcs[i].id) + "|" + std::to_string(npcs[i].pos.x) +
			"|" + std::to_string(npcs[i].pos.y) + "|" + std::to_string(npcs[i].paths.size());
		for (size_t k = 0; k < npcs[i].paths.size(); k++) {
			tempEntities += "|" + std::to_string(npcs[i].paths[k].x) + "|" + std::to_string(npcs[i].paths[k].y) + "|" + std::to_string(npcs[i].paths[k].met);
		}
		tempEntities += " ";
	}
	file << tempEntities << std::endl;

	tempISize = "Item Size - " + std::to_string(items.size());
	file << tempISize << std::endl;
	tempItems = "Items - ";
	for (size_t i = 0; i < items.size(); i++) {
		std::string tempShards = "0";
		if (items[i].id == 2) {
			tempShards = std::to_string(items[i].damage);
		}
		tempItems += std::to_string(items[i].id) + "|" + std::to_string(items[i].pos.x) + "|" + std::to_string(items[i].pos.y) + "|" + tempShards + " ";
	}
	file << tempItems << std::endl;

	file << "Warp Size - " << warps.size() << std::endl;
	file << "Warps - ";
	for (size_t i = 0; i < warps.size(); i++) {
		file << std::to_string(warps[i].teleID) + "|" + std::to_string(warps[i].pos.x) + "|" + std::to_string(warps[i].pos.y)
			+ "|" + std::to_string(warps[i].pos.w) + "|" + std::to_string(warps[i].pos.h) + "|" + std::to_string(warps[i].dest.x) +
				"|" + std::to_string(warps[i].dest.y) + "|" + std::to_string(warps[i].pos.w) + "|" + std::to_string(warps[i].dest.h) + " ";
	}
	file << std::endl;

	file << "Object Size - " << objects.size() << std::endl;
	file << "Objects - ";
	for (size_t i = 0; i < objects.size(); i++) {
		file << std::to_string(objects[i].id) + "|" + std::to_string(objects[i].pos.x) + "|" + std::to_string(objects[i].pos.y) + " ";
	}
	file << std::endl;

	file << "Effect Size - " << effects.size() << std::endl;
	file << "Effects - ";
	for (size_t i = 0; i < effects.size(); i++) {
		file << std::to_string(effects[i].id) + "|" + std::to_string(effects[i].pos.x) + "|" + std::to_string(effects[i].pos.y) + " ";
	}
	file << std::endl;

	file << tempESize << std::endl;
	file << tempEntities << std::endl;
	file << tempISize << std::endl;
	file << tempItems << std::endl;

	file.close();

	std::cout << "MAP SAVED at - " << getFileIDPath(map.id) << std::endl;
}

void Editor::load() {
	map.id = getMapIDInput(NULL);
	loadMap(map.id);
	selected = -1;
	selectType = NULL;
}

void Editor::create() {
	std::string path = "";
	std::string title = "";
	int selection = 0;
	int id = 0;
	int width;
	int height;
	int baseTileID = 0;
	bool valid = false;
	File file;

	if (map.id != 0) {
		save();
	}

	map.map.clear();
	enemies.clear();
	npcs.clear();
	warps.clear();
	objects.clear();
	items.clear();

	std::cout << "1 - Load Map" << std::endl;
	std::cout << "2 - Create New Map" << std::endl;
	std::cin >> selection;

	while (selection != 1 && selection != 2) {
		std::cout << "Selection Error" << std::endl;
		std::cin >> selection;
	}

	if (selection == 1) {
		load();
	}

	if (selection == 2) {
		Tile tempTile;
		map.id = getMapIDInput(NEW);

		std::cout << "Enter Map Name" << std::endl;
		std::cin >> map.name;

		std::cout << "Enter Width" << std::endl;
		std::cin >> width;

		while (width < 0 || width > Map::MAX_WIDTH) {
			std::cout << "Invalid Width" << std::endl;
			std::cin >> width;
		}

		map.width = width;

		std::cout << "Enter Height" << std::endl;
		std::cin >> height;

		while (height < 0 || height > Map::MAX_HEIGHT) {
			std::cout << "Invalid Height" << std::endl;
			std::cin >> height;
		}

		map.height = height;

		map.area = map.height * map.width;

		std::cout << "Enter Base Tile" << std::endl;
		std::cin >> baseTileID;

		while (baseTileID < 0 || baseTileID > int(mainMap.size())) {
			std::cout << "Invalid ID" << std::endl;
			std::cin >> baseTileID;
		}

		for (int i = 0; i < map.area; i++) {
			tempTile.id = baseTileID;
			map.map.push_back(tempTile);
		}

		title = "Level Editor - " + map.name;
		SDL_SetWindowTitle(_mainWindow.getWindow(), title.c_str());

		save();
		loadMap(map.id);
	}
}

std::string Editor::getFileIDPath(const int& id) {
	return "Data/Maps/map" + std::to_string(id) + ".txt";
}

int Editor::getMapIDInput(const int& type) {
	File file;
	std::string path = "";
	int id = 0;
	bool valid = false;

	while (valid != true) {
		valid = true;
		std::cout << "Enter Map ID" << std::endl;
		std::cin >> id;

		path = getFileIDPath(id);
		file.read(path);

		if (type == NEW && file.exists(path) != false) {
			std::cout << "Map with that ID already exists" << std::endl;
			valid = false;
		}
		else if (type == OLD && file.exists(path) != false) {
			valid = true;
		}
	}
	return id;
}

void Editor::loadMap(const int& id) {
	map.Init(id);
	Object::loadMapObjects(id, objects);
	Effect::loadEffects(id, effects);
	Editor::loadMapEntities(id, enemies, npcs, true);
	Editor::loadMapItems(id, items, true);
	Warp::loadWarps(id, warps);
}

void Editor::loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs, bool isFresh) {
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
		entitySize = file.getInt(15);
		fileData = file.getStr(16);
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

void Editor::loadMapItems(const int& mapID, std::vector<Item>& items, bool isFresh) {
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
		itemSize = file.getInt(17);
		fileData = file.getStr(18);
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