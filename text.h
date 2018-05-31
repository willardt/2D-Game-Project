#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "options.h"

#ifndef TEXT_H
#define TEXT_H

// Text types, affects font / color if undefined
#define TEXT_NORMAL 1
#define TEXT_DAMAGE 2
#define TEXT_BOX 3

class Text {
private:
	enum { _PTSIZE = 30, _OUTLINE = 2 };
	const static std::string _NORMAL_FONT_PATH;
	const static std::string _DAMAGE_FONT_PATH;
	const static std::string _RUSSIAN_FONT_PATH;

	std::string _text;
	u16string _utext;
public:
	enum { TEXT_SPACING_W = 20, DAMAGE_SIZE_Y = 30, NORMAL_SIZE_Y = 30};
	enum { MAX_NORMAL_FRAME = 300, MAX_DAMAGE_FRAME = 50 };

	static TTF_Font* NORMAL_FONT() {
		static TTF_Font* f = TTF_OpenFont(_NORMAL_FONT_PATH.c_str(), _PTSIZE);
		return f;
	}
	static TTF_Font* DAMAGE_FONT() {
		static TTF_Font* f = TTF_OpenFont(_DAMAGE_FONT_PATH.c_str(), _PTSIZE);
		return f;
	}
	static TTF_Font* RUSSIAN_FONT() {
		static TTF_Font* f = TTF_OpenFont(_RUSSIAN_FONT_PATH.c_str(), _PTSIZE);
		return f;
	}

	const static SDL_Color RED;
	const static SDL_Color BLUE;
	const static SDL_Color GREEN;
	const static SDL_Color BLACK;
	const static SDL_Color WHITE;
	const static SDL_Color NORMAL_COLOR;
	const static SDL_Color DAMAGE_COLOR;

	SDL_Texture* texture;  // Texture of the text with basic font
	SDL_Texture* oTexture;  // Outline texture

	SDL_Rect pos;
	SDL_Color color;

	bool isLoaded;
	bool isEnd;
	bool isUnicode;

	int type;
	int frame;

	bool load(std::string text, SDL_Renderer* renderer);
	bool load(u16string utext, SDL_Renderer* renderer);
	bool load(SDL_Renderer* renderer);
	bool render(SDL_Rect& rect, SDL_Renderer* renderer);
	bool render(SDL_Renderer* renderer);
	bool renderNoCam(SDL_Renderer* renderer);
	bool renderOutline(SDL_Rect& rect, SDL_Renderer* renderer);
	bool renderOutline(SDL_Renderer* renderer);

	void update();

	void setText(std::string text);
	void setUText(u16string utext);
	void setSpacing();
	void destroy();
	std::string getText();
	u16string getUText();
	TTF_Font* getFontType();

	static void printT(const int& ntype, std::string text, SDL_Rect npos, std::vector<Text>& texts);
	static void printT(const int& ntype, std::string text, SDL_Rect npos, std::vector<Text>& texts, SDL_Color ncolor);
	static void printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts);
	static void printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts, SDL_Color ncolor);
	static Text printT(const int& ntype, std::string text, SDL_Rect npos, SDL_Color ncolor);
	static Text printT(const int& ntype, u16string utext, SDL_Rect npos, SDL_Color ncolor);
};



#endif