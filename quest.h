#include <string>

#include "file.h"
#include "options.h"

#ifndef QUEST_H
#define QUEST_H

#define QUEST_KILL_COUNT 1
#define QUEST_TALK 2

class Quest {
private:
public:
	std::string name;
	std::string targetName;
	std::string activeCount;
	u16string npcName;
	u16string question;
	u16string active;
	u16string complete;

	int id;
	int type;
	int count;
	int total;
	int targetID;
	bool isComplete;
	bool isActive;
	bool isEnd;

	void Init(const int& npcID);
	void update();
	void addCount(const int& enemyID);
};

#endif
