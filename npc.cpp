#include "npc.h"

void Npc::speech(TextBox& t, std::string s) {
	t.print(s);
}

void Npc::speech(TextBox& t, u16string s) {
	t.print(s);
}

bool Npc::promptQuest(Quest& q, TextBox& t) {
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
			return true;
		}
	}
	return false;
}

void Npc::promptDialoge(TextBox& t) {
	if (quest.lines > 0) {
		t.setSpeaker(quest.npcName, Text::WHITE);
		speech(t, quest.dialoge[quest.line]);
		quest.nextLine();
	}
}

void Npc::collision(const int& dir, const int& dis, std::vector<Npc>& npcs, const int& selfIndex) {
	int length = int(npcs.size());
	for (int i = 0; i < length; i++) {
		if (i == selfIndex) {
			i++;
			if (i >= length) {
				break;
			}
		}

		if (Collision::seperateAxis(pos, npcs[i].pos) == true) {
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
	file.read("Data/Entities/Npcs/quests.txt");

	hasQuest = bool(file.getInt(id + 1));
	quest.Init(id);
}