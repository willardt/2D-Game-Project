#include <SDL.h>
#include <string>

#ifndef WINDOW_H
#define WINDOW_H

//Creates window entiries using SDL
class Window {
	
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
public:
	bool update;

	void Init(const char* title, const int& x, const int& y, const int& w, const int& h, const int& flag);

	void render();
	
	int getWidth();
	int getHeight();

	std::string getTitle();

	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
};

#endif