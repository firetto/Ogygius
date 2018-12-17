#pragma once
#include "GameDeclarations.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <map>

// the texture for the player sprite
sf::Texture playerTex;

// fist animation textures
sf::Texture anim_fistTex, anim_axeTex, anim_pickTex;

sf::Texture hud_craftingTex;

sf::Texture lootOutlineTex;

// a list of breakable textures
std::map<int, sf::Texture> breakableTextureMap;

// a list of breakable texture directories that the breakable textures load from when declared
std::map<int, std::string> breakableTextureDirectoryMap;

// a list of item textures
std::map<int, sf::Texture> itemTextureMap;

// a list of item directories that the textures load from when declared
std::map<int, std::string> itemTextureDirectoryMap;

// a list of item textures
std::map<int, sf::Texture> mobTextureMap;

// a list of item directories that the textures load from when declared
std::map<int, std::string> mobTextureDirectoryMap;

// the font used in the game
sf::Font gameFont;



//  outputs an error with a specified directory as a parameter
void loadTexture(sf::Texture &texture, std::string directory) {
	if (!texture.loadFromFile(directory))
		outputError("Texture at \"" + directory + "\" not loaded!");
	else outputInfo("Texture at \"" + directory + "\" has loaded!");
}
// this function loads all of the textures from the directories into the texture variables
void textureLoad() {
	// sets player texture
	loadTexture(playerTex, "textures/playerBody.png");

	loadTexture(anim_fistTex, "textures/fist.png");

	loadTexture(anim_axeTex, "textures/axestrike.png");

	loadTexture(anim_pickTex, "textures/pickaxestrike.png");

	loadTexture(hud_craftingTex, "textures/HUD_craft.png");

	loadTexture(lootOutlineTex, "textures/lootsquare.png");

	// load number of textures for breakables
	for (int i = 0; i < BREAKABLETYPE_COUNT; i++) {
		breakableTextureDirectoryMap.insert(std::pair<int, std::string>(i, std::string()));
		breakableTextureMap.insert(std::pair<int, sf::Texture>(i, sf::Texture()));
	}

	breakableTextureDirectoryMap[BREAKABLE_TREE] = "textures/collidables/treeleaves.png";
	breakableTextureDirectoryMap[BREAKABLE_ROCK] = "textures/collidables/rock.png";
	breakableTextureDirectoryMap[BREAKABLE_GRASS] = "textures/collidables/tallgrass.png";
	breakableTextureDirectoryMap[BREAKABLE_CRAFTING_STUMP] = "textures/collidables/log.png";
	breakableTextureDirectoryMap[BREAKABLE_WALL_WOOD] = "textures/collidables/fencewood.png";

	for (int i = 0; i < breakableTextureMap.size(); i++) {
		loadTexture(breakableTextureMap[i], breakableTextureDirectoryMap[i]);
	}

	// load number of textures for items
	for (int i = 0; i < ITEMTYPE_COUNT; i++) {
		itemTextureDirectoryMap.insert(std::pair<int, std::string>(i, std::string()));
		itemTextureMap.insert(std::pair<int, sf::Texture>(i, sf::Texture()));
	}

	itemTextureDirectoryMap[ITEM_WOOD] = "textures/drops/wood.png";
	itemTextureDirectoryMap[ITEM_STONE] = "textures/drops/stone.png";
	itemTextureDirectoryMap[ITEM_GRASS] = "textures/drops/grass.png";
	itemTextureDirectoryMap[ITEM_ROPE] = "textures/drops/rope.png";
	itemTextureDirectoryMap[ITEM_STICK] = "textures/drops/stick.png";
	itemTextureDirectoryMap[ITEM_IRON_INGOT] = "textures/drops/ingot.png";
	itemTextureDirectoryMap[ITEM_IRON_ORE] = "textures/drops/ore.png";
	itemTextureDirectoryMap[ITEM_AXE] = "textures/drops/axe.png";
	itemTextureDirectoryMap[ITEM_PICKAXE] = "textures/drops/pick.png";
	itemTextureDirectoryMap[ITEM_CRAFT_STATION] = "textures/drops/crafting_station.png";
	itemTextureDirectoryMap[ITEM_WALL_WOOD] = "textures/drops/fence.png";

	for (int i = 0; i < itemTextureMap.size(); i++) {
		loadTexture(itemTextureMap[i], itemTextureDirectoryMap[i]);
	}

	for (int i = 0; i < MOBTYPE_COUNT; i++) {
		mobTextureDirectoryMap.insert(std::pair<int, std::string>(i, std::string()));
		mobTextureMap.insert(std::pair<int, sf::Texture>(i, sf::Texture()));
	}
	mobTextureDirectoryMap[MOB_COW] = "textures/mobs/cow.png";

	for (int i = 0; i < MOBTYPE_COUNT; i++) loadTexture(mobTextureMap[i], mobTextureDirectoryMap[i]);

	
}

void fontLoad() {
	gameFont.loadFromFile("gameFont.ttf");
}