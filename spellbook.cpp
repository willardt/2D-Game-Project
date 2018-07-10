#include "spellbook.h"

const SDL_Rect Spellbook::RECT_NAME = { 150, 150, NULL, 35 };
const SDL_Rect Spellbook::RECT_NAME_BACK = { 150, 145, 390, 45 };
const int Spellbook::RECT_BACK_SIZE = 60;

void Spellbook::spellBook(Player* player, Window* mainWindow, std::vector<Texture>& spellsMem) {
	_player = player;
	_window = mainWindow;
	_isChoosing = true;
	_selectionRect = { 0, 0, RECT_BACK_SIZE, RECT_BACK_SIZE };
	setSpellName();

	while (_isChoosing) {
		input();
		display(spellsMem);
		_fps.calcFPS();
	}

	exit();
}

void Spellbook::input() {
	_isChoosing = _input.get(_window->getWindow());
	if (_isChoosing) {
		_isChoosing = !_input.isKey(SDL_SCANCODE_ESCAPE);
	}

	if (_input.isKey(SDL_SCANCODE_W)) {
		if (_selection < 4) {
			_selection = Player::SPELLBOOK_SIZE - 4 + _selection;
		}
		else {
			_selection -= 4;
		}

		setSpellName();
	}

	if (_input.isKey(SDL_SCANCODE_S)) {
		if (_selection > Player::SPELLBOOK_SIZE - 5) {
			_selection = _selection % 4;
		}
		else {
			_selection += 4;
		}

		setSpellName();
	}

	if (_input.isKey(SDL_SCANCODE_D)) {
		if (((_selection + 1) % 4) == 0 && _selection != 0) {
			_selection -= 3;
		}
		else {
			_selection++;
		}
		setSpellName();
	}

	if (_input.isKey(SDL_SCANCODE_A)) {
		if (((_selection) % 4) == 0) {
			_selection += 3;
		}
		else {
			_selection--;
		}

		setSpellName();
	}

	if (_player->spellBook[_selection].id != -1) {
		if (_input.isMouse(_mouseX, _mouseY, SDL_BUTTON_LEFT, _window->getWindow())) {
			if (_player->spellBook[_selection].id != _player->secondary.id) {
				_player->spell = _player->spellBook[_selection];
				setSpellName();
				_player->currentSpell = _selection;
				_player->spell.setSpaceUI(UI::SPELL_BOX.w, UI::SPELL_BOX.h, UI::SPELL_BOX.x, UI::SPELL_BOX.y);
			}
			else {
				_player->swapSpells(_player->spell, _player->secondary);
				int temp = _player->currentSpell;
				_player->currentSpell = _player->currentSecondary;
				_player->currentSecondary = temp;
				_player->spell.setSpaceUI(UI::SPELL_BOX.w, UI::SPELL_BOX.h, UI::SPELL_BOX.x, UI::SPELL_BOX.y);
				_player->secondary.setSpaceUI(UI::SECONDARY_BOX.w, UI::SECONDARY_BOX.h, UI::SECONDARY_BOX.x, UI::SECONDARY_BOX.y);
			}
		}
		if (_input.isMouse(_mouseX, _mouseY, SDL_BUTTON_RIGHT, _window->getWindow())) {
			if (_player->spellBook[_selection].id != _player->spell.id) {
				_player->secondary = _player->spellBook[_selection];
				setSpellName();
				_player->currentSecondary = _selection;
				_player->secondary.setSpaceUI(UI::SECONDARY_BOX.w, UI::SECONDARY_BOX.h, UI::SECONDARY_BOX.x, UI::SECONDARY_BOX.y);
			}
			else {
				_player->swapSpells(_player->spell, _player->secondary);
				int temp = _player->currentSpell;
				_player->currentSpell = _player->currentSecondary;
				_player->currentSecondary = temp;
				_player->spell.setSpaceUI(UI::SPELL_BOX.w, UI::SPELL_BOX.h, UI::SPELL_BOX.x, UI::SPELL_BOX.y);
				_player->secondary.setSpaceUI(UI::SECONDARY_BOX.w, UI::SECONDARY_BOX.h, UI::SECONDARY_BOX.x, UI::SECONDARY_BOX.y);
			}
		}
	}
}

void Spellbook::display(std::vector<Texture>& spellsMem) {
	int x = 200;
	int y = 200;

	for (int i = 0; i < Player::SPELLBOOK_SIZE; i++) {
		if (i == _selection) {
			_selectionRect.x = x;
			_selectionRect.y = y;
		}

		Texture::drawRectTransNoCam({ x, y, RECT_BACK_SIZE, RECT_BACK_SIZE }, Text::BLACK, _window->getRenderer());
		if (_player->spellBook[i].id != -1) {
			_player->spellBook[i].setSpaceUI(RECT_BACK_SIZE, RECT_BACK_SIZE, x, y);
			spellsMem[_player->spellBook[i].id].renderNoCamSprite(_player->spellBook[i].pos, { 0, 0, NULL }, _window->getRenderer());
		}
		x += 75;
		if ((i + 1) % 4 == 0 && i != 0) {
			x = 200;
			y += 75;
		}
	}

	Texture::drawRectTransNoCam(_selectionRect, Text::CLEAR, _window->getRenderer());
	Texture::drawRectTransNoCam(RECT_NAME_BACK, Text::BLACK, _window->getRenderer());
	_name.renderNoCam(_window->getRenderer());

	Texture::drawRectTransNoCam(UI::SPELL_BOX, Text::BLACK, _window->getRenderer());
	spellsMem[_player->spell.id].renderNoCamSprite(_player->spell.pos, _player->spell.sprite, _window->getRenderer());
	Texture::drawRectTransNoCam(UI::SECONDARY_BOX, Text::BLACK, _window->getRenderer());
	spellsMem[_player->secondary.id].renderNoCamSprite(_player->secondary.pos, _player->secondary.sprite, _window->getRenderer());

	_window->render();
}

void Spellbook::setSpellName() {
	_name.destroy();
	_name = Text::printT(TEXT_NORMAL, _player->spellBook[_selection].uname, RECT_NAME, Text::WHITE);
	_name.pos.x = 150 + (RECT_NAME_BACK.w - _name.pos.w) / 2;
}

void Spellbook::exit() {
	_name.destroy();
}