#include "text.h"
#include "texture.h"
#include "options.h"

#ifndef TEXTBOX_H
#define TEXTBOX_H

class TextBox {
private:
	enum { _BOX_WIDTH = 900, _BOX_HEIGHT = 180, _BOX_X = 100 ,_BOX_Y = 620, _LINE_Y = 619, _LINE_SPACING_Y = 30, _LINE_HEIGHT = 35, _MAX_LINES = 6, _MAX_LINE_LENGTH = 45,
		   _NAME_WIDTH = 250, _NAME_HEIGHT = 50, _NAME_X = 100, _NAME_Y = _BOX_Y - _NAME_HEIGHT};
	const static SDL_Rect _pos;
	const static SDL_Rect _namepos;
	const static SDL_Color _color;
	std::vector<Text> _lines;
	Text _speaker;
	size_t _numLines;
	bool _isSpeaker;
public:
	bool isActive;

	void display(SDL_Renderer* renderer);
	void print(std::string line);
	void print(std::string line, SDL_Color color);
	void print(u16string uline);
	void print(u16string uline, SDL_Color color);
	void setSpeaker(std::string speaker, SDL_Color color);
	void setSpeaker(u16string speaker, SDL_Color color);
	void removeSpeaker();
};
#endif