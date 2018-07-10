#include "player.h"
#include "input.h"
#include "texture.h"
#include "ui.h"
#include "window.h"
#include "fps.h"

#ifndef SPELLBOOK_H
#define SPELLBOOK_H

class Spellbook {
private:
	const static SDL_Rect RECT_NAME;
	const static SDL_Rect RECT_NAME_BACK;
	const static int RECT_BACK_SIZE;

	SDL_Rect _selectionRect;
	FPS _fps;

	Text _name;

	Player* _player;
	Window* _window;
	Input _input;

	int _selection;
	bool _isChoosing;

	int _mouseX;
	int _mouseY;
public:
	void spellBook(Player* player, Window* mainWindow, std::vector<Texture>& spellsMem);
	void input();
	void display(std::vector<Texture>& spellsMem);
	void setSpellName();
	void exit();
};

#endif