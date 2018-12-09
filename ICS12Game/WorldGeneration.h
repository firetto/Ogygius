#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"
#include <algorithm>
	
sf::RectangleShape ground;
void generateWorld() {
	generateBiomes();
	//generateSquares();
	for (int y = 0; y < GAME_CHUNKS_PER_WORLD_AMOUNT.y; y++) {
		chunkVector.push_back(std::vector<Chunk>());
		for (int x = 0; x < GAME_CHUNKS_PER_WORLD_AMOUNT.x; x++) {
			chunkVector.back().push_back(Chunk());
			chunkVector.back().back().setPosition(sf::Vector2f(x, y));
			chunkVector.back().back().generateChunk();
		}
	}
}