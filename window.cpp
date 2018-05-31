#include "window.h"

void Window::Init(const char* title, const int& x, const int& y, const int& w, const int& h, const int& flag) {
	_window = SDL_CreateWindow(title, x, y, w, h, flag);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

void Window::render() {
	if (update == true) {
		SDL_RenderPresent(_renderer);
	}
}

int Window::getWidth() {
	int width = 0;
	SDL_GetWindowSize(_window, &width, NULL); 
	return width;
}

int Window::getHeight() {
	int height = 0;
	SDL_GetWindowSize(_window, NULL, &height);
	return height;
}

std::string Window::getTitle() {
	return SDL_GetWindowTitle(_window);
}

SDL_Renderer* Window::getRenderer() {
	return _renderer;
}

SDL_Window* Window::getWindow() {
	return _window;
}