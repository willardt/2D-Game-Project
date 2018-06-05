#include "text.h"

const std::string Text::_NORMAL_FONT_PATH = "Data/Fonts/normalfont.ttf";
const std::string Text::_DAMAGE_FONT_PATH = "Data/Fonts/damagefont.ttf";
const std::string Text::_RUSSIAN_FONT_PATH = "Data/Fonts/russianfont.ttf";
const SDL_Color Text::RED = { 255, 0, 0, 150 };
const SDL_Color Text::BLUE = { 0, 0, 255, 150 };
const SDL_Color Text::GREEN = { 50, 250, 50, 150 };
const SDL_Color Text::BLACK = { 0, 0, 0, 255 };
const SDL_Color Text::WHITE = { 255, 255, 255, 255 };
const SDL_Color Text::PURPLE = { 200, 25, 225, 150 };
const SDL_Color Text::NORMAL_COLOR = { 255, 255, 255, 255 };
const SDL_Color Text::DAMAGE_COLOR = { 50, 150, 200, 255 };

// Create a texture from a string, called in .render()
bool Text::load(std::string text, SDL_Renderer* renderer) {
	SDL_Surface* surface = nullptr;
	SDL_Surface* oSurface = nullptr;
	isUnicode = false;
	if (isLoaded == true) {
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(oTexture);
	}
	_text = text;
	
	if (isUnicode == false) {
		TTF_SetFontOutline(getFontType(), NULL);
		surface = TTF_RenderText_Solid(getFontType(), _text.c_str(), color);
		TTF_SetFontOutline(getFontType(), _OUTLINE);
		oSurface = TTF_RenderText_Solid(getFontType(), _text.c_str(), Text::BLACK);
	}
	else {
		TTF_SetFontOutline(getFontType(), NULL);
		surface = TTF_RenderUNICODE_Solid(getFontType(), _utext.c_str(), color);
		TTF_SetFontOutline(getFontType(), _OUTLINE);
		oSurface = TTF_RenderUNICODE_Solid(getFontType(), _utext.c_str(), Text::BLACK);
	}

	if (surface != nullptr && oSurface != nullptr) {
		pos.w = int(_text.length()) * TEXT_SPACING_W;
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		oTexture = SDL_CreateTextureFromSurface(renderer, oSurface);
		SDL_FreeSurface(surface);
		SDL_FreeSurface(oSurface);
		return true;
	}
	
	return false;
}

// Creates a texture from a unicode string, called in .render()
bool Text::load(u16string utext, SDL_Renderer* renderer) {
	SDL_Surface* surface = nullptr;
	SDL_Surface* oSurface = nullptr;
	isUnicode = true;
	if (isLoaded == true) {
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(oTexture);
	}
	_utext = utext;

	if (isUnicode == false) {
		TTF_SetFontOutline(getFontType(), NULL);
		surface = TTF_RenderText_Solid(getFontType(), _text.c_str(), color);
		TTF_SetFontOutline(getFontType(), _OUTLINE);
		oSurface = TTF_RenderText_Solid(getFontType(), _text.c_str(), Text::BLACK);
	}
	else {
		TTF_SetFontOutline(getFontType(), NULL);
		surface = TTF_RenderUNICODE_Solid(getFontType(), _utext.c_str(), color);
		TTF_SetFontOutline(getFontType(), _OUTLINE);
		oSurface = TTF_RenderUNICODE_Solid(getFontType(), _utext.c_str(), Text::BLACK);
	}

	if (surface != nullptr && oSurface != nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		oTexture = SDL_CreateTextureFromSurface(renderer, oSurface);
		SDL_FreeSurface(surface);
		SDL_FreeSurface(oSurface);
		return true;
	}

	return false;
}

