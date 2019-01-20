#include <SFML/Graphics.hpp>
#include <vector>
#include "ItemBar.h"

// class for a crafting recipe
struct CraftingRecipe {

	// list of ingredients
	std::vector<Item> ingredients;

	// icon for the product
	ItemSection productIcon;

	// product item
	Item product;

	// constructor that takes in 3 ingredients
	CraftingRecipe(Item ingredient1, Item ingredient2, Item ingredient3, Item prod) {
		ingredients.push_back(ingredient1);
		ingredients.push_back(ingredient2);
		ingredients.push_back(ingredient3);
		product = prod;
		productIcon.setItem(product);
	}

	// constructor that takes in 2 ingredients
	CraftingRecipe(Item ingredient1, Item ingredient2, Item prod) {
		ingredients.push_back(ingredient1);
		ingredients.push_back(ingredient2);
		product = prod;
		productIcon.setItem(product);
	}

	// one ingredient
	CraftingRecipe(Item ingredient, Item prod) {
		ingredients.push_back(ingredient);
		product = prod;
		productIcon.setItem(product);
	}
	
	// empty constructor
	CraftingRecipe() {}
};
class CraftingHUD {
public:
	// constructor for the crafting HUD
	CraftingHUD() {

		// sets up background rectangle
		background.setSize(sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2));
		background.setFillColor(sf::Color(0, 0, 0, 128));
		background.setOutlineThickness(4);
		background.setOutlineColor(sf::Color(255, 255, 255, 128));
		background.setPosition(WINDOW_DIMENSIONS.x / 4, WINDOW_DIMENSIONS.y / 4);

		// sets up divider rectangle
		dividerLine.setSize(sf::Vector2f(4, background.getSize().y - 16));
		dividerLine.setFillColor(sf::Color(255, 255, 255, 150));
		dividerLine.setPosition(background.getPosition().x + 80, background.getPosition().y + 8);

		// cleares recipes and list of plus icons (just in case)
		recipes.clear(); productPlus.clear();

		// sets texture of arrow icon, as well as the sprite position in the sprite sheet
		productArrow.setTexture(hud_craftingTex); productArrow.setTextureRect(sf::IntRect(64, 0, 64, 64));

		// sets up craft button
		craftButton.setSize(sf::Vector2f(128, 64));
		craftButton.setFillColor(sf::Color(0, 200, 0));
		craftButton.setPosition(background.getPosition().x + background.getSize().x - craftButton.getSize().x - 16,
			background.getPosition().y + background.getSize().y - craftButton.getSize().y - 16);

		// sets up craft text in button
		craftText.setCharacterSize(32);
		craftText.setFont(gameFont);
		craftText.setString("Craft");
		craftText.setOrigin(craftText.getGlobalBounds().width / 2, craftText.getCharacterSize() / 2);
		craftText.setPosition(craftButton.getPosition().x + craftButton.getSize().x / 2, craftButton.getPosition().y + craftButton.getSize().y / 2);
		
		
		
		// adds recipes
		for (int i = 0; i < recipes.size(); i++) {
			for (int a = 0; a < recipes[i].ingredients.size(); a++) {
				itemSections.push_back(ItemSection());
				itemSections.back().setItem(recipes[i].ingredients[a]);
			}
		}

	}

	// the "level" of the crafting (crafting station has not yet been added)
	static enum HUDLevel { STUMP, STATION };

	// opens hud
	void open(HUDLevel _level) {
		if (!_open) {
			level = _level;
			// if it hasn't been opened, set up.
			setup();

			// sets open to true
			_open = true;
		}
	}

	// close
	void close() {
		_open = false;
	}

	// returns whether it is open
	bool isOpen() {
		return _open;
	}

	// whether the mouse is INSIDE of the HUD rect
	bool mouseInHUD() {
		return background.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(WINDOW)));
	}

	// draws everything to the screen
	void draw() {
		if (_open) {
			// draws background rect
			WINDOW.draw(background);

			// draw divider line
			WINDOW.draw(dividerLine);

			// draw recipe product icons
			for (int i = 0; i < recipes.size(); i++) {
				recipes[i].productIcon.draw();
			}

			// draw item section icons
			for (int i = 0; i < itemSections.size(); i++) {
				itemSections[i].draw();
			}

			// draw the plus icons
			for (int i = 0; i < productPlus.size(); i++) {
				WINDOW.draw(productPlus[i]);
			}

			// draw product arrow
			WINDOW.draw(productArrow);

			// if you can craft, draw the button
			if (canCraft) {
				WINDOW.draw(craftButton);
				WINDOW.draw(craftText);
			}
		}
	}
	
	// update
	void update(ItemBar &bar) {
		if (_open) {
			// handle clicks
			handleClick(bar);

			// allow crafting
			bool _craftAllow = true;

			// checks if the player has all of the ingredients to craft the selected recipe
			for (int i = 0; i < selectedRecipe.ingredients.size(); i++) {
				int getLocation = bar.getItemLocation(selectedRecipe.ingredients[i]);
				if (getLocation == -1) _craftAllow = false;
			}
			// set can craft to true
			canCraft = _craftAllow;

			// update mouse in hud bool
			MOUSE_IN_HUD = mouseInHUD();
		}
		else MOUSE_IN_HUD = false;
	}
