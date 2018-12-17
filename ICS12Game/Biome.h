#pragma once
#include "Breakable.h"
struct BiomePair {
	BreakableType type;
	int rarity;

	BiomePair(BreakableType _type, int _rarity) {
		type = _type;
		rarity = _rarity;
	}
};
class Biome {
public:
	std::vector<BiomePair> breakables;
	void addBreakable(BreakableType _type, int _rarity) {
		breakables.push_back(BiomePair(_type, _rarity));
		totalRarity += breakables.back().rarity;
	}

	// what the denominator is for spawning any breakable, 1 for always, 2 for half, etc
	int rarity = 0, totalRarity = 0;
};
std::map<BiomeType, Biome> biomeMap;
void generateBiomes() {
	if (biomeMap.size() > 0) return;
	for (int i = 1; i < BIOMETYPE_COUNT; i++) {
		biomeMap.insert(std::pair<BiomeType, Biome>(BiomeType(i), Biome()));
	}
	biomeMap[BIOME_FOREST].addBreakable(BREAKABLE_TREE, 12);
	biomeMap[BIOME_FOREST].addBreakable(BREAKABLE_GRASS, 3);
	biomeMap[BIOME_FOREST].addBreakable(BREAKABLE_ROCK, 2);
	biomeMap[BIOME_FOREST].rarity = 4;

	biomeMap[BIOME_GRASSLANDS].addBreakable(BREAKABLE_GRASS, 5);
	biomeMap[BIOME_GRASSLANDS].addBreakable(BREAKABLE_TREE, 1);
	biomeMap[BIOME_GRASSLANDS].rarity = 5;

	biomeMap[BIOME_SAVANNAH].addBreakable(BREAKABLE_GRASS, 7);
	biomeMap[BIOME_SAVANNAH].addBreakable(BREAKABLE_ROCK, 1);
	biomeMap[BIOME_SAVANNAH].addBreakable(BREAKABLE_TREE, 1);
	biomeMap[BIOME_SAVANNAH].rarity = 10;

	biomeMap[BIOME_DESERT].addBreakable(BREAKABLE_GRASS, 1);
	biomeMap[BIOME_DESERT].addBreakable(BREAKABLE_ROCK, 5);
	biomeMap[BIOME_DESERT].rarity = 15;
}