#pragma once
#include "Square.h"
#include "Biome.h"

// creates chunk class
class Chunk;

// creates list of chunks
std::vector<std::vector<Chunk>> chunks;

// class declaration
class Chunk {
public:

	// list of squares within a chunk
	std::vector<std::vector<Square>> squares;

	// function to generate chunk
	void generateChunk() {

		// sets up the squares
		for (int y = 0; y < GAME_SQUARE_PER_CHUNK_AMOUNT.y; y++) {
			squares.push_back(std::vector<Square>());
			for (int x = 0; x < GAME_SQUARE_PER_CHUNK_AMOUNT.x; x++) {
				squares.back().push_back(Square());
				squares.back().back().ground.setPosition(sf::Vector2f(x * GAME_SQUARE_SIZE + position.x, y * GAME_SQUARE_SIZE + position.y));
			}
		}
		
		// function to recursively generate the world
		//        y  x
		dfsSquare(0, 0, BIOME_NONE);

		// spawn breakables within the chunk
		Chunk::generateObjs();
		
	}
	// set position of chunk
	void setPosition(sf::Vector2f pos) {
		position = sf::Vector2f(GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_SQUARE_SIZE * pos.x, GAME_SQUARE_PER_CHUNK_AMOUNT.y * GAME_SQUARE_SIZE * pos.y);
		index = sf::Vector2i(pos);
	}
	
private:
	sf::Vector2i index = sf::Vector2i(0,0);
	sf::Vector2f position;

	// recursive function to generate biomes
	void dfsSquare(int y, int x, BiomeType desiredType) {
		// if the biome type is none
		if (squares[y][x].getType() == BIOME_NONE && !squares[y][x].found) {
			// a boolean to control whether the biome has already been set
			squares[y][x].found = true;

			// local type variable
			BiomeType type;

			// denominators for determining the chance for the biome to switch, depending whether if its ground or water
			// water has a higher denominator, which means that there will be larger lakes
			int ifWater = 200, ifGround = 100;
			
			// if the index of the chunk is 0 (top left
			if (index.x == 0 && index.y == 0)
				// sets type to be random IF biome is none or it is switching biomes, otherwise sets it to be the same desired type
				type = (desiredType == BIOME_NONE || getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : desiredType;

			// if the chunk is below another chunk
			else if (y == 0 && index.y > 0) {
				// sets the top squares to take information from the chunk above it
				type = (getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : chunks[index.y - 1][index.x].squares[GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1][x].getType();
			}
			// if the chunk has another chunk to the right of it
			else if (x == 0 && index.x > 0) {
				// sets the left squares to take information from the chunk to the right of it
				type = (getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : chunks[index.y][index.x - 1].squares[y][GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1].getType();
			}

			// otherwise, just pulls information from desiredType
			else type = (desiredType == BIOME_NONE || getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : desiredType;

			// sets the type of the square
			squares[y][x].setType(type);

			// randomly goes left, right, up or down (so the world doesn't look like strips)
			switch (getRandomInt(1, 5)) {
			case 1:
				// goes left first
				if (x > 0 && !squares[y][x - 1].found)										dfsSquare(y, x - 1, type); // x

				// then right
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squares[y][x + 1].found)	dfsSquare(y, x + 1, type); // x

				// then up
				if (y > 0 && !squares[y - 1][x].found)										dfsSquare(y - 1, x, type); // y

				// then down
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squares[y + 1][x].found)	dfsSquare(y + 1, x, type); // y
				break;
			case 2:
				// goes right first
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squares[y][x + 1].found)	dfsSquare(y, x + 1, type); // x

				// then left
				if (x > 0 && !squares[y][x - 1].found)										dfsSquare(y, x - 1, type); // x

				// then up
				if (y > 0 && !squares[y - 1][x].found)										dfsSquare(y - 1, x, type); // y

				// then down
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squares[y + 1][x].found)	dfsSquare(y + 1, x, type); // y
				break;
			case 3:

				// goes up first
				if (y > 0 && !squares[y - 1][x].found)										dfsSquare(y - 1, x, type); // y

				// then left
				if (x > 0 && !squares[y][x - 1].found)										dfsSquare(y, x - 1, type); // x

				// then right
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squares[y][x + 1].found)	dfsSquare(y, x + 1, type); // x

				// then down
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squares[y + 1][x].found)	dfsSquare(y + 1, x, type); // y
				break;
			default:

				// goes down first
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squares[y + 1][x].found)	dfsSquare(y + 1, x, type); // y

				// then left
				if (x > 0 && !squares[y][x - 1].found)										dfsSquare(y, x - 1, type); // x

				// then right
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squares[y][x + 1].found)	dfsSquare(y, x + 1, type); // x

				// then up
				if (y > 0 && !squares[y - 1][x].found)										dfsSquare(y - 1, x, type); // y
				break;
			}
		}
	}

	// function to generate breakables in the chunk
	void generateObjs() {
		// for every square
		for (int y = 0; y < squares.size(); y++) {
			for (int x = 0; x < squares[y].size(); x++) {

				// if the type of the biome isn't 0 and there is a chance
				if (biomeMap[squares[y][x].getType()].rarity != 0 && getChance(biomeMap[squares[y][x].getType()].rarity)) {
					// sets position of the sprite to be a bit off from the center of the square
					sf::Vector2f _position(getRandomFloat(GAME_SQUARE_SIZE / 10, GAME_SQUARE_SIZE * 0.9), getRandomFloat(GAME_SQUARE_SIZE / 10, GAME_SQUARE_SIZE * 0.9));
					
					// loops through all of the breakables and then picks one based on its rarity
					int p = getRandomInt(0, biomeMap[squares[y][x].getType()].totalRarity);
					int cumulative = 0;
					BreakableType type;
					for (BiomePair pair : biomeMap[squares[y][x].getType()].breakables) {
						cumulative += pair.rarity;
						if (p <= cumulative) {
							type = pair.type;
							break;
						}
					}

					// sets the breakable type, rotation, position
					squares[y][x].obj = Breakable(type);
					squares[y][x].obj.vis.rotate(getRandomFloat(0, 90));
					squares[y][x].obj.setPosition(sf::Vector2f(index.x*GAME_CHUNK_SIZE + x*GAME_SQUARE_SIZE + _position.x, index.y*GAME_CHUNK_SIZE + y*GAME_SQUARE_SIZE + _position.y));
				}
			}
		}
	}
};
