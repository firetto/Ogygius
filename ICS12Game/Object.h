/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Object.h 

This file is for declarations of the most basic game object, and the drawable object
*/




#pragma once
#include <SFML/Graphics.hpp>
#include "Textures.h"
#include "GameDeclarations.h"
/*
0 - ground
1 - grass
2 - droppables
4.5 - particles
3 - rocks, logs, etc
4 - fence
5-mob
5.001- mob head
5.99 - player fists
6 - player
10 - trees


*/

// drawable class
struct ObjDrawable {

	// sprite
	sf::Sprite vis;

	// height
	float height;

	// constructor
	ObjDrawable(sf::Sprite &_vis, int _height) {

		// set sprite and height
		vis = _vis;
		height = _height;
	}

	// draw
	void draw() {
		WINDOW.draw(vis);
	}
};
// list of drawables
std::vector<ObjDrawable> drawableVector;

// positionable object
class Positionable {
public:
	// returns position of the movable object
	sf::Vector2f getPosition() { return position; }

	// set position of the movable object
	void setPosition(sf::Vector2f _pos) { position = _pos; }

	// change position
	void changePosition(sf::Vector2f _pos) {
		position.x += _pos.x;
		position.y += _pos.y;
	}
protected:

	// position
	sf::Vector2f position;
};

/*
basically everything is an object, has the basic functionality for hitboxes & sprites
*/
class Object : virtual public Positionable {
public:
	// height of drawable
	float height;

	// sprite(s) to be displayed
	sf::Sprite vis;

	// constructor
	Object() { // object size
	} 

	// draws the sprite (and hitbox if debug mode is enabled)
	void draw() {
		drawableVector.emplace_back(ObjDrawable(vis, height));
	}

	// whether the object is to be deleted from its respective array
	bool isDeleted = false;

	// sets the position of the sprite and hitbox
	void setPosition(sf::Vector2f pos) {
		Positionable::setPosition(pos);
		vis.setPosition(pos);
	}
};
