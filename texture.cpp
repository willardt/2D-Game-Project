#include "Texture.h"

const SDL_Color Texture::healthBarFullGreen = { 20, 250, 20, 255 };
const SDL_Color Texture::healthBarFull = { 20, 250, 20, 255 };
const SDL_Color Texture::healthBarEmpty = { 0, 0, 0, 255 };
const SDL_Color Texture::manaBarFull = { 20, 20, 250, 255 };
const SDL_Color Texture::manaBarEmpty = { 20, 20, 20, 255 };
const SDL_Color Texture::mapSolid = { 200, 25, 25, 100 };

// Loads a texture from a filepath
bool Texture::load(std::string& filepath, SDL_Renderer* renderer) {
	//If there is already a texture then delete it
	if (isLoaded == true) {
		SDL_DestroyTexture(texture);
	}

	SDL_Surface* surface = IMG_Load(filepath.c_str());

	if (surface != nullptr) {
		//Set the color transparency
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0x00, 0xff));
		//Create the texture from the image surface
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		//Delete surface from memory
		SDL_FreeSurface(surface);
		return true;
	}
	else {
		std::cout << "TEXTURE ERROR at - " << filepath << std::endl;
		return false;
	}
}

// Loads a texture from '_path', called in .render()
bool Texture::load(SDL_Renderer* renderer) {
	if (isLoaded == true) {
		SDL_DestroyTexture(texture);
	}

	SDL_Surface* surface = IMG_Load(_path.c_str());

	if (surface != nullptr) {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0x00, 0xff));
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return true;
	}
	else {
		if (_path == "") {
			std::cout << "TEXTURE ERROR, NO PATH" << std::endl;
		}
		else {
			std::cout << "TEXTURE ERROR at - " << _path << std::endl;
		}
		return false;
	}
}

// Renders the texture to the screen at positions 'rect'
bool Texture::render(SDL_Rect& rect, SDL_Renderer* renderer) {
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}
	if (isLoaded == true) {
		SDL_Rect position = { rect.x - options.camX, rect.y - options.camY, rect.w , rect.h};
		SDL_RenderCopy(renderer, texture, NULL, &position);
		return true;
	}
	return false;
}

// Renders the texture to the screen at positions 'rect' from the texture bounded by 'sprite'
bool Texture::renderSprite(SDL_Rect& rect, Sprite& sprite, SDL_Renderer* renderer) {
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}
	if (isLoaded == true) {
		SDL_Rect position = { rect.x - options.camX, rect.y - options.camY, rect.w , rect.h };
		SDL_Rect destination = { sprite.x, sprite.y, rect.w, rect.h };
		SDL_RenderCopy(renderer, texture, &destination, &position);
		return true;
	}
	return false;
}

// Renders the texture to the screen at positions 'rect' with no camera offset
bool Texture::renderNoCam(SDL_Rect rect, SDL_Renderer* renderer) {
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}
	if (isLoaded == true) {
		SDL_Rect position = { rect.x, rect.y, rect.w , rect.h };
		SDL_RenderCopy(renderer, texture, NULL, &position);
		return true;
	}
	return false;
}

// Renders the texture to the screen at position 'rect' from the texture bounded by 'sprite' with no camera offset
bool Texture::renderNoCamSprite(SDL_Rect rect, Sprite sprite, SDL_Renderer* renderer) {
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}
	if (isLoaded == true) {
		SDL_Rect position = { rect.x, rect.y, rect.w , rect.h };
		SDL_Rect destination = { sprite.x, sprite.y, rect.w, rect.h };
		SDL_RenderCopy(renderer, texture, &destination, &position);
		return true;
	}
	return false;
}

bool Texture::renderNoCamTrans(SDL_Rect& rect, SDL_Renderer* renderer, Uint8 alpha) {
	if (isLoaded != true) {
		isLoaded = load(renderer);
	}
	if (isLoaded == true) {
		SDL_SetTextureAlphaMod(texture, alpha);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		return true;
	}
	return false;
}

// Set the filepath of the texture
void Texture::setPath(std::string path) {
	if (isLoaded == true) {
		SDL_DestroyTexture(texture);
		isLoaded = false;
	}
	_path = path;
}

// Draws a rectangle at position 'rect'
void Texture::drawRect(const SDL_Rect& rect, const SDL_Color& color, SDL_Renderer* renderer) {
	Options& options = options.Instance();
	SDL_Rect position = { rect.x - options.camX, rect.y - options.camY, rect.w, rect.h };
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &position);
	SDL_RenderFillRect(renderer, &position);
}

// Draws a rectangle at position 'rect' with no camera offset
void Texture::drawRectNoCam(const SDL_Rect& rect, const SDL_Color& color, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderFillRect(renderer, &rect);
}

void Texture::drawRectTrans(const SDL_Rect& rect, const SDL_Color& color, SDL_Renderer* renderer) {
	Options& options = options.Instance();
	SDL_Rect position = { rect.x - options.camX, rect.y - options.camY, rect.w, rect.h };
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &position);
	SDL_RenderFillRect(renderer, &position);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

// Draws a transparent rectangle at position 'rect' with no camera offset, transparency is passed throught color.a
void Texture::drawRectTransNoCam(const SDL_Rect& rect, const SDL_Color& color, SDL_Renderer* renderer) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Texture::drawRectOutline(SDL_Rect rect, SDL_Color color, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, color.a, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &rect);
}

// Updates the sprite frame and position
Sprite Texture::updateSprite(SDL_Rect& rect, const int& frame) {
	int destX = 0;
	int destY = 0;

	if (frame > MAX_SPRITE_SHEET_WIDTH - 1) {
		destY = (frame / MAX_SPRITE_SHEET_WIDTH);
		destX = (frame % MAX_SPRITE_SHEET_WIDTH);
		destX = destX * rect.w;
		destY = destY * rect.h;
	}
	else if (frame > 0) {
		destX = frame * rect.w;
	}

	return { destX, destY, frame };
}

std::string Texture::getPath() {
	return _path;
}