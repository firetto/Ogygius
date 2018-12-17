#pragma once
#include "Declarations.h"
// this file is for game declarations of 6physics and other stuff.

// whether the game is running
bool GAME_RUNNING = false;

// if the game is running, if it has been paused.
bool GAME_PAUSED = false;

sf::RectangleShape pauseOverlayRect;

// acceleration and velocity divides by the friction (not real friction, but who cares, this isn't a physics simulation :P)
const float GAME_FRICTION = 1.25; 

// a debug mode that shows a bunch of values for debugging, as well as hitboxes of objects
bool GAME_DEBUG_MODE = true;

// how much entities bounce
const float GAME_ENTITY_BOUNCE_AMOUNT = 1.75;

const sf::Vector2f GAME_SQUARE_PER_CHUNK_AMOUNT(16, 16);
const sf::Vector2f GAME_CHUNKS_PER_WORLD_AMOUNT(8, 8);
const int GAME_SQUARE_SIZE = 128;
const int GAME_CHUNK_SIZE = GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_SQUARE_SIZE;
const int GAME_TOTAL_SIZE = GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE;

const int GAME_RENDER_DISTANCE = sqrt(pow(WINDOW.getSize().x / 2, 2) + pow(WINDOW.getSize().y / 2, 2));
int GAME_VIEW_LEFTCHUNK, GAME_VIEW_RIGHTCHUNK, GAME_VIEW_TOPCHUNK, GAME_VIEW_BOTCHUNK;
const double GAME_WATER_SLOW_AMOUNT = 0.75; // multiplier
const int GAME_MOB_SPAWN_FREQUENCY = 100000; // denominator
const int GAME_MOB_MAX_COUNT = 20;
const float GAME_DAY_LENGTH = 100; // seconds, set to 600 for 10 minutes 
sf::RectangleShape dayOverlayRect; // rectangle that is an overlay for the screen, becomes darker if it's night.
bool MOUSE_IN_HUD = false;

const sf::Vector2i GAME_WORLD_SIZE(GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_SQUARE_SIZE, GAME_SQUARE_PER_CHUNK_AMOUNT.y * GAME_SQUARE_SIZE);

// a clock that prevents holding buttons when crafting, placing, etc
sf::Clock GAME_CLICK_DELAY_CLOCK, DEPTH_BUFFER_SORT_DELAY_CLOCK;

// time (in seconds) until a click may again activate
const float GAME_CLICK_DELAY_TIME = 0.3, DEPTH_BUFFER_SORT_DELAY_TIME = 2;

// whether game click delay has passed
bool GAME_CLICK_DELAY_PASSED = true;

// whether the map is open
bool GAME_MAP_OPEN = false;

// item type
enum ItemType {
	ITEM_NONE,
	ITEM_WOOD, ITEM_STONE, ITEM_GRASS, ITEM_ROPE, ITEM_STICK, ITEM_IRON_INGOT, ITEM_IRON_ORE,

	ITEM_AXE, ITEM_PICKAXE,

	ITEM_CRAFT_STATION,

	ITEM_WALL_WOOD,

	ITEMTYPE_COUNT // this is used to track the amount of items
};

enum BreakableType { 
	BREAKABLE_NONE,
	BREAKABLE_TREE, BREAKABLE_ROCK, BREAKABLE_GRASS, BREAKABLE_ORE_IRON, 
	
	BREAKABLE_CRAFTING_STUMP, BREAKABLE_CRAFTING_STATION,

	BREAKABLE_WALL_WOOD,

	BREAKABLETYPE_COUNT //  used to track the number of breakables
};

enum BiomeType {
	BIOME_NONE, BIOME_FOREST, BIOME_GRASSLANDS, BIOME_SAVANNAH, BIOME_DESERT, BIOME_WATER,

	BIOMETYPE_COUNT // used to track the nubmer of BIOMEs
};

sf::Color BiomeColor[6] = { 
	sf::Color(0,0,0,255), // NONE
	sf::Color(136,182,96), // FOREST
	sf::Color(136,182,96), // GRASSLANDS
	sf::Color(222, 192, 120), // SAVANNAH
	sf::Color(237, 232, 192), // DESERT
	sf::Color(128, 128, 255) // WATER
};

enum MobType {
	MOB_COW,

	MOBTYPE_COUNT // used to track the number of mobs
};