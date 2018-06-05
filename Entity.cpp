#include "Entity.h"

std::vector<Texture> Entity::memInit(const int& type) {
	File file;

	switch (type) {
	case ENEMY:		file.read("Data/Entities/Enemies/enemies.txt"); 				 break;
	case NPC:		file.read("Data/Entities/Npcs/npcs.txt");						 break;
	}

	std::vector<Texture> tempVec;
	Texture temp;

	for (int i = 1; i <= file.getSize(); i++) {
		temp.setPath(file.getStr(i));
		tempVec.push_back(temp);
	}

	return tempVec;
}

void Entity::Init(const int& ntype, const int& nid, const int& x, const int& y) {
	File file;
	std::string path = "";
	pos.x = x;
	pos.y = y;
	sprite.frame = 0;
	sprite.x = 0;
	sprite.y = 0;
	id = nid;

	type = ntype;

	path = findName(type, id);

	switch (type) {
	case PLAYER:	file.read("Data/Entities/player.txt");						    break;
	case ENEMY:		file.read("Data/Entities/Enemies/" + path + ".txt");			break;
	case NPC:		file.read("Data/Entities/Npcs/" + path + ".txt");				break;
	}

	name = file.getStr(1);
	//	std::cout << texture.getPath() << std::endl;
	pos.w = file.getInt(3);
	pos.h = file.getInt(4);
	posWidthHalf = pos.w / 2;
	posHeightHalf = pos.h / 2;
	ani.maxFrame = file.getInt(5);
	ani.runFrame = file.getInt(6);
	ani.minDown = file.getInt(7);
	ani.maxDown = file.getInt(8);
	ani.minUp = file.getInt(9);
	ani.maxUp = file.getInt(10);
	ani.minRight = file.getInt(11);
	ani.maxRight = file.getInt(12);
	ani.minLeft = file.getInt(13);
	ani.maxLeft = file.getInt(14);
	level = file.getInt(15);
	exp = file.getInt(16);
	maxHealth = file.getInt(17);
	health = maxHealth;
	hps = file.getInt(18);
	maxMana = file.getInt(19);
	mana = maxMana;
	mps = file.getInt(20);
	damage = file.getInt(21);
	defense = file.getInt(22);
	leech = file.getInt(23);
	drain = file.getInt(24);
	luck = file.getInt(25);
	speed = file.getInt(26);
	shards = file.getInt(27);
	spellAni.totalFrames = file.getInt(28);
	spellAni.up = file.getInt(29);
	spellAni.down = file.getInt(30);
	spellAni.left = file.getInt(31);
	spellAni.right = file.getInt(32);
	spellAni.uright = file.getInt(33);
	spellAni.uleft = file.getInt(34);
	spellAni.dright = file.getInt(35);
	spellAni.dleft = file.getInt(36);
	spell.Init(file.getInt(37));

	combatRange.x = pos.x;
	combatRange.y = pos.y;
	combatRange.w = file.getInt(40);
	combatRange.h = file.getInt(41);
	combatRangeWidthHalf = combatRange.w / 2;
	combatRangeHeightHalf = combatRange.h / 2;
	
	lootTable = loadLootTable(id, type);
	
	sprite = Texture::updateSprite(pos, sprite.frame);

	fireRate = spell.fireRate - level;

	combat.tock(0);
	regen.tock(0);
	casting.tock(0);
	castingAni.tock(0);
	
	isCasting = false;
	isCombat = false;
	isDead = false;
	isCastable = true;
}

void Entity::update() {
	
	updateCombat();

	//Regen
	updateRegen();

	//CASTING
	updateCasting();
	updateCastingAni();
	updateSpells();
	updatePath();
}

void Entity::updateCombat() {
	if (isCombat == true) {
		if (combat.tick(HEALTH_BAR_MAX_FRAME)) {
			isCombat = false;
		}
	}
}

void Entity::updateRegen() {
	if (regen.tick(REGEN_TIME) == true) {
		if (health < maxHealth) {
			health += hps;
			if (health > maxHealth) {
				health = maxHealth;
			}
		}
		if (mana < maxMana) {
			mana += mps;
			if (mana > maxMana) {
				mana = maxMana;
			}
		}
	}
}

