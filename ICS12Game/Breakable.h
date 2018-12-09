#pragma once
#include "CollidableObject.h"
#include "DroppedItem.h"
#include "Crafting.h"

class Breakable;
std::map<int, Breakable> breakableMap;

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

	void update() {
		float scale = float(health)/maxHealth*(1.f-minSize)+minSize;
		vis.setScale(scale, scale);
		if (health <= 0 && !isDeleted && type != BREAKABLE_NONE) {
			spawnItem(itemMap[itemToDrop], getPosition(), sf::Vector2f());
			isDeleted = true;
		}
		if (isDeleted) {
			*this = Breakable(BREAKABLE_NONE);
			isDeleted = false;
		}
		draw();
	}
	void changeHealth(int hp) {
		health += hp;
		height -= 0.01;
	}
};
std::vector<Breakable> breakableVector;

void declareBreakables() {
	for (int i = 0; i < BREAKABLETYPE_COUNT - 1; i++) {
		breakableMap.insert(std::pair<int, Breakable>(i, Breakable()));
		breakableMap[i].type = BreakableType(i);
	}

	breakableMap[BREAKABLE_TREE].maxHealth = 10;
	breakableMap[BREAKABLE_TREE].breakableBy = ITEM_AXE;
	breakableMap[BREAKABLE_TREE].itemToDrop = ITEM_WOOD;
	breakableMap[BREAKABLE_TREE].height = 10;

	breakableMap[BREAKABLE_ROCK].maxHealth = 20;
	breakableMap[BREAKABLE_ROCK].breakableBy = ITEM_PICKAXE;
	breakableMap[BREAKABLE_ROCK].itemToDrop = ITEM_STONE;
	breakableMap[BREAKABLE_ROCK].height = 3;

	breakableMap[BREAKABLE_GRASS].maxHealth = 4;
	breakableMap[BREAKABLE_GRASS].itemToDrop = ITEM_GRASS;
	breakableMap[BREAKABLE_GRASS].height = 1;
	breakableMap[BREAKABLE_GRASS].isCollidable = false;

	breakableMap[BREAKABLE_ORE_IRON].maxHealth = 30;
	breakableMap[BREAKABLE_ORE_IRON].breakableBy = ITEM_PICKAXE;
	breakableMap[BREAKABLE_ORE_IRON].itemToDrop = ITEM_IRON_ORE;
	breakableMap[BREAKABLE_ORE_IRON].height = 3;
	breakableMap[BREAKABLE_ORE_IRON].dropExclusive = true;

	breakableMap[BREAKABLE_CRAFTING_STUMP].maxHealth = 5;
	breakableMap[BREAKABLE_CRAFTING_STUMP].breakableBy = ITEM_AXE;
	breakableMap[BREAKABLE_CRAFTING_STUMP].itemToDrop = ITEM_WOOD;
	breakableMap[BREAKABLE_CRAFTING_STUMP].height = 3;

	breakableMap[BREAKABLE_WALL_WOOD].maxHealth = 10;
	breakableMap[BREAKABLE_WALL_WOOD].breakableBy = ITEM_AXE;
	breakableMap[BREAKABLE_WALL_WOOD].itemToDrop = ITEM_WALL_WOOD;
	breakableMap[BREAKABLE_WALL_WOOD].height = 4;
}