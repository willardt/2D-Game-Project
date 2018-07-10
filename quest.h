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
	std::vector<u16string> dialoge;

	int id;
	int type;
	int count;
	int total;
	int targetID;
	int rewardID;
	int line;
	int lines;
	bool isComplete;
	bool isActive;
	bool isEnd;

	void Init(const int& npcID);
	void update();
	void nextLine();
	void addCount(const int& enemyID);
};

#endif
