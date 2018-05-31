#include "textbox.h"

const SDL_Rect TextBox::_pos = { _BOX_X, _BOX_Y, _BOX_WIDTH, _BOX_HEIGHT };
const SDL_Rect TextBox::_namepos = { _NAME_X, _NAME_Y, _NAME_WIDTH, _NAME_HEIGHT };
const SDL_Color TextBox::_color = { 0, 0, 0, 100 };

// Display the textbox on the screen
void TextBox::display(SDL_Renderer* renderer) {
	Options& options = options.Instance();
	if (isActive == true) {
		Texture::drawRectTransNoCam(_pos, _color, renderer);
		if (_isSpeaker == true) {
			Texture::drawRectTransNoCam(_namepos, _color, renderer);
			_speaker.renderNoCam(renderer);
		}
		for (size_t i = 0; i < _lines.size(); i++) {
			_lines[i].renderNoCam(renderer);
		}
	}
}

// add a line of text to the textbox to print
// Also removes previous lines if the textbox is full
void TextBox::print(std::string line) {
	std::vector<std::string> nlines;
	size_t lines = 1;
	size_t find = 0;
	std::string temp = "";

	if (line.size() <= _MAX_LINE_LENGTH) {
		nlines.push_back(line);
	}

	while (line.size() > _MAX_LINE_LENGTH) {
		if (lines == 1) {
			temp = line.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(' ');
			nlines.push_back(line.substr(0, find));
			line.erase(0, find);
		}
		else {
			temp = line.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(' ');
			nlines.push_back(line.substr(1, find));
			line.erase(0, find);
		}
		lines++;
	}

	if (line.size() < _MAX_LINE_LENGTH && line.size() > 1) {
		nlines.push_back(line.substr(1, line.size() - 1));
	}

	_numLines = nlines.size() + _lines.size() - 1;
	while (_numLines > _MAX_LINES) {
		SDL_DestroyTexture(_lines[0].texture);
		SDL_DestroyTexture(_lines[0].oTexture);
		_lines.erase(_lines.begin() + 0);
		_numLines--;
	}

	for (size_t i = 0; i < _lines.size(); i++) {
		_lines[i].pos.y += _LINE_SPACING_Y * lines;
	}

	while (lines >= 1) {
		Text::printT(TEXT_BOX, nlines[lines - 1], { _BOX_X, _LINE_Y, NULL, _LINE_HEIGHT }, _lines, Text::WHITE);
		lines--;
	}
}

// add a line with a color
void TextBox::print(std::string line, SDL_Color color) {
	std::vector<std::string> nlines;
	size_t lines = 1;
	size_t find = 0;
	std::string temp = "";

	if (line.size() <= _MAX_LINE_LENGTH) {
		nlines.push_back(line);
	}

	while (line.size() > _MAX_LINE_LENGTH) {
		if (lines == 1) {
			temp = line.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(' ');
			nlines.push_back(line.substr(0, find));
			line.erase(0, find);
		}
		else {
			temp = line.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(' ');
			nlines.push_back(line.substr(1, find));
			line.erase(0, find);
		}
		lines++;
	}

	if (line.size() < _MAX_LINE_LENGTH && line.size() > 1) {
		nlines.push_back(line.substr(1, line.size() - 1));
	}

	_numLines = nlines.size() + _lines.size() - 1;
	while (_numLines > _MAX_LINES) {
		SDL_DestroyTexture(_lines[0].texture);
		SDL_DestroyTexture(_lines[0].oTexture);
		_lines.erase(_lines.begin() + 0);
		_numLines--;
	}

	for (size_t i = 0; i < _lines.size(); i++) {
		_lines[i].pos.y += _LINE_SPACING_Y * lines;
	}

	while (lines >= 1) {
		Text::printT(TEXT_BOX, nlines[lines - 1], { _BOX_X, _LINE_Y, NULL, _LINE_HEIGHT }, _lines, color);
		lines--;
	}
}

