/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

CollidableObject.h

Contains the class definitions for entities that can be collided with by other collidable entities
*/

#pragma once
#include "Object.h"
// class for classes that are collidable
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