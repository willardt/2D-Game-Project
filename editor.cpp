#include "editor.h"

void Editor::Init() {
	Options& options = options.Instance();
	_mainWindow.Init(mainTitle.c_str(), Editor::MAIN_WINDOW_X, Editor::MAIN_WINDOW_Y, Editor::MAIN_WINDOW_WIDTH, Editor::MAIN_WINDOW_HEIGHT, 0);
	_mainWindow.update = true;
	_tileWindow.Init(tileTitle.c_str(), Editor::TILE_WINDOW_X, Editor::TILE_WINDOW_Y, Editor::TILE_WINDOW_WIDTH, Editor::TILE_WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
	_tileWindow.update = true;

	map.Init(0);
	tileMap = Map::memInit();
	tileEnemyMem = Entity::memInit(ENEMY);
	mainEnemyMem = Entity::memInit(ENEMY);
	tileNpcMem = Entity::memInit(NPC);
	mainNpcMem = Entity::memInit(NPC);
	tileItemsMem = Item::memInit();
	mainItemsMem = Item::memInit();

	backRect = { 0, 0, _mainWindow.getWidth(), _mainWindow.getHeight() };
	tileRect = { 0, 0, _tileWindow.getWidth(), _tileWindow.getHeight() };
	selectedRect = { (_tileWindow.getWidth() / 2) - (Map::TILE_SIZE / 2), 200, Map::TILE_SIZE, Map::TILE_SIZE };
	selectedTextureTile.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureEnemy.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureNpc.setPath("Data/Maps/Tiles/empty.png");
	selectedTextureItem.setPath("Data/Maps/Tiles/empty.png");

	options.camX = 0;
	options.camY = 0;
	options.isCam = false;

	isRunning = true;
	isTiles = true;
	isEnemies = false;
	isNpcs = false;

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
	static Input in;

	static bool keyDownL = true;
	static bool keyDownO = true;
	static bool keyDownLeftMouse = true;
	static bool keyDown1 = true;
	static bool keyDown2 = true;
	static bool keyDown3 = true;
	static bool keyDown4 = true;

	Options& options = options.Instance();

	isRunning = in.getQuit(_mainWindow.getWindow());

	if (options.isCam == false && in.isPressed(SDL_SCANCODE_W) == true) {
		options.camY -= options.camSpeed;
	}
	if (options.isCam == false && in.isPressed(SDL_SCANCODE_A) == true) {
		options.camX -= options.camSpeed;
	}
	if (options.isCam == false && in.isPressed(SDL_SCANCODE_S) == true) {
		options.camY += options.camSpeed;
	}
	if (options.isCam == false && in.isPressed(SDL_SCANCODE_D) == true) {
		options.camX += options.camSpeed;
	}

	//SELECT TILE ON TILE WINDOW
	if (in.leftClick(mouseX, mouseY, _tileWindow.getWindow()) == true) {
		if (isTiles == true) {
			currentT = setCurrentTile();
		}
		else if (isEnemies == true) {
			currentE = setCurrentEntity();
		}
		else if (isNpcs == true) {
			currentN = setCurrentNpc();
		}
		else if (isItems == true) {
			currentI = setCurrentItem();
		}
	}

	//PLACE TILE ON MAIN WINDOW
	if (in.leftClick(mouseX, mouseY, _mainWindow.getWindow()) == true) {
		if (isTiles == true) {
			placeTile();
		}
	}

	if (in.leftClick(mouseX, mouseY, _mainWindow.getWindow()) == false) {
		keyDownLeftMouse = false;
	}

	if (keyDownLeftMouse == false && in.leftClick(mouseX, mouseY, _mainWindow.getWindow()) == true) {
		if (isEnemies == true || isNpcs == true) {
			placeEntity();
		}
		else if (isItems == true) {
			placeItem();
		}
		keyDownLeftMouse = true;
	}

	if (in.isPressed(SDL_SCANCODE_ESCAPE) == true) {
		isRunning = false;
	}

	//Create or Load Map
	if (in.isPressed(SDL_SCANCODE_L) == false) {
		keyDownL = false;
	}

	if (keyDownL == false && in.isPressed(SDL_SCANCODE_L) == true) {
		create();
		keyDownL = true;
	}

	//Save Map
	if (in.isPressed(SDL_SCANCODE_O) == false) {
		keyDownO = false;
	}

	if (keyDownO == false && in.isPressed(SDL_SCANCODE_O) == true) {
		save();
		keyDownO = true;
	}

	//Switch Tiles / Entities
	if (in.isPressed(SDL_SCANCODE_1) == false) {
		keyDown1 = false;
	}

	if (keyDown1 == false && in.isPressed(SDL_SCANCODE_1) == true) {
		isTiles = true;
		isEnemies = false;
		isNpcs = false;
		isItems = false;
		selectedRect.w = 32;
		selectedRect.h = 32;
		keyDown1 = true;
	}

	if (in.isPressed(SDL_SCANCODE_2) == false) {
		keyDown2 = false;
	}

	if (keyDown2 == false && in.isPressed(SDL_SCANCODE_2) == true) {
		isEnemies = false;
		isNpcs = true;
		isTiles = false;
		isItems = false;
		selectedRect.w = currentN.pos.w;
		selectedRect.h = currentN.pos.h;
		keyDown2 = true;
	}

	if (in.isPressed(SDL_SCANCODE_3) == false) {
		keyDown3 = false;
	}

	if (keyDown3 == false && in.isPressed(SDL_SCANCODE_3) == true) {
		isEnemies = true;
		isNpcs = false;
		isTiles = false;
		isItems = false;
		selectedRect.w = currentE.pos.w;
		selectedRect.h = currentE.pos.h;
		keyDown3 = true;
	}

	if (in.isPressed(SDL_SCANCODE_4) == false) {
		keyDown4 = false;
	}

	if (keyDown4 == false && in.isPressed(SDL_SCANCODE_4) == true) {
		isEnemies = false;
		isNpcs = false;
		isTiles = false;
		isItems = true;
		selectedRect.w = currentI.pos.w;
		selectedRect.h = currentI.pos.h;
		keyDown4 = true;
	}
}

void Editor::update() {
}

void Editor::display() {

	// ----------MAIN WINDOW----------
	background.drawRectNoCam(backRect, { 255, 0, 255, 255 }, _mainWindow.getRenderer());
	background.drawRectNoCam({ (_mainWindow.getWidth() / 2) - 3, 0, 3, _mainWindow.getHeight() }, { 255, 255, 255, 0 }, _mainWindow.getRenderer());
	background.drawRectNoCam({ 0, (_mainWindow.getHeight() / 2) - 3, _mainWindow.getWidth(), 3 }, { 255, 255, 255, 0 }, _mainWindow.getRenderer());

	// ----------MAP----------
	for (int i = 0; i < map.area; i++) {
		map.tiles[map.map[i].id].render(map.map[i].pos, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < items.size(); i++) {
		mainItemsMem[items[i].id].render(items[i].pos, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < enemies.size(); i++) {
		mainEnemyMem[enemies[i].id].renderSprite(enemies[i].pos, enemies[i].sprite, _mainWindow.getRenderer());
	}

	for (size_t i = 0; i < npcs.size(); i++) {
		mainNpcMem[npcs[i].id].renderSprite(npcs[i].pos, npcs[i].sprite, _mainWindow.getRenderer());
	}


	// ----------TILE WINDOW----------
	background.drawRectNoCam(tileRect, { 55, 55, 55, 255 }, _tileWindow.getRenderer());

	if (isTiles == true) {
		selectedTextureTile.renderNoCam(selectedRect, _tileWindow.getRenderer());

		for (int i = 0; i < Map::TOTAL_TEXTURES; i++) {
			tileMap[i].renderNoCam({ (i * Map::TILE_SIZE), Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, _tileWindow.getRenderer());
		}
	}
	else if (isEnemies == true) {
		selectedTextureEnemy.renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileEnemyMem.size()); i++) {
			tileEnemyMem[i].renderNoCamSprite({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
	}
	else if (isNpcs == true) {
		selectedTextureNpc.renderNoCamSprite(selectedRect, { 0, 0, NULL }, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileNpcMem.size()); i++) {
			tileNpcMem[i].renderNoCamSprite({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, { 0, 0, NULL }, _tileWindow.getRenderer());
		}
	}
	else if (isItems == true) {
		selectedTextureItem.renderNoCam(selectedRect, _tileWindow.getRenderer());

		for (int i = 0; i < int(tileItemsMem.size()); i++) {
			tileItemsMem[i].renderNoCam({ i * Map::TILE_SIZE, Editor::TILE_LAYOUT_Y, Map::TILE_SIZE, Map::TILE_SIZE }, _tileWindow.getRenderer());
		}
	}

	_mainWindow.render();
	_tileWindow.render();
}

int Editor::setCurrentTile() {
	Options& options = options.Instance();

	int widthPos = 0;
	int heightPos = 0;
	int width = 12;
	int n = 0;
	int length = Map::TILE_SIZE;

	widthPos = mouseX / length;
	heightPos = (mouseY - Editor::TILE_LAYOUT_Y) / length;

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
		n = (heightPos * map.width) + widthPos;
	}

	if (n < Map::TOTAL_TEXTURES) {
		selectedTextureTile.setPath(tileMap[n].getPath());
	}

	selectedRect.w = 32;
	selectedRect.h = 32;

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

	selectedRect.w = temp.pos.w;
	selectedRect.h = temp.pos.h;

	std::cout << temp.id << std::endl;

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

	if (isEnemies == true) {
		if (widthPos > map.width - (currentE.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	else if (isNpcs == true) {
		if (widthPos > map.width - (currentN.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	if (mouseX + options.camX < 0) {
		isValid = false;
	}
	if (isEnemies == true) {
		if (heightPos > map.height - (currentE.pos.h / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	else if (isNpcs == true) {
		if (heightPos > map.height - (currentN.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	if (mouseY + options.camY < 0) {
		isValid = false;
	}

	if (isValid == true) {
		if (isNpcs == true) {
			currentN.pos.x = x + options.camX;
			currentN.pos.y = y + options.camY;
			npcs.push_back(currentN);
		}
		else if (isEnemies == true) {
			currentE.pos.x = x + options.camX;
			currentE.pos.y = y + options.camY;
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

	if (isEnemies == true) {
		if (widthPos > map.width - (currentE.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	else if (isNpcs == true) {
		if (widthPos > map.width - (currentN.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	if (mouseX + options.camX < 0) {
		isValid = false;
	}
	if (isEnemies == true) {
		if (heightPos > map.height - (currentE.pos.h / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	else if (isNpcs == true) {
		if (heightPos > map.height - (currentN.pos.w / Map::TILE_SIZE) - 1) {
			isValid = false;
		}
	}
	if (mouseY + options.camY < 0) {
		isValid = false;
	}

	if (isValid == true) {
		currentI.pos.x = x + options.camX;
		currentI.pos.y = y + options.camY;
		items.push_back(currentI);
	}

}

void Editor::save() {
	std::ofstream file;

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

	file << std::endl << "Entity Size - " << enemies.size() + npcs.size() << std::endl;
	file << "Entities - ";

	for (size_t i = 0; i < enemies.size(); i++) {
		file << std::to_string(enemies[i].type) + "|" + std::to_string(enemies[i].id) + "|" << std::to_string(enemies[i].pos.x) + "|" + std::to_string(enemies[i].pos.y) + " ";
	}
	for (size_t i = 0; i < npcs.size(); i++) {
		file << std::to_string(npcs[i].type) + "|" + std::to_string(npcs[i].id) + "|" << std::to_string(npcs[i].pos.x) + "|" + std::to_string(npcs[i].pos.y) + " ";
	}
	file << std::endl;

	file << "Item Size - " << items.size() << std::endl;
	file << "Items - ";
	for (size_t i = 0; i < items.size(); i++) {
		file << std::to_string(items[i].id) + "|" + std::to_string(items[i].pos.x) + "|" + std::to_string(items[i].pos.y) + " ";
	}
	file << std::endl;

	file.close();

	std::cout << "MAP SAVED at - " << getFileIDPath(map.id) << std::endl;
}

void Editor::load() {
	map.id = getMapIDInput(NULL);

	loadMap(map.id);
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

	map.map.clear();

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

		while (baseTileID < 0 || baseTileID > int(map.tiles.size())) {
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
	std::string path = "";

	path = "Data/Maps/map" + std::to_string(id) + ".txt";

	return path;
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
		file.setPath(path);

		if (file.readStr(1) != "" && type == NEW) {
			std::cout << "Map with that ID already exists" << std::endl;
			valid = false;
		}

	}

	return id;
}

void Editor::loadMap(const int& id) {
	map.Init(id);
	Editor::loadMapEntities(id, enemies, npcs);
	Editor::loadMapItems(id, items);
}

void Editor::loadMapEntities(const int& mapID, std::vector<Entity>& enemies, std::vector<Npc>& npcs) {
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
			if (dataType >= 0 && dataType < Entity::LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}
			}
			else if (dataType == 3) {
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
			npcs.push_back(tempNPC);
			break;
		}
	}
	std::cout << "Entities Loaded for map" << mapID << std::endl;
}

void Editor::loadMapItems(const int& mapID, std::vector<Item>& items) {
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