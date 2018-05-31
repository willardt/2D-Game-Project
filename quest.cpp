#include "quest.h"

void Quest::Init(const int& npcID) {
	Options& options = options.Instance();
	File file;
	File fileDialoge;
	file.setPath("Data/Quests/En/" + std::to_string(npcID) + ".txt");
	switch (options.lang) {
	case ENGLISH:
		fileDialoge.setPath("Data/Quests/En/" + std::to_string(npcID) + "dialoge.txt");
		break;
	case RUSSIAN:
		fileDialoge.setPath("Data/Quests/Ru/" + std::to_string(npcID) + "dialoge.txt");
		break;
	}
	size_t find = 0;

	id = file.readInt(1);
	type = file.readInt(2);
	name = file.readStr(3);
	count = 0;
	total = file.readInt(4);
	targetID = file.readInt(5);
	targetName = file.readStr(6);
	isComplete = false;
	isActive = false;
	isEnd = false;
	question = fileDialoge.readUnicodeStr(1);
	active = fileDialoge.readUnicodeStr(2);
	activeCount = std::to_string(count) + " / " + std::to_string(total);
	complete = fileDialoge.readUnicodeStr(3);
	npcName = fileDialoge.readUnicodeStr(4);
}

void Quest::addCount(const int& enemyID) {
	if (type == QUEST_KILL_COUNT && isComplete != true && isActive == true && targetID == enemyID) {
		count++;
		activeCount = std::to_string(count) + " / " + std::to_string(total);
		if (count == total) {
			isComplete = true;
		}
	}
}