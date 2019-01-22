/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Biome.h

loads all of the biomes
*/

#pragma once
#include "Breakable.h"

// a struct holding the type and rarity of a biome
struct BiomePair {

	// type
	BreakableType type;

	// rarity of the biome (denominator)
	int rarity;

	// constructor
	BiomePair(BreakableType _type, int _rarity) {
		type = _type;
		rarity = _rarity;
	}
};

// biome class, holding possible breakables that might spawn (objects), as well as a rarity
class Biome {
public:
	// array holding breakables
	std::vector<BiomePair> breakables;

	// function for adding a breakable type, as well as a rarity
	void addBreakable(BreakableType _type, int _rarity) {
		breakables.push_back(BiomePair(_type, _rarity));
		totalRarity += breakables.back().rarity;
	}

	// what the denominator is for spawning any breakable, 1 for always, 2 for half, etc
	int rarity = 0, totalRarity = 0;
};

// list holding different types of biomes
std::map<BiomeType, Biome> biomeMap;

// function to generate different types of biomes
void loadBiomes() {
	// if already generated biomes, exit
	if (biomeMap.size() > 0) return;

	// adds biomes to list (starts from 1 because 0 is none)
	for (int i = 1; i < BIOMETYPE_COUNT; i++) biomeMap.insert(std::pair<BiomeType, Biome>(BiomeType(i), Biome()));

	// sets up forest biome
	biomeMap[BIOME_FOREST].addBreakable(BREAKABLE_TREE, 12);
	biomeMap[BIOME_FOREST].addBreakable(BREAKABLE_GRASS, 3);
	biomeMap[BIOME_FOREST].addBreakable(BREAKABLE_ROCK, 2);
	biomeMap[BIOME_FOREST].rarity = 4;

	// sets up grassland biome
	biomeMap[BIOME_GRASSLANDS].addBreakable(BREAKABLE_GRASS, 5);
	biomeMap[BIOME_GRASSLANDS].addBreakable(BREAKABLE_TREE, 1);
	biomeMap[BIOME_GRASSLANDS].rarity = 5;

	// sets up savannah biome
	biomeMap[BIOME_SAVANNAH].addBreakable(BREAKABLE_GRASS, 7);
	biomeMap[BIOME_SAVANNAH].addBreakable(BREAKABLE_ROCK, 1);
	biomeMap[BIOME_SAVANNAH].addBreakable(BREAKABLE_TREE, 1);
	biomeMap[BIOME_SAVANNAH].rarity = 10;

	// sets up desert biome
	biomeMap[BIOME_DESERT].addBreakable(BREAKABLE_GRASS, 1);
	biomeMap[BIOME_DESERT].addBreakable(BREAKABLE_ROCK, 5);
	biomeMap[BIOME_DESERT].rarity = 15;
}