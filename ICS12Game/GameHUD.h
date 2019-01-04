#pragma once
#include <SFML/Graphics.hpp>
#include "ItemBar.h"
#include "Player.h"

class GameHUD {
public:
	GameHUD() {}
	GameHUD(Player &p) {
		player = &p;
		hpBox.setSize(sf::Vector2f(player->items.gridSize / 2.5, player->items.gridSize / 2.5));
		hpBox.setFillColor(sf::Color(255, 0, 0, 200));
		hpBox.setOutlineThickness(2);
		hpBox.setOutlineColor(sf::Color(128, 128, 128, 200));
		std::cout << "aaa" << player->getHealth() << std::endl;
		for (int i = 0; i < player->getHealth(); i++) {
			hpBoxes.push_back(hpBox);
			hpBoxes.back().setPosition(player->items.bar.getPosition().x - player->items.bar.getSize().x / 2 + (i*hpBox.getSize().x*1.33), player->items.bar.getPosition().y - player->items.gridSize * 0.75);
		}
	}
	void update() {
		player->items.update();
	}
	void eventUpdate() {
		player->items.eventUpdateBar();
	}
	void draw() {
		player->items.draw();
		for (int i = 0; i < player->getHealth(); i++) {
			WINDOW.draw(hpBoxes[i]);
		}
	}
private:
	sf::RectangleShape hpBox;
	std::vector<sf::RectangleShape> hpBoxes;
	Player *player;
};
GameHUD gameHUD;