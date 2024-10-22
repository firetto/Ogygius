// the main source file where the main function is

#include <SFML/Graphics.hpp>
#include "GameSetup.h"
#include "GameUpdate.h"
#include "GameDraw.h"
#include "MainMenuAnimation.h"

int main() { // main method
	// window setup
	windowSetup();

	// load all of the textures
	textureLoad();

	// load font
	fontLoad();

	// game setup
	gameLoader.gameSetup();

	// creates a player object
	Player player;
	globalPlayer = &player;
	gameHUD = GameHUD(player);

	// set up buttons and GUIs
	guiSetup();

	// while the displayed window is open
	while (WINDOW.isOpen()) { 
		// adds the time elapsed since last time the clock was restarted to the sf::time
		WINDOW_TIME_SINCE_LAST_UPDATE += WINDOW_FRAME_CLOCK.restart();

		while (WINDOW.pollEvent(event)) {

			// if the event is that the window is closed, close the window
			if (event.type == sf::Event::Closed) WINDOW.close();

			// if the game is running
			if (GAME_RUNNING) {

				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {

						// if the m key is pressed
					case sf::Keyboard::M:

						// invert the map boolean
						GAME_MAP_OPEN = !GAME_MAP_OPEN; break;

						// if the escape key is pressed
					case sf::Keyboard::Escape:

						// if the map is open, close the map
						if (GAME_MAP_OPEN) GAME_MAP_OPEN = false;

						// otherwise, pause the game
						else pauseGame(); 
						break;
					}
				}

				// if the map isn't open
				if (!GAME_MAP_OPEN) {
					
					// update the game HUD
					gameHUD.eventUpdate();

					// update the craft HUD
					craftHUD.update(player.items);

					// update the player's events
					player.eventUpdate();
				}
			}

			// if the screen was JUST turned fullscreen/not fullscreen
			if (isFullscreen != wasFullscreen) {

				// set it
				wasFullscreen = isFullscreen;

				// if just reverted back to not fullscreen, set the window dimesions back
				if (!isFullscreen) WINDOW_DIMENSIONS = defaultWindowDimension;
				
				// call the update resolution function
				updateResolution();
			}
		}

		//Lets the physics & etc. run if the time elapsed since last time physics ran was more than the time per frame
		while (WINDOW_TIME_SINCE_LAST_UPDATE > WINDOW_TIME_PER_FRAME) {  

			// clear all of the drawables
			drawableVector.clear();
			
			if (GAME_RUNNING) {
				// update the game physics
				gameUpdate(player);
			}

			// once everything is done, subtracts the time per frame from the time since last update
			WINDOW_TIME_SINCE_LAST_UPDATE -= WINDOW_TIME_PER_FRAME; 
		}
		// updates game click delay
		if (!GAME_CLICK_DELAY_PASSED && GAME_CLICK_DELAY_CLOCK.getElapsedTime().asSeconds() >= GAME_CLICK_DELAY_TIME) {
			GAME_CLICK_DELAY_PASSED = true;
			GAME_CLICK_DELAY_CLOCK.restart();
		}
		else if (GAME_CLICK_DELAY_PASSED) GAME_CLICK_DELAY_CLOCK.restart();


		// set position of cursor
		cursor.setPosition(sf::Vector2f(sf::Mouse::getPosition(WINDOW)));

		// draws the game
		if (GAME_RUNNING) gameDraw(player);
		else {
			WINDOW.clear(sf::Color(64,64,64));

			// update the main menu animation
			mainMenuAnimation.update();

			// update the current GUI screen
			currentGUIScreen->update();

			// draw the cursor
			WINDOW.draw(cursor);

			// display everything to the screen
			WINDOW.display();
		}
	}
}