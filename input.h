#include <SDL.h>
#include <iostream>

#ifndef INPUT_H
#define INPUT_H

class Input {
private:
	const Uint8* _keys = SDL_GetKeyboardState(NULL);
	SDL_Event _event;
public:
	bool get(const SDL_Keycode& e, SDL_Window* window);
	bool getQuit(SDL_Window* window);
	bool isPressed(const SDL_Scancode& key);
	bool leftClick(int& nx, int& ny, SDL_Window* window);
	bool rightClick(int& nx, int& ny, SDL_Window* window);
};
#endif