#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

// dropped item class
class DroppedItem;

// array of item drops
std::vector<DroppedItem> dropItemVector;

// class declaration for item drop
class DroppedItem : public Entity {
public:
	// the item
	Item item;

	// outline sprite
	sf::Sprite outline;

	// whether you can pick it up
	bool canPickUp = false;

	// clock
	sf::Clock clock;

	// constructor with the item to drop
	DroppedItem(Item &itemInput) : Entity() {

		// set the height
		height = 2;

		// set th eitem
		item = itemInput;

		// setup outline sprite
		outline.setTexture(lootOutlineTex);
		outline.setOrigin(outline.getGlobalBounds().width / 2, outline.getGlobalBounds().height / 2);
		outline.setRotation(45);
		outline.setScale(0, 0);

		// set up item sprite
		vis.setTexture(itemTextureMap[itemInput.type]);
		vis.setOrigin(vis.getGlobalBounds().width / 2, vis.getGlobalBounds().height / 2);
		vis.setScale(0, 0);
	}

	// item for zooming in the icon
	void itemLoad() {
		// since the item zoom is quadratic, this is the a variable for the parabola
		int aVal = 6;

		// calculate scale
		float scale = pow(clock.getElapsedTime().asSeconds(), 2) * aVal;

		// if the scale is less than one, set it to be the scale
		if (vis.getScale().x < 1) {
			vis.setScale(scale, scale);
			outline.setScale(vis.getScale());
		}

		// otherwise, allow the player to pick it up
		if (scale >= 1 && clock.getElapsedTime().asSeconds() > sqrt(aVal) / aVal + 0.5) canPickUp = true;
	}

	// physics
	void dropItemPhysics() {
		// if the item type is none, delete.
		if (item.type == ITEM_NONE) isDeleted = true;

		// physics
		physics();
		
		// if you can't pick up the item, zoom in
		if (!canPickUp) itemLoad();

		// set position of outline
		outline.setPosition(Object::getPosition());

		// draw
		dropItemDraw();
	}
	
	// draw function
	void dropItemDraw() {
		// draw
		draw();

		// add 
		drawableVector.emplace_back(ObjDrawable(outline, height-0.1));
	}
};

// spawn an item
void spawnItem(Item item, sf::Vector2f newPos, sf::Vector2f newAccel) {
	
	// if the amount of the item is 0, return
	if (item.amount < 1) return;

	// push back
	dropItemVector.push_back(item);

	// set the texture
	dropItemVector.back().vis.setTexture(itemTextureMap[item.type]);

	// set position
	dropItemVector.back().Object::setPosition(newPos);

	// set acceleration
	dropItemVector.back().setAcceleration(newAccel);
}