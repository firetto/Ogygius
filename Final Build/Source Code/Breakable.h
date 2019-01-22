/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Breakable.h

Contains the class definitions for the breakable obstacles (trees, rocks) found around the world.

*/

#pragma once
#include "CollidableObject.h"
#include "DroppedItem.h"
#include "Crafting.h"

// create a class
class Breakable;

// create a list holding different types of breakables
std::map<int, Breakable> breakableMap;

// class declaration
class Breakable : public Collidable {
private:
	// current health
	int health;

	// minimum size of the visual sprite
	float minSize = 0.5f;

public:

	// maximum health
	int maxHealth;

	// spawn frequency
	float spawnFreq = 0;

	// the assigned type of breakable
	BreakableType type;

	// what the item can be broken by
	ItemType breakableBy;

	ItemType itemToDrop;

	// whether the item can only be broken with the item type defined above ^
	bool breakableExclusive = false, dropExclusive = false;

	// if placing more precisely
	bool placePrecise = false;

	// basic constructor
	Breakable() {}

	// constructor that is called when type of breakable is figured out
	Breakable(BreakableType _type) {
		*this = breakableMap[_type];
		type = _type;
		// set texture
		vis.setTexture(breakableTextureMap[_type]);

		// set origin of sprite to middle
		vis.setOrigin(getMiddle(vis));

		// set health and maxhealth
		health = maxHealth = breakableMap[_type].maxHealth;
	}

	// function that updates the breakable
	void update() {
		
		// calculates scale of the sprite
		float scale = float(health)/maxHealth*(1.f-minSize)+minSize;

		// sets the sprite scale
		vis.setScale(scale, scale);

		// if it has been broken AND its not air
		if (health <= 0 && !isDeleted && type != BREAKABLE_NONE) {
			// drops items
			spawnItem(itemMap[itemToDrop], getPosition(), sf::Vector2f());

			// "deletes" itself
			isDeleted = true;
		}

		// if it has been deleted
		if (isDeleted) {

			// sets type to none
			*this = Breakable(BREAKABLE_NONE);

			// sets deleted to false
			isDeleted = false;
		}

		// draw (add to draw array)
		draw();
	}

	// change the health
	void changeHealth(int hp) {
		// adds health
		health += hp;

		// changes height
		height += ((hp>0) ? 1 : -1) * 0.01;
	}
};
// a list of all current breakables in the world
std::vector<Breakable> breakableVector;

// declares different types of breakables
void declareBreakables() {
	// if the function has been already called, return
	if (breakableMap.size() > 0) return;

	// creates the list
	for (int i = 0; i < BREAKABLETYPE_COUNT - 1; i++) {
		breakableMap.insert(std::pair<int, Breakable>(i, Breakable()));

		// sets type of breakable
		breakableMap[i].type = BreakableType(i);
	}

	// set up tree
	breakableMap[BREAKABLE_TREE].maxHealth = 10;
	breakableMap[BREAKABLE_TREE].breakableBy = ITEM_AXE;
	breakableMap[BREAKABLE_TREE].itemToDrop = ITEM_WOOD;
	breakableMap[BREAKABLE_TREE].height = 10;

	// set up rock
	breakableMap[BREAKABLE_ROCK].maxHealth = 20;
	breakableMap[BREAKABLE_ROCK].breakableBy = ITEM_PICKAXE;
	breakableMap[BREAKABLE_ROCK].itemToDrop = ITEM_STONE;
	breakableMap[BREAKABLE_ROCK].height = 3;

	// set up grass
	breakableMap[BREAKABLE_GRASS].maxHealth = 4;
	breakableMap[BREAKABLE_GRASS].itemToDrop = ITEM_GRASS;
	breakableMap[BREAKABLE_GRASS].height = 1;
	breakableMap[BREAKABLE_GRASS].isCollidable = false;

	// set up iron ore
	breakableMap[BREAKABLE_ORE_IRON].maxHealth = 30;
	breakableMap[BREAKABLE_ORE_IRON].breakableBy = ITEM_PICKAXE;
	breakableMap[BREAKABLE_ORE_IRON].itemToDrop = ITEM_IRON_ORE;
	breakableMap[BREAKABLE_ORE_IRON].height = 3;
	breakableMap[BREAKABLE_ORE_IRON].dropExclusive = true;

	// set up crafting stump
	breakableMap[BREAKABLE_CRAFTING_STUMP].maxHealth = 5;
	breakableMap[BREAKABLE_CRAFTING_STUMP].breakableBy = ITEM_AXE;
	breakableMap[BREAKABLE_CRAFTING_STUMP].itemToDrop = ITEM_WOOD;
	breakableMap[BREAKABLE_CRAFTING_STUMP].height = 3;

	// set up wood wall
	breakableMap[BREAKABLE_WALL_WOOD].maxHealth = 10;
	breakableMap[BREAKABLE_WALL_WOOD].breakableBy = ITEM_AXE;
	breakableMap[BREAKABLE_WALL_WOOD].itemToDrop = ITEM_WALL_WOOD;
	breakableMap[BREAKABLE_WALL_WOOD].height = 4;
	breakableMap[BREAKABLE_WALL_WOOD].placePrecise = true;
}