#include "ui.h"

const SDL_Color UI::ITEM_INFO_COLOR = { 25, 25, 25, 100 };

void UI::drawItemInfo(SDL_Rect& mouse, SDL_Rect size, u16string name, int& drop, std::vector<Text>& texts, SDL_Renderer* renderer) {
	Text::clear(texts);
	SDL_Rect infoName = { mouse.x + 30, mouse.y - 40, NULL, 30};
	Text textName = Text::printT(TEXT_DAMAGE, name, infoName, Item::getRarity(drop));
	texts.push_back(textName);
	SDL_Rect infoBox = { mouse.x + 15, mouse.y - 50, textName.pos.w + 30, 50};
	Texture::drawRectTrans(infoBox, ITEM_INFO_COLOR, renderer);
}

void UI::drawBar(SDL_Rect& rect, int height, const int& maxL, const int& l, SDL_Color front, SDL_Color back, SDL_Renderer* renderer) {
	float percent = 0;
	SDL_Rect newPos = { NULL, NULL, NULL, NULL };
	int width = rect.w + (BAR_SPACING_X * 2);

	if (maxL == 0) {
		percent = 0;
	}
	else {
		percent = (float(l) / float(maxL));
	}

	newPos = { rect.x - BAR_SPACING_X, rect.y - BAR_SPACING_Y - height, width, BAR_H };

	Texture::drawRect(newPos, back, renderer);
	newPos.w = newPos.w * percent;
	Texture::drawRect(newPos, front, renderer);
}