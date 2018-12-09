#pragma once
#include "Player.h"
#include "DayCycle.h"
// main function that updates the entire game
void gameUpdate(Player &player) {
	player.update();
	processDayCycle();
	for (int i = 0; i < dropItemVector.size(); i++) {
		dropItemVector[i].dropItemPhysics();
		if (Collision::PixelPerfectTest(dropItemVector[i].outline, player.vis) && !dropItemVector[i].isDeleted && dropItemVector[i].canPickUp) {
			player.items.addItem(dropItemVector[i].item);
			dropItemVector[i].isDeleted = true;
		}
		if (dropItemVector[i].isDeleted) dropItemVector.erase(dropItemVector.begin() + i);
	}
	for (int i = 0; i < mobVector.size(); i++) {
		for (int a = 0; a < mobVector.size(); a++) {
			if (a == i) continue;
			mobVector[i].checkCollision(mobVector[a]);
		}
		mobVector[i].update();
		player.interactWithMob(mobVector[i]);
		if (mobVector[i].isDeleted) mobVector.erase(mobVector.begin() + i);
	}

	for (int q = GAME_VIEW_TOPCHUNK; q <= GAME_VIEW_BOTCHUNK; q++) {
		for (int w = GAME_VIEW_LEFTCHUNK; w <= GAME_VIEW_RIGHTCHUNK; w++) {
			for (auto &rows : chunkVector[q][w].squareVector) {
				for (Square &square : rows) {
					if (square.getType() == BIOME_GRASSLANDS && getDistanceBetweenPoints(player.getPosition(), square.ground.getPosition()) > GAME_RENDER_DISTANCE) Mob::spawnMob(square);
					if (getDistanceBetweenPoints(player.getPosition(), square.obj.getPosition()) < GAME_RENDER_DISTANCE + std::max(square.obj.vis.getGlobalBounds().height / 2, square.obj.vis.getGlobalBounds().width / 2)) {
						square.obj.update();
						player.attackBreakables(square.obj);
						player.checkCollision(square.obj);
					}
					for (Mob &mob : mobVector) mob.checkCollision(square.obj);
				}
			}
		}
	}
}