/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Particle.h 

This file is for declarations of the particle class
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "Textures.h"
#include "GameDeclarations.h"
#include "Object.h"

// particle class
class Particle;

// array of particles
std::vector<Particle> particleVector;

// class declaration
class Particle {
public:

	// main constructor
	Particle(sf::Color color, float t, sf::Vector2f pos, sf::Vector2f origPos, sf::Vector2f accel, bool radial) {

		// sets duration of particle
		time = t;
		
		// set up sprite
		vis.setTexture(particleTex);
		vis.setOrigin(getMiddle(vis));
		vis.setPosition(pos);
		vis.setColor(color);
		vis.setPosition(pos);

		// get random rotation amount
		rotateAmount = getRandomFloat(-10, 10);

		// radial spread
		radialSpread = radial;

		// get the original position
		originalPos = origPos;

		// if its spreading radially, calculate velocities.
		if (radialSpread) {
			float angle = getAngleBetweenPoints(origPos, pos);
			velocity.x = -accel.x*cos(angle * PI / 180);
			velocity.y = -accel.x*sin(angle * PI / 180);
		}

		// otherwise just a normal acceleration
		else velocity = accel;
	}

	// constructor without radial spreads
	Particle(sf::Color color, float t, sf::Vector2f pos) {
		Particle(color, t, pos, pos, sf::Vector2f(0, 0), false);
	}

	// constructor without time
	Particle(sf::Color color, sf::Vector2f pos) {
		Particle(color, 0.5, pos, pos, sf::Vector2f(0, 0), false);
	}

	// update particle
	void update() {

		// set color
		vis.setColor(sf::Color(vis.getColor().r, vis.getColor().g, vis.getColor().b, 255 - 255 * (clock.getElapsedTime().asSeconds() / time)));
		
		// rotate
		vis.rotate(rotateAmount);
		
		// update position
		vis.setPosition(vis.getPosition().x + velocity.x, vis.getPosition().y + velocity.y);

		// if elapsed, delete
		if (clock.getElapsedTime().asSeconds() >= time) isDeleted = true;

		// otherwise, draw
		else drawableVector.push_back(ObjDrawable(vis, 4.5));
	}

	// radial spawn
	static void spawnParticlesRadial(sf::Color color, int amount, float t, sf::Vector2f pos, float accel) {

		// for amount
		for (int i = 0; i < amount; i++) {

			// calculate a random position that it starts in
			sf::Vector2f rand(getRandomFloat(-25, 25), getRandomFloat(-25, 25));

			// push back particle
			particleVector.push_back(Particle(color, t, sf::Vector2f(pos.x + rand.x, pos.y + rand.y), pos, sf::Vector2f(accel, 0), true));
		}
	}

	// normal spawn
	static void spawnParticles(sf::Color color, int amount, float t, sf::Vector2f pos, sf::Vector2f accel) {
		for (int i = 0; i < amount; i++) {

			// calculate a random position offset
			sf::Vector2f rand(getRandomFloat(-25, 25), getRandomFloat(-25, 25));

			// push back particle
			particleVector.push_back(Particle(color, t, sf::Vector2f(pos.x + rand.x, pos.y + rand.y), pos, accel, false));
		}
	}

	// spawn particle without acceleration
	static void spawnParticles(sf::Color color, int amount, float t, sf::Vector2f pos) {
		spawnParticles(color, amount, t, pos, sf::Vector2f(0, 0));
	}

	// spawn particle without time
	static void spawnParticles(sf::Color color, int amount, sf::Vector2f pos) {
		spawnParticles(color, amount, 0.5, pos);
	}

	// update all particles
	static void updateParticles() {
		for (int i = 0; i < particleVector.size();) {

			// if deleted, erase from array
			if (particleVector[i].isDeleted) particleVector.erase(particleVector.begin() + i);

			// otherwise, update it
			else {
				particleVector[i].update();
				i++;
			}
		}
	}
private:

	// time (duration)
	float time;

	// clock
	sf::Clock clock;

	// sprite
	sf::Sprite vis;

	// velocity
	sf::Vector2f velocity;

	// original position
	sf::Vector2f originalPos;

	// radial spread
	bool radialSpread = false;

	// whether it is deleted
	bool isDeleted = false;

	// rotate amount
	float rotateAmount = 0;
};