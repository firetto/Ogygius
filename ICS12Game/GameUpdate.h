#pragma once
#include "Player.h"
#include "DayCycle.h"
#include "Map.h"
#include "GameHUD.h"
// main function that updates the entire game
void gameUpdate(Player &player) {
	if (!player.isDead) player.update();
	gameHUD.update();
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

	for (int y = GAME_VIEW_TOPSQUARE; y <= GAME_VIEW_BOTSQUARE; y++) {
		for (int x = GAME_VIEW_LEFTSQUARE; x <= GAME_VIEW_RIGHTSQUARE; x++) {
			Square *s = &squareVector[y][x];
			if (getDistanceBetweenPoints(player.getPosition(), s->ground.getPosition()) > GAME_RENDER_DISTANCE) Mob::spawnMob(*s);
			if (getDistanceBetweenPoints(player.getPosition(), s->obj.getPosition()) < GAME_RENDER_DISTANCE + std::max(s->obj.vis.getGlobalBounds().height / 2, s->obj.vis.getGlobalBounds().width / 2)) {
				s->obj.update();
				player.attackBreakables(s->obj);
				player.checkCollision(s->obj);
			}
			for (Mob &mob : mobVector) mob.checkCollision(s->obj);
		}
	}
	map.update(player.getPosition());
}