private:
	// declarations for rectangles
	sf::RectangleShape background, dividerLine, craftButton;

	// craft text
	sf::Text craftText;

	// array of recipes
	std::vector<CraftingRecipe> recipes;

	// array of item sections
	std::vector<ItemSection> itemSections;

	// selected recipe
	CraftingRecipe selectedRecipe;

	// product arrow
	sf::Sprite productArrow;

	// array of plus icons
	std::vector<sf::Sprite> productPlus;

	// the level of the crafting table
	HUDLevel level;

	// sets up
	void setup() {
		// clears recipes
		recipes.clear();
		
		// adds all recipes
		if (level == STUMP) {
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_GRASS], 3), itemMap[ITEM_ROPE]));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_WOOD], 1), Item(itemMap[ITEM_STICK], 3)));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_ROPE], 2), Item(itemMap[ITEM_STICK], 3), Item(itemMap[ITEM_STONE], 2), itemMap[ITEM_AXE]));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_ROPE], 2), Item(itemMap[ITEM_STICK], 3), Item(itemMap[ITEM_STONE], 2), itemMap[ITEM_PICKAXE]));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_STICK],	1), Item(itemMap[ITEM_WALL_WOOD], 50)));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_ROPE], 2), Item(itemMap[ITEM_STICK], 3), Item(itemMap[ITEM_WOOD], 5), Item(itemMap[ITEM_WALL_WOOD], 8)));
		}

		// sets position of product icons
		for (int i = 0; i < recipes.size(); i++) {
			recipes[i].productIcon.setPosition(sf::Vector2f(background.getPosition().x + background.getOutlineThickness() * 2, background.getPosition().y + i*(recipes[i].productIcon.icon.getGlobalBounds().height + 8)));
		}

		// opens the first recipe
		openRecipe(0);
	}
	void openRecipe(int n) {
		// clears item sections
		itemSections.clear();

		// removes all plus icons
		productPlus.clear();

		// set selected recipe
		selectedRecipe = CraftingRecipe(recipes[n]);

		// for every ingrdient
		for (int i = 0; i < recipes[n].ingredients.size(); i++) {

			// add item section
			itemSections.push_back(ItemSection());

			// set item section to be the item
			itemSections.back().setItem(recipes[n].ingredients[i]);

			// set position of item section
			itemSections.back().setPosition(sf::Vector2f(background.getPosition().x + 96 + i * (itemSections.back().icon.getGlobalBounds().width + 72), background.getPosition().y + background.getOutlineThickness() * 2));

			// adds plus icons
			if (i < recipes[n].ingredients.size() - 1) {
				productPlus.push_back(sf::Sprite(hud_craftingTex)); productPlus.back().setTextureRect(sf::IntRect(0, 0, 64, 64));
				productPlus.back().setPosition(itemSections.back().icon.getPosition().x + itemSections.back().icon.getGlobalBounds().width + 8, itemSections.back().icon.getPosition().y);
			}
		}
		// sets position of arrow icon
		productArrow.setPosition(itemSections[itemSections.size() - 1].icon.getPosition().x + itemSections[itemSections.size() - 1].icon.getGlobalBounds().width + 8, itemSections[itemSections.size() - 1].icon.getPosition().y);
		
		// adds product item section
		itemSections.push_back(ItemSection());

		// set the item icon to be the product
		itemSections.back().setItem(recipes[n].product);

		// set position of product item section
		itemSections.back().setPosition(sf::Vector2f(productArrow.getPosition().x + productArrow.getGlobalBounds().width + 8, productArrow.getPosition().y));
	}

	// function for handling HUD clicks
	void handleClick(ItemBar &bar) {
		// if open and the mouse is inside of the HUD
		if (_open && mouseInHUD()) {

			// if the mouse button is hovering over the craft button, has clicked, and can craft
			if (craftButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(WINDOW))) && canCraft && sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && GAME_CLICK_DELAY_PASSED) {
				
				// subtracts items from item bar
				for (int i = 0; i < selectedRecipe.ingredients.size(); i++) {
					bar.subtractItems(bar.getItemLocation(selectedRecipe.ingredients[i]), selectedRecipe.ingredients[i].amount);
				}

				// adds product item to bar
				bar.addItem(selectedRecipe.product);
				GAME_CLICK_DELAY_PASSED = false;
			}
			else {
				for (int i = 0; i < recipes.size(); i++) {
					// if the mouse is hovering over the product list (on the left)
					if (recipes[i].productIcon.icon.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(WINDOW)))
						&& sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
						// if he has clicked, open the selected recipe
						openRecipe(i);
					}
				}
			}
		}
	}
	bool _open = false, canCraft = false;
};
CraftingHUD craftHUD;