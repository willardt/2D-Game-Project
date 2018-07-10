#include "Entity.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player : public Entity {
private:
	enum { QUEST_LOAD_DATA_SIZE = 1};
public:
	enum { EQUIPPED_SIZE = 10, MAX_BAG_SIZE = 359, SPELLBOOK_SIZE = 12};
	std::vector<Quest> quests;
	std::vector<Item> items;
	std::vector<Item> buffs;

	Item equipped[EQUIPPED_SIZE];
	Spell spellBook[SPELLBOOK_SIZE];

	int currentSpell;
	int currentSecondary;

	void playerUpdate();

	void pickupShards(std::vector<Item>& items, std::vector<Text>& texts);

	// Return postion of quest in the vector
	int findQuestNpc(const int& npcID);
	int findQuestTarget(const int& enemyID);

	void save(const int& mapID);

	void loadItems();
	void loadQuests();

	void equipItem(const int& index);
	void unequipItem(const int& slot);
	void useItem(const int& index);

	void addBuff(Item& i);
	void removeBuff(int index);
	void updateBuffs();

	void selectSpell(int index);
	void selectSecondary(int index);
	void swapSpells(Spell& s1, Spell& s2);
	void addSpell(int spellID, TextBox& t);

	bool spellCollision(Spell& s);

};
#endif