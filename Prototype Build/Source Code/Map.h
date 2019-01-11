#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"
#include "GUIScreen.h"

// map declaration
class GameMap : public GUIScreen {
public:

	// constructor
	GameMap() {

		// gets the width and height in squares
		width = GAME_SQUARES_PER_WORLD_AMOUNT.x;
		height = GAME_SQUARES_PER_WORLD_AMOUNT.y;
		
		// set global square size 
		squareSize = std::min(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y) / GAME_SQUARES_PER_WORLD_AMOUNT.x;

		// set scale of the map
		scale = squareSize / float(GAME_SQUARE_SIZE);
		
		// for every square array
		for (int i = 0; i < width; i++) {

			// push back array into array
			squares.push_back(std::vector<GameMapSquare>());

			// for every square horizontal
			for (int a = 0; a < height; a++) {

				// push bakc a new square
				squares.back().push_back(GameMapSquare());

				// set the type to be blank
				squares.back().back().type = BIOME_NONE;

				// get the size
				float size = squareSize;

				// set size
				squares.back().back().rect.setSize(sf::Vector2f(size, size));

				// set position
				squares.back().back().rect.setPosition(WINDOW_DIMENSIONS.x / 2 - size * width / 2 + a * size, WINDOW_DIMENSIONS.y / 2 - size * height / 2 + i * size);
			}
		}
		// set up screen overlay
		screenOverlay.setFillColor(sf::Color(0, 0, 0, 128));
		screenOverlay.setSize(sf::Vector2f(WINDOW_DIMENSIONS));

		// set up player position diamond
		playerPosition.setFillColor(sf::Color::Green);
		playerPosition.setSize(playerPositionSize);
		playerPosition.setOutlineThickness(playerPositionSize.x/4);
		playerPosition.setOutlineColor(sf::Color::Black);
		playerPosition.setOrigin(sf::Vector2f(playerPosition.getSize().x / 2, playerPosition.getSize().y / 2));
		playerPosition.rotate(45);
		
		// set up view
		view.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
		view.setCenter(sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2));