// Creates a texture from a string or unicode string, called in .render()
bool Text::load(SDL_Renderer* renderer) {
	SDL_Surface* surface = nullptr;
	SDL_Surface* oSurface = nullptr;
	if (isLoaded == true) {
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(oTexture);
	}

	if (isUnicode == false) {
		TTF_SetFontOutline(getFontType(), NULL);
		surface = TTF_RenderText_Solid(getFontType(), _text.c_str(), color);
		TTF_SetFontOutline(getFontType(), _OUTLINE);
		oSurface = TTF_RenderText_Solid(getFontType(), _text.c_str(), Text::BLACK);
	}
	else {
		TTF_SetFontOutline(getFontType(), NULL);
		surface = TTF_RenderUNICODE_Solid(getFontType(), _utext.c_str(), color);
		TTF_SetFontOutline(getFontType(), _OUTLINE);
		oSurface = TTF_RenderUNICODE_Solid(getFontType(), _utext.c_str(), Text::BLACK);
	}

	if (surface != nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		oTexture = SDL_CreateTextureFromSurface(renderer, oSurface);
		SDL_FreeSurface(surface);
		SDL_FreeSurface(oSurface);
		return true;
	}

	return false;
}

// Renders the text's texture to the screen at position 'rect'
bool Text::render(SDL_Rect& rect, SDL_Renderer* renderer) {
	Options& options = options.Instance();
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}

	if (isLoaded == true) {
		SDL_Rect position = { rect.x - options.camX, rect.y - options.camY, pos.w, rect.h };
		SDL_RenderCopy(renderer, texture, NULL, &position);
		return true;
	}

	return false;
} 

// Renders the text's texture to the screen at the text's positions 'pos'
bool Text::render(SDL_Renderer* renderer) {
	Options& options = options.Instance();
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}

	if (isLoaded == true) {
		SDL_Rect position = { pos.x - options.camX, pos.y - options.camY, pos.w, pos.h };
		SDL_RenderCopy(renderer, texture, NULL, &position);
		return true;
	}

	return false;
}

// Renders the text's texture to the screen at the text's position 'pos' with no camera offset
bool Text::renderNoCam(SDL_Renderer* renderer) {
	Options& options = options.Instance();
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}

	if (isLoaded == true) {
		SDL_RenderCopy(renderer, texture, NULL, &pos);
		return true;
	}

	return false;
}

// Renders the text's texture and oTexture to the screen at position 'rect'
bool Text::renderOutline(SDL_Rect& rect, SDL_Renderer* renderer) {
	Options& options = options.Instance();
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}

	if (isLoaded == true) {
		SDL_Rect position = { rect.x - options.camX, rect.y - options.camY, pos.w, rect.h };
		SDL_RenderCopy(renderer, oTexture, NULL, &position);
		SDL_RenderCopy(renderer, texture, NULL, &position);
		return true;
	}

	return false;
}

// Renders the text's texture and oTexture to the screen at the text's position 'pos'
bool Text::renderOutline(SDL_Renderer* renderer) {
	Options& options = options.Instance();
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}

	if (isLoaded == true) {
		SDL_Rect position = { pos.x - options.camX, pos.y - options.camY, pos.w, pos.h };
		SDL_RenderCopy(renderer, oTexture, NULL, &position);
		SDL_RenderCopy(renderer, texture, NULL, &position);
		return true;
	}

	return false;
}

// Update the text frame to check if the text can be deleted
void Text::update() {
	if (isLoaded == true) {
		frame++;
		switch (type) {
		case TEXT_NORMAL:
			if (frame > MAX_NORMAL_FRAME) {
				isEnd = true;
			}
			break;
		case TEXT_DAMAGE:
			pos.y--;
			if (frame > MAX_DAMAGE_FRAME) {
				isEnd = true;
			}
			break;
		}
	}

	if (isEnd == true) {
		destroy();
	}
}

void Text::setText(std::string text) {
	isUnicode = false;
	_text = text;
}

// Set unicode text
void Text::setUText(u16string utext) {
	isUnicode = true;
	_utext = utext;
}

std::string Text::getText() {
	return _text;
}

// Get unicoide text
u16string Text::getUText() {
	return _utext;
}

// Creates a width for text's positions 'pos.w' based on the character size of '_text' or '_utext'
void Text::setSpacing() {
	if (isUnicode == true) {
		pos.w = (int(_utext.length()) * TEXT_SPACING_W);
	}
	else {
		pos.w = int(_text.length()) * TEXT_SPACING_W;
	}
}

// Returns a font type based on text's type or Unicode
TTF_Font* Text::getFontType() {
	if (isUnicode == true) {
		return RUSSIAN_FONT();
	}

	switch (type) {
	case TEXT_NORMAL:
		return NORMAL_FONT();
		break;
	case TEXT_DAMAGE:
		return DAMAGE_FONT();
		break;
	case TEXT_BOX:
		return NORMAL_FONT();
		break;
	default:
		return nullptr;
		break;
	}
}

