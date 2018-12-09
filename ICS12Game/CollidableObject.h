#pragma once
#include "Object.h"
class Collidable : virtual public Entity {
public:
	// whether you can collide with it
	bool isCollidable = true;

	// if colliding with other entities or players, does it get pushed back?
	bool canMove = false;

	// draw the collidable
	void draw() {
		Object::draw();
	}
};