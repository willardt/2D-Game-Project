#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "options.h"

#ifndef TEXT_H
#define TEXT_H

// Text types, affects font / color if undefined
#define TEXT_NORMAL 0
#define TEXT_DAMAGE 1
#define TEXT_BOX 2

class Text {
private:
	enum { _PTSIZE = 50, _OUTLINE = 2 };
	const static std::string FONT_PATH;

	std::string _text;
	u16string _utext;
public:
	enum { TEXT_SPACING_W = 20, DAMAGE_SIZE_Y = 30, NORMAL_SIZE_Y = 30};
	enum { MAX_NORMAL_FRAME = 300, MAX_DAMAGE_FRAME = 50 };

	static TTF_Font* FONT() {
		static TTF_Font* f = TTF_OpenFont(FONT_PATH.c_str(), _PTSIZE);
		return f;
	}

	const static SDL_Color RED;
	const static SDL_Color BLUE;
	const static SDL_Color GREEN;
	const static SDL_Color BLACK;
	const static SDL_Color WHITE;
	const static SDL_Color PURPLE;
	const static SDL_Color CLEAR;
	const static SDL_Color NORMAL_COLOR;
	const static SDL_Color DAMAGE_COLOR;
	const static SDL_Color SYSTEM_COLOR;
	const static SDL_Color BUFF_DURATION;

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
	static void printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts, bool spacing);
	static void printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts, SDL_Color ncolor);
	static void printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts, SDL_Color ncolor, bool spacing);
	static Text printT(const int& ntype, std::string text, SDL_Rect npos, SDL_Color ncolor);
	static Text printT(const int& ntype, u16string utext, SDL_Rect npos, SDL_Color ncolor);
	static void clear(std::vector<Text>& texts);
};



#endif