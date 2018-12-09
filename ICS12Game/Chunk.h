#pragma once
#include "Square.h"
#include "Biome.h"
class Chunk;
std::vector<std::vector<Chunk>> chunkVector;
class Chunk {
public:
	std::vector<std::vector<Square>> squareVector;
	void generateChunk() {
		for (int y = 0; y < GAME_SQUARE_PER_CHUNK_AMOUNT.y; y++) {
			squareVector.push_back(std::vector<Square>());
			for (int x = 0; x < GAME_SQUARE_PER_CHUNK_AMOUNT.x; x++) {
				squareVector.back().push_back(Square());
				//squareVector.back().back().setPosition(sf::Vector2f(, i));
				squareVector.back().back().ground.setPosition(sf::Vector2f(x * GAME_SQUARE_SIZE + position.x, y * GAME_SQUARE_SIZE + position.y));
			}
		}
		//        y  x
		dfsSquare(0, 0, BIOME_NONE);
		Chunk::generateObjs();
	}
	void setPosition(sf::Vector2f pos) {
		position = sf::Vector2f(GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_SQUARE_SIZE * pos.x, GAME_SQUARE_PER_CHUNK_AMOUNT.y * GAME_SQUARE_SIZE * pos.y);
		index = sf::Vector2i(pos);
	}
	
private:
	sf::Vector2i index = sf::Vector2i(0,0);
	sf::Vector2f position;
	void dfsSquare(int y, int x, BiomeType desiredType) {
		if (squareVector[y][x].getType() == BIOME_NONE && !squareVector[y][x].found) {
			squareVector[y][x].found = true;
			BiomeType type;
			int ifWater = 125, ifGround = 100;
			if (index.x == 0 && index.y == 0)
				type = (desiredType == BIOME_NONE || getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : desiredType;
			else if (y == 0 && index.y > 0) {
				type = (getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : chunkVector[index.y - 1][index.x].squareVector[GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1][x].getType();
			}
			else if (x == 0 && index.x > 0) {
				type = (getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : chunkVector[index.y][index.x - 1].squareVector[y][GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1].getType();
			}
			else type = (desiredType == BIOME_NONE || getChance((desiredType == BIOME_WATER) ? ifWater : ifGround)) ? BiomeType(getRandomInt(1, int(BIOMETYPE_COUNT))) : desiredType;
			squareVector[y][x].setType(type);
			switch (getRandomInt(1, 5)) {
			case 1:
				if (x > 0 && !squareVector[y][x - 1].found)										dfsSquare(y, x - 1, type); // x
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squareVector[y][x + 1].found)	dfsSquare(y, x + 1, type); // x
				if (y > 0 && !squareVector[y - 1][x].found)										dfsSquare(y - 1, x, type); // y
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squareVector[y + 1][x].found)	dfsSquare(y + 1, x, type); // y
				break;
			case 2:
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squareVector[y][x + 1].found)	dfsSquare(y, x + 1, type); // x
				if (x > 0 && !squareVector[y][x - 1].found)										dfsSquare(y, x - 1, type); // x
				if (y > 0 && !squareVector[y - 1][x].found)										dfsSquare(y - 1, x, type); // y
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squareVector[y + 1][x].found)	dfsSquare(y + 1, x, type); // y
				break;
			case 3:
				if (y > 0 && !squareVector[y - 1][x].found)										dfsSquare(y - 1, x, type); // y
				if (x > 0 && !squareVector[y][x - 1].found)										dfsSquare(y, x - 1, type); // x
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squareVector[y][x + 1].found)	dfsSquare(y, x + 1, type); // x
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squareVector[y + 1][x].found)	dfsSquare(y + 1, x, type); // y
				break;
			default:
				if (y < GAME_SQUARE_PER_CHUNK_AMOUNT.y - 1 && !squareVector[y + 1][x].found)	dfsSquare(y + 1, x, type); // y
				if (x > 0 && !squareVector[y][x - 1].found)										dfsSquare(y, x - 1, type); // x
				if (x < GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1 && !squareVector[y][x + 1].found)	dfsSquare(y, x + 1, type); // x
				if (y > 0 && !squareVector[y - 1][x].found)										dfsSquare(y - 1, x, type); // y
				break;
			}
		}
	}
	void generateObjs() {
		for (int y = 0; y < squareVector.size(); y++) {
			for (int x = 0; x < squareVector[y].size(); x++) {
				if (biomeMap[squareVector[y][x].getType()].rarity != 0 && getChance(biomeMap[squareVector[y][x].getType()].rarity)) {
					sf::Vector2f _position(getRandomFloat(GAME_SQUARE_SIZE / 10, GAME_SQUARE_SIZE * 0.9), getRandomFloat(GAME_SQUARE_SIZE / 10, GAME_SQUARE_SIZE * 0.9));
					int p = getRandomInt(0, biomeMap[squareVector[y][x].getType()].totalRarity);
					int cumulative = 0;
					BreakableType type;
					for (BiomePair pair : biomeMap[squareVector[y][x].getType()].breakables) {
						cumulative += pair.rarity;
						if (p <= cumulative) {
							type = pair.type;
							break;
						}
					}
					squareVector[y][x].obj = Breakable(type);
					squareVector[y][x].obj.vis.rotate(getRandomFloat(0, 90));
					squareVector[y][x].obj.setPosition(sf::Vector2f(index.x*GAME_CHUNK_SIZE + x*GAME_SQUARE_SIZE + _position.x, index.y*GAME_CHUNK_SIZE + y*GAME_SQUARE_SIZE + _position.y));
				}
			}
		}
	}
};
