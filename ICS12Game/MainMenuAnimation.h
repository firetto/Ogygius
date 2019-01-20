#pragma once
#include <SFML/Graphics.hpp>
#include "GameDeclarations.h"

// class declaration
class MainMenuAnimation {
public:

	// main update function
	void update() {

		// spawns new squares every 100ms
		if (clock.getElapsedTime().asSeconds() > 0.05) {
			squareVector.push_back(Square());
			clock.restart();
		}

		// for every square
		for (int i = 0; i < squareVector.size();) {

			// if its over the screen, delete
			if (squareVector[i].rect.getPosition().x > WINDOW_DIMENSIONS.x || squareVector[i].rect.getPosition().y > WINDOW_DIMENSIONS.y ||
				squareVector[i].rect.getPosition().x < -WINDOW_DIMENSIONS.x || squareVector[i].rect.getPosition().y < -WINDOW_DIMENSIONS.y) {
				squareVector.erase(squareVector.begin() + i);
			}
			// otherwise, update position, rotate, draw
			else {
				switch (squareVector[i].type) {
				case 0:
					squareVector[i].rect.setPosition(squareVector[i].rect.getPosition().x, squareVector[i].rect.getPosition().y + squareVector[i].velocity);
					break;
				case 1:
					squareVector[i].rect.setPosition(squareVector[i].rect.getPosition().x, squareVector[i].rect.getPosition().y - squareVector[i].velocity);
					break;
				case 2:
					squareVector[i].rect.setPosition(squareVector[i].rect.getPosition().x + squareVector[i].velocity, squareVector[i].rect.getPosition().y);
					break;
				case 3:
					squareVector[i].rect.setPosition(squareVector[i].rect.getPosition().x - squareVector[i].velocity, squareVector[i].rect.getPosition().y);
					break;
				}
				WINDOW.draw(squareVector[i].rect);
				i++;
			}
		}
	}
private:

	// private square class
	struct Square {

		// constructor
		Square() {
			type = getRandomInt(0, 4);
			// gets a random size
			sf::Vector2f size;
			switch (type) {
			case 0: case 1: size = sf::Vector2f(getRandomInt(widthMin, widthMax), WINDOW_DIMENSIONS.y); break;
			case 2: case 3: size = sf::Vector2f(WINDOW_DIMENSIONS.x, getRandomInt(widthMin, widthMax)); break;
			}

			// sets the size
			rect.setSize(size);

			switch (type) {
			case 0:
				rect.setPosition(getRandomInt(-size.x+1, WINDOW_DIMENSIONS.x - 1), -WINDOW_DIMENSIONS.y);
				break;
			case 1:
				rect.setPosition(getRandomInt(-size.x+1, WINDOW_DIMENSIONS.x - 1), WINDOW_DIMENSIONS.y);
				break;
			case 2:
				rect.setPosition(-size.x, getRandomInt(-size.y+1, WINDOW_DIMENSIONS.y -1));
				break;
			case 3:
				rect.setPosition(WINDOW_DIMENSIONS.x+size.x, getRandomInt(-size.y+1, WINDOW_DIMENSIONS.y - 1));
				break;
			}

			// random velocity
			velocity = getRandomFloat(velocityMin, velocityMax);

			// set fill color to be random
			rect.setFillColor(sf::Color(255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, alphaVariety)));
		}

		// rectangle
		sf::RectangleShape rect;

		// velocity
		float velocity;

		// minimum and maximum velocity variants
		int velocityMin = 4;
		int velocityMax = 8;

		// color variety (rgb)
		int colorVariety = 150;

		// alpha variety
		int alphaVariety = 4;

		// width variables
		int width, widthMin = WINDOW_DIMENSIONS.x*3, widthMax = WINDOW_DIMENSIONS.x*3;

		int type = 0; // 0 - down, 1 - up, 2 - left, 3 - right
	};

	// array of squares
	std::vector<Square> squareVector;

	// clock
	sf::Clock clock;
};

// main menu element
MainMenuAnimation mainMenuAnimation;