#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
class DroppedItem : public Entity {
public:
	Item item;
	sf::Sprite outline;
	bool canPickUp = false;
	sf::Clock clock;
	DroppedItem(Item &itemInput) : Entity() {
		height = 2;
		item = itemInput;
		outline.setTexture(lootOutlineTex);
		outline.setOrigin(outline.getGlobalBounds().width / 2, outline.getGlobalBounds().height / 2);
		vis.setTexture(itemTextureMap[itemInput.type]);
		vis.setOrigin(vis.getGlobalBounds().width / 2, vis.getGlobalBounds().height / 2);
		outline.setRotation(45);
		vis.setScale(0, 0);
		outline.setScale(0, 0);
	}
	void itemLoad() {
		int aVal = 6;
		float scale = pow(clock.getElapsedTime().asSeconds(), 2) * aVal;
		if (vis.getScale().x < 1) {
			vis.setScale(scale, scale);
			outline.setScale(vis.getScale());
		}
		if (scale >= 1 && clock.getElapsedTime().asSeconds() > sqrt(aVal) / aVal + 0.5) canPickUp = true;
	}
	void dropItemPhysics() {
		if (item.type == ITEM_NONE) isDeleted = true;
		physics();
		if (!canPickUp) itemLoad();
		outline.setPosition(Object::getPosition());
		dropItemDraw();
	}
	void dropItemDraw() {
		draw();
		drawableVector.emplace_back(ObjDrawable(outline, height-0.1));

	}
};
std::vector<DroppedItem> dropItemVector;
void spawnItem(Item item, sf::Vector2f newPos, sf::Vector2f newAccel) {
	dropItemVector.push_back(item);
	dropItemVector.back().vis.setTexture(itemTextureMap[item.type]);
	dropItemVector.back().Object::setPosition(newPos);
	dropItemVector.back().setAcceleration(newAccel);
}