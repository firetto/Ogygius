#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"
#include <algorithm>

// function to generate the world
void generateWorld() {
	// set up all biomes
	loadBiomes();

	// clear all chunks if they already exist
	if (chunks.size() > 0) chunks.clear();

	// clear all squares if they already exist
	if (squareVector.size() > 0) squareVector.clear();

	// push back new chunks
	for (int y = 0; y < GAME_CHUNKS_PER_WORLD_AMOUNT.y; y++) {
		chunks.push_back(std::vector<Chunk>());
		
		// push back new squares
		for (int i = 0; i < GAME_SQUARE_PER_CHUNK_AMOUNT.y; i++) squareVector.push_back(std::vector<Square>());

		// push back other chunks
		for (int x = 0; x < GAME_CHUNKS_PER_WORLD_AMOUNT.x; x++) {

			// set up chunks
			chunks.back().push_back(Chunk());
			chunks.back().back().setPosition(sf::Vector2f(x, y));

			// generate the biomes of that chunk
			chunks.back().back().generateChunk();
		}
	}
}

/*
This function basically loops through every single chunk left to right and
adds them to the square array (please ask me for a more indepth explanation)
*/
void addWorldToSquares() {

	// local done boolean
	bool done = false;

	// loops through every chunk
	for (int y = 0; y < GAME_CHUNKS_PER_WORLD_AMOUNT.y; y++) {
		if (done) break;
		for (int x = 0; x < GAME_CHUNKS_PER_WORLD_AMOUNT.x; x++) {
			if (done) break;

			// loops through every square in that chunk
			for (int sy = 0; sy < GAME_SQUARE_PER_CHUNK_AMOUNT.y; sy++) {
				if (done) break;
				for (int sx = 0; sx < GAME_SQUARE_PER_CHUNK_AMOUNT.x; sx++) {
					
					// add that square to the array
					squareVector[y*GAME_SQUARE_PER_CHUNK_AMOUNT.y + sy].push_back(chunks[y][x].squares[sy][sx]);
					
					// if it reached the end of the chunk
					if (sx == GAME_SQUARE_PER_CHUNK_AMOUNT.x - 1) {

						// increment the X chunk
						x++;

						// set sx to -1 because it would increment sx once the loop is done, so it will be set to 0.
						sx = -1;

						// if it reached the end the horizontal line of chunks
						if (x == GAME_CHUNKS_PER_WORLD_AMOUNT.x) {

							// set x back to 0
							x = 0;

							// increment the Y of square
							sy++;

							// if it reached the y maximum of the chunk
							if (sy == GAME_SQUARE_PER_CHUNK_AMOUNT.y) {
								// go back to sy 0
								sy = 0;

								// go to the next row of chunks
								y++;

								// once it's done
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