void Entity::updateCasting() {
	if (isCastable == false) {
		if (fireRate <= 0) {
			isCastable = true;
		}
		else if (casting.tick(fireRate) == true) {
			isCastable = true;
		}
	}
}

void Entity::updateCastingAni() {
	if (isCasting == true) {
		updateSprite(CAST, castDir);
		if (castingAni.tick(CASTING_ANI_TIME) == true) {
			isCasting = false;
			updateSprite(NULL, NULL);
		}
	}
}

void Entity::updateSpells() {
	for (size_t i = 0; i < spells.size(); i++) {
		spells[i].update();
		if (spells[i].isEnd == true) {
			spells.erase(spells.begin() + i);
		}
	}
}

void Entity::updatePath() {
	for (size_t i = 0; i < paths.size(); i++) {
		if (paths[i].met == false) {
			activePath = i;
			break;
		}
		if (i == paths.size() - 1 && paths.size() > 1) {
			reversePath();
		}
	}
}

void Entity::updateCombat(SDL_Rect& player) {
	combatRange.x = pos.x - combatRangeWidthHalf + posWidthHalf;
	combatRange.y = pos.y - combatRangeHeightHalf + posHeightHalf;
	if (Collision::seperateAxis(combatRange, player)) {
		isCombat = true;
		combat.tock(0);
	}
	else {
		isCombat = false;
	}

	if (isCombat && isCastable) {
		castSpell(player.x, player.y);
	}
}

int Entity::getPathDir() {
	if (paths.size() == 0) {
		return NULL;
	}
	int width = pos.x + pos.w;
	int height = pos.y + pos.h;

	if (paths[activePath].y <= pos.y && paths[activePath].x >= width) {
		return UPRIGHT;
	}
	if (paths[activePath].y >= height && paths[activePath].x >= width) {
		return DOWNRIGHT;
	}
	if (paths[activePath].x <= pos.x && paths[activePath].y <= pos.y) {
		return UPLEFT;
	}
	if (paths[activePath].x <= pos.x && paths[activePath].y >= height) {
		return DOWNLEFT;
	}
	if (paths[activePath].x > pos.x && paths[activePath].x < width && paths[activePath].y < pos.y) {
		return UP;
	}
	if (paths[activePath].y > pos.y && paths[activePath].y < height && paths[activePath].x > width) {
		return RIGHT;
	}
	if (paths[activePath].x > pos.x && paths[activePath].x < width && paths[activePath].y > height) {
		return DOWN;
	}
	if (paths[activePath].y > pos.y && paths[activePath].y < height && paths[activePath].x < pos.x) {
		return LEFT;
	}

	paths[activePath].met = true;
	return NULL;
}

void Entity::reversePath() {
	std::vector<Path> temp;

	for (int i = int(paths.size() - 1); i >= 0; i--) {
		paths[i].met = false;
		temp.push_back(paths[i]);
	}
	paths.clear();
	paths = temp;
	activePath = 0;
}

