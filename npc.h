#include "Entity.h"

#ifndef NPC_H
#define NPC_H

class Npc : public Entity {
private:
public:
	Quest quest;

	bool hasQuest;

	void NPCInit();
	void speech(TextBox& t, std::string s);
	void speech(TextBox& t, u16string s);
	void promptQuest(Quest& q, TextBox& t);

	static int collisionNPCSpeech(std::vector<Npc>& objs, SDL_Rect& pos);
};

#endif