#include "text.h"
#include "texture.h"
#include "options.h"
#include "time.h"
#include "file.h"

#ifndef TEXTBOX_H
#define TEXTBOX_H

#define MESSAGE_GAMESAVED TextBox::getSystemMessage(1)
#define MESSAGE_GAMELOADED TextBox::getSystemMessage(2)
#define MESSAGE_LEVEL TextBox::getSystemMessage(3)
#define MESSAGE_NEWSPELL TextBox::getSystemMessage(4)

class TextBox {
private:
	enum {_BOX_WIDTH = 900, _BOX_HEIGHT = 180, _BOX_X = 350, _BOX_Y = 720, _LINE_Y = 719, _LINE_SPACING_Y = 30, _LINE_HEIGHT = 35, _MAX_LINES = 6, _MAX_LINE_LENGTH = 45,
	   	_NAME_WIDTH = 350, _NAME_HEIGHT = 50, _NAME_X = 350, _NAME_Y = _BOX_Y - _NAME_HEIGHT, ACTIVE_TIME = 300};
	const static SDL_Rect _pos;
	const static SDL_Rect _namepos;
	const static SDL_Color _color;
	std::vector<Text> _lines;
	Text _speaker;
	size_t _numLines;
	bool _isSpeaker;
public:
	Time activeTime;
	bool isActive;

	void Init();
	void display(SDL_Renderer* renderer);
	void update();
	void print(std::string line);
	void print(std::string line, SDL_Color color);
	void print(u16string uline);
	void print(u16string uline, SDL_Color color);
	void setSpeaker(std::string speaker, SDL_Color color);
	void setSpeaker(u16string speaker, SDL_Color color);
	void removeSpeaker();

	static u16string getSystemMessage(int lineNum);
};
#endif