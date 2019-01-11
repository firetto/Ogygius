#pragma once
#include "Player.h"
#include "DayCycle.h"
#include "Map.h"
#include "GameHUD.h"
#include "Particle.h"

// main function that updates the entire game
void gameUpdate(Player &player) {

	// if the player isn't dead, update
	if (!player.isDead) player.update();

	// update game HUD
	gameHUD.update();

	// process the day cycle
	processDayCycle();

	// update all particles
	Particle::updateParticles();

	// update all dropped items
	for (int i = 0; i < dropItemVector.size(); i++) {

		// dropped item physics
		dropItemVector[i].dropItemPhysics();

		// if the player is colliding with the dropped item, deletes both
		if (Collision::PixelPerfectTest(dropItemVector[i].outline, player.vis) && !dropItemVector[i].isDeleted && dropItemVector[i].canPickUp) {
			player.items.addItem(dropItemVector[i].item);
			dropItemVector[i].isDeleted = true;
		}
		// if it's deleted, it erases it from the array
		if (dropItemVector[i].isDeleted) dropItemVector.erase(dropItemVector.begin() + i);
	}

	// update all mobs
	for (int i = 0; i < mobVector.size(); i++) {

		// for every mob (again)
		for (int a = 0; a < mobVector.size(); a++) {
			
			// if its the same mob, continue
			if (a == i) continue;

			// check collision
			mobVector[i].checkCollision(mobVector[a]);
		}

		// update mob
		mobVector[i].update();

		// player interact with mob
		player.interactWithMob(mobVector[i]);

		// if deleted, erase from array
		if (mobVector[i].isDeleted) mobVector.erase(mobVector.begin() + i);
	}

	// for every square that you can see
	for (int y = GAME_VIEW_TOPSQUARE; y <= GAME_VIEW_BOTSQUARE; y++) {
		for (int x = GAME_VIEW_LEFTSQUARE; x <= GAME_VIEW_RIGHTSQUARE; x++) {

			// create square pointer
			Square *s = &squareVector[y][x];

			// if the distance is greater than the render distance, spawn mobs
			if (getDistanceBetweenPoints(player.getPosition(), s->ground.getPosition()) > GAME_RENDER_DISTANCE) Mob::spawnMob(*s);

			// if the distance is less than the render distance
			if (getDistanceBetweenPoints(player.getPosition(), s->obj.getPosition()) < GAME_RENDER_DISTANCE + std::max(s->obj.vis.getGlobalBounds().height / 2, s->obj.vis.getGlobalBounds().width / 2)) {
				
				// update the square breakable
				s->obj.update();

				// call player breakables
				player.attackBreakables(s->obj);

				// check collision between player and breakable
				player.checkCollision(s->obj);
			}

			// check collision between all mobs and objects
			for (Mob &mob : mobVector) mob.checkCollision(s->obj);
		}
	}

	// update the map
	map.update(player.getPosition());
}