		// set up GUI screen
		setupGUIScreen();

	}

	// add title text and button
	void setupGUIScreen() {
		
		// add title text
		setTitleText("Game Map");

		// add button to close map and go back to game
		addButton("Close Map", sf::Vector2f(WINDOW_DIMENSIONS.x / 5, WINDOW_DIMENSIONS.y / 12), sf::Vector2f(WINDOW_DIMENSIONS.x * 0.875, WINDOW_DIMENSIONS.y * 0.95), [] {
			GAME_MAP_OPEN = false;
		});
	}

	// update function, taking the player position as a parameter
	void update(sf::Vector2f position) {

		// get the square the player is on
		sf::Vector2i square = sf::Vector2i(position.x / GAME_SQUARE_SIZE, position.y / GAME_SQUARE_SIZE);

		// get the squares that the player can see
		int leftSquare = square.x - radius; leftSquare = (leftSquare < 0) ? 0 : leftSquare;
		int rightSquare = square.x + radius; if (rightSquare > GAME_SQUARE_PER_CHUNK_AMOUNT.x*GAME_CHUNKS_PER_WORLD_AMOUNT.x - 1) rightSquare = GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_CHUNKS_PER_WORLD_AMOUNT.x - 1;
		int topSquare = square.y - radius; topSquare = (topSquare < 0) ? 0 : topSquare;
		int botSquare = square.y + radius; botSquare = (botSquare > GAME_SQUARE_PER_CHUNK_AMOUNT.y *GAME_CHUNKS_PER_WORLD_AMOUNT.y - 1) ? GAME_SQUARE_PER_CHUNK_AMOUNT.y * GAME_CHUNKS_PER_WORLD_AMOUNT.y - 1 : botSquare;
		
		// set the position of the player diamond
		playerPosition.setPosition(squares[0][0].rect.getPosition().x + position.x*scale, position.y*scale);

		// for every square that the player can see
		for (int i = topSquare; i < botSquare; i++) {
			for (int a = leftSquare; a < rightSquare; a++) {

				// if the biome is none
				if (squares[i][a].type == BIOME_NONE) {

					// reveal the type and color of the biome
					squares[i][a].type = squareVector[square.y+(i-square.y)][square.x+(a-square.x)].getType();
					squares[i][a].rect.setFillColor(BiomeColor[squares[i][a].type]);

					// adjust farthest square variables (for optimization)
					if (farthestTop == -1 || i < farthestTop) farthestTop = i;
					if (farthestBot == -1 || i > farthestBot) farthestBot = i;
					if (farthestLeft == -1 || a < farthestLeft) farthestLeft = a;
					if (farthestRight == -1 || a > farthestRight) farthestRight = a;
				}
			}
		}

		// if the map is open
		if (GAME_MAP_OPEN) {

			// if the GUI screen hasn't been set yet, set it to this
			if (resetMap) currentGUIScreen = this;
			resetMap = false;

			// local mouse wheel variable
			int mouse_wheel;

			// looks at the events
			switch (event.type) {

			// if scrolled
			case sf::Event::MouseWheelScrolled:

				// gets direction of scroll
				mouse_wheel = (event.mouseWheel.x < 0) ? 1 : -1;

				// zoom in/out based on scroll
				if (mouse_wheel > 0 && view.getCameraZoom() < 1) view.cameraZoom(1.1);
				else if (view.getCameraZoom() > 0.2) view.cameraZoom(0.9);

				// update the player position rect to not change size with zoom
				updatePlayerPositionRect();

				// no longer following player
				followPlayer = false;
				break;

			// if a mouse button has been pressed
			case sf::Event::MouseButtonPressed:

				// if its the left button
				if (event.mouseButton.button == 0) {

					// yep, dragging screen
					draggingScreen = true;

					// get the old mouse position
					oldMousePos = WINDOW.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

					// no longer following player
					followPlayer = false;
				}
				break;

			// done dragging
			case sf::Event::MouseButtonReleased:

				// if left click
				if (event.mouseButton.button == 0) {

					// done dragging
					draggingScreen = false;
					
					// nope, not following player
					followPlayer = false;
				}
				break;
			case sf::Event::MouseMoved:
				// if not dragging screen, break
				if (!draggingScreen) break;

				// follow player is false
				followPlayer = false;

				//  get the new mouse position and delta
				sf::Vector2f
					newPos = WINDOW.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)),
					deltaPos = sf::Vector2f((oldMousePos.x - newPos.x)*(view.getCameraZoom()), (oldMousePos.y - newPos.y)*(view.getCameraZoom()));
				
				// set the view center
				view.setCenter(view.getCenter() + deltaPos);

				// set back to the original view
				WINDOW.setView(WINDOW.getDefaultView());

				// set new mouse pos
				oldMousePos = WINDOW.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				break;
			}
		
			// if the player icon isn't visible on the map, restrict map from moving
			if (view.getCenter().x - playerPosition.getPosition().x > view.getSize().x / 2)
				view.setCenter(playerPosition.getPosition().x + view.getSize().x / 2, view.getCenter().y);
			else if (playerPosition.getPosition().x - view.getCenter().x > view.getSize().x / 2)
				view.setCenter(playerPosition.getPosition().x - view.getSize().x / 2, view.getCenter().y);

			if (view.getCenter().y - playerPosition.getPosition().y > view.getSize().y / 2)
				view.setCenter(view.getCenter().x, playerPosition.getPosition().y + view.getSize().y / 2);
			else if (playerPosition.getPosition().y - view.getCenter().y > view.getSize().y / 2)
				view.setCenter(view.getCenter().x, playerPosition.getPosition().y - view.getSize().y / 2);

			// if following the player, set the center to be the player position
			if (followPlayer) view.setCenter(playerPosition.getPosition());
		}

		// if not in map BUT hasn't reset the map yet
		else if (!resetMap) {

			// set gui screen back to game screen
			currentGUIScreen = &gameScreen;

			// reset the camera zoom
			view.cameraZoomReset();

			// zoom again
			view.cameraZoom((float(std::min(farthestBot - farthestTop, farthestRight - farthestLeft)) / (GAME_CHUNKS_PER_WORLD_AMOUNT.x*GAME_SQUARE_PER_CHUNK_AMOUNT.x / 1.5)));
			
			// set center
			view.setCenter(playerPosition.getPosition());

			// update player position rect
			updatePlayerPositionRect();

			// set follow player to true so the view follows the player next time the map is opened
			followPlayer = true;

			// set reset map to true so this isn't executed indefinitely
			resetMap = true;
		}
	}

	// debug function that reveals every tile (is laggy)
	void revealAll() {

		// for every square
		for (int i = 0; i < GAME_SQUARES_PER_WORLD_AMOUNT.y; i++) {
			for (int a = 0; a < GAME_SQUARES_PER_WORLD_AMOUNT.x; a++) {

				// set square type
				squares[i][a].type = squareVector[i][a].getType();

				// set square fill color
				squares[i][a].rect.setFillColor(BiomeColor[squares[i][a].type]);
			}
		}

		// adjust farthest square variables
		farthestTop = farthestLeft = 0;
		farthestRight = GAME_SQUARES_PER_WORLD_AMOUNT.x - 1;
		farthestBot = GAME_SQUARES_PER_WORLD_AMOUNT.y - 1;
	}

	// draw the map
	void draw() {

		// draw the screen overlay
		WINDOW.draw(screenOverlay);

		// set window view
		WINDOW.setView(view);

		// draw every tile
		// TODO: Optimize further by not drawing the squares that are not in the view, mathematically. 
		for (int i = farthestTop; i < farthestBot; i++) {
			for (int a = farthestLeft; a < farthestRight; a++) {

				// if the biome type isn't none and its visible
				if (squares[i][a].type != BIOME_NONE && view.inView(squares[i][a].rect.getGlobalBounds())) {
					WINDOW.draw(squares[i][a].rect);
				}
			}
		}

		// draw the player diamond
		WINDOW.draw(playerPosition);

		// set back to normal view
		WINDOW.setView(WINDOW.getDefaultView());

		// update the GUI
		GUIScreen::update();
	}

	// return the view
	Camera &getView() { return view; }