// add a unicode line of type u16string
void TextBox::print(u16string uline) {
	std::vector<u16string> nlines;
	size_t lines = 1;
	size_t find = 0;
	u16string temp; // this may break dont know how to initilize this to NULL

	if (uline.size() <= _MAX_LINE_LENGTH) {
		nlines.push_back(uline);
	}

	while (uline.size() > _MAX_LINE_LENGTH) {
		if (lines == 1) {
			temp = uline.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(L' ');
			nlines.push_back(uline.substr(0, find));
			uline.erase(0, find);
		}
		else {
			temp = uline.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(L' ');
			nlines.push_back(uline.substr(1, find));
			uline.erase(0, find);
		}
		lines++;
	}

	if (uline.size() < _MAX_LINE_LENGTH && uline.size() > 1) {
		nlines.push_back(uline.substr(1, uline.size() - 1));
	}

	_numLines = nlines.size() + _lines.size() - 1;
	while (_numLines > _MAX_LINES) {
		SDL_DestroyTexture(_lines[0].texture);
		SDL_DestroyTexture(_lines[0].oTexture);
		_lines.erase(_lines.begin() + 0);
		_numLines--;
	}

	for (size_t i = 0; i < _lines.size(); i++) {
		_lines[i].pos.y += _LINE_SPACING_Y * lines;
	}

	while (lines >= 1) {
		Text::printT(TEXT_BOX, nlines[lines - 1], { _BOX_X, _LINE_Y, NULL, _LINE_HEIGHT }, _lines, Text::WHITE);
		lines--;
	}

	//Text::printT(TEXT_BOX, uline, { _BOX_X, lineHeight(), NULL, _LINE_HEIGHT }, _lines, Text::WHITE);
}

// add a unicode line with a color
void TextBox::print(u16string uline, SDL_Color color) {
	std::vector<u16string> nlines;
	size_t lines = 1;
	size_t find = 0;
	u16string temp; // this may break dont know how to initilize this to NULL

	if (uline.size() <= _MAX_LINE_LENGTH) {
		nlines.push_back(uline);
	}

	while (uline.size() > _MAX_LINE_LENGTH) {
		if (lines == 1) {
			temp = uline.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(L' ');
			nlines.push_back(uline.substr(0, find));
			uline.erase(0, find);
		}
		else {
			temp = uline.substr(0, _MAX_LINE_LENGTH);
			find = temp.find_last_of(L' ');
			nlines.push_back(uline.substr(1, find));
			uline.erase(0, find);
		}
		lines++;
	}

	if (uline.size() < _MAX_LINE_LENGTH && uline.size() > 1) {
		nlines.push_back(uline.substr(1, uline.size() - 1));
	}

	_numLines = nlines.size() + _lines.size() - 1;
	while (_numLines > _MAX_LINES) {
		SDL_DestroyTexture(_lines[0].texture);
		SDL_DestroyTexture(_lines[0].oTexture);
		_lines.erase(_lines.begin() + 0);
		_numLines--;
	}

	for (size_t i = 0; i < _lines.size(); i++) {
		_lines[i].pos.y += _LINE_SPACING_Y * lines;
	}

	while (lines >= 1) {
		Text::printT(TEXT_BOX, nlines[lines - 1], { _BOX_X, _LINE_Y, NULL, _LINE_HEIGHT }, _lines, color);
		lines--;
	}

	//Text::printT(TEXT_BOX, uline, { _BOX_X, lineHeight(), NULL, _LINE_HEIGHT }, _lines, Text::WHITE);
}

// set the speaker's name to be displayed
void TextBox::setSpeaker(std::string speaker, SDL_Color color) {
	SDL_DestroyTexture(_speaker.texture);
	SDL_DestroyTexture(_speaker.oTexture);
	_speaker = Text::printT(TEXT_BOX, speaker, { _NAME_X, _NAME_Y, NULL, _LINE_HEIGHT }, color);
	_isSpeaker = true;
}

// set the speaker's anem to be displayed in unicode of type u16string
void TextBox::setSpeaker(u16string speaker, SDL_Color color) {
	SDL_DestroyTexture(_speaker.texture);
	SDL_DestroyTexture(_speaker.oTexture);
	_speaker = Text::printT(TEXT_BOX, speaker, { _NAME_X, _NAME_Y, NULL, _LINE_HEIGHT }, color);
	_isSpeaker = true;
}

// do not display a speaker
void TextBox::removeSpeaker() {
	_isSpeaker = false;
}