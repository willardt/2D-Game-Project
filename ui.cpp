#include "ui.h"

const SDL_Color UI::ITEM_INFO_COLOR = { 25, 25, 25, 100 };
const SDL_Rect UI::MAP_NAME = { 600, 0, 400, 30 };
const SDL_Rect UI::PLAYER_HEALTH = { 25, 50, 300, 50 };
const SDL_Rect UI::PLAYER_MANA = { 25, 70, 300, 50 };
const SDL_Rect UI::SPELL_BOX = { 7, 825, 50, 50 };
const SDL_Rect UI::SPELL_BOX_SPELL = { 32, 740, NULL, NULL };
const SDL_Rect UI::SECONDARY_BOX = { 73, 825, 50, 50 };

void UI::drawItemInfo(SDL_Rect& mouse, u16string name, int& drop, std::vector<Text>& texts, SDL_Renderer* renderer) {
	Text::clear(texts);
	SDL_Rect infoName = { mouse.x + 30, mouse.y - 40, NULL, 30};
	Text textName = Text::printT(TEXT_DAMAGE, name, infoName, Item::getRarity(drop));
	texts.push_back(textName);
	SDL_Rect infoBox = { mouse.x + 15, mouse.y - 50, textName.pos.w + 30, 50};
	Texture::drawRectTrans(infoBox, ITEM_INFO_COLOR, renderer);
}

void UI::drawEnemyInfo(SDL_Rect& mouse, u16string name, int level, int health, int maxHealth, int mana, int maxMana, int damage, int defense, std::vector<Text>& texts, SDL_Renderer* renderer) {
	Options& options = options.Instance();
	File infoFile;
	if (options.lang == ENGLISH) {
		infoFile.uread("Data/UI/Bag/en.txt");
	}
	else if (options.lang == RUSSIAN) {
		infoFile.uread("Data/UI/Bag/ru.txt");
	}

	Text::clear(texts);

	SDL_Rect infoName = { mouse.x + 30, mouse.y - 40, NULL, 30 };
	Text textName = Text::printT(TEXT_DAMAGE, name, infoName, Text::WHITE);
	texts.push_back(textName);

	SDL_Rect infoLevel = { infoName.x, mouse.y - 10, NULL, 30 };
	Text textLevel = Text::printT(TEXT_DAMAGE, infoFile.getU16(4), infoLevel, Text::LEVEL);
	texts.push_back(textLevel);
	SDL_Rect infoLevelNum = { infoLevel.x + textLevel.pos.w, infoLevel.y, NULL, infoLevel.h };
	Text textLevelNum = Text::printT(TEXT_DAMAGE, std::to_string(level), infoLevelNum, Text::LEVEL);
	texts.push_back(textLevelNum);

	SDL_Rect infoHealth = { infoName.x, mouse.y + 20, NULL, 30 };
	Text textHealth = Text::printT(TEXT_DAMAGE, infoFile.getU16(6), infoHealth, Text::HEALTH);
	texts.push_back(textHealth);
	SDL_Rect infoHealthNum = { infoHealth.x + textHealth.pos.w, infoHealth.y, NULL, infoHealth.h };
	Text textHealthNum = Text::printT(TEXT_DAMAGE, std::to_string(health) + "/" + std::to_string(maxHealth), infoHealthNum, Text::HEALTH);
	texts.push_back(textHealthNum);

	SDL_Rect infoMana = { infoName.x, mouse.y + 50, NULL, 30 };
	Text textMana = Text::printT(TEXT_DAMAGE, infoFile.getU16(8), infoMana, Text::MANA);
	texts.push_back(textMana);
	SDL_Rect infoManaNum = { infoMana.x + textMana.pos.w, infoMana.y, NULL, infoMana.h };
	Text textManaNum = Text::printT(TEXT_DAMAGE, std::to_string(mana) + "/" + std::to_string(maxMana), infoManaNum, Text::MANA);
	texts.push_back(textManaNum);

	SDL_Rect infoDamage = { infoName.x, mouse.y + 80, NULL, 30 };
	Text textDamage = Text::printT(TEXT_DAMAGE, infoFile.getU16(10), infoDamage, Text::DAMAGE);
	texts.push_back(textDamage);
	SDL_Rect infoDamageNum = { infoDamage.x + textDamage.pos.w, infoDamage.y, NULL, infoDamage.h };
	Text textDamageNum = Text::printT(TEXT_DAMAGE, std::to_string(damage), infoDamageNum, Text::DAMAGE);
	texts.push_back(textDamageNum);

	SDL_Rect infoDefense = { infoName.x, mouse.y + 110, NULL, 30 };
	Text textDefense = Text::printT(TEXT_DAMAGE, infoFile.getU16(11), infoDefense, Text::DEFENSE);
	texts.push_back(textDefense);
	SDL_Rect infoDefenseNum = { infoDefense.x + textDefense.pos.w, infoDefense.y, NULL, infoDefense.h };
	Text textDefenseNum = Text::printT(TEXT_DAMAGE, std::to_string(defense), infoDefenseNum, Text::DEFENSE);
	texts.push_back(textDefenseNum);
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