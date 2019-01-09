#pragma once
#include <SFML/Graphics.hpp>
#include "Textures.h"
#include "GameDeclarations.h"
#include "Object.h"
class Particle;
std::vector<Particle> particleVector;
class Particle {
public:
	Particle(sf::Color color, float t, sf::Vector2f pos, sf::Vector2f origPos, sf::Vector2f accel, bool radial) {
		time = t;
		vis.setTexture(particleTex);
		vis.setOrigin(getMiddle(vis));
		vis.setPosition(pos);
		vis.setColor(color);
		vis.setPosition(pos);
		rotateAmount = getRandomFloat(-20, 20);

		radialSpread = radial;
		originalPos = origPos;
		if (radialSpread) {
			float angle = getAngleBetweenPoints(origPos, pos);
			velocity.x = -accel.x*cos(angle * PI / 180);
			velocity.y = -accel.x*sin(angle * PI / 180);
		}
		else velocity = accel;
	}
	Particle(sf::Color color, float t, sf::Vector2f pos) {
		Particle(color, t, pos, pos, sf::Vector2f(0, 0), false);
	}
	Particle(sf::Color color, sf::Vector2f pos) {
		Particle(color, 0.5, pos, pos, sf::Vector2f(0, 0), false);
	}
	void update() {
		vis.setColor(sf::Color(vis.getColor().r, vis.getColor().g, vis.getColor().b, 255 - 255 * (clock.getElapsedTime().asSeconds() / time)));
		vis.rotate(rotateAmount);
		vis.setPosition(vis.getPosition().x + velocity.x, vis.getPosition().y + velocity.y);

		if (clock.getElapsedTime().asSeconds() >= time) isDeleted = true;
		else drawableVector.push_back(ObjDrawable(vis, 4.5));
	}
	static void spawnParticlesRadial(sf::Color color, int amount, float t, sf::Vector2f pos, float accel) {
		for (int i = 0; i < amount; i++) {
			sf::Vector2f rand(getRandomFloat(-25, 25), getRandomFloat(-25, 25));
			particleVector.push_back(Particle(color, t, sf::Vector2f(pos.x + rand.x, pos.y + rand.y), pos, sf::Vector2f(accel, 0), true));
		}
	}
	static void spawnParticles(sf::Color color, int amount, float t, sf::Vector2f pos, sf::Vector2f accel) {
		for (int i = 0; i < amount; i++) {
			sf::Vector2f rand(getRandomFloat(-25, 25), getRandomFloat(-25, 25));
			particleVector.push_back(Particle(color, t, sf::Vector2f(pos.x + rand.x, pos.y + rand.y), pos, accel, false));
		}
	}
	static void spawnParticles(sf::Color color, int amount, float t, sf::Vector2f pos) {
		spawnParticles(color, amount, t, pos, sf::Vector2f(0, 0));
	}
	static void spawnParticles(sf::Color color, int amount, sf::Vector2f pos) {
		spawnParticles(color, amount, 0.5, pos);
	}
	static void updateParticles() {
		for (int i = 0; i < particleVector.size();) {
			if (particleVector[i].isDeleted) particleVector.erase(particleVector.begin() + i);
			else {
				particleVector[i].update();
				i++;
			}
		}
	}
private:
	float time;
	sf::Clock clock;
	sf::Sprite vis;
	sf::Vector2f velocity;
	sf::Vector2f originalPos;
	bool radialSpread = false;
	bool isDeleted = false;
	float rotateAmount = 0;
};