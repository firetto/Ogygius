#pragma once
#include <SFML/Graphics.hpp>
#include "Textures.h"
#include "GameDeclarations.h"
#include "Object.h"
class Particle;
std::vector<Particle> particleVector;
class Particle {
public:
	Particle(sf::Color color, float t, sf::Vector2f pos) {
		time = t;
		vis.setTexture(particleTex);
		vis.setOrigin(getMiddle(vis));
		vis.setPosition(pos);
		vis.setColor(color);
		vis.setPosition(pos);
		rotateAmount = getRandomFloat(-20, 20);
	}
	Particle(sf::Color color, sf::Vector2f pos) {
		Particle(color, 0.5, pos);
	}
	void update() {
		vis.setColor(sf::Color(vis.getColor().r, vis.getColor().g, vis.getColor().b, 255 - 255 * (clock.getElapsedTime().asSeconds() / time)));
		vis.rotate(rotateAmount);
		if (clock.getElapsedTime().asSeconds() >= time) isDeleted = true;
		else drawableVector.push_back(ObjDrawable(vis, 4.5));

	}
	static void spawnParticles(sf::Color color, int amount, float t, sf::Vector2f pos) {
		for (int i = 0; i < amount; i++) {
			sf::Vector2f rand(getRandomFloat(-25, 25), getRandomFloat(-25,25));
			particleVector.push_back(Particle(color, t, sf::Vector2f(pos.x + rand.x, pos.y + rand.y)));
		}
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
	bool isDeleted = false;
	float rotateAmount = 0;
};