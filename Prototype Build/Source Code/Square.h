#pragma once
#include "Breakable.h"
#include <random>

// square class
class Square {
public:

	// empty constructor
	Square() { 

		// set type to blank
		type = BIOME_NONE; 

		// set square size
		ground.setSize(sf::Vector2f(GAME_SQUARE_SIZE, GAME_SQUARE_SIZE));
	}

	// get the type of square
	BiomeType getType() { return type; }

	// set the type of square
	void setType(BiomeType _type) { 
		type = _type; 

		// set the fill color
		ground.setFillColor(BiomeColor[type]);
	}

	// set position of square
	void setPosition(sf::Vector2f pos) {
		ground.setPosition(pos.x * GAME_SQUARE_SIZE, pos.y * GAME_SQUARE_SIZE);
		index = pos;
	}

	// rectangle ground
	sf::RectangleShape ground;

	// whether it was already "found" by the world generation algorithm
	bool found = false;

	// what kind of breakable it holds
	Breakable obj = Breakable(BREAKABLE_NONE);
private:

	// the type of biome
	BiomeType type;

	// index in the chunk
	sf::Vector2f index;
};

// 2d array of all squares in the game
std::vector<std::vector<Square>>squareVector;