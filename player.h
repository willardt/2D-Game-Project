#include "Entity.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player : public Entity {
private:
	enum { QUEST_LOAD_DATA_SIZE = 1};
public:
	enum { EQUIPPED_SIZE = 10, MAX_BAG_SIZE = 200};
	std::vector<Quest> quests;
	std::vector<Item> items;

	Item equipped[EQUIPPED_SIZE];

	void playerUpdate();

	// Return postion of quest in the vector
	int findQuestNpc(const int& npcID);
	int findQuestTarget(const int& enemyID);

	void save(const int& mapID);

	void loadItems();
	void loadQuests();

	void equipItem(const int& index);
	void unequipItem(const int& slot);

	bool spellCollision(Spell& s);
};
#endif