#include "input.h"

bool Input::get(SDL_Window* window) {
	SDL_Event e;
	_keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&e)) {
		if (e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window)) {
			return false;
		}
	}
	return true;
}

bool Input::isKey(const SDL_Keycode& key) {
	if (_keys[key]) {
		if (!_keymap[key]) {
			_keymap[key] = true;
			return true;
		}
		return false;
	}
	_keymap[key] = false;
	return false;
}

bool Input::isHeld(const SDL_Keycode& key) {
	if (_keys[key]) {
		return true;
	}
	return false;
}

bool Input::isMouse(int& nx, int& ny, const unsigned int& button, SDL_Window* window) {
	if (SDL_GetMouseState(&nx, &ny) & SDL_BUTTON(button) && SDL_GetMouseFocus() == window) {
		if (_mousemap[button]) {
			return false;
		}
		_mousemap[button] = true;
		return true;
	}
	_mousemap[button] = false;
	return false;
}

bool Input::isMouseHeld(int& nx, int& ny, const unsigned int& button, SDL_Window* window) {
	if (SDL_GetMouseState(&nx, &ny) & SDL_BUTTON(button) && SDL_GetMouseFocus() == window) {
		return true;
	}
	return false;
}