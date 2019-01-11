#pragma once
#include "Player.h"
#include "WorldGeneration.h"
#include "AnimationDeclaration.h"
#include "Map.h"
#include "GUIScreen.h"
#include "DayCycle.h"
#include <SFML/Graphics.hpp>

// game loader
class GameLoader {
public:

	// game set up
	void gameSetup() {

		// load items
		itemMapFill();

		// declare breakables
		declareBreakables();

		// load animations
		animationLoad();

		// declare mobs
		declareMobs();

		// setup overlay rects
		setupOverlayRects();
	}

	// set up overlay rectangles
	void setupOverlayRects() {
		// set up pause overlay rectangle
		pauseOverlayRect.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
		pauseOverlayRect.setFillColor(sf::Color(0, 0, 0, 128));

		// set overlay rectangle for night overlayto be the size of the screen
		dayOverlayRect.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
	}
	void gameLoad() {
		// set up load text
		setupLoadText();

		// set up load bar (8 checkpoints)
		setupLoadBar(8);

		// draw load text
		drawLoadText();

		// set gui screen
		currentGUIScreen = &gameScreen;

		// activate next checkpoint
		nextLoadCheckpoint();

		// generate the world
		generateWorld();

		// next load checkpoint
		nextLoadCheckpoint();

		// add the world to the main square vector
		addWorldToSquares();
		
		nextLoadCheckpoint();

		// setup craft hud
		craftHUD = CraftingHUD();
		nextLoadCheckpoint();

		// clear arrays
		mobVector.clear();
		dropItemVector.clear();

		// reset days
		resetDay();
		nextLoadCheckpoint();

		// spawn some mobs
		Mob::spawnMob(10, sf::IntRect(int(GAME_CHUNKS_PER_WORLD_AMOUNT.x / 2) - 2, int(GAME_CHUNKS_PER_WORLD_AMOUNT.y / 2) - 2, 4, 4));
		nextLoadCheckpoint();

		// reset player
		globalPlayer->reset();
		nextLoadCheckpoint();

		// generate map
		map = GameMap();

		// reveal mapa
		if (GAME_MAP_REVEAL_ALL) map.revealAll();
		nextLoadCheckpoint();

		// clear map after drawing the text and load bar
		WINDOW.clear();
	}
private:
	// load text
	sf::Text gameLoadText;

	// loading bars
	sf::RectangleShape loadBarFull, loadBarPart;

	// loading checkpoints
	int maxLoadCheckpoints, loadCheckpoint = 0;

	// set up load text
	void setupLoadText() {
		// set up load text
		gameLoadText = sf::Text("Loading Game...", gameFont, WINDOW_DIMENSIONS.y / 10);
		gameLoadText.setLetterSpacing(1);
		gameLoadText.setOrigin(gameLoadText.getGlobalBounds().width / 2, gameLoadText.getGlobalBounds().height / 2);
		gameLoadText.setPosition(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2);
	}
	void drawLoadText() {
		// draw load text
		WINDOW.clear();
		WINDOW.draw(gameLoadText);
	}
	void setupLoadBar(int checkpoints) {
		// set up load bar
		loadBarFull.setFillColor(sf::Color(100, 100, 100));
		loadBarFull.setSize(sf::Vector2f(WINDOW_DIMENSIONS.x / 2, 16));
		loadBarFull.setPosition(WINDOW_DIMENSIONS.x / 4, gameLoadText.getPosition().y + gameLoadText.getCharacterSize() / 2 + 32);
		loadBarPart.setFillColor(sf::Color(0, 200, 0));
		loadBarPart.setSize(sf::Vector2f(0, 16));
		loadBarPart.setPosition(loadBarFull.getPosition());

		// reset checkpoints
		maxLoadCheckpoints = checkpoints;
		loadCheckpoint = 0;
	}
	void nextLoadCheckpoint() {
		// increment checkpoint
		loadCheckpoint++;

		// change size of load bar
		loadBarPart.setSize(sf::Vector2f(loadBarFull.getSize().x * ((float)loadCheckpoint / maxLoadCheckpoints), 16));

		// draw bar
		drawLoadBar();
	}
	void drawLoadBar() {
		// draw bars
		WINDOW.draw(loadBarFull);
		WINDOW.draw(loadBarPart);

		// display
		WINDOW.display();
	}
};

