#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

#include "options.h"

#ifndef TEXTURE_H
#define TEXTURE_H

// * pos.w
#define MAX_SPRITE_SHEET_WIDTH 8

// Bounds of an image to load
struct Sprite {
	int x;
	int y;
	int frame;
};

class Texture {
private:
	std::string _path;  // File path | Image path
public:
	enum { BAR_SPACING_X = 8, BAR_SPACING_Y = 20, BAR_H = 8 };

	const static SDL_Color healthBarFullGreen;
	const static SDL_Color healthBarFull;
	const static SDL_Color healthBarEmpty;
	const static SDL_Color manaBarFull;
	const static SDL_Color manaBarEmpty;

	bool isLoaded;

	Options& options = options.Instance();

	SDL_Texture* texture;

	bool load(std::string& filepath, SDL_Renderer* renderer);
	bool load(SDL_Renderer* renderer);
	bool render(SDL_Rect& rect, SDL_Renderer* renderer);
	bool renderSprite(SDL_Rect& rect, Sprite& sprite, SDL_Renderer* renderer);
	bool renderNoCam(SDL_Rect rect, SDL_Renderer* renderer);
	bool renderNoCamSprite(SDL_Rect rect, Sprite sprite, SDL_Renderer* renderer);
	bool renderNoCamTrans(SDL_Rect& rect, SDL_Renderer* renderer, Uint8 alpha);

	void setPath(std::string path);

	static void drawRect(const SDL_Rect& rect, const SDL_Color& color, SDL_Renderer* renderer);
	static void drawRectNoCam(const SDL_Rect& rect, const SDL_Color& color, SDL_Renderer* renderer);
	static void drawRectTransNoCam(const SDL_Rect& rect, const SDL_Color& color, SDL_Renderer* renderer);
	static void drawBar(SDL_Rect& rect, int height, const int& maxH, const int& h, SDL_Color front, SDL_Color Back, SDL_Renderer* renderer);
	static Sprite updateSprite(SDL_Rect& rect, const int& frame);
	
	std::string getPath();

};

#endif