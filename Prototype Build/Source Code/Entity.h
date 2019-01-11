#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "GameDeclarations.h"
//#include "Textures.h"
class Movable : virtual public Positionable {
public:

	// returns velocity of the movable object
	sf::Vector2f getVelocity() { return velocity; }

	// returns acceleration of the movable object
	sf::Vector2f getAcceleration() { return acceleration; }

	// set velocity of the movable object
	void setVelocity(sf::Vector2f _vel) {
		velocity = _vel;
	}
	void changeVelocity(sf::Vector2f _vel) {
		velocity.x += _vel.x;
		velocity.y += _vel.y;
	}

	// set acceleration of the movable object
	void setAcceleration(sf::Vector2f _accel) {
		acceleration = _accel;
	}

	// change acceleration of the movable object
	void changeAcceleration(sf::Vector2f _accel) {
		acceleration.x += _accel.x;
		acceleration.y += _accel.y;
	}

private:
	sf::Vector2f velocity, acceleration;
};

class Entity : virtual public Object, public Movable {
public:
	bool inWater = false;
	Entity() {
		time = clock.restart();
	}

	void physics() {
		// adds velocities to coordinates
		Object::changePosition(sf::Vector2f(
			(inWater) ? getVelocity().x * GAME_WATER_SLOW_AMOUNT : getVelocity().x,
			(inWater) ? getVelocity().y * GAME_WATER_SLOW_AMOUNT : getVelocity().y));

		// adds velocities to acceleration
		changeVelocity(sf::Vector2f(
			(inWater) ? getAcceleration().x * GAME_WATER_SLOW_AMOUNT : getAcceleration().x,
			(inWater) ? getAcceleration().y * GAME_WATER_SLOW_AMOUNT : getAcceleration().y));

		// set velocity
		setVelocity(sf::Vector2f(
			getVelocity().x / GAME_FRICTION,
			getVelocity().y / GAME_FRICTION
		));

		// set acceleration
		setAcceleration(sf::Vector2f(
			getAcceleration().x / GAME_FRICTION,
			getAcceleration().y / GAME_FRICTION));

		// sets positions of sprite and hitbox
		setPosition(getPosition());
	}

protected:
	// a clock for whatever the entity has to do with it
	sf::Clock clock;

	// a time that does stuff with the clock
	sf::Time time;

};
