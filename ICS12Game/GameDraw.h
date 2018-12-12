#pragma once
#include "Breakable.h"
#include "DroppedItem.h"
#include "Square.h"
#include "DayCycle.h"
#include "Map.h"
void gameDraw(Player &player) {
	// clears everything that is on the window (color blue for sea)
	WINDOW.clear(sf::Color(128, 128, 255));

	// sorts all of the drawables based on height
	std::sort(drawableVector.begin(), drawableVector.end(), [](const ObjDrawable& uno, const ObjDrawable& dos) {
		return uno.height < dos.height;
	});
	
	// sets camera view
	WINDOW.setView(player.getView());
	WINDOW.draw(ground);
	GAME_VIEW_LEFTCHUNK = player.getCurrChunk().x - 1; if (GAME_VIEW_LEFTCHUNK < 0) GAME_VIEW_LEFTCHUNK = 0;
	GAME_VIEW_RIGHTCHUNK = player.getCurrChunk().x + 1; if (GAME_VIEW_RIGHTCHUNK >= GAME_CHUNKS_PER_WORLD_AMOUNT.x) GAME_VIEW_RIGHTCHUNK = GAME_CHUNKS_PER_WORLD_AMOUNT.x - 1;
	GAME_VIEW_TOPCHUNK = player.getCurrChunk().y - 1; if (GAME_VIEW_TOPCHUNK < 0) GAME_VIEW_TOPCHUNK = 0;
	GAME_VIEW_BOTCHUNK = player.getCurrChunk().y + 1; if (GAME_VIEW_BOTCHUNK >= GAME_CHUNKS_PER_WORLD_AMOUNT.y) GAME_VIEW_BOTCHUNK = GAME_CHUNKS_PER_WORLD_AMOUNT.y - 1;
	for (int q = GAME_VIEW_TOPCHUNK; q <= GAME_VIEW_BOTCHUNK; q++) {
		for (int w = GAME_VIEW_LEFTCHUNK; w <= GAME_VIEW_RIGHTCHUNK; w++) {
			for (int i = 0; i < chunkVector[q][w].squareVector.size(); i++) {
				for (int a = 0; a < chunkVector[q][w].squareVector[i].size(); a++) {
					if (player.getView().inView(chunkVector[q][w].squareVector[i][a].ground.getGlobalBounds())) WINDOW.draw(chunkVector[q][w].squareVector[i][a].ground);
				}
			}
		}
	}
	
	for (ObjDrawable d : drawableVector) { if (player.getView().inView(d.vis.getGlobalBounds())) d.draw(); }

	// resets camera view
	WINDOW.setView(WINDOW.getDefaultView());

	// draw night filter
	WINDOW.draw(dayOverlayRect);

	// draws player itembar
	player.items.draw();

	// draw crafting hud
	craftHUD.draw();
	if (GAME_MAP_OPEN) map.draw();

	// displays everything that was drawn to the window
	WINDOW.display();
}