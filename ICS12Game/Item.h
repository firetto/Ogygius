#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Declarations.h"
#include "Camera.h"
#include "Item.h"
#include <sstream>

class Item {
public:
	std::string name;

	ItemType type = ITEM_NONE;
	BreakableType placeType = BREAKABLE_NONE;
	int amount = 1;
	int damage = 1;
	int durability, MAX_DURABILITY;
	bool isUsable = false;
	int healAmount = 0;
	float modifier = 1;
	bool isTool = false;
	Item() {}
	Item(Item &_item, int _amount) {
		*this = _item;
		amount = _amount;
	}
};
std::map<int, Item> itemMap{
	{ 0, Item() }, // none
{ 1, Item() }, // wood
{ 2, Item() }, // stone
{ 3, Item() }, // grass
{ 4, Item() }, // rope
{ 5, Item() }, // axe
{ 6, Item() }, // pickaxe
{ 7, Item() }, // crafting station
{ 8, Item() }, // stick
{ 9, Item() }, // iron ingot
{ 10, Item() }, // iron ore
{ 11, Item() }, // meat
};

void itemMapFill() {
	itemMap[ITEM_NONE].name = "Air";

	itemMap[ITEM_WOOD].name = "Wood";
	itemMap[ITEM_WOOD].placeType = BREAKABLE_CRAFTING_STUMP;

	itemMap[ITEM_STONE].name = "Stone";

	itemMap[ITEM_GRASS].name = "Grass";

	itemMap[ITEM_ROPE].name = "Rope";

	itemMap[ITEM_STICK].name = "Stick";

	itemMap[ITEM_AXE].name = "Axe";
	itemMap[ITEM_AXE].MAX_DURABILITY = 100;
	itemMap[ITEM_AXE].modifier = 2;
	itemMap[ITEM_AXE].isTool = true;
	itemMap[ITEM_AXE].damage = 3;

	itemMap[ITEM_PICKAXE].name = "Pickaxe";
	itemMap[ITEM_PICKAXE].MAX_DURABILITY = 100;
	itemMap[ITEM_PICKAXE].modifier = 3;
	itemMap[ITEM_PICKAXE].isTool = true;
	itemMap[ITEM_PICKAXE].damage = 2;

	itemMap[ITEM_CRAFT_STATION].name = "Crafting Station";


	itemMap[ITEM_IRON_INGOT].name = "Iron Ingot";

	itemMap[ITEM_IRON_ORE].name = "Iron Ore";

	itemMap[ITEM_WALL_WOOD].name = "Wooden Fence";
	itemMap[ITEM_WALL_WOOD].placeType = BREAKABLE_WALL_WOOD;

	itemMap[ITEM_MEAT].name = "Meat";
	itemMap[ITEM_MEAT].isUsable = true;
	itemMap[ITEM_MEAT].healAmount = 1;

	for (int i = 1; i < itemMap.size(); i++) {
		itemMap[i].durability = itemMap[i].MAX_DURABILITY;
		itemMap[i].type = ItemType(i);
	}
}

struct ItemSection {
	// the actual square outline
	sf::RectangleShape square;

	// if the weapon is a tool, they have durabilities, and the partDura is a fraction of the size of fullDura
	sf::RectangleShape fullDura, partDura;
	Item itm;
	sf::Text amountTxt;
	sf::Sprite icon;
	bool isEmpty = true, isSelected = false, isTool = false;
	ItemSection() {
		amountTxt.setCharacterSize(24);
		amountTxt.setOutlineThickness(2);
		amountTxt.setOutlineColor(sf::Color::Black);
		amountTxt.setFont(gameFont);
	}
	void setTexture(sf::Texture &texture) {
		icon.setTexture(texture);
	}
	void setItem(Item _item) {
		itm = _item;
		std::stringstream ss;
		ss << _item.amount;
		amountTxt.setString(ss.str());
		amountTxt.setOrigin(amountTxt.getGlobalBounds().width, amountTxt.getGlobalBounds().height);
		setTexture(itemTextureMap[_item.type]);
	}
	void clear() {
		itm = itemMap[0];
		isEmpty = true;
	}
	void draw() {
		if (itm.type != ITEM_NONE) WINDOW.draw(icon);
		if (itm.amount > 1) WINDOW.draw(amountTxt);
		if (itm.durability < itm.MAX_DURABILITY && itm.type != ITEM_NONE) {
			WINDOW.draw(fullDura);
			WINDOW.draw(partDura);
		}
	}
	void setPosition(sf::Vector2f pos) {
		icon.setPosition(pos);
		amountTxt.setPosition(icon.getPosition().x + icon.getGlobalBounds().width * 0.9,
			icon.getPosition().y + icon.getGlobalBounds().height * 0.9);
	}
};