// a gameloader object
GameLoader gameLoader;

// pause/unpause game
void pauseGame() {
	// invert pause variable
	GAME_PAUSED = !GAME_PAUSED;

	// close map
	GAME_MAP_OPEN = false;

	// set the GUI screen to the corresponding screen
	currentGUIScreen = (GAME_PAUSED) ? &pauseMenuScreen : &gameScreen;
}

// exit the game
void exitGame() {
	GAME_RUNNING = false;
	GAME_PAUSED = false;

	// clear arrays
	mobVector.clear();
	globalPlayer->reset();

	// set GUI screen
	currentGUIScreen = &mainMenuScreen;
}
void windowSetup() {
	// set antialias level of window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// create window
	if (isFullscreen) {
		WINDOW_DIMENSIONS.x = sf::VideoMode::getDesktopMode().width;
		WINDOW_DIMENSIONS.y = sf::VideoMode::getDesktopMode().height;
		WINDOW.create(sf::VideoMode(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y), WINDOW_NAME, sf::Style::Fullscreen, settings);
	}
	else {
		WINDOW.create(sf::VideoMode(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y), WINDOW_NAME, sf::Style::Close, settings);
	}

	// make the mouse cursor not visible
	WINDOW.setMouseCursorVisible(false);
}
// gui set up function
void guiSetup();
void guiRefresh() {
	// clears all screens
	mainMenuScreen.clear();
	infoScreen.clear();
	settingsScreen.clear();
	deadScreen.clear();
	pauseMenuScreen.clear();
	map.clear();
	gameScreen.clear();

	// call GUI set up
	guiSetup();

	// set up map GUI screen
	map.setupGUIScreen();
}
void updateResolution() {
	// set up window
	windowSetup();

	// refresh GUI
	guiRefresh();

	// reset player view
	globalPlayer->resetView();

	// set up overlayrects of gameloader
	gameLoader.setupOverlayRects();
}

