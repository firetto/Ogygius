#pragma once
#include "Player.h"
#include "WorldGeneration.h"
#include "AnimationDeclaration.h"
void gameSetup() {
	// set antialias level of window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	WINDOW.create(sf::VideoMode(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y), WINDOW_NAME, sf::Style::Close, settings);

	// load all of the textures
	textureLoad();

	// load items
	itemMapFill();

	// declar3e breakables
	declareBreakables();

	// load animations
	animationLoad();
	
	// generate the world
	generateWorld();

	declareMobs();

	for (int i = 0; i < 1; i++) {
		mobVector.push_back(Mob(MOB_COW));
		mobVector.back().setPosition(sf::Vector2f(GAME_TOTAL_SIZE / 2 +getRandomInt(-40,40), GAME_TOTAL_SIZE / 2 + getRandomInt(-40, 40)));
	}
}