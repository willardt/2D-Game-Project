#include "player.h"

void Player::playerUpdate() {

	// UPDATE QUESTS
	for (size_t i = 0; i < quests.size(); i++) {
		if (quests[i].isEnd == true) {
			quests.erase(quests.begin() + i);
		}
	}

	//Regen
	updateRegen();

	//CASTING
	updateCastingAni();
	updateCasting();
	updateSpells();
	updateBuffs();
}

void Player::pickupShards(std::vector<Item>& items, std::vector<Text>& texts) {
	int q = collision(items);
	if (q != -1 && items[q].id == 2) {
		shards += items[q].damage;
		Text::printT(TEXT_DAMAGE, "+" + std::to_string(items[q].damage), { pos.x + 50, pos.y - 50, NULL, 25 }, texts, Text::PURPLE);
		items.erase(items.begin() + q);
	}
}

void Player::equipItem(const int& index) {
	int slot = items[index].type;
	if (equipped[slot].isEquipped == true && equipped[slot].id != -1) {
		unequipItem(slot);
	}
	equipped[slot] = items[index];
	items.erase(items.begin() + index);

	maxHealth += equipped[slot].health;
	hps += equipped[slot].hps;
	maxMana += equipped[slot].mana;
	mps += equipped[slot].mps;
	damage += equipped[slot].damage;
	defense += equipped[slot].defense;
	leech += equipped[slot].leech;
	drain += equipped[slot].drain;
	luck += equipped[slot].luck;
	speed += equipped[slot].speed;

	equipped[slot].isEquipped = true;
}

void Player::unequipItem(const int& slot) {
	if (equipped[slot].isEquipped == true) {
		maxHealth -= equipped[slot].health;
		if (health > maxHealth) {
			health = maxHealth;
		}
		hps -= equipped[slot].hps;
		maxMana -= equipped[slot].mana;
		if (mana > maxMana) {
			mana = maxMana;
		}
		mps -= equipped[slot].mps;
		damage -= equipped[slot].damage;
		defense -= equipped[slot].defense;
		leech -= equipped[slot].leech;
		drain -= equipped[slot].drain;
		luck -= equipped[slot].luck;
		speed -= equipped[slot].speed;

		items.push_back(equipped[slot]);

		equipped[slot].isEquipped = false;
		equipped[slot].id = -1;
	}
}

void Player::useItem(const int& index) {
	if (index > items.size()) {
		return;
	}

	damage += items[index].damage;
	defense += items[index].defense;
	health += items[index].health;
	if (health > maxHealth) {
		health = maxHealth;
	}
	maxHealth += items[index].maxHealth;
	hps += items[index].hps;
	mana += items[index].mana;
	if (mana > maxMana) {
		mana = maxMana;
	}
	maxMana += items[index].maxMana;
	mps += items[index].mps;
	leech += items[index].leech;
	drain += items[index].drain;
	luck += items[index].luck;
	speed += items[index].speed;

	if (items[index].duration == 0) {
		items.erase(items.begin() + index);
	}
	else {
		items[index].pos.x = 50;
		items[index].pos.y = 200;
		items[index].pos.w = 15;
		items[index].pos.h = 15;
		buffs.push_back(items[index]);
		items.erase(items.begin() + index);
	}
}

void Player::removeBuff(int index) {
	damage -= buffs[index].damage;
	defense -= buffs[index].defense;
	maxHealth -= buffs[index].maxHealth;
	if (health > maxHealth) {
		health = maxHealth;
	}
	maxMana -= buffs[index].maxMana;
	if (mana > maxMana) {
		mana = maxMana;
	}
	hps -= buffs[index].hps;
	mps -= buffs[index].mps;
	leech -= buffs[index].leech;
	drain -= buffs[index].drain;
	luck -= buffs[index].luck;
	speed -= buffs[index].speed;

	buffs.erase(buffs.begin() + index);
}

void Player::updateBuffs() {
	for (size_t i = 0; i < buffs.size(); i++) {
		buffs[i].cduration--;
		if (buffs[i].cduration <= 0) {
			removeBuff(i);
		}
	}
}

void Player::selectSpell(int index) {
	if (spellBook[index].id != -1) {
		currentSpell = index;
		Entity::spell = spellBook[currentSpell];
	}
}

void Player::selectSecondary(int index) {
	if (spellBook[index].id != -1) {
		currentSecondary = index;
		Entity::secondary = spellBook[currentSecondary];
	}
}

void Player::swapSpells(Spell& s1, Spell& s2) {
	Spell temp = s1;
	s1 = s2;
	s2 = temp;
}

void Player::addSpell(int spellID, TextBox& t) {
	Spell s;
	s.Init(spellID);
	int i = 0;
	while (spellBook[i].id != -1) {
		i++;
	}

	if (i > Player::SPELLBOOK_SIZE) {
		std::cout << "Exceeded spellbook size" << std::endl;
	}
	else {
		spellBook[i] = s;
		t.print(MESSAGE_NEWSPELL + s.uname, s.color);
	}
}

