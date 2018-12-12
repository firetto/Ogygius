#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"

class GameMap {
public:
	GameMap() {
		width = GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_SQUARE_PER_CHUNK_AMOUNT.x;
		height = GAME_CHUNKS_PER_WORLD_AMOUNT.y * GAME_SQUARE_PER_CHUNK_AMOUNT.y;
		
		for (int i = 0; i < width; i++) {
			squares.push_back(std::vector<GameMapSquare>());
			for (int a = 0; a < height; a++) {
				squares.back().push_back(GameMapSquare());
				squares.back().back().type = BIOME_NONE;
				float size = std::min(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y) / (GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_SQUARE_PER_CHUNK_AMOUNT.x);
				scale = size / float(GAME_SQUARE_SIZE);
				squares.back().back().rect.setSize(sf::Vector2f(size, size));
				squares.back().back().rect.setPosition(WINDOW_DIMENSIONS.x / 2 - size * width / 2 + a * size, WINDOW_DIMENSIONS.y / 2 - size * height / 2 + i * size);
			}
		}
		screenOverlay.setFillColor(sf::Color(0, 0, 0, 128));
		screenOverlay.setSize(sf::Vector2f(WINDOW_DIMENSIONS));

		playerPosition.setFillColor(sf::Color::Green);
		playerPosition.setSize(playerPositionSize);
		playerPosition.setOutlineThickness(3);
		playerPosition.setOutlineColor(sf::Color::Black);
		playerPosition.setOrigin(sf::Vector2f(playerPosition.getSize().x / 2, playerPosition.getSize().y / 2));
		playerPosition.rotate(45);
		
		view.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
		view.setCenter(sf::Vector2f(WINDOW_DIMENSIONS.x / 2, WINDOW_DIMENSIONS.y / 2));

		mapText.setCharacterSize(std::min(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y) / 16);
		mapText.setFont(gameFont);
		mapText.setString("Game Map");
		mapText.setOutlineThickness(mapText.getCharacterSize() / 16);
		mapText.setOrigin(mapText.getGlobalBounds().width / 2, mapText.getGlobalBounds().height / 2);
		mapText.setPosition(WINDOW_DIMENSIONS.x / 2, mapText.getCharacterSize());
	}
	void update(sf::Vector2f position) {
		sf::Vector2i square = sf::Vector2i(position.x / GAME_SQUARE_SIZE, position.y / GAME_SQUARE_SIZE);
		int leftSquare = square.x - radius; leftSquare = (leftSquare < 0) ? 0 : leftSquare;
		int rightSquare = square.x + radius; rightSquare = (rightSquare > GAME_SQUARE_PER_CHUNK_AMOUNT.x*GAME_CHUNKS_PER_WORLD_AMOUNT.x - 1) ? GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_CHUNKS_PER_WORLD_AMOUNT.x - 1 : rightSquare;
		int topSquare = square.y - radius; topSquare = (topSquare < 0) ? 0 : topSquare;
		int botSquare = square.y + radius; botSquare = (botSquare > GAME_SQUARE_PER_CHUNK_AMOUNT.y *GAME_CHUNKS_PER_WORLD_AMOUNT.y - 1) ? GAME_SQUARE_PER_CHUNK_AMOUNT.y * GAME_CHUNKS_PER_WORLD_AMOUNT.y - 1 : botSquare;
		playerPosition.setPosition(squares[0][0].rect.getPosition().x + position.x*scale, position.y*scale);
		for (int i = topSquare; i < botSquare; i++) {
			for (int a = leftSquare; a < rightSquare; a++) {
				sf::Vector2i chunk(a / GAME_SQUARE_PER_CHUNK_AMOUNT.x, i / GAME_SQUARE_PER_CHUNK_AMOUNT.y);
				sf::Vector2i square(a%int(GAME_SQUARE_PER_CHUNK_AMOUNT.x), i%int(GAME_SQUARE_PER_CHUNK_AMOUNT.y));
				if (squares[i][a].type == BIOME_NONE) {
					squares[i][a].type = chunkVector[chunk.y][chunk.x].squareVector[square.y][square.x].getType();
					squares[i][a].rect.setFillColor(BiomeColor[squares[i][a].type]);
					if (farthestTop == -1 || i < farthestTop) farthestTop = i;
					if (farthestBot == -1 || i > farthestBot) farthestBot = i;
					if (farthestLeft == -1 || a < farthestLeft) farthestLeft = a;
					if (farthestRight == -1 || a > farthestRight) farthestRight = a;
				}
			}
		}
		if (GAME_MAP_OPEN) {
			int mouse_wheel;
			switch (event.type) {
			case sf::Event::MouseWheelScrolled:
				mouse_wheel = (event.mouseWheel.x < 0) ? 1 : -1;
				if (mouse_wheel > 0 && view.getCameraZoom() < 3) view.cameraZoom(1.1);
				else if (view.getCameraZoom() > 0.2) view.cameraZoom(0.9);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == 0) {
					draggingScreen = true;
					oldMousePos = WINDOW.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == 0) {
					draggingScreen = false;
				}
				break;
			case sf::Event::MouseMoved:
				if (!draggingScreen) break;
				sf::Vector2f
					newPos = WINDOW.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)),
					deltaPos = sf::Vector2f((oldMousePos.x - newPos.x)*(view.getCameraZoom()), (oldMousePos.y - newPos.y)*(view.getCameraZoom()));
				view.setCenter(view.getCenter() + deltaPos);
				WINDOW.setView(WINDOW.getDefaultView());
				oldMousePos = WINDOW.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				break;
			}
		}
		else {
			view.cameraZoomReset();
			view.setCenter(sf::Vector2f(WINDOW_DIMENSIONS / 2));
		}
	}
	void draw() {
		WINDOW.draw(screenOverlay);
		WINDOW.setView(view);
		for (int i = farthestTop; i < farthestBot; i++) {
			for (int a = farthestLeft; a < farthestRight; a++) {
				if (squares[i][a].type != BIOME_NONE) WINDOW.draw(squares[i][a].rect);
			}
		}
		WINDOW.draw(playerPosition);
		WINDOW.setView(WINDOW.getDefaultView());
		WINDOW.draw(mapText);
	}
	Camera &getView() { return view; }
private:
	struct GameMapSquare {
		BiomeType type;
		sf::RectangleShape rect;
	};
	std::vector<std::vector<GameMapSquare>> squares;
	int width, height;
	float scale;
	int radius = 6;
	bool draggingScreen = false;
	sf::Vector2f oldMousePos;
	sf::RectangleShape screenOverlay;
	sf::RectangleShape playerPosition;
	sf::Vector2f playerPositionSize = sf::Vector2f(12, 12);
	Camera view;
	sf::Text mapText;
	int farthestTop = -1, farthestBot = -1, farthestLeft = -1, farthestRight = -1;
};
GameMap map;