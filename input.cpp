#include "input.h"

bool Input::get(const SDL_Keycode& e, SDL_Window* window) {
	while (SDL_PollEvent(&_event)) {
		if (_event.window.windowID == SDL_GetWindowID(window) && _event.key.keysym.sym == e) {
			return true;
		}
	}
	return false;
}

bool Input::getQuit(SDL_Window* window) {
	while (SDL_PollEvent(&_event)) {
		if (_event.window.windowID == SDL_GetWindowID(window) && _event.window.event == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		_keys = SDL_GetKeyboardState(NULL);
	}
	return true;
}

bool Input::isPressed(const SDL_Scancode& key) {
	if (_keys[key]) {
		return true;
	}
	return false;
}

bool Input::leftClick(int& nx, int& ny, SDL_Window* window) {
	if (SDL_GetMouseState(&nx, &ny) & SDL_BUTTON(SDL_BUTTON_LEFT) && (SDL_GetMouseFocus() == window)) {
		return true;
	}
	return false;
}

bool Input::rightClick(int& nx, int& ny, SDL_Window* window) {
	if (SDL_GetMouseState(&nx, &ny) & SDL_BUTTON(SDL_BUTTON_RIGHT) && (SDL_GetMouseFocus() == window)) {
		return true;
	}
	return false;
}