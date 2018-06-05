#include "texture.h"
#include "text.h"
#include "item.h"

#ifndef UI_H
#define UI_H

class UI {
private:
public:
	enum { BAR_SPACING_X = 8, BAR_SPACING_Y = 20, BAR_H = 8 };

	const static SDL_Color ITEM_INFO_COLOR;

	static void drawItemInfo(SDL_Rect& mouse, SDL_Rect size, u16string name, int& drop, std::vector<Text>& texts, SDL_Renderer* renderer);
	static void drawBar(SDL_Rect& rect, int height, const int& maxL, const int& l, SDL_Color front, SDL_Color back, SDL_Renderer* renderer);
};
#endif