#pragma once
#include "Breakable.h"
#include <random>
class Square {
public:
	Square() { 
		type = BIOME_NONE; 
		ground.setSize(sf::Vector2f(GAME_SQUARE_SIZE, GAME_SQUARE_SIZE));
	}
	BiomeType getType() { return type; }
	void setType(BiomeType _type) { 
		type = _type; 
		ground.setFillColor(BiomeColor[type]);
	}
	void setPosition(sf::Vector2f pos) {
		ground.setPosition(pos.x * GAME_SQUARE_SIZE, pos.y * GAME_SQUARE_SIZE);
		index = pos;
	}
	sf::RectangleShape ground;
	bool found = false;
	Breakable obj = Breakable(BREAKABLE_NONE);
private:
	BiomeType type;
	sf::Vector2f index;
};
std::vector<std::vector<Square>>squareVector;