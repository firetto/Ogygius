/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

GameDraw.h 

This file is for providing the main game draw function, that draws everything.
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "Breakable.h"
#include "DroppedItem.h"
#include "Square.h"
#include "DayCycle.h"
#include "Map.h"
#include "GameHUD.h"

// function for drawing the game
void gameDraw(Player &player) {
	// clears everything that is on the window (color blue for sea)
	WINDOW.clear(sf::Color(128, 128, 255));

	// sorts all of the drawables based on height
	std::stable_sort(drawableVector.begin(), drawableVector.end(), [](const ObjDrawable& one, const ObjDrawable& two) {
		return one.height < two.height;
	});
	
	// sets camera view
	WINDOW.setView(player.getView());
	
	// draw the squares
	for (int h = GAME_VIEW_TOPSQUARE; h <= GAME_VIEW_BOTSQUARE; h++)
		for (int w = GAME_VIEW_LEFTSQUARE; w <= GAME_VIEW_RIGHTSQUARE; w++)
			WINDOW.draw(squareVector[h][w].ground);
		
	
	// draw all drawables
	for (ObjDrawable d : drawableVector) { if (player.getView().inView(d.vis.getGlobalBounds())) d.draw(); }

	// resets camera view to the normal view
	WINDOW.setView(WINDOW.getDefaultView());

	// draw night overlay rect
	WINDOW.draw(dayOverlayRect);

	// if the game is paused, draw the pause overlay reect
	if (GAME_PAUSED) WINDOW.draw(pauseOverlayRect);
	else {
		if (GAME_MAP_OPEN) map.draw();
		else {
			// draw game HUD
			gameHUD.draw();

			// draw crafting hud
			craftHUD.draw();
		}
	}

	// update the current GUI screen
	currentGUIScreen->update();

	// draw cursor
	WINDOW.draw(cursor);

	// displays everything that was drawn to the window
	WINDOW.display();
}