void Entity::move(const int& dir, int dis, const bool& update) {
	Options& options = options.Instance();

	if (dis == NULL) {
		dis = speed;
	}

	switch (dir) {
	case UP:		pos.y -= dis;				break;
	case DOWN:		pos.y += dis;				break;
	case LEFT:		pos.x -= dis;				break;
	case RIGHT:		pos.x += dis;				break;
	case UPRIGHT:	pos.x += dis;	pos.y -= dis;	break;
	case UPLEFT:	pos.x -= dis;	pos.y -= dis;	break;
	case DOWNRIGHT:	pos.x += dis;	pos.y += dis;	break;
	case DOWNLEFT:	pos.x -= dis;	pos.y += dis;	break;
	case NULL:	break;
	}

	checkCam(type, dis);

	if (pos.x < 0) {
		pos.x = 0;
	}
	else if ((pos.x + pos.w) > (options.mapX * Map::TILE_SIZE)) {
		pos.x = options.mapX * Map::TILE_SIZE - pos.w;
	}

	if (pos.y < 0) {
		pos.y = 0;
	}
	else if ((pos.y + pos.h) > (options.mapY * Map::TILE_SIZE)) {
		pos.y = options.mapY * Map::TILE_SIZE - pos.h;
	}

	if (type == PLAYER) {
		if (options.isCam == true) {
			options.camX = pos.x + posWidthHalf - options.windowWidthHalf;
			options.camY = pos.y + posHeightHalf - options.windowHeightHalf;

			if (options.camX < 0) {
				options.camX = 0;
			}
			else if (options.camX > ((options.mapX * Map::TILE_SIZE) - options.windowWidth)) {
				options.camX = (options.mapX * Map::TILE_SIZE) - options.windowWidth;
			}

			if (options.camY < 0) {
				options.camY = 0;
			}
			else if (options.camY > ((options.mapY * Map::TILE_SIZE) - options.windowHeight)) {
				options.camY = (options.mapY * Map::TILE_SIZE) - options.windowHeight;
			}
		}
	}

	if (update == true) {
		updateSprite(MOVE, dir);
	}

}

void Entity::checkCam(const int& type, const int& dis) {
	if (type != PLAYER) {
		return;
	}
	Options& options = options.Instance();

	if ((pos.x - options.camX) <= 0) {
		options.camX -= dis;
	}
	else if (((pos.x + pos.w) - options.camX) >= options.windowWidth) {
		options.camX += dis;
	}
	if ((pos.y - options.camY) <= 0) {
		options.camY -= dis;
	}
	else if (((pos.y + pos.h) - options.camY) >= options.windowHeight) {
		options.camY += dis;
	}
}

void Entity::updateSprite(const int& type, const int& dir) {
	sprite.frame++;

	if (type == NULL) {
		sprite.frame = 0;
	}
	else if (type == MOVE && !isCasting) {

		if (dir == NULL) {
			sprite.frame = 0;
		}
		else if (dir == UP) {
			if (sprite.frame < ani.minUp || sprite.frame > ani.maxUp) {
				sprite.frame = ani.minUp;
			}
		}
		else if (dir == DOWN) {
			if (sprite.frame < ani.minDown || sprite.frame > ani.maxDown) {
				sprite.frame = ani.minDown;
			}
		}
		else if (dir == RIGHT || dir == UPRIGHT || dir == DOWNRIGHT) {
			if (sprite.frame < ani.minRight || sprite.frame > ani.maxRight) {
				sprite.frame = ani.minRight;
			}
		}
		else if (dir == LEFT || dir == UPLEFT || dir == DOWNLEFT) {
			if (sprite.frame < ani.minLeft || sprite.frame > ani.maxLeft) {
				sprite.frame = ani.minLeft;
			}
		}

	}
	else if (type == CAST) {
		switch (dir) {
		case UP:		sprite.frame = spellAni.up;				break;
		case DOWN:		sprite.frame = spellAni.down;			break;
		case RIGHT:		sprite.frame = spellAni.right;			break;
		case LEFT:		sprite.frame = spellAni.left;			break;
		case UPRIGHT:	sprite.frame = spellAni.uright;			break;
		case UPLEFT:	sprite.frame = spellAni.uleft;			break;
		case DOWNRIGHT:	sprite.frame = spellAni.dright;			break;
		case DOWNLEFT:	sprite.frame = spellAni.dleft;			break;
		}
	}
	else if (type == SPECIAL && !isCasting) {
		if (sprite.frame < ani.specialFrame || sprite.frame > ani.specialFrame) {
			sprite.frame = ani.specialFrame;
		}
	}

	sprite = Texture::updateSprite(pos, sprite.frame);
}

