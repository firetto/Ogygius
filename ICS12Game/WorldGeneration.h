#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"
#include <algorithm>
	
sf::RectangleShape ground;
void generateWorld() {
	generateBiomes();
	//generateSquares();
	if (chunks.size() > 0) chunks.clear();
	if (squareVector.size() > 0) squareVector.clear();
	for (int y = 0; y < GAME_CHUNKS_PER_WORLD_AMOUNT.y; y++) {
		chunks.push_back(std::vector<Chunk>());
		for (int i = 0; i < GAME_SQUARE_PER_CHUNK_AMOUNT.y; i++) squareVector.push_back(std::vector<Square>());
		for (int x = 0; x < GAME_CHUNKS_PER_WORLD_AMOUNT.x; x++) {
			chunks.back().push_back(Chunk());
			chunks.back().back().setPosition(sf::Vector2f(x, y));
			chunks.back().back().generateChunk();
		}
	}
	bool done = false;
	for (int y = 0; y < GAME_CHUNKS_PER_WORLD_AMOUNT.y; y++) {
		if (done) break;
		for (int x = 0; x < GAME_CHUNKS_PER_WORLD_AMOUNT.x; x++) {
			if (done) break;
			for (int sy = 0; sy < GAME_SQUARE_PER_CHUNK_AMOUNT.y; sy++) {
				if (done) break;
				for (int sx = 0; sx < GAME_SQUARE_PER_CHUNK_AMOUNT.x; sx++) {
					squareVector[y*GAME_SQUARE_PER_CHUNK_AMOUNT.y+sy].push_back(chunks[y][x].squares[sy][sx]);
					//std::cout << y << ", " << x << " | " << sy << ", " << sx << std::endl;
					if (sx == GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1) {
						x++;
						sx = -1;
						if (x == GAME_CHUNKS_PER_WORLD_AMOUNT.x) {
							x = 0;
							sy++;
							if (sy == GAME_SQUARE_PER_CHUNK_AMOUNT.y) {
								sy = 0;
								y++;
								if (y == GAME_CHUNKS_PER_WORLD_AMOUNT.y) {
									done = true; 
									break;
								}
							}
						}
						
					}
				}
			}
		}
	}
}