private:

	// private game map square class
	struct GameMapSquare {

		// type
		BiomeType type;
		
		// square shape
		sf::RectangleShape rect;
	};

	// update the player position diamond
	void updatePlayerPositionRect() {

		// set size based on camera zoom
		playerPosition.setSize(sf::Vector2f(playerPositionSize.x*view.getCameraZoom(), playerPositionSize.y*view.getCameraZoom()));
		
		// set outline thickness
		playerPosition.setOutlineThickness(playerPositionSize.x / 4 * view.getCameraZoom());

		// set origin
		playerPosition.setOrigin(playerPosition.getSize().x / 2, playerPosition.getSize().y / 2);
	}

	// array of squares
	std::vector<std::vector<GameMapSquare>> squares;

	// width and height of the map
	int width, height;

	// scale and size of the square
	float scale, squareSize;

	// radius of the player map visibility
	int radius = 6;

	// whether the player is dragging the screen
	bool draggingScreen = false;

	// following the player?, has reset the map
	bool followPlayer = true, resetMap = false;

	// old mouse position
	sf::Vector2f oldMousePos;

	// screen dark overlay
	sf::RectangleShape screenOverlay;

	// player position
	sf::RectangleShape playerPosition;

	// size of the player position diamond
	sf::Vector2f playerPositionSize = sf::Vector2f(12, 12);

	// view
	Camera view;
	
	// farthest square variables for optimization
	int farthestTop = -1, farthestBot = -1, farthestLeft = -1, farthestRight = -1;
};

// map variable
GameMap map;