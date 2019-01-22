/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

ItemBar.h 

This file is for declarations of the player item bar.
*/




#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Textures.h"
class ItemBar {
public:
	
	// the actual bar
	sf::RectangleShape bar;

	// size of the bar
	int barSize;

	// the item currently selected in the bar
	int itemSelected = 0;

	// maximum item stack size
	int maxStackSize = 100;

	// size of each grid
	int gridSize = 64;

	// whether you are switching items, and whether you are hovering.
	bool isSwitching = false, isHover = false;

	// the array of all of the items
	std::vector<ItemSection> item;

	// constructor
	ItemBar(int size) {

		// set bar size
		barSize = size;

		// reserve the space in the array
		item.reserve(size);

		// set size of the bar rect
		bar.setSize(sf::Vector2f(gridSize*size, gridSize));

		// set fill color
		bar.setFillColor(sf::Color(0, 0, 0, 20));

		// set origin
		bar.setOrigin(size * (gridSize / 2), 0);

		// set position of the bar
		bar.setPosition(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y - bar.getSize().y - 16);

		// loop through the size
		for (int i = 0; i < barSize; i++) {
			// add an item section
			item.push_back(ItemSection());

			// set size
			item.back().square.setSize(sf::Vector2f(gridSize, gridSize));

			// set up durability bars
			item.back().fullDura.setSize(sf::Vector2f(48, 8));
			item.back().fullDura.setFillColor(sf::Color(0, 0, 0, 50));
			item.back().partDura.setSize(sf::Vector2f(48, 8));
			item.back().partDura.setFillColor(sf::Color(0, 255, 0, 200));
		}

		// output confirmation
		outputInfo("Item bar has been loaded!");

		// select item
		selectItem(itemSelected);
	}

	// select item
	void selectItem(int itemNo) {

		// set current selected item to not selected
		item[itemSelected].isSelected = false;

		// select new item
		itemSelected = itemNo;
		item[itemSelected].isSelected = true;
	}

	// returns the selected item
	Item &getItemSelected() { return item[itemSelected].itm; }

	// update the item bar
	void update() {

		// set bar position (right now only here for resolution changes, please remove this later!!!!!)
		bar.setPosition(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y - bar.getSize().y - 16);

		// for every tile
		for (int i = 0; i < barSize; i++) {
			// if selected
			if (item[i].isSelected) {

				// set fill color and outline
				item[i].square.setFillColor(sf::Color(100, 100, 100, 150));
				item[i].square.setOutlineThickness(4);
				item[i].square.setOutlineColor(sf::Color(100, 200, 100));
			}

			// if not selected
			else {

				// set fill color and outline
				item[i].square.setFillColor(sf::Color(0, 0, 0, 150));
				item[i].square.setOutlineThickness(2);
				item[i].square.setOutlineColor(sf::Color(150, 150, 150, 200));
			}
			
			// set square position (also change)
			item[i].square.setPosition(bar.getPosition().x - (bar.getSize().x / 2) + (i*gridSize), bar.getPosition().y);

			// set icon position
			item[i].icon.setPosition(item[i].square.getPosition());

			// stringstream for amount
			std::ostringstream amountStr;
			amountStr << item[i].itm.amount;

			// set the amount string to be the amount
			item[i].amountTxt.setString(amountStr.str());

			// set origin
			item[i].amountTxt.setOrigin(item[i].amountTxt.getGlobalBounds().width, item[i].amountTxt.getGlobalBounds().height);
			
			// set position of amount txt
			item[i].amountTxt.setPosition(item[i].square.getPosition().x + gridSize * 0.9,
				item[i].square.getPosition().y + gridSize * 0.9);

			// set position of durability
			item[i].fullDura.setPosition(item[i].square.getPosition().x + gridSize / 8, item[i].square.getPosition().y + gridSize * 0.75);
			item[i].partDura.setPosition(item[i].fullDura.getPosition());

			// if the item is a tool, set size of part durability rect
			if (item[i].itm.isTool) item[i].partDura.setSize(sf::Vector2f((float(item[i].itm.durability) / item[i].itm.MAX_DURABILITY) * 48, 8));

			// if the item is broken and is a tool, clear it.
			if ((item[i].itm.durability <= 0 && item[i].itm.isTool) || (item[i].itm.amount <= 0 && !item[i].isEmpty)) {
				item[i].clear();
				isSwitching = true;
			}
		}
		// update bar
		updateBar();
	}
	
	// set item
	void setItem(Item itemTo, int barNo) {
		// set item
		item[barNo].itm = itemTo;
		item[barNo].icon.setTexture(itemTextureMap[itemTo.type]);
		item[barNo].isEmpty = false;
	}

	// add item
	void addItem(Item itemToAdd) {

		// local boolean to store whether its done or not
		bool done = false;
		
		// if the bar is not full
		if (!isFull()) {

			// loops through every element
			for (int i = 0; i < barSize; i++) {

				// checks if its not empty
				if (!item[i].isEmpty) {

					// if the types are the same and they're not tools (because tools don't stack)
					if (item[i].itm.type == itemToAdd.type && !itemToAdd.isTool) {

						// adds amount
						item[i].itm.amount += itemToAdd.amount;

						// is done
						done = true;
					}
				}

				// if its empty
				else {

					// sets the item
					setItem(itemToAdd, i);

					// is done
					done = true;
				}

				// break out of the loop if its done
				if (done) break;
			}
		}
		// update bar
		updateBar();
	}

