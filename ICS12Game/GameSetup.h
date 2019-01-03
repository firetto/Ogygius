#pragma once
#include "Player.h"
#include "WorldGeneration.h"
#include "AnimationDeclaration.h"
#include "Map.h"
#include "GUIScreen.h"
#include <SFML/Graphics.hpp>

class GameLoader {
public:
	void gameSetup() {
		
		// load all of the textures
		textureLoad();

		// load items
		itemMapFill();

		// declar3e breakables
		declareBreakables();

		// load animations
		animationLoad();

		// declare mobs
		declareMobs();

		pauseOverlayRect.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
		pauseOverlayRect.setFillColor(sf::Color(0, 0, 0, 128));

		// set overlay rectangle for night overlayto be the size of the screen
		dayOverlayRect.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
	}
	void gameLoad() {
		setupLoadText();
		drawLoadText();

		// generate the world
		generateWorld();

		// setup craft hud
		craftHUD = CraftingHUD();

		// spawn some mobs
		Mob::spawnMob(10, sf::IntRect(int(GAME_CHUNKS_PER_WORLD_AMOUNT.x / 2) - 2, int(GAME_CHUNKS_PER_WORLD_AMOUNT.y / 2) - 2, 4, 4));

		// generate map
		map = GameMap();
		currentGUIScreen = &gameScreen;
		WINDOW.clear();
	}
private:
	sf::Text gameLoadText;
	void setupLoadText() {
		gameLoadText = sf::Text("Loading Game...", gameFont, WINDOW_DIMENSIONS.y / 10);
		gameLoadText.setLetterSpacing(1);
		gameLoadText.setOrigin(gameLoadText.getGlobalBounds().width / 2, gameLoadText.getGlobalBounds().height / 2);
		gameLoadText.setPosition(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2);
	}
	void drawLoadText() {
		WINDOW.clear();
		WINDOW.draw(gameLoadText);
		WINDOW.display();
	}
};
GameLoader gameLoader;
void pauseGame() {
	GAME_PAUSED = !GAME_PAUSED;
	GAME_MAP_OPEN = false;
	currentGUIScreen = (GAME_PAUSED) ? &pauseMenuScreen : &gameScreen;
}
void windowSetup() {
	// set antialias level of window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// create window
	WINDOW.create(sf::VideoMode(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y), WINDOW_NAME, sf::Style::Close, settings);

	// load font
	fontLoad();
}

void guiSetup() {
	mainMenuScreen.setTitleText("OGYGIUS", sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 3.5), 4);
	mainMenuScreen.addButton("Play Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 1.5), [] {
		GAME_RUNNING = true;
		gameLoader.gameLoad();
		currentGUIScreen = &gameScreen;
	});
	mainMenuScreen.addButton("About", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 1.5 + WINDOW_DIMENSIONS.y / 9), [] {
		currentGUIScreen = &infoScreen;
	});
	mainMenuScreen.addButton("Exit", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 1.5 + WINDOW_DIMENSIONS.y / 4.5), [] {
		WINDOW.close();
	});
	
	infoScreen.setTitleText("About Ogygius");
	infoScreen.setBodyText("Ogygius is a survival adventure game, similar to Minecraft. To survive, you must craft weapons and tools and battle hostile creatures that will try to eat you. Keep hunger and thirst in mind. Build walls and structures to defend yourself from the creatures that the darkness brings. \n Game developed by: Anatoly Zavyalov, Jonathan Sumabat.");
	infoScreen.addButton("Return to Menu", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y * 0.9), [] {
		currentGUIScreen = &mainMenuScreen;
	});

	pauseMenuScreen.setTitleText("Game Menu");
	pauseMenuScreen.addButton("Resume Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2), [] {
		GAME_PAUSED = false;
		currentGUIScreen = &gameScreen;
	});
	pauseMenuScreen.addButton("Exit Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2 + WINDOW_DIMENSIONS.y / 8), [] {
		GAME_RUNNING = false;
		GAME_PAUSED = false;
		mobVector.clear();
		globalPlayer->reset();
		currentGUIScreen = &mainMenuScreen;
	});
	currentGUIScreen = &mainMenuScreen;
}