#include "texture.h"
#include "text.h"
#include "item.h"
#include "time.h"
#include "input.h"

#ifndef UI_H
#define UI_H

class UI {
private:
public:
	enum { BAR_SPACING_X = 8, BAR_SPACING_Y = 20, BAR_H = 8, PLAYER_BAR_H = 15, MAP_NAME_TIME = 60};

	const static SDL_Color ITEM_INFO_COLOR;
	const static SDL_Rect MAP_NAME;
	const static SDL_Rect PLAYER_HEALTH;
	const static SDL_Rect PLAYER_MANA;
	const static SDL_Rect SPELL_BOX;
	const static SDL_Rect SPELL_BOX_SPELL;
	const static SDL_Rect SECONDARY_BOX;

	Text mapName;
	Time mapNameTime;
	bool bmapName;

	static void drawItemInfo(SDL_Rect& mouse, u16string name, int& drop, std::vector<Text>& texts, SDL_Renderer* renderer);
	static void drawEnemyInfo(SDL_Rect& mouse, u16string name, int level, int health, int maxhealth, int mana, int maxMana, int damage, int defense, std::vector<Text>& texts, SDL_Renderer* renderer);
	static void drawBar(const SDL_Rect& rect, int height, const int& maxL, const int& l, SDL_Color front, SDL_Color back, SDL_Renderer* renderer);
	static void drawBarNoCam(const SDL_Rect& rect, int height, const int& maxL, const int& l, SDL_Color front, SDL_Color back, SDL_Renderer* renderer);
	void setMap(u16string name);
	void drawMapName(SDL_Renderer* renderer);
};
#endif