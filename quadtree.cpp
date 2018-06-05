#include "quadtree.h"

void Object::Init(int nid, SDL_Rect& nrect) {
	id = nid;
	rect = nrect;
}

void Quadtree::Init(int level, SDL_Rect bounds) {
	_level = level;
	_bounds = bounds;
	_objects.clear();
	_nodes.clear();
}

void Quadtree::clear() {
	_objects.clear();

	if (_nodes.size() != 0) {
		for (int i = 0; i < int(_nodes.size()); i++) {
			_nodes[i].clear();
		}
	}
}

void Quadtree::split() {
	int widthHalf = int(_bounds.w / 2);
	int heightHalf = int(_bounds.h / 2);

	Quadtree n0, n1, n2, n3;
	n0.Init(_level + 1, { _bounds.x + widthHalf, _bounds.y, widthHalf, heightHalf });					// top right
	n1.Init(_level + 1, { _bounds.x + widthHalf, _bounds.y + heightHalf, widthHalf, heightHalf });		// bottom right
	n2.Init(_level + 1, { _bounds.x, _bounds.y, widthHalf, heightHalf });								// top left
	n3.Init(_level + 1, { _bounds.x, _bounds.y + heightHalf, widthHalf, heightHalf });					// bottom left
	_nodes.push_back(n0);
	_nodes.push_back(n1);
	_nodes.push_back(n2);
	_nodes.push_back(n3);
}

int Quadtree::getIndex(Object& object) {
	float verticalMidpoint = float(_bounds.x + (_bounds.w / 2));
	float horizonMidpoint = float(_bounds.y + (_bounds.h / 2));

	bool top = (object.rect.y < horizonMidpoint && object.rect.y + object.rect.h < horizonMidpoint);
	bool bottom = (object.rect.y > horizonMidpoint);

	if (object.rect.y < verticalMidpoint && object.rect.x + object.rect.h < verticalMidpoint) {
		if (top == true) {
			return TOP_LEFT;
		}
		else if (bottom == true) {
			return BOTTOM_LEFT;
		}
	}
	else if (object.rect.x > verticalMidpoint) {
		if (top == true) {
			return TOP_RIGHT;
		}
		else if (bottom == true) {
			return BOTTOM_RIGHT;
		}
	}
	return -1;
}

void Quadtree::insert(Object object) {
	if (_nodes.size() != 0) {
		int index = getIndex(object);

		if (index != -1) {
			_nodes[index].insert(object);
			return;
		}
	}

	_objects.push_back(object);

	if (_objects.size() > MAX_OBJECTS && _level < MAX_DEPTH) {
		if (_nodes.size() == 0) {
			split();
		}

		int i = 0;
		while (i < int(_objects.size())) {
			int index = getIndex(_objects[i]);
			if (index != -1) {
				_nodes[index].insert(_objects[i]);
				_objects.erase(_objects.begin() + i);
			}
			else {
				i++;
			}
		}
	}
}

void Quadtree::retrieve(std::vector<Object>& objects, Object& object) {
	int index = getIndex(object);
	if (index != -1 && _nodes.size() != 0) {
		_nodes[index].retrieve(objects, object);
	}
	else if (index == -1 && _nodes.size() != 0) {
		_nodes[0].retrieve(objects, object);
		_nodes[1].retrieve(objects, object);
		_nodes[2].retrieve(objects, object);
		_nodes[3].retrieve(objects, object);
	}

	for (size_t i = 0; i < _objects.size(); i++) {
		if (object.id != _objects[i].id) {
			objects.push_back(_objects[i]);
		}
	}
}
