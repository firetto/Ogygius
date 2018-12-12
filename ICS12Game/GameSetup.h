#pragma once
#include "Player.h"
#include "WorldGeneration.h"
#include "AnimationDeclaration.h"
#include "Map.h"
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
	map = GameMap();

	declareMobs();

	Mob::spawnMob(10, sf::IntRect(int(GAME_CHUNKS_PER_WORLD_AMOUNT.x / 2) - 2, int(GAME_CHUNKS_PER_WORLD_AMOUNT.y / 2) - 2, 4, 4));
	
	dayOverlayRect.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
}