#include "Entity.h"

std::vector<Texture> Entity::memInit(const int& type) {
	File file;
	int total = 0;

	switch (type) {
	case ENEMY:		file.setPath("Data/Entities/Enemies/enemies.txt"); 
					total = Entity::TOTAL_ENEMY_TEXTURES;					 break;
	case NPC:		file.setPath("Data/Entities/Npcs/npcs.txt");
					total = Entity::TOTAL_NPC_TEXTURE;						 break;
	}

	std::vector<Texture> tempVec;
	Texture temp;

	for (int i = 1; i <= total; i++) {
		temp.setPath(file.readStr(i));
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
	case PLAYER:	file.setPath("Data/Entities/player.txt");				    break;
	case ENEMY:		file.setPath("Data/Entities/Enemies/" + path + ".txt");		break;
	case NPC:		file.setPath("Data/Entities/Npcs/" + path + ".txt");		break;
	}

	name = file.readStr(1);
	//	std::cout << texture.getPath() << std::endl;
	pos.w = file.readInt(3);
	pos.h = file.readInt(4);
	ani.maxFrame = file.readInt(5);
	ani.runFrame = file.readInt(6);
	ani.minDown = file.readInt(7);
	ani.maxDown = file.readInt(8);
	ani.minUp = file.readInt(9);
	ani.maxUp = file.readInt(10);
	ani.minRight = file.readInt(11);
	ani.maxRight = file.readInt(12);
	ani.minLeft = file.readInt(13);
	ani.maxLeft = file.readInt(14);
	level = file.readInt(15);
	exp = file.readInt(16);
	maxHealth = file.readInt(17);
	health = maxHealth;
	hps = file.readInt(18);
	maxMana = file.readInt(19);
	mana = maxMana;
	mps = file.readInt(20);
	damage = file.readInt(21);
	defense = file.readInt(22);
	leech = file.readInt(23);
	drain = file.readInt(24);
	luck = file.readInt(25);
	speed = file.readInt(26);
	shards = file.readInt(27);
	spellAni.totalFrames = file.readInt(28);
	spellAni.up = file.readInt(29);
	spellAni.down = file.readInt(30);
	spellAni.left = file.readInt(31);
	spellAni.right = file.readInt(32);
	spellAni.uright = file.readInt(33);
	spellAni.uleft = file.readInt(34);
	spellAni.dright = file.readInt(35);
	spellAni.dleft = file.readInt(36);
	spell.Init(file.readInt(37));

	sprite = Texture::updateSprite(pos, sprite.frame);

	regen.tock(0);
	casting.tock(0);

	isCasting = false;
	isCombat = false;
	isDead = false;
	isCastable = true;
}

void Entity::update() {
	//HEALTH BAR
	if (isCombat == true) {
		if (combatFrame > HEALTH_BAR_MAX_FRAME) {
			isCombat = false;
			combatFrame = 0;
		}
		combatFrame++;
	}

	//Regen
	updateRegen();

	//CASTING
	updateCasting();
	updateCastingAni();
	updateSpells();
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
	int fireRate = spell.fireRate - level;
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

void Entity::move(const int& dir, const int& ndis, const bool& update) {
	Options& options = options.Instance();
	int dis = ndis;
	int disHalf = 0;

	if (dis == NULL) {
		dis = speed;
	}

	switch (dir) {
	case UP:
		pos.y -= dis;

		if (type == PLAYER && (pos.y - options.camY) <= 0) {
			options.camY -= dis;
		}
		break;
	case DOWN:
		pos.y += dis;

		if (type == PLAYER && ((pos.y + pos.h) - options.camY) >= options.windowHeight) {
			options.camY += dis;
		}
		break;
	case LEFT:
		pos.x -= dis;

		if (type == PLAYER && (pos.x - options.camX) <= 0) {
			options.camX -= dis;
		}
		break;
	case RIGHT:
		pos.x += dis;

		if (type == PLAYER && ((pos.x + pos.w) - options.camX) >= options.windowWidth) {
			options.camX += dis;
		}
		break;
	case NULL:
		break;
	}

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
			options.camX = pos.x + (pos.w / 2) - (options.windowWidth / 2);
			options.camY = pos.y + (pos.h / 2) - (options.windowHeight / 2);

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
		if (isCasting == false) {
			updateSprite(MOVE, dir);
		}
	}
}

void Entity::updateSprite(const int& type, const int& dir) {
	sprite.frame++;

	if (type == NULL) {
		sprite.frame = 0;
	}
	else if (type == MOVE) {

		switch (dir) {
		case UP:
			if (sprite.frame < ani.minUp || sprite.frame > ani.maxUp) {
				sprite.frame = ani.minUp;
			}
			break;
		case DOWN:
			if (sprite.frame < ani.minDown || sprite.frame > ani.maxDown) {
				sprite.frame = ani.minDown;
			}
			break;
		case RIGHT:
			if (sprite.frame < ani.minRight || sprite.frame > ani.maxRight) {
				sprite.frame = ani.minRight;
			}
			break;
		case LEFT:
			if (sprite.frame < ani.minLeft || sprite.frame > ani.maxLeft) {
				sprite.frame = ani.minLeft;
			}
			break;
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
	else if (type == SPECIAL) {
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
			case UP:
				move(DOWN, dis, false);
				break;
			case DOWN:
				move(UP, dis, false);
				break;
			case RIGHT:
				move(LEFT, dis, false);
				break;
			case LEFT:
				move(RIGHT, dis, false);
				break;
			}
			break;
		}
	}

}

void Entity::collision(const int& dir, const int& dis, std::vector<Entity>& obj) {
	for (size_t i = 0; i < obj.size(); i++) {
		if (Collision::seperateAxis(pos, obj[i].pos) == true) {
			switch (dir) {
			case UP:
				move(DOWN, dis, false);
				break;
			case DOWN:
				move(UP, dis, false);
				break;
			case RIGHT:
				move(LEFT, dis, false);
				break;
			case LEFT:
				move(RIGHT, dis, false);
				break;
			}
			break;
		}
	}
}

void Entity::collision(const int& dir, const int& dis, std::vector<Tile>& tile) {
	for (size_t i = 0; i < tile.size(); i++) {
		if (Collision::rectCollision(pos, tile[i].pos) == true) {
			switch (dir) {
			case UP:
				move(DOWN, dis, false);
				break;
			case DOWN:
				move(UP, dis, false);
				break;
			case RIGHT:
				move(LEFT, dis, false);
				break;
			case LEFT:
				move(RIGHT, dis, false);
				break;
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
		case UP:
			move(DOWN, dis, false);
			break;
		case DOWN:
			move(UP, dis, false);
			break;
		case RIGHT:
			move(LEFT, dis, false);
			break;
		case LEFT:
			move(RIGHT, dis, false);
			break;
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
		isDead = true;
	}
}

void Entity::applyDamage(const int& damage, std::vector<Text>& t) {
	applyDamage(damage);
	int randX = rand() % Entity::RAND_DAMAGE_TEXT_X;
	int randY = rand() % Entity::RAND_DAMAGE_TEXT_Y;
	SDL_Rect newPos = { (pos.x + randX) - Entity::DAMAGE_TEXT_SPACEING_X, (pos.y - randY) - Entity::DAMAGE_TEXT_SPACEING_Y, NULL, Text::DAMAGE_SIZE_Y };
	Text::printT(TEXT_DAMAGE, std::to_string(damage), newPos, t);
}

int Entity::calcDamage(int d) {
	return d + damage;
}

void Entity::castSpell(int mX, int mY) {
	if (isCastable == true && mana >= spell.cost) {
		Options& options = options.Instance();
		Spell s;

		s.Init(spell.id);
		s.pos.x = pos.x + (pos.w / 2);
		s.pos.y = pos.y + (pos.h / 2);
		s.x = float(s.pos.x);
		s.y = float(s.pos.y);
		s.dX = (options.camX + mX) - s.pos.x;
		s.dY = (options.camY + mY) - s.pos.y;
		s.speedX = s.dX * s.speed;
		s.speedY = s.dY * s.speed;
		s.disInc = abs(s.speedX) + abs(s.speedY);

		isCasting = true;
		castDir = calcCastDir(mX, mY);

		spells.push_back(s);

		isCastable = false;
		mana -= spell.cost;
	}
}

int Entity::calcCastDir(int mX, int mY) {
	Options& options = options.Instance();
	int width = pos.x + pos.w;
	int height = pos.y + pos.h;
	mX += options.camX;
	mY += options.camY;

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