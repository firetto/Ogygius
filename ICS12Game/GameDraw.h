#pragma once
#include <SFML/Graphics.hpp>
#include "Breakable.h"
#include "DroppedItem.h"
#include "Square.h"
#include "DayCycle.h"
#include "Map.h"
#include "GameHUD.h"
void gameDraw(Player &player) {
	// clears everything that is on the window (color blue for sea)
	WINDOW.clear(sf::Color(128, 128, 255));

	// sorts all of the drawables based on height
	std::stable_sort(drawableVector.begin(), drawableVector.end(), [](const ObjDrawable& uno, const ObjDrawable& dos) {
		return uno.height < dos.height;
	});
	
	// sets camera view
	WINDOW.setView(player.getView());
	WINDOW.draw(ground);
	
	for (int h = GAME_VIEW_TOPSQUARE; h <= GAME_VIEW_BOTSQUARE; h++) {
		for (int w = GAME_VIEW_LEFTSQUARE; w <= GAME_VIEW_RIGHTSQUARE; w++) {
			WINDOW.draw(squareVector[h][w].ground);
		}
	}
	
	for (ObjDrawable d : drawableVector) { if (player.getView().inView(d.vis.getGlobalBounds())) d.draw(); }

	// resets camera view
	WINDOW.setView(WINDOW.getDefaultView());

	// draw night filter
	WINDOW.draw(dayOverlayRect);
	if (GAME_PAUSED) {
		WINDOW.draw(pauseOverlayRect);
	}
	else {
		if (GAME_MAP_OPEN) map.draw();
		else {
			gameHUD.draw();

			// draw crafting hud
			craftHUD.draw();
		}
	}

	currentGUIScreen->update();

	// draw cursor
	cursor.setPosition(sf::Vector2f(sf::Mouse::getPosition(WINDOW)));
	WINDOW.draw(cursor);

	// displays everything that was drawn to the window
	WINDOW.display();
}