#include "quest.h"

void Quest::Init(const int& npcID) {
	Options& options = options.Instance();
	File file;
	File fileDialoge;
	file.read("Data/Quests/En/" + std::to_string(npcID) + ".txt");
	switch (options.lang) {
	case ENGLISH:
		fileDialoge.uread("Data/Quests/En/" + std::to_string(npcID) + "dialoge.txt");
		break;
	case RUSSIAN:
		fileDialoge.uread("Data/Quests/Ru/" + std::to_string(npcID) + "dialoge.txt");
		break;
	}
	size_t find = 0;

	id = file.getInt(1);
	type = file.getInt(2);
	name = file.getStr(3);
	count = 0;
	total = file.getInt(4);
	targetID = file.getInt(5);
	targetName = file.getStr(6);
	rewardID = file.getInt(7);
	isComplete = false;
	isActive = false;
	isEnd = false;
	question = fileDialoge.getU16(1);
	active = fileDialoge.getU16(2);
	activeCount = std::to_string(count) + " / " + std::to_string(total);
	complete = fileDialoge.getU16(3);
	npcName = fileDialoge.getU16(4);
	lines = file.getInt(8);
	line = 0;

	for (int i = 0; i < lines; i++) {
		dialoge.push_back(fileDialoge.getU16(i + 5));
	}
}

void Quest::addCount(const int& enemyID) {
	if (type == QUEST_KILL_COUNT && isComplete != true && isActive == true && targetID == enemyID) {
		count++;
		activeCount = std::to_string(count) + " / " + std::to_string(total);
	}
}

void Quest::nextLine() {
	line++;
	if (line > lines - 1) {
		line = 0;
	}
}