// SDL_RECT W AND H ARE NOT WIDTH AND HEIGHT FOR THIS INSTANCE
// W - pos.x + pos.w on the x-axis
// H - pos.y + pos.h on the y-axis
void Entity::collision(const int& dir, const int& dis, std::vector<Entity>& obj, const int& selfIndex) {
	int length = int(obj.size());
	for (int i = 0; i < length; i++) {
		if (i == selfIndex) {
			i++;
			if (i >= length) {
				break;
			}
		}

		if (Collision::seperateAxis(pos, obj[i].pos) == true) {
			switch (dir) {
			case UP:		move(DOWN, dis, false);			break;
			case DOWN:		move(UP, dis, false);			break;
			case RIGHT:		move(LEFT, dis, false);			break;
			case LEFT:		move(RIGHT, dis, false);		break;
			case UPRIGHT:	move(DOWNLEFT, dis, false);		break;
			case DOWNLEFT:	move(UPRIGHT, dis, false);		break;
			case UPLEFT:	move(DOWNRIGHT, dis, false);	break;
			case DOWNRIGHT:	move(UPLEFT, dis, false);		break;
			}
			break;
		}
	}
}

void Entity::collision(const int& dir, const int& dis, std::vector<Entity>& obj) {
	for (size_t i = 0; i < obj.size(); i++) {
		if (Collision::seperateAxis(pos, obj[i].pos) == true) {
			switch (dir) {
			case UP:		move(DOWN, dis, false);			break;
			case DOWN:		move(UP, dis, false);			break;
			case RIGHT:		move(LEFT, dis, false);			break;
			case LEFT:		move(RIGHT, dis, false);		break;
			case UPRIGHT:	move(DOWNLEFT, dis, false);		break;
			case DOWNLEFT:	move(UPRIGHT, dis, false);		break;
			case UPLEFT:	move(DOWNRIGHT, dis, false);	break;
			case DOWNRIGHT:	move(UPLEFT, dis, false);		break;
			}
			break;
		}
	}
}

void Entity::collision(const int& dir, const int& dis, std::vector<Tile>& tile) {
	for (size_t i = 0; i < tile.size(); i++) {
		if (Collision::rectCollision(pos, tile[i].pos) == true) {
			switch (dir) {
			case UP:		move(DOWN, dis, false);			break;
			case DOWN:		move(UP, dis, false);			break;
			case RIGHT:		move(LEFT, dis, false);			break;
			case LEFT:		move(RIGHT, dis, false);		break;
			case UPRIGHT:	move(DOWNLEFT, dis, false);		break;
			case DOWNLEFT:	move(UPRIGHT, dis, false);		break;
			case UPLEFT:	move(DOWNRIGHT, dis, false);	break;
			case DOWNRIGHT:	move(UPLEFT, dis, false);		break;
			}
			break;
		}
	}
}

int Entity::collision(std::vector<Entity>& obj) {
	for (size_t i = 0; i < obj.size(); i++) {
	if (Collision::seperateAxis(pos, obj[i].pos) == true) {
		return int(i);
		}
	}
	return -1;
}

bool Entity::collision(std::vector<Tile>& tile) {
	for (size_t i = 0; i < tile.size(); i++) {
		if (Collision::rectCollision(pos, tile[i].pos) == true) {
			return true;
		}
	}
	return false;
}

int Entity::collision(std::vector<Item>& items) {
	for (size_t i = 0; i < items.size(); i++) {
		if (Collision::seperateAxis(pos, items[i].pos) == true) {
			return int(i);
		}
	}
	return -1;
}

void Entity::collision(const int& dir, const int& dis, Entity& obj) {
	if (Collision::seperateAxis(pos, obj.pos) == true) {
		switch (dir) {
		case UP:		move(DOWN, dis, false);				break;
		case DOWN:		move(UP, dis, false);				break;
		case RIGHT:		move(LEFT, dis, false);				break;
		case LEFT:		move(RIGHT, dis, false);			break;
		case UPRIGHT:	move(DOWNLEFT, dis, false);			break;
		case DOWNLEFT:	move(UPRIGHT, dis, false);			break;
		case UPLEFT:	move(DOWNRIGHT, dis, false);		break;
		case DOWNRIGHT:	move(UPLEFT, dis, false);			break;
		}
	}
}

int Entity::collision(Spell& s, std::vector<Entity>& obj) {
	for (size_t i = 0; i < obj.size(); i++) {
		if (Collision::seperateAxis(s.pos, obj[i].pos) == true) {
			s.hit();
			return i;
		}
	}
	return -1;
}