	// if the bar is full
	bool isFull() {

		// loops through everything until it finds an empty
		for (int i = 0; i < barSize; i++) if (item[i].isEmpty) return false;

		// if not found, return true
		return true;
	}

	// if the bar contains an item
	bool hasItem(Item itm, int amount) {

		// loops through
		for (int i = 0; i < barSize; i++) {
			if (item[i].itm.type == itm.type && item[i].itm.amount >= itm.amount) return true;
		}

		// return false if not found
		return false;
	}

	// get the item location
	int getItemLocation(Item itm) {
		
		// loops through
		for (int i = 0; i < barSize; i++) {
			if (item[i].itm.type == itm.type && item[i].itm.amount >= itm.amount) return i;
		}

		// return -1 if not found
		return -1;
	}

	// subtract item amount from a location
	void subtractItems(int location, int amount) {
		if (item[location].itm.amount <= amount) item[location].clear();
		else item[location].itm.amount -= amount;
	}

	// return the size of the item bar
	int getSize() { return item.size(); }

	// item that it must drop
	Item itemToDrop = Item(itemMap[0], 0);
	
	// clears the bar by looping through and clearing every item
	void clear() {
		for (int i = 0; i < barSize; i++) item[i].clear();
	}

	// draw the bar
	void draw() {

		// draw background rect
		WINDOW.draw(bar);

		// draw every square
		for (int i = 0; i < barSize; i++) {
			// only draws if not selected
			if (i != itemSelected) {
				WINDOW.draw(item[i].square);
				item[i].draw();
			}
		}

		// draws the selected item last (so the outline is on top)
		WINDOW.draw(item[itemSelected].square);
		item[itemSelected].draw();
	}	
	
	// update bar using events
	void eventUpdateBar() {
		
		// if scrolling, selects next item based on mouse wheel position
		if (event.type == sf::Event::MouseWheelScrolled) {	
			int mouse_wheel = (event.mouseWheel.x < 0) ? 1 : -1;
			int itemToSelect = (mouse_wheel < 0 && itemSelected == 0) ? barSize - 1 : (mouse_wheel > 0 && itemSelected == barSize - 1) ? 0 : itemSelected + mouse_wheel;
			selectItem(itemToSelect);
		}
		
		// selects an item square with key presses (1 to 0)
		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Num1: selectItem(0); break;
			case sf::Keyboard::Num2: selectItem(1); break;
			case sf::Keyboard::Num3: selectItem(2); break;
			case sf::Keyboard::Num4: selectItem(3); break;
			case sf::Keyboard::Num5: selectItem(4); break;
			case sf::Keyboard::Num6: selectItem(5); break;
			case sf::Keyboard::Num7: selectItem(6); break;
			case sf::Keyboard::Num8: selectItem(7); break;
			case sf::Keyboard::Num9: selectItem(8); break;
			case sf::Keyboard::Num0: selectItem(9); break;
			}
		}

		// drop item by right clicking
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && GAME_CLICK_DELAY_PASSED) {

			// get the position of the mouse
			sf::Vector2i position = sf::Mouse::getPosition(WINDOW);

			// if it fits
			if (bar.getGlobalBounds().contains(sf::Vector2f(position))) {
				
				// get the square that the mouse is rightclicking
				int gridSelected = (position.x - bar.getPosition().x + (barSize / 2)*gridSize) / gridSize;

				// checks if the item is not empty
				if (item[gridSelected].itm.type != ITEM_NONE) {

					// drops item
					itemToDrop = Item(item[gridSelected].itm, item[gridSelected].itm.amount);

					// clear that square
					item[gridSelected].clear();

					// updates bar
					updateBar();
				}
				GAME_CLICK_DELAY_PASSED = false;
			}
		}
	}


private:

	// update bar by checking if there are any spaces in the item bar
	void updateBar() {

		// local item type variable
		ItemType type = ITEM_NONE;

		// if its done
		bool done = false;

		// loops through every square
		for (int i = 0; i < barSize; i++) {

			// if its done
			if (done) {
				type = ITEM_NONE;
				done = false;
			}

			// if the item type is none, continue
			if (item[i].itm.type == ITEM_NONE) continue;

			// set the item type
			type = item[i].itm.type;

			// loops through every square BEFORE this square
			for (int a = 0; a < i; a++) {

				// if that square is blank, shift down
				if (item[a].itm.type == ITEM_NONE) {

					// set item
					setItem(item[i].itm, a);

					// clear current item square
					item[i].clear();

					// yep, done
					done = true;
				}

				// if they're the same type and NOT tools
				else if (item[i].itm.type == item[a].itm.type && !item[i].itm.isTool) {

					// add on amount
					item[a].itm.amount += item[i].itm.amount;

					// clear item
					item[i].clear();

					// done
					done = true;
				}
			}
		}
	}
};
