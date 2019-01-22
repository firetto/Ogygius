/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

GameHUD.h

This file is for the declarations of the Game HUD (health bar, item bar)
*/




#pragma once
#include <SFML/Graphics.hpp>
#include "ItemBar.h"
#include "Player.h"

// game hud class
class GameHUD {
public:
	// empty constructor
	GameHUD() {}

	// constructor with player
	GameHUD(Player &p) {
		// set the player pointer to parameter
		player = &p;

		// hp box set up
		hpBox.setSize(sf::Vector2f(player->items.gridSize / 2.5, player->items.gridSize / 2.5));
		hpBox.setFillColor(sf::Color(255, 32, 32, 200));
		hpBox.setOutlineThickness(2);
		hpBox.setOutlineColor(sf::Color(64, 64, 64, 200));

		// add the hp boxes and set their positions
		for (int i = 0; i < player->getHealth(); i++) {
			hpBoxes.push_back(hpBox);
			hpBoxes.back().setPosition(player->items.bar.getPosition().x - player->items.bar.getSize().x / 2 + (i*hpBox.getSize().x*1.33), player->items.bar.getPosition().y - player->items.gridSize * 0.75);
		}
	}

	// update the hud
	void update() {

		// update the player
		player->items.update();

		// update health boxes
		for (int i = 0; i < player->getHealth(); i++) {
			hpBoxes[i].setPosition(player->items.bar.getPosition().x - player->items.bar.getSize().x / 2 + (i*hpBox.getSize().x*1.33), player->items.bar.getPosition().y - player->items.gridSize * 0.75);
		}
	}

	// event update
	void eventUpdate() {
		// update the item bar with events
		player->items.eventUpdateBar();
	}

	// draw hud
	void draw() {
		// draw item bar
		player->items.draw();

		// draw hp boxes
		for (int i = 0; i < player->getHealth(); i++) WINDOW.draw(hpBoxes[i]);
	}
private:
	// one hp box template
	sf::RectangleShape hpBox;

	// hp boxes list
	std::vector<sf::RectangleShape> hpBoxes;

	// player pointer
	Player *player;
};

//  game hud
GameHUD gameHUD;