bool Entity::collision(Spell& s, std::vector<Tile>& tile) {
	for (size_t i = 0; i < tile.size(); i++) {
		if (Collision::rectCollision(s.pos, tile[i].pos) == true) {
			s.hit();
			return true;
		}
	}
	return false;
}

void Entity::applyDamage(const int& damage) {
	isCombat = true;
	combatFrame = 0;
	health -= damage;
	if (health <= 0) {
		health = 0;
		isDead = true;
	}
}

void Entity::applyDamage(const int& damage, std::vector<Text>& t, SDL_Color& color) {
	applyDamage(damage);
	int randX = rand() % Entity::RAND_DAMAGE_TEXT_X;
	int randY = rand() % Entity::RAND_DAMAGE_TEXT_Y;
	Text::printT(TEXT_DAMAGE, std::to_string(damage), { (pos.x + randX) - Entity::DAMAGE_TEXT_SPACEING_X, (pos.y - randY) - Entity::DAMAGE_TEXT_SPACEING_Y, NULL, Text::DAMAGE_SIZE_Y }, t, color);
}

int Entity::calcDamage(int d, int def, int lvl) {
	int ndamage = d + damage;
	ndamage -= def;
	int nleech = leech - lvl;
	int ndrain = drain - lvl;
	if (nleech > 0) {
		if (health < maxHealth) {
			health += nleech;
			if (health > maxHealth) {
				health = maxHealth;
			}
		}
	}
	if (ndrain > 0) {
		if (mana < maxMana) {
			mana += ndrain;
			if (mana > maxMana) {
				mana = maxMana;
			}
		}
	}

	if ((rand() % 100 <= luck - 1)) {
		ndamage = ndamage * 2;
	}

	return ndamage;
}

void Entity::castSpellMouse(int mX, int mY) {
	if (isCastable == true && mana >= spell.cost) {
		Options& options = options.Instance();
		Spell s = spell;

		s.pos.x = pos.x + posWidthHalf;
		s.pos.y = pos.y + posHeightHalf;
		s.x = float(s.pos.x);
		s.y = float(s.pos.y);
		s.dX = (options.camX + mX) - s.pos.x;
		s.dY = (options.camY + mY) - s.pos.y;
		s.speedX = s.dX * s.speed;
		s.speedY = s.dY * s.speed;
		s.disInc = abs(s.speedX) + abs(s.speedY);

		isCasting = true;
		castingAni.tock(0);
		castDir = calcCastDir(mX + options.camX, mY + options.camY);

		spells.push_back(s);

		isCastable = false;
		mana -= spell.cost;
	}
}

void Entity::castSpell(int nX, int nY) {
	if (isCastable == true && mana >= spell.cost) {
		Spell s = spell;

		s.pos.x = pos.x + posWidthHalf;
		s.pos.y = pos.y + posHeightHalf;
		s.x = float(s.pos.x);
		s.y = float(s.pos.y);
		s.dX = nX - s.pos.x;
		s.dY = nY - s.pos.y;
		s.speedX = s.dX * s.speed;
		s.speedY = s.dY * s.speed;
		s.disInc = abs(s.speedX) + abs(s.speedY);

		isCasting = true;
		castingAni.tock(0);
		castDir = calcCastDir(nX, nY);

		spells.push_back(s);

		isCastable = false;
		mana -= spell.cost;
	}
}

void Entity::addExp(int xp, TextBox& t) {
	exp += xp;
	while (exp >= 100) {
		Options& options = options.Instance();
		File file;
		if (options.lang == ENGLISH) {
			file.uread("Data/Messages/En/messages.txt");
		}
		else if (options.lang == RUSSIAN) {
			file.uread("Data/Messages/Ru/messages.txt");
		}
		
		t.print(file.getU16(1));
		t.isActive = true;

		level++;
		damage += 2;
		defense++;
		health += 10;
		maxHealth += 10;
		mana += 20;
		maxMana += 20;
		hps++;
		mps += 2;
		if (level % 5 == 0) {
			speed++;
			luck++;
		}
		if (level % 10 == 0) {
			leech++;
			drain++;
		}
		exp -= 100;
	}
	fireRate = spell.fireRate - level;
}

