#pragma once
#include <SFML/Graphics.hpp>
#include "GameDeclarations.h"
class MainMenuAnimation {
public:
	void update() {
		if (clock.getElapsedTime().asSeconds() > 0.1) {
			squareVector.push_back(Square());
			clock.restart();
		}
		for (int i = 0; i < squareVector.size();) {
			if (squareVector[i].rect.getPosition().y - squareVector[i].rect.getSize().y / 2 > WINDOW_DIMENSIONS.y) {
				squareVector.erase(squareVector.begin() + i);
			}
			else {
				squareVector[i].rect.setPosition(squareVector[i].rect.getPosition().x, squareVector[i].rect.getPosition().y + squareVector[i].velocity);
				squareVector[i].rect.rotate(squareVector[i].rotationAmount);
				WINDOW.draw(squareVector[i].rect);
				i++;
			}
		}
	}
private:
	struct Square {
		Square() {
			int size = getRandomInt(squareMinSize, squareMinSize + squareSizeVariety);
			rect.setSize(sf::Vector2f(size, size));
			rect.setOrigin(sf::Vector2f(sf::Vector2i(rect.getSize()) / 2));
			rect.setPosition(getRandomInt(0, WINDOW_DIMENSIONS.x), -rect.getSize().y / 2);
			velocity = getRandomFloat(velocityMin, velocityMax);
			rect.setFillColor(sf::Color(255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, colorVariety), 255 - getRandomInt(0, alphaVariety)));
			rotationAmount = getRandomFloat(-velocity / 2, velocity / 2);
		}
		sf::RectangleShape rect;
		float velocity;
		int squareMinSize = WINDOW_DIMENSIONS.y / 3;
		int squareSizeVariety = WINDOW_DIMENSIONS.y / 3;
		int velocityMin = 1;
		int velocityMax = 3;
		int colorVariety = 100;
		int alphaVariety = 128;
		float rotationAmount;
	};
	std::vector<Square> squareVector;
	sf::Clock clock;
	
};
MainMenuAnimation mainMenuAnimation;