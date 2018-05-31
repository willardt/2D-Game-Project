#include "npc.h"

void Npc::speech(TextBox& t, std::string s) {
	t.isActive = true;
	t.print(s);
}

void Npc::speech(TextBox& t, u16string s) {
	t.isActive = true;
	t.print(s);
}

void Npc::promptQuest(Quest& q, TextBox& t) {
	t.setSpeaker(q.npcName, Text::WHITE);
	if (quest.isEnd != true) {
		if (q.isActive == false) {
			q.isActive = true;
			speech(t, q.question);
		}
		else if (q.isActive == true && q.isComplete == false) {
			speech(t, q.activeCount);
			speech(t, q.active);
		}
		else if (q.isActive == true && q.isComplete == true) {
			speech(t, q.complete);
			q.isEnd = true;
		}
	}
}

int Npc::collisionNPCSpeech(std::vector<Npc>& objs, SDL_Rect& pos) {
	for (size_t i = 0; i < objs.size(); i++) {
		SDL_Rect newPos = { objs[i].pos.x - (Entity::NPCSRC / 2), objs[i].pos.y - (Entity::NPCSRC / 2), objs[i].pos.w + Entity::NPCSRC, objs[i].pos.h + Entity::NPCSRC };
		if (Collision::seperateAxis(pos, newPos) == true) {
			return int(i);
		}
	}
	return -1;
}

void Npc::NPCInit() {
	File file;
	file.setPath("Data/Entities/Npcs/quests.txt");

	if (file.readInt(id) == 1) {
		hasQuest = true;
		quest.Init(id);
	}
	else {
		hasQuest = false;
	}

}