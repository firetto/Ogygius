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
		switch (type) {
		case BIOME_FOREST: case BIOME_GRASSLANDS: ground.setFillColor(sf::Color(136,182,96)); break;
		case BIOME_SAVANNAH: ground.setFillColor(sf::Color(222, 192, 120)); break;
		case BIOME_DESERT: ground.setFillColor(sf::Color(237, 232, 192)); break;
		case BIOME_WATER: ground.setFillColor(sf::Color(128, 128, 255)); break;
		}
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

void generateSquares() {
	// creates squares
	
}