void Entity::dropLoot(std::vector<Item>& items) {
	Item temp;
	temp.Init(2);
	temp.pos.x = pos.x;
	temp.pos.y = pos.y;
	temp.damage = shards;
	items.push_back(temp);

	for (size_t i = 0; i < lootTable.size(); i++) {
		if (rand() % 100 <= lootTable[i].dropChance) {
			lootTable[i].pos.x = pos.x - 20 + rand() % 40;
			lootTable[i].pos.y = pos.y - 20 + rand() % 40;
			items.push_back(lootTable[i]);
		}
	}
}

int Entity::calcCastDir(int mX, int mY) {
	Options& options = options.Instance();
	int width = pos.x + pos.w;
	int height = pos.y + pos.h;

	if (mY <= pos.y && mX >= width) {
		return UPRIGHT;
	}
	if (mY >= height && mX >= width) {
		return DOWNRIGHT;
	}
	if (mX <= pos.x && mY <= pos.y) {
		return UPLEFT;
	}
	if (mX <= pos.x && mY >= height) {
		return DOWNLEFT;
	}
	if (mX > pos.x && mX < width && mY < pos.y) {
		return UP;
	}
	if (mY > pos.y && mY < height && mX > width) {
		return RIGHT;
	}
	if (mX > pos.x && mX < width && mY > height) {
		return DOWN;
	}
	if (mY > pos.y && mY < height && mX < pos.x) {
		return LEFT;
	}
	return UP;
}

std::vector<Item> Entity::loadLootTable(const int& id, const int& type) {
	File file;
	Item tempItem;
	std::string fileData = " ";
	std::string itemData = " ";
	std::vector<Item> lootTable;
	int fileDataLength = 0;
	int data = 0;
	int lootSize = 0;

	if (type == ENEMY) {
		file.read("Data/Entities/Enemies/" + findName(type, id) + ".txt");
	}
	else if (type == NPC) {
		file.read("Data/Entities/Npcs/" + findName(type, id) + ".txt");
	}
	else {
		file.read("Data/Entities/player.txt");
	}

	lootSize = file.getInt(38);
	fileData = file.getStr(39);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;

	for (int i = 0; i < lootSize; i++) {
		while (p <= fileDataLength && fileData[p] != ' ') {
			p++;
		}

		itemData = fileData.substr(q, p - q);
		data = std::stoi(itemData);

		q = p + 1;
		p = q;

		tempItem.Init(data);

		lootTable.push_back(tempItem);
	}

	return lootTable;
}

void Entity::loadPaths(int& size, std::string& fileData, const int& fileDataLength, int& p, int& q) {
	Path tempPath;
	std::string pathData = "";
	int data = 0;
	int dataType = 0;
	paths.clear();

	for (int i = 0; i < size; i++) {
		dataType = 0;
		while (dataType < PATH_LOAD_DATA_SIZE) {
			if (i == size - 1 && dataType == PATH_LOAD_DATA_SIZE - 1) {
				while (p <= fileDataLength && fileData[p] != ' ') {
					p++;
				}
			}
			else {
				while (p <= fileDataLength && fileData[p] != '|') {
					p++;
				}
			}


			pathData = fileData.substr(q, p - q);
			data = std::stoi(pathData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		tempPath.x = data;			break;
			case 1:		tempPath.y = data;			break;
			case 2:		tempPath.met = bool(data);	break;
			}

			dataType++;
		}

		paths.push_back(tempPath);
	}
	activePath = 0;
}

std::string Entity::findName(const int& type, const int& id) {
	switch (type) {
	case PLAYER:	return "player";	break;
	case ENEMY:
		switch (id) {
		case 1:		return "dummy";		break;
		case 2:		return "link";		break;
		}
	case NPC:
		switch (id) {
		case 1:		return "greg";		break;
		}
	}

	return "UNKOWN";
}