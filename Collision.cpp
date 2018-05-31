#include "Collision.h"

bool Collision::seperateAxis(SDL_Rect& a, SDL_Rect& b) {
	if ((a.x + a.w) <= b.x) {
		return false;
	}
	if (a.x >= (b.x + b.w)) {
		return false;
	}
	if ((a.y + a.h) <= b.y) {
		return false;
	}
	if (a.y >= (b.y + b.h)) {
		return false;
	}
	return true;
}

bool Collision::rectCollision(SDL_Rect& a, SDL_Rect& b) {
	if (b.x > (a.x + a.w)) {
		return false;
	}
	if ((b.x + b.w) < a.x) {
		return false;
	}
	if (b.y > (a.y + a.h)) {
		return false;
	}
	if ((b.y + b.h) < (a.y + a.h - Y_SPACING)) {
		return false;
	}
	return true;
}