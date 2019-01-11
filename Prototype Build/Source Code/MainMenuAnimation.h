#pragma once
#include <SFML/Graphics.hpp>
#include "GameDeclarations.h"

// class declaration
class MainMenuAnimation {
public:

	// main update function
	void update() {

		// spawns new squares every 100ms
		if (clock.getElapsedTime().asSeconds() > 0.1) {
			squareVector.push_back(Square());
			clock.restart();
		}

		// for every square
		for (int i = 0; i < squareVector.size();) {

			// if its over the screen, delete
			if (squareVector[i].rect.getPosition().y - squareVector[i].rect.getSize().y / 2 > WINDOW_DIMENSIONS.y) {
				squareVector.erase(squareVector.begin() + i);
			}

			// otherwise, update position, rotate, draw
			else {
				squareVector[i].rect.setPosition(squareVector[i].rect.getPosition().x, squareVector[i].rect.getPosition().y + squareVector[i].velocity);
				squareVector[i].rect.rotate(squareVector[i].rotationAmount);
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
			
			// gets a random size
			int size = getRandomInt(squareMinSize, squareMinSize + squareSizeVariety);

			// sets the size
			rect.setSize(sf::Vector2f(size, size));

			// set origin
			rect.setOrigin(sf::Vector2f(sf::Vector2i(rect.getSize()) / 2));

			// set position to be random
			rect.setPosition(getRandomInt(0, WINDOW_DIMENSIONS.x), -rect.getSize().y / 2);

			// random velocity
			velocity = getRandomFloat(velocityMin, velocityMax);

			// set fill color to be random
			rect.setFillColor(sf::Color(255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, alphaVariety)));
			
			// set rotation to be random
			rotationAmount = getRandomFloat(-velocity / 2, velocity / 2);
		}

		// rectangle
		sf::RectangleShape rect;

		// velocity
		float velocity;

		// minimum and maximum square sizes
		int squareMinSize = WINDOW_DIMENSIONS.y / 3;
		int squareSizeVariety = WINDOW_DIMENSIONS.y / 3;

		// minimum and maximum velocity variants
		int velocityMin = 1;
		int velocityMax = 3;

		// color variety (rgb)
		int colorVariety = 100;

		// alpha variety
		int alphaVariety = 128;

		// by how much it rotates
		float rotationAmount;
	};

	// array of squares
	std::vector<Square> squareVector;

	// clock
	sf::Clock clock;
};

// main menu element
MainMenuAnimation mainMenuAnimation;