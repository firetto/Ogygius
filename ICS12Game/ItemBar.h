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

	ItemBar(int size) {
		barSize = size;
		item.reserve(size);
		bar.setSize(sf::Vector2f(gridSize*size, gridSize));
		bar.setFillColor(sf::Color(0, 0, 0, 20));
		bar.setOrigin(size * (gridSize / 2), 0);
		for (int i = 0; i < barSize; i++) {
			item.push_back(ItemSection());
			item.back().square.setSize(sf::Vector2f(gridSize, gridSize));

			item.back().fullDura.setSize(sf::Vector2f(48, 8));
			item.back().fullDura.setFillColor(sf::Color(0, 0, 0, 50));
			item.back().partDura.setSize(sf::Vector2f(48, 8));
			item.back().partDura.setFillColor(sf::Color(0, 255, 0, 200));
		}
		outputInfo("Item bar has been loaded!");
		selectItem(itemSelected);
		addItem(Item(itemMap[ITEM_AXE]));
		//addItem(Item(itemMap[ITEM_WALL_WOOD], 999999));
	}
	void selectItem(int itemNo) {
		item[itemSelected].isSelected = false;
		itemSelected = itemNo;
		item[itemSelected].isSelected = true;
	}
	Item &getItemSelected() { return item[itemSelected].itm; }
	void update() {
		bar.setPosition(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y - bar.getSize().y  - 16);
		for (int i = 0; i < barSize; i++) {
			if (item[i].isSelected) {
				item[i].square.setFillColor(sf::Color(100, 100, 100, 150));
				item[i].square.setOutlineThickness(4);
				item[i].square.setOutlineColor(sf::Color(100, 200, 100));
			}
			else {
				item[i].square.setFillColor(sf::Color(0, 0, 0, 150));
				item[i].square.setOutlineThickness(2);
				item[i].square.setOutlineColor(sf::Color(150, 150, 150, 200));
			}
			item[i].square.setPosition(bar.getPosition().x - (bar.getSize().x / 2) + (i*gridSize), bar.getPosition().y);
			item[i].icon.setPosition(item[i].square.getPosition());
			std::ostringstream amountStr;
			amountStr << item[i].itm.amount;
			item[i].amountTxt.setString(amountStr.str());
			item[i].amountTxt.setOrigin(item[i].amountTxt.getGlobalBounds().width, item[i].amountTxt.getGlobalBounds().height);
			item[i].amountTxt.setPosition(item[i].square.getPosition().x + gridSize * 0.9,
				item[i].square.getPosition().y + gridSize * 0.9);
			item[i].fullDura.setPosition(item[i].square.getPosition().x + gridSize / 8, item[i].square.getPosition().y + gridSize * 0.75);
			item[i].partDura.setPosition(item[i].fullDura.getPosition());

			if (item[i].itm.isTool) item[i].partDura.setSize(sf::Vector2f((float(item[i].itm.durability) / item[i].itm.MAX_DURABILITY) * 48, 8));

			if ((item[i].itm.durability <= 0 && item[i].itm.isTool) || (item[i].itm.amount <= 0 && !item[i].isEmpty)) {
				item[i].clear();
				isSwitching = true;
			}
		}
		updateBar();
	}
	void setItem(Item itemTo, int barNo) {
		item[barNo].itm = itemTo;
		item[barNo].icon.setTexture(itemTextureMap[itemTo.type]);
		item[barNo].isEmpty = false;
	}
	void addItem(Item itemToAdd) {
		bool done = false;
		if (!isFull()) {
			for (int i = 0; i < barSize; i++) {
				if (!item[i].isEmpty) {
					if (item[i].itm.type == itemToAdd.type && !itemToAdd.isTool) {
						//std::cout << "Same item! " << item[i].itm.type << std::endl;
						item[i].itm.amount += itemToAdd.amount;
						done = true;
					}
				}
				else {
					std::cout << "Different item!" << std::endl;
					setItem(itemToAdd, i);
					done = true;
				}
				if (done) break;
			}
		}
		updateBar();
	}
	bool isFull() {
		for (int i = 0; i < barSize; i++) if (item[i].isEmpty) return false;
		return true;
	}
	bool hasItem(Item itm, int amount) {
		for (int i = 0; i < barSize; i++) {
			if (item[i].itm.type == itm.type && item[i].itm.amount >= itm.amount) return true;
		}
		return false;
	}
	int getItemLocation(Item itm) {
		for (int i = 0; i < barSize; i++) {
			if (item[i].itm.type == itm.type && item[i].itm.amount >= itm.amount) return i;
		}
		return -1;
	}
	void subtractItems(int location, int amount) {
		if (item[location].itm.amount <= amount) item[location].clear();
		else item[location].itm.amount -= amount;
	}
	int getSize() { return item.size(); }
	Item itemToDrop = Item(itemMap[0], 0);

	void draw() {
		WINDOW.draw(bar);
		for (int i = 0; i < barSize; i++) {
			if (i != itemSelected) {
				WINDOW.draw(item[i].square);
				item[i].draw();
			}

		}
		WINDOW.draw(item[itemSelected].square);
		item[itemSelected].draw();
	}	

	void eventUpdateBar() {
		// update current item
		if (event.type == sf::Event::MouseWheelScrolled) {	
			int mouse_wheel = (event.mouseWheel.x < 0) ? 1 : -1;
			int itemToSelect = (mouse_wheel < 0 && itemSelected == 0) ? barSize - 1 : (mouse_wheel > 0 && itemSelected == barSize - 1) ? 0 : itemSelected + mouse_wheel;
			selectItem(itemToSelect);
		}
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
			if (event.key.code == sf::Keyboard::Num1) selectItem(0);
			else if (event.key.code == sf::Keyboard::Num1) selectItem(0);
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && GAME_CLICK_DELAY_PASSED) {
			sf::Vector2i position = sf::Mouse::getPosition(WINDOW);

			if (bar.getGlobalBounds().contains(sf::Vector2f(position))) {
				int gridSelected = (position.x - bar.getPosition().x + (barSize / 2)*gridSize) / gridSize;
				if (item[gridSelected].itm.type != ITEM_NONE) {
					itemToDrop = Item(item[gridSelected].itm, item[gridSelected].itm.amount);
					//itemToDrop.type = item[gridSelected].itm.type;
					item[gridSelected].clear();
					updateBar();
				}
				GAME_CLICK_DELAY_PASSED = false;
			}
		}
	}


private:
	void updateBar() {
		ItemType type = ITEM_NONE;
		bool done = false;
		for (int i = 0; i < barSize; i++) {
			if (done) {
				type = ITEM_NONE;
				done = false;
			}

			if (item[i].itm.type == ITEM_NONE) continue;

			type = item[i].itm.type;

			for (int a = 0; a < i; a++) {
				if (item[a].itm.type == ITEM_NONE) {
					setItem(item[i].itm, a);
					item[i].clear();
					done = true;
				}
				else if (item[i].itm.type == item[a].itm.type && !item[i].itm.isTool) {
					item[a].itm.amount += item[i].itm.amount;
					item[i].clear();
					done = true;
				}
			}
		}
	}
};
