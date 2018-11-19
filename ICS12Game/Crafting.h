#include <SFML/Graphics.hpp>
#include <vector>
#include "ItemBar.h"

struct CraftingRecipe {
	std::vector<Item> ingredients;
	ItemSection productIcon;
	Item product;

	CraftingRecipe(Item ingredient1, Item ingredient2, Item ingredient3, Item prod) {
		ingredients.push_back(ingredient1);
		ingredients.push_back(ingredient2);
		ingredients.push_back(ingredient3);
		product = prod;
		productIcon.setItem(product);
	}
	CraftingRecipe(Item ingredient1, Item ingredient2, Item prod) {
		ingredients.push_back(ingredient1);
		ingredients.push_back(ingredient2);
		product = prod;
		productIcon.setItem(product);
	}
	CraftingRecipe(Item ingredient, Item prod) {
		ingredients.push_back(ingredient);
		product = prod;
		productIcon.setItem(product);
	}
	CraftingRecipe() {}
};
class CraftingHUD {
public:
	CraftingHUD() {
		background.setSize(sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2));
		background.setFillColor(sf::Color(0, 0, 0, 128));
		background.setOutlineThickness(4);
		background.setOutlineColor(sf::Color(255, 255, 255, 128));

		dividerLine.setSize(sf::Vector2f(4, background.getSize().y - 16));
		dividerLine.setFillColor(sf::Color(255, 255, 255, 150));
		recipes.clear(); productPlus.clear();
		productArrow.setTexture(hud_craftingTex); productArrow.setTextureRect(sf::IntRect(64, 0, 64, 64));

		craftButton.setSize(sf::Vector2f(128, 64));
		craftButton.setFillColor(sf::Color(0, 200, 0));

		craftText.setCharacterSize(32);
		craftText.setFont(gameFont);
		craftText.setString("CRAFT");
		craftText.setOrigin(craftText.getGlobalBounds().width / 2, craftText.getGlobalBounds().height / 2);

		background.setPosition(WINDOW_DIMENSIONS.x / 4, WINDOW_DIMENSIONS.y / 4);
		dividerLine.setPosition(background.getPosition().x + 80, background.getPosition().y + 8);
		craftButton.setPosition(background.getPosition().x + background.getSize().x - craftButton.getSize().x - 16, 
								background.getPosition().y + background.getSize().y - craftButton.getSize().y - 16);
		craftText.setPosition(craftButton.getPosition().x + craftButton.getSize().x / 2, craftButton.getPosition().y + craftButton.getSize().y / 2);
		for (int i = 0; i < recipes.size(); i++) {
			for (int a = 0; a < recipes[i].ingredients.size(); a++) {
				itemSections.push_back(ItemSection());
				itemSections.back().setItem(recipes[i].ingredients[a]);
			}
		}

	}

	static enum HUDLevel { STUMP, STATION };
	void open(HUDLevel _level) {
		if (!_open) {
			level = _level;
			setup();
			_open = true;
		}
	}
	void close() {
		_open = false;
	}
	bool isOpen() {
		return _open;
	}
	bool mouseInHUD() {
		return background.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(WINDOW)));
	}
	void draw() {
		if (_open) {
			WINDOW.draw(background);
			WINDOW.draw(dividerLine);
			for (int i = 0; i < recipes.size(); i++) {
				recipes[i].productIcon.draw();
			}
			for (int i = 0; i < itemSections.size(); i++) {
				itemSections[i].draw();
			}
			for (int i = 0; i < productPlus.size(); i++) {
				WINDOW.draw(productPlus[i]);
			}
			WINDOW.draw(productArrow);
			if (canCraft) {
				WINDOW.draw(craftButton);
				WINDOW.draw(craftText);
			}
		}
	}
	void update(ItemBar &bar) {
		if (_open) {
			handleClick(bar);
			bool _craftAllow = true;
			for (int i = 0; i < selectedRecipe.ingredients.size(); i++) {
				int getLocation = bar.getItemLocation(selectedRecipe.ingredients[i]);
				if (getLocation == -1) _craftAllow = false;
			}
			canCraft = _craftAllow;
			MOUSE_IN_HUD = mouseInHUD();
		}
		else MOUSE_IN_HUD = false;
	}
