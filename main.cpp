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

	srand(time(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);
	if (SDL_Init != nullptr) {
		std::cout << "SDL LOADED" << std::endl;
	}
	TTF_Init();
	if (TTF_Init != nullptr) {
		std::cout << "SDL_TTF LOADED" << std::endl;
	}

	Options& options = options.Instance();
	options.Init();

	File file;
	file.setPath("Data/temp.txt");
	int c = file.readInt(1);

	if (c == 1) {
		Game game;
		game.Init();
		game.begin();
	}
	else if (c == 0) {
		Editor editor;
		editor.Init();
		editor.begin();
	}

	TTF_Quit();
	SDL_Quit();
	std::cout << "SDL QUIT" << std::endl;
	return 0;
}