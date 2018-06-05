#include <SDL.h>
#include <iostream>
#include <vector>


#ifndef QUADTREE_H
#define QUADTREE_H

#define TOP_RIGHT 0
#define BOTTOM_RIGHT 1
#define TOP_LEFT 2
#define BOTTOM_LEFT 3

struct Object {
	int id;
	SDL_Rect rect;
	void Init(int nid, SDL_Rect& nrect);
};

class Quadtree {
private:
	enum {MAX_OBJECTS = 5, MAX_DEPTH = 10};

	int _level;
	std::vector<Object> _objects;
	SDL_Rect _bounds;
	std::vector<Quadtree> _nodes;
public:
	void Init(int level, SDL_Rect bounds);
	void clear();
	void split();
	int getIndex(Object& rect);
	void insert(Object rect);
	void retrieve(std::vector<Object>& objects, Object& object);
};

#endif