private:
	sf::RectangleShape background, dividerLine, craftButton;
	sf::Text craftText;
	std::vector<CraftingRecipe> recipes;
	std::vector<ItemSection> itemSections;
	CraftingRecipe selectedRecipe;
	sf::Sprite productArrow;
	std::vector<sf::Sprite> productPlus;
	HUDLevel level;

	void setup() {
		recipes.clear();
		if (level == STUMP) {
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_GRASS], 3), itemMap[ITEM_ROPE]));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_WOOD], 1), Item(itemMap[ITEM_STICK], 3)));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_ROPE], 2), Item(itemMap[ITEM_STICK], 3), Item(itemMap[ITEM_STONE], 2), itemMap[ITEM_AXE]));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_ROPE], 2), Item(itemMap[ITEM_STICK], 3), Item(itemMap[ITEM_STONE], 2), itemMap[ITEM_PICKAXE]));
			recipes.push_back(CraftingRecipe(Item(itemMap[ITEM_ROPE], 2), Item(itemMap[ITEM_STICK], 3), Item(itemMap[ITEM_WOOD], 5), Item(itemMap[ITEM_WALL_WOOD], 4)));
		}
		for (int i = 0; i < recipes.size(); i++) {
			recipes[i].productIcon.setPosition(sf::Vector2f(background.getPosition().x + background.getOutlineThickness() * 2, background.getPosition().y + i*(recipes[i].productIcon.icon.getGlobalBounds().height + 8)));
		}
		openRecipe(0);
	}
	void openRecipe(int n) {
		itemSections.clear();
		productPlus.clear();
		selectedRecipe = CraftingRecipe(recipes[n]);
		for (int i = 0; i < recipes[n].ingredients.size(); i++) {
			itemSections.push_back(ItemSection());
			itemSections.back().setItem(recipes[n].ingredients[i]);
			itemSections.back().setPosition(sf::Vector2f(background.getPosition().x + 96 + i * (itemSections.back().icon.getGlobalBounds().width + 72), background.getPosition().y + background.getOutlineThickness() * 2));
			if (i < recipes[n].ingredients.size() - 1) {
				productPlus.push_back(sf::Sprite(hud_craftingTex)); productPlus.back().setTextureRect(sf::IntRect(0, 0, 64, 64));
				productPlus.back().setPosition(itemSections.back().icon.getPosition().x + itemSections.back().icon.getGlobalBounds().width + 8, itemSections.back().icon.getPosition().y);
			}
		}
		productArrow.setPosition(itemSections[itemSections.size() - 1].icon.getPosition().x + itemSections[itemSections.size() - 1].icon.getGlobalBounds().width + 8, itemSections[itemSections.size() - 1].icon.getPosition().y);
		itemSections.push_back(ItemSection());
		itemSections.back().setItem(recipes[n].product);
		itemSections.back().setPosition(sf::Vector2f(productArrow.getPosition().x + productArrow.getGlobalBounds().width + 8, productArrow.getPosition().y));
	}
	void handleClick(ItemBar &bar) {
		if (_open) {
			if (mouseInHUD()) {
				if (craftButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(WINDOW)))) {
					if (canCraft && sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && GAME_CLICK_DELAY_PASSED) {
						for (int i = 0; i < selectedRecipe.ingredients.size(); i++) {
							bar.subtractItems(bar.getItemLocation(selectedRecipe.ingredients[i]), selectedRecipe.ingredients[i].amount);
						}
						bar.addItem(selectedRecipe.product);
						GAME_CLICK_DELAY_PASSED = false;
					}
				}
				else {
					for (int i = 0; i < recipes.size(); i++) {
						if (recipes[i].productIcon.icon.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(WINDOW)))
							&& sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
							openRecipe(i);
						}
					}
				}
			}
		}
	}
	bool _open = false, canCraft = false;
};
CraftingHUD craftHUD;