// Adds a text object to a vector of type 'Text'
void Text::printT(const int& ntype, std::string text, SDL_Rect npos, std::vector<Text>& texts) {
	Text temp;
	temp.type = ntype;
	temp.setText(text);
	temp.pos = npos;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = false;
	temp.frame = 0;
	temp.texture = nullptr;
	temp.oTexture = nullptr;

	switch (temp.type) {
	case TEXT_NORMAL:
		temp.color = NORMAL_COLOR;
		break;
	case TEXT_DAMAGE:
		temp.color = DAMAGE_COLOR;
		break;
	}

	temp.setSpacing();

	texts.push_back(temp);
}

void Text::printT(const int& ntype, std::string text, SDL_Rect npos, std::vector<Text>& texts, SDL_Color ncolor) {
	Text temp;
	temp.type = ntype;
	temp.setText(text);
	temp.pos = npos;
	temp.color = ncolor;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = false;
	temp.frame = 0;
	temp.texture = nullptr;
	temp.oTexture = nullptr;
	temp.setSpacing();

	texts.push_back(temp);
}

void Text::printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts) {
	Text temp;
	temp.type = ntype;
	temp.setUText(utext);
	temp.pos = npos;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = true;
	temp.texture = nullptr;
	temp.oTexture = nullptr;
	temp.frame = 0;

	switch (temp.type) {
	case TEXT_NORMAL:
		temp.color = NORMAL_COLOR;
		break;
	case TEXT_DAMAGE:
		temp.color = DAMAGE_COLOR;
		break;
	}

	temp.setSpacing();

	texts.push_back(temp);
}

void Text::printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts, bool spacing) {
	Text temp;
	temp.type = ntype;
	temp.setUText(utext);
	temp.pos = npos;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = true;
	temp.texture = nullptr;
	temp.oTexture = nullptr;
	temp.frame = 0;

	switch (temp.type) {
	case TEXT_NORMAL:
		temp.color = NORMAL_COLOR;
		break;
	case TEXT_DAMAGE:
		temp.color = DAMAGE_COLOR;
		break;
	}

	if (spacing != true) {
		temp.setSpacing();
	}

	texts.push_back(temp);
}

void Text::printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts, SDL_Color ncolor) {
	Text temp;
	temp.type = ntype;
	temp.setUText(utext);
	temp.pos = npos;
	temp.color = ncolor;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = true;
	temp.frame = 0;
	temp.texture = nullptr;
	temp.oTexture = nullptr;
	temp.setSpacing();

	texts.push_back(temp);
}

void Text::printT(const int& ntype, u16string utext, SDL_Rect npos, std::vector<Text>& texts, SDL_Color ncolor, bool spacing) {
	Text temp;
	temp.type = ntype;
	temp.setUText(utext);
	temp.pos = npos;
	temp.color = ncolor;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = true;
	temp.frame = 0;
	temp.texture = nullptr;
	temp.oTexture = nullptr;
	if (spacing != false) {
		temp.setSpacing();
	}

	texts.push_back(temp);
}

// Returns a text object
Text Text::printT(const int& ntype, std::string text, SDL_Rect npos, SDL_Color ncolor) {
	Text temp;
	temp.type = ntype;
	temp.setText(text);
	temp.pos = npos;
	temp.color = ncolor;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = false;
	temp.frame = 0;
	temp.texture = nullptr;
	temp.oTexture = nullptr;
	temp.setSpacing();

	return temp;
}

Text Text::printT(const int& ntype, u16string utext, SDL_Rect npos, SDL_Color ncolor) {
	Text temp;
	temp.type = ntype;
	temp.setUText(utext);
	temp.pos = npos;
	temp.color = ncolor;
	temp.isLoaded = false;
	temp.isEnd = false;
	temp.isUnicode = true;
	temp.frame = 0;
	temp.texture = nullptr;
	temp.oTexture = nullptr;
	temp.setSpacing();

	return temp;
}

void Text::clear(std::vector<Text>& texts) {
	for (int i = 0; i < texts.size(); i++) {
		texts[i].destroy();
	}
	texts.clear();
}

void Text::destroy() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
	SDL_DestroyTexture(oTexture);
	oTexture = nullptr;
	isLoaded = false;
}