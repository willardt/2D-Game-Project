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
	//Returns true if complete
	bool promptQuest(Quest& q, TextBox& t);

	void collision(const int& dir, const int& dis, std::vector <Npc>& npcs, const int& selfIndex);

	static int collisionNPCSpeech(std::vector<Npc>& objs, SDL_Rect& pos);

};

#endif