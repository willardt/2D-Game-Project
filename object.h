#include <SDL.h>

#include "file.h"
#include "texture.h"

#ifndef OBJECT_H
#define OBJECT_H

class Object {
private:
public:
	enum {LOAD_DATA_SIZE = 2};
	int id;
	SDL_Rect pos;
	std::string name;
	bool isSolid;

	void Init(int nid);

	static std::vector<Texture> memInit();
	static std::vector<Object> createObjectSolids(std::vector<Object>& o);
	static void loadMapObjects(const int& id, std::vector<Object>& o);
};

#endif