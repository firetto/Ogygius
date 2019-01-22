/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Item.h 

This file is for declarations of the item class, and different types of items
*/




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

// class declaration for item
class Item {
public:

	// item name
	std::string name;

	// enum type var
	ItemType type = ITEM_NONE;

	// enum place type (if right clicked, does it place something?)
	BreakableType placeType = BREAKABLE_NONE;

	// amount
	int amount = 1;

	// damage that it does when punching
	int damage = 1;

	// durabilities
	int durability, MAX_DURABILITY;

	// whether it can be used (not placed, but eaten)
	bool isUsable = false;

	// how much the usable item heals for
	int healAmount = 0;

	// for special breakables that can be broken faster with special items, this is the multiplier
	float modifier = 1;

	// is the item a tool
	bool isTool = false;

	// empty constructor
	Item() {}

	// constructor taking in a reference to an existing item and an amount
	Item(Item &_item, int _amount) {
		*this = _item;
		amount = _amount;
	}
};

// map of items
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

// set up all items
void itemMapFill() {

	// air
	itemMap[ITEM_NONE].name = "Air";

	// wood
	itemMap[ITEM_WOOD].name = "Wood";
	itemMap[ITEM_WOOD].placeType = BREAKABLE_CRAFTING_STUMP; // can place a crafting stump

	// stone
	itemMap[ITEM_STONE].name = "Stone";

	// grass
	itemMap[ITEM_GRASS].name = "Grass";

	// rope
	itemMap[ITEM_ROPE].name = "Rope";

	// stick
	itemMap[ITEM_STICK].name = "Stick";

	// axe
	itemMap[ITEM_AXE].name = "Axe";
	itemMap[ITEM_AXE].MAX_DURABILITY = 100; // durability
	itemMap[ITEM_AXE].modifier = 2; // breaks wood 2x faster
	itemMap[ITEM_AXE].isTool = true; // tool yes
	itemMap[ITEM_AXE].damage = 3; // does 3 damage to mobs

	// pickaxe
	itemMap[ITEM_PICKAXE].name = "Pickaxe";
	itemMap[ITEM_PICKAXE].MAX_DURABILITY = 100; // durability
	itemMap[ITEM_PICKAXE].modifier = 3; // breaks stone 3x faster
	itemMap[ITEM_PICKAXE].isTool = true; // tool yes
	itemMap[ITEM_PICKAXE].damage = 2; // does 2x damage to mobs

	// not actually in the game yet :P
	itemMap[ITEM_CRAFT_STATION].name = "Crafting Station";

	// iron ingot
	itemMap[ITEM_IRON_INGOT].name = "Iron Ingot";

	// iron ore
	itemMap[ITEM_IRON_ORE].name = "Iron Ore";

	// wood fence
	itemMap[ITEM_WALL_WOOD].name = "Wooden Fence";
	itemMap[ITEM_WALL_WOOD].placeType = BREAKABLE_WALL_WOOD; // can be placed to make a wall

	// meat
	itemMap[ITEM_MEAT].name = "Meat";
	itemMap[ITEM_MEAT].isUsable = true; // can be used
	itemMap[ITEM_MEAT].healAmount = 1; // heal for 1 hp

	// set all durabilities to the max durability and set types
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

	// the item it holds
	Item itm;

	// the amount text
	sf::Text amountTxt;

	// the icon
	sf::Sprite icon;
	
	// whether its empty, selected, or is a tool
	bool isEmpty = true, isSelected = false, isTool = false;

	// constructor
	ItemSection() {
		// set up amount text
		amountTxt.setCharacterSize(24);
		amountTxt.setOutlineThickness(2);
		amountTxt.setOutlineColor(sf::Color::Black);
		amountTxt.setFont(gameFont);
	}

	// load texture for icon
	void setTexture(sf::Texture &texture) {
		icon.setTexture(texture);
	}

	// set item
	void setItem(Item _item) {
		// set the item
		itm = _item;

		// get the amount and set it as a string
		std::stringstream ss;
		ss << _item.amount;
		amountTxt.setString(ss.str());

		// set text origin
		amountTxt.setOrigin(amountTxt.getGlobalBounds().width, amountTxt.getGlobalBounds().height);

		// set texture of icon
		setTexture(itemTextureMap[_item.type]);
	}

	// clear
	void clear() {
		itm = itemMap[0];
		isEmpty = true;
	}

	// draw the item section
	void draw() {

		//  if not air, draw the icon
		if (itm.type != ITEM_NONE) WINDOW.draw(icon);

		// if the amount is 2 or more, draw the amount txt
		if (itm.amount > 1) WINDOW.draw(amountTxt);

		// draw the durability rects if the item is a tool
		if (itm.durability < itm.MAX_DURABILITY && itm.type != ITEM_NONE) {
			WINDOW.draw(fullDura);
			WINDOW.draw(partDura);
		}
	}

	// set position of the item
	void setPosition(sf::Vector2f pos) {
		// set position  of item
		icon.setPosition(pos);

		// set position of amount txt
		amountTxt.setPosition(icon.getPosition().x + icon.getGlobalBounds().width * 0.9,
			icon.getPosition().y + icon.getGlobalBounds().height * 0.9);
	}
};