int Player::findQuestNpc(const int& npcID) {
	for (size_t i = 0; i < quests.size(); i++) {
		if (quests[i].id == npcID) {
			return i;
		}
	}
	return -1;
}

int Player::findQuestTarget(const int& enemyID) {
	for (size_t i = 0; i < quests.size(); i++) {
		if (quests[i].targetID == enemyID) {
			return i;
		}
	}
	return -1;
}

bool Player::spellCollision(Spell& s) {
	if (Collision::seperateAxis(s.pos, pos) == true) {
		s.hit();
		return true;
	}
	return false;
}

void Player::loadItems() {
	File file;
	Item tempItem;
	std::string fileData = " ";
	std::string itemData = " ";
	int fileDataLength = 0;
	int itemSize = 0;
	int data = 0;

	file.read("Data/player.txt");
	std::cout << "Loading Items" << std::endl;
	items.clear();

	itemSize = file.getInt(6);
	fileData = file.getStr(7);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	
	for (int i = 0; i < itemSize; i++) {
		while (fileData[p] != ' ' && p < fileDataLength) {
			p++;
		}

		itemData = fileData.substr(q, p - q);
		data = std::stoi(itemData);

		q = p + 1;
		p = q;

		tempItem.Init(data);

		items.push_back(tempItem);
	}

	fileData = file.getStr(23);
	fileDataLength = int(fileData.length());

	p = 0;
	q = 0;

	for (int i = 0; i < EQUIPPED_SIZE; i++) {
		while (fileData[p] != ' ' && p < fileDataLength) {
			p++;
		}

		itemData = fileData.substr(q, p - q);
		data = std::stoi(itemData);

		q = p + 1;
		p = q;

		tempItem.Init(data);
		tempItem.isEquipped = true;

		equipped[i] = tempItem;
	}

	std::cout << "Items Loaded" << std::endl;
}

void Player::loadQuests() {
	File file;
	Quest tempQuest;
	std::string fileData = " ";
	std::string questData = " ";
	int fileDataLength = 0;
	int questSize = 0;
	int data = 0;
	file.read("Data/player.txt");
	std::cout << "Loading Quests" << std::endl;
	quests.clear();

	questSize = file.getInt(4);
	fileData = file.getStr(5);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;

	for (int i = 0; i < questSize; i++) {
		dataType = 0;
		while (dataType <= Player::QUEST_LOAD_DATA_SIZE) {
			if (dataType < Player::QUEST_LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}

			}
			else {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			questData = fileData.substr(q, p - q);
			data = std::stoi(questData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		tempQuest.id = data;
						tempQuest.Init(tempQuest.id);
						break;
			case 1:		tempQuest.count = data;	
						tempQuest.activeCount = std::to_string(tempQuest.count) + " / " + std::to_string(tempQuest.total);
						break;
			}

			dataType++;
		}

		tempQuest.isActive = true;
		quests.push_back(tempQuest);

	}

	std::cout << "Quests Loaded" << std::endl;
}

void Player::loadBuffs() {
	File file;
	Item tempBuff;
	std::string fileData = " ";
	std::string buffData = " ";
	int fileDataLength = 0;
	int buffSize = 0;
	int data = 0;
	file.read("Data/player.txt");
	std::cout << "Loading Buffs" << std::endl;
	buffs.clear();

	buffSize = file.getInt(39);
	fileData = file.getStr(40);
	fileDataLength = int(fileData.length());

	int p = 0;
	int q = 0;
	int dataType = 0;

	for (int i = 0; i < buffSize; i++) {
		dataType = 0;
		while (dataType <= Player::BUFF_LOAD_DATA_SIZE) {
			if (dataType < Player::BUFF_LOAD_DATA_SIZE) {
				while (fileData[p] != '|' && p <= fileDataLength) {
					p++;
				}

			}
			else {
				while (fileData[p] != ' ' && p <= fileDataLength) {
					p++;
				}
			}

			buffData = fileData.substr(q, p - q);
			data = std::stoi(buffData);

			q = p + 1;
			p = q;

			switch (dataType) {
			case 0:		
				tempBuff.id = data;
				tempBuff.Init(tempBuff.id);
				break;
			case 1:		
				tempBuff.cduration = data;
				break;
			}

			dataType++;
		}

		damage += tempBuff.damage;
		defense += tempBuff.defense;
		health += tempBuff.health;
		if (health > maxHealth) {
			health = maxHealth;
		}
		maxHealth += tempBuff.maxHealth;
		hps += tempBuff.hps;
		mana += tempBuff.mana;
		if (mana > maxMana) {
			mana = maxMana;
		}
		maxMana += tempBuff.maxMana;
		mps += tempBuff.mps;
		leech += tempBuff.leech;
		drain += tempBuff.drain;
		luck += tempBuff.luck;
		speed += tempBuff.speed;

		tempBuff.pos.x = 50;
		tempBuff.pos.y = 200;
		tempBuff.pos.w = 15;
		tempBuff.pos.h = 15;

		buffs.push_back(tempBuff);

	}

	std::cout << "Buffs Loaded" << std::endl;
}

void Player::save(const int& mapID) {
	std::ofstream file;
	file.open("Data/player.txt");
	file << "Name - " << "Will" << std::endl;
	file << "Position.x - " << pos.x << std::endl;
	file << "Position.y - " << pos.y << std::endl;

	file << "Quest Size - " << quests.size() << std::endl;
	file << "Quests - ";
	// ----- Quests -----
	for (size_t i = 0; i < quests.size(); i++) {
		file << std::to_string(quests[i].id) + "|" + std::to_string(quests[i].count) + " ";
	}
	file << std::endl;

	file << "Item Size - " << items.size() << std::endl;
	file << "Items - ";
	for (size_t i = 0; i < items.size(); i++) {
		file << std::to_string(items[i].id) + " ";
	}
	file << std::endl;

	for (int i = 0; i < EQUIPPED_SIZE; i++) {
		if (equipped[i].isEquipped == true && equipped[i].id != -1) {
			maxHealth -= equipped[i].health;
			hps -= equipped[i].hps;
			maxMana -= equipped[i].mana;
			mps -= equipped[i].mps;
			damage -= equipped[i].damage;
			defense -= equipped[i].defense;
			leech -= equipped[i].leech;
			drain -= equipped[i].drain;
			luck -= equipped[i].luck;
			speed -= equipped[i].speed;
		}
	}

	for (size_t i = 0; i < buffs.size(); i++) {
		damage -= buffs[i].damage;
		defense -= buffs[i].defense;
		maxHealth -= buffs[i].maxHealth;
		if (health > maxHealth) {
			health = maxHealth;
		}
		maxMana -= buffs[i].maxMana;
		if (mana > maxMana) {
			mana = maxMana;
		}
		hps -= buffs[i].hps;
		mps -= buffs[i].mps;
		leech -= buffs[i].leech;
		drain -= buffs[i].drain;
		luck -= buffs[i].luck;
		speed -= buffs[i].speed;
	}

	file << "Level - " << level << std::endl;
	file << "Exp - " << exp << std::endl;
	file << "Max Health - " << maxHealth << std::endl;
	file << "Health - " << health << std::endl;
	file << "Health Regen - " << hps << std::endl;
	file << "Max Mana - " << maxMana << std::endl;
	file << "Mana - " << mana << std::endl;
	file << "Mana Regen - " << mps << std::endl;
	file << "Damage - " << damage << std::endl;
	file << "Defense - " << defense << std::endl;
	file << "Leech - " << leech << std::endl;
	file << "Drain - " << drain << std::endl;
	file << "Luck - " << luck << std::endl;
	file << "Speed - " << speed << std::endl;
	file << "Shards - " << shards << std::endl;

	for (int i = 0; i < EQUIPPED_SIZE; i++) {
		if (equipped[i].isEquipped == true && equipped[i].id != -1) {
			maxHealth += equipped[i].health;
			hps += equipped[i].hps;
			maxMana += equipped[i].mana;
			mps += equipped[i].mps;
			damage += equipped[i].damage;
			defense += equipped[i].defense;
			leech += equipped[i].leech;
			drain += equipped[i].drain;
			luck += equipped[i].luck;
			speed += equipped[i].speed;
		}
	}

	for (size_t i = 0; i < buffs.size(); i++) {
		damage += buffs[i].damage;
		defense += buffs[i].defense;
		maxHealth += buffs[i].maxHealth;
		if (health > maxHealth) {
			health = maxHealth;
		}
		maxMana += buffs[i].maxMana;
		if (mana > maxMana) {
			mana = maxMana;
		}
		hps += buffs[i].hps;
		mps += buffs[i].mps;
		leech += buffs[i].leech;
		drain += buffs[i].drain;
		luck += buffs[i].luck;
		speed += buffs[i].speed;
	}

	file << "Equipped - ";
	for (int i = 0; i < Player::EQUIPPED_SIZE; i++) {
		file << std::to_string(equipped[i].id) + " ";
	}
	file << std::endl;

	file << "Map - " << mapID << std::endl;

	file << "Selected Spell - " << currentSpell << std::endl;
	file << "Selected Secondary - " << currentSecondary << std::endl;

	for (int i = 0; i < SPELLBOOK_SIZE; i++) {
		file << "Spell " + std::to_string(i) + "- " << spellBook[i].id << std::endl;
	}

	file << "Buffs Size - " << int(buffs.size()) << std::endl;
	file << "Buffs - ";
	for (size_t i = 0; i < buffs.size(); i++) {
		file << std::to_string(buffs[i].id) + "|" + std::to_string(buffs[i].cduration) + " ";
	}

	file.close();
}