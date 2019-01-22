/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

GameDeclarations.h 

This file is for game declarations of physics and other stuff.
*/



#pragma once
#include "Declarations.h"
// this file is for game declarations of physics and other stuff.

// whether the game is running
bool GAME_RUNNING = false;

// if the game is running, if it has been paused.
bool GAME_PAUSED = false;

sf::RectangleShape pauseOverlayRect;

// acceleration and velocity divides by the friction (not real friction, but who cares, this isn't a physics simulation :P)
const float GAME_FRICTION = 1.25; 

// a debug mode that shows a bunch of values for debugging, as well as hitboxes of objects
bool GAME_DEBUG_MODE = true;

// whether it is day
bool isDay = true;

// how much entities bounce after collision
const float GAME_ENTITY_BOUNCE_AMOUNT = 1.75;

// how many squares are in a chunk (leave at 16, don't set too high if you don't want stack overflow)
const sf::Vector2f GAME_SQUARE_PER_CHUNK_AMOUNT(16, 16);

const sf::Vector2f GAME_CHUNK_SIZE_TYPES[5] = {
	sf::Vector2f(8,8),
	sf::Vector2f(16,16),
	sf::Vector2f(32,32),
	sf::Vector2f(48,48),
	sf::Vector2f(64,64),
};
const std::string GAME_CHUNK_SIZE_LABELS[5] = {
	"Tiny",
	"Medium",
	"Large",
	"Gigantic",
	"LAGtastic",
};
int GAME_CURRENT_CHUNK_SIZE_POSITION = 1;

// how many chunks are in a world
sf::Vector2f GAME_CHUNKS_PER_WORLD_AMOUNT = GAME_CHUNK_SIZE_TYPES[GAME_CURRENT_CHUNK_SIZE_POSITION];

// how many squares are in a world
sf::Vector2f GAME_SQUARES_PER_WORLD_AMOUNT(GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_CHUNKS_PER_WORLD_AMOUNT.x, GAME_SQUARE_PER_CHUNK_AMOUNT.y * GAME_CHUNKS_PER_WORLD_AMOUNT.y);

// the square size (in pixels)
const int GAME_SQUARE_SIZE = 128;

// chunk size (in pixels)
int GAME_CHUNK_SIZE = GAME_SQUARE_PER_CHUNK_AMOUNT.x * GAME_SQUARE_SIZE;

// total pixel count of the wolrd
int GAME_TOTAL_SIZE = GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE;

// whether the map reveals the entire world on creation
bool GAME_MAP_REVEAL_ALL = false;

// render distance of the game
const int GAME_RENDER_DISTANCE = sqrt(pow(WINDOW.getSize().x / 2, 2) + pow(WINDOW.getSize().y / 2, 2));

// the square rect that the player can see
int GAME_VIEW_LEFTSQUARE, GAME_VIEW_RIGHTSQUARE, GAME_VIEW_TOPSQUARE, GAME_VIEW_BOTSQUARE;

// a multiplier of how much water slows you
const double GAME_WATER_SLOW_AMOUNT = 0.75;

// the maximum mob count
const int GAME_MOB_MAX_COUNT = 20;

// player default health
const int GAME_PLAYER_DEFAULT_HEALTH = 10;

// day length in seconds
const float GAME_DAY_LENGTH = 100; 

// rectangle that is an overlay for the screen, becomes darker if it's night.
sf::RectangleShape dayOverlayRect; 

// whether the mouse is in the crafting HUD
bool MOUSE_IN_HUD = false;

// a clock that prevents holding buttons when crafting, placing, etc
sf::Clock GAME_CLICK_DELAY_CLOCK, DEPTH_BUFFER_SORT_DELAY_CLOCK;

// time (in seconds) until a click may again activate
const float GAME_CLICK_DELAY_TIME = 0.3, DEPTH_BUFFER_SORT_DELAY_TIME = 2;

// whether game click delay has passed
bool GAME_CLICK_DELAY_PASSED = false;

// whether the map is open
bool GAME_MAP_OPEN = false;

// item type
enum ItemType {
	ITEM_NONE,
	ITEM_WOOD, ITEM_STONE, ITEM_GRASS, ITEM_ROPE, ITEM_STICK, ITEM_IRON_INGOT, ITEM_IRON_ORE,

	ITEM_AXE, ITEM_PICKAXE,

	ITEM_CRAFT_STATION,

	ITEM_WALL_WOOD,

	ITEM_MEAT,

	ITEMTYPE_COUNT // this is used to track the amount of items
};

// types of breakables
enum BreakableType { 
	BREAKABLE_NONE,
	BREAKABLE_TREE, BREAKABLE_ROCK, BREAKABLE_GRASS, BREAKABLE_ORE_IRON, 
	
	BREAKABLE_CRAFTING_STUMP, BREAKABLE_CRAFTING_STATION,

	BREAKABLE_WALL_WOOD,

	BREAKABLETYPE_COUNT //  used to track the number of breakables
};

// colors of breakables
sf::Color BreakableColor[BREAKABLETYPE_COUNT] = {
	sf::Color(0,0,0,0), // NONE
	sf::Color(68, 56, 27), // TREE
	sf::Color(163, 163, 163), // ROCK
	sf::Color(0, 115, 0), //GRASS
	sf::Color(163, 163, 163), // IRON ORE
	sf::Color(192, 158, 76), // crafting stump
	sf::Color(192, 158, 76), // crafting station
	sf::Color(140, 115, 56), // WOOD FENCE
};

// types of biomes
enum BiomeType {
	BIOME_NONE, BIOME_FOREST, BIOME_GRASSLANDS, BIOME_SAVANNAH, BIOME_DESERT, BIOME_WATER,

	BIOMETYPE_COUNT // used to track the nubmer of BIOMEs
};

// biome colors
sf::Color BiomeColor[6] = { 
	sf::Color(0,0,0,255), // NONE
	sf::Color(136,182,96), // FOREST
	sf::Color(136,182,96), // GRASSLANDS
	sf::Color(222, 192, 120), // SAVANNAH
	sf::Color(237, 232, 192), // DESERT
	sf::Color(128, 128, 255) // WATER
};

// types of mob
enum MobType {
	MOB_COW,

	MOB_MOUSE, MOB_MOUSE_ANGRY,

	MOBTYPE_COUNT // used to track the number of mobs
};

