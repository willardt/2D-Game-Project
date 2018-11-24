#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>

#include "game.h"
#include "editor.h"
#include "options.h"
#include "input.h"
#include "file.h"

int main(int args, char* argc[]) {
	Options& options = options.Instance();
	options.Init();

	HWND console = GetConsoleWindow();
	MoveWindow(console, 0, 450, 1100, 600, TRUE);

	srand(time(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);
	if (SDL_Init != nullptr) {
		std::cout << "SDL LOADED" << std::endl;
	}
	TTF_Init();
	if (TTF_Init != nullptr) {
		std::cout << "SDL_TTF LOADED" << std::endl;
	}

	char in;
	std::cin >> in;

	if (in != '1') {
		Game game;
		game.Init();
		game.begin();
	}
	else {
		Editor editor;
		editor.Init();
		editor.begin();
	}

	/*
	if (args > 1) {
		if (argc[1] == std::string("-Editor")) {
			Editor editor;
			editor.Init();
			editor.begin();
		}
		else {
			Game game;
			game.Init();
			game.begin();
		}
	}
	else {
		Game game;
		game.Init();
		game.begin();
	}
	*/

	TTF_Quit();
	SDL_Quit();
	std::cout << "SDL QUIT" << std::endl;
	return 0;
}