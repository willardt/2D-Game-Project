#include <SDL.h>

#ifndef COLLISION_H
#define COLLISION_H

class Collision {
private:
public:
	enum { Y_SPACING = 5 };

	static bool seperateAxis(SDL_Rect& a, SDL_Rect& b);
	static bool rectCollision(SDL_Rect& a, SDL_Rect& b);
};

#endif