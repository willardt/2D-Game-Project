#include "ui.h"

const SDL_Color UI::ITEM_INFO_COLOR = { 25, 25, 25, 100 };
const SDL_Rect UI::MAP_NAME = { 600, 0, 400, 30 };
const SDL_Rect UI::PLAYER_HEALTH = { 25, 50, 300, 50 };
const SDL_Rect UI::PLAYER_MANA = { 25, 70, 300, 50 };
const SDL_Rect UI::SPELL_BOX = { 7, 825, 50, 50 };
const SDL_Rect UI::SPELL_BOX_SPELL = { 32, 740, NULL, NULL };
const SDL_Rect UI::SECONDARY_BOX = { 73, 825, 50, 50 };

void UI::drawItemInfo(SDL_Rect& mouse, SDL_Rect size, u16string name, int& drop, std::vector<Text>& texts, SDL_Renderer* renderer) {
	Text::clear(texts);
	SDL_Rect infoName = { mouse.x + 30, mouse.y - 40, NULL, 30};
	Text textName = Text::printT(TEXT_DAMAGE, name, infoName, Item::getRarity(drop));
	texts.push_back(textName);
	SDL_Rect infoBox = { mouse.x + 15, mouse.y - 50, textName.pos.w + 30, 50};
	Texture::drawRectTrans(infoBox, ITEM_INFO_COLOR, renderer);
}

void UI::drawBar(const SDL_Rect& rect, int height, const int& maxL, const int& l, SDL_Color front, SDL_Color back, SDL_Renderer* renderer) {
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

void UI::drawBarNoCam(const SDL_Rect& rect, int height, const int& maxL, const int& l, SDL_Color front, SDL_Color back, SDL_Renderer* renderer) {
	float percent = 0;
	SDL_Rect newPos = { NULL, NULL, NULL, NULL };
	int width = rect.w + (BAR_SPACING_X * 2);

	if (maxL == 0) {
		percent = 0;
	}
	else {
		percent = (float(l) / float(maxL));
	}

	newPos = { rect.x - BAR_SPACING_X, rect.y - BAR_SPACING_Y - height, width, PLAYER_BAR_H };

	Texture::drawRectNoCam(newPos, back, renderer);
	newPos.w = newPos.w * percent;
	Texture::drawRectNoCam(newPos, front, renderer);;
}

void UI::setMap(u16string uname) {
	mapName = Text::printT(TEXT_BOX, uname, MAP_NAME, Text::WHITE);
	bmapName = true;
	mapNameTime.tock(0);

	for (int i = 0; i < uname.size(); i++) {
		if (uname[i] == '_') {
			uname[i] = ' ';
		}
	}
	mapName.pos.x += (MAP_NAME.w - mapName.pos.w) / 2;
}

void UI::drawMapName(SDL_Renderer* renderer) {
	if (bmapName) {
		if (!mapNameTime.tick(MAP_NAME_TIME)) {
			Texture::drawRectTransNoCam(MAP_NAME, Text::BLACK, renderer);
			mapName.renderNoCam(renderer);
		}
		else {
			bmapName = false;
			mapNameTime.tock(0);
			mapName.destroy();
		}
	}
}