// definition for GUI setup function
void guiSetup() {

	// set up main menu screen
	mainMenuScreen.setTitleText("OGYGIUS", sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 3.5), 4);

	// add play game button
	mainMenuScreen.addButton("Play Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 1.75), [] {
		// load game
		GAME_RUNNING = true;
		gameLoader.gameLoad();

		// set GUI screen to game screen
		currentGUIScreen = &gameScreen;
	});
	mainMenuScreen.addButton("About", sf::Vector2f(WINDOW_DIMENSIONS.x / 6 - 6, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2 - WINDOW_DIMENSIONS.x / 12 - 3, WINDOW_DIMENSIONS.y / 1.75 + WINDOW_DIMENSIONS.y / 9), [] {
		// set GUI screen to info
		currentGUIScreen = &infoScreen;
	});
	mainMenuScreen.addButton("Settings", sf::Vector2f(WINDOW_DIMENSIONS.x / 6 - 6, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2 + WINDOW_DIMENSIONS.x / 12 + 3, WINDOW_DIMENSIONS.y / 1.75 + WINDOW_DIMENSIONS.y / 9), [] {
		// set GUI screen to settings
		currentGUIScreen = &settingsScreen;
	});
	mainMenuScreen.addButton("Exit Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 1.75 + WINDOW_DIMENSIONS.y / 4.5), [] {
		// close window
		WINDOW.close();
	});
	
	// set up info screen
	infoScreen.setTitleText("About Ogygius");

	// add body text
	infoScreen.setBodyText("Ogygius is a survival adventure game, similar to Minecraft. To survive, you must craft weapons and tools and battle hostile creatures that will try to eat you. Build walls and structures to defend yourself from the creatures that the darkness brings. _NEWLINE_ _NEWLINE_ Game developed by: Anatoly Zavyalov, Jonathan Sumabat.");
	
	// return to menu button
	infoScreen.addButton("Return to Menu", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y * 0.9), [] {
		currentGUIScreen = &mainMenuScreen;
	});

	// set up settings screen
	settingsScreen.setTitleText("Settings");

	// add screen resolution header text
	settingsScreen.addText("Screen Resolution", sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 4), true, true);

	// add screen resolution text
	settingsScreen.addText(std::to_string(WINDOW_DIMENSIONS.x) + "x" + std::to_string(WINDOW_DIMENSIONS.y), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 4 + WINDOW_DIMENSIONS.y / 10), true, true);
	
	// add return to menu button
	settingsScreen.addButton("Return to Menu", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y * 0.9), [] {
		currentGUIScreen = &mainMenuScreen;
	});
	
	// decrease window resolution
	settingsScreen.addButton("<", sf::Vector2f(WINDOW_DIMENSIONS.x / 18, WINDOW_DIMENSIONS.y / 15), sf::Vector2f(WINDOW_DIMENSIONS.x / 2 - WINDOW_DIMENSIONS.x / 6, WINDOW_DIMENSIONS.y / 4 + WINDOW_DIMENSIONS.y / 10), [] {
		
		// decrements window dimensions pos
		currentWindowDimensionsPos--;

		// loops through window dimension positions
		if (currentWindowDimensionsPos < 0) currentWindowDimensionsPos = windowDimensionsMap.size() - 1;

		// sets window dimensions
		WINDOW_DIMENSIONS = windowDimensionsMap[currentWindowDimensionsPos];

		// updates resolutions
		updateResolution();

		// updates the text
		settingsScreen.getTextByNumber(1).setString(std::to_string(WINDOW_DIMENSIONS.x) + "x" + std::to_string(WINDOW_DIMENSIONS.y));

		// sets the GUI screen
		currentGUIScreen = &settingsScreen;
	});
	settingsScreen.addButton(">", sf::Vector2f(WINDOW_DIMENSIONS.x / 18, WINDOW_DIMENSIONS.y / 15), sf::Vector2f(WINDOW_DIMENSIONS.x / 2 + WINDOW_DIMENSIONS.x / 6, WINDOW_DIMENSIONS.y / 4 + WINDOW_DIMENSIONS.y / 10), [] {
		
		// increments window dimensions pos
		currentWindowDimensionsPos++;

		// loops through window dimension positions
		if (currentWindowDimensionsPos >= windowDimensionsMap.size()) currentWindowDimensionsPos = 0;

		// sets window dimensions
		WINDOW_DIMENSIONS = windowDimensionsMap[currentWindowDimensionsPos];

		// update resolution
		updateResolution();

		// updates the text
		settingsScreen.getTextByNumber(1).setString(std::to_string(WINDOW_DIMENSIONS.x) + "x" + std::to_string(WINDOW_DIMENSIONS.y));

		// set GUI screen
		currentGUIScreen = &settingsScreen;
	});

	// add fullscreen text
	settingsScreen.addText("Fullscreen", sf::Vector2f(WINDOW_DIMENSIONS.x / 3 - WINDOW_DIMENSIONS.x / 36, WINDOW_DIMENSIONS.y / 2), false, true);

	// add fullscreen checkmark
	settingsScreen.addCheckmark(WINDOW_DIMENSIONS.x / 30, sf::Vector2f(WINDOW_DIMENSIONS.x / 2 + WINDOW_DIMENSIONS.x / 6, WINDOW_DIMENSIONS.y / 2), &isFullscreen);

	// set up pause menu screen
	pauseMenuScreen.setTitleText("Game Paused");

	// add resume button
	pauseMenuScreen.addButton("Resume Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2), [] {
		GAME_PAUSED = false;
		currentGUIScreen = &gameScreen;
	});

	// add exit button
	pauseMenuScreen.addButton("Exit Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2 + WINDOW_DIMENSIONS.y / 8), [] {
		exitGame();
	});

	// set up dead screen
	deadScreen.setTitleText("YOU ARE DEAD", 2);

	// add restart button
	deadScreen.addButton("Restart Game", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 1.5), [] {
		
		// running true
		GAME_RUNNING = true;

		// load game
		gameLoader.gameLoad();

		// set GUI screen
		currentGUIScreen = &gameScreen;

		// set paused to false
		GAME_PAUSED = false;
	});

	// add menu button
	deadScreen.addButton("Return to Menu", sf::Vector2f(WINDOW_DIMENSIONS.x / 3, WINDOW_DIMENSIONS.y / 10), sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 1.5 + WINDOW_DIMENSIONS.y / 8), [] {
		
		// exit game
		exitGame();
	});

	// set GUI screen to be the main menu screen
	currentGUIScreen = &mainMenuScreen;
}