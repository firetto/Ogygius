#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "declarations.h"
#include "Entity.h"

/*
Camera for the player
*/
class Camera : public sf::View, public Movable {
public:

	// clock for the camera
	sf::Clock cameraClock; 

	// time
	sf::Time cameraTime; 

	// camera physics
	void cameraPhysics(); 

	// camera shake basic
	void cameraShake(double accel); 

	// camera shake with a time modifier
	void cameraShake(double accel, double timeMod); 

	// zoom the camera
	void cameraZoom(float factor); 

	// reset the zoom of the camera
	void cameraZoomReset(); 

	// returns camera zoom
	double getCameraZoom(); 

	// if an entity is in a camera view
	bool inView(sf::FloatRect object); 

	// if the camera is shaking
	bool isShaking; 
private:
	// shake modifier for the camera
	double shakeMod; 

	// time modifier for the
	double timeMod;  

	// zoom factor
	double zoomFactor = 1; // how much the camera is zoomed
};

void Camera::cameraPhysics() { // the physics of the camera
	//basic camera physics
	// this multiples by 2 so the shake is faster

	// adds velocity X to x
	changePosition(sf::Vector2f(getVelocity().x * 2, getVelocity().y * 2));
	changeVelocity(sf::Vector2f(getAcceleration().x * 2, getAcceleration().y * 2));
	setVelocity(sf::Vector2f(getVelocity().x / (GAME_FRICTION * 2), getVelocity().y / (GAME_FRICTION * 2)));
	setAcceleration(sf::Vector2f(getAcceleration().x / (GAME_FRICTION * 2), getAcceleration().y / (GAME_FRICTION * 2)));

	// set the center to be the x, y
	setCenter(getPosition());

	//shake
	if (isShaking) {
		for (int i = shakeMod; i >= 0; i--) {
			//random acceleration
			double presetAccelX = getRandomFloat(-shakeMod, shakeMod); // random acceleration x
			double presetAccelY = getRandomFloat(-shakeMod, shakeMod); // random acceleration y

			int left = getRandomBool(); // if shaking left
			int up = getRandomBool();	// if shaking up

			changeAcceleration(sf::Vector2f((left) ? -presetAccelX / 2 : presetAccelX / 2, (up) ? -presetAccelY / 2 : presetAccelY / 2));
		}
		//stop shaking once time is over
		if (cameraClock.getElapsedTime() >= sf::seconds(timeMod*(shakeMod / 15))) isShaking = false;
	}
}

// camera shake without time modifier, just sets time modifier to 1
void Camera::cameraShake(double accel) { cameraShake(accel, 1); }

//camera shake with time modifier
void Camera::cameraShake(double accel, double time) {
	//shakes the camera
	shakeMod = accel;
	timeMod = time;

	isShaking = true;
	cameraClock.restart();
}

//zooms the camera a specific amount
void Camera::cameraZoom(float factor) {
	zoom(factor);
	zoomFactor *= factor;
}

// resets camera zoom
void Camera::cameraZoomReset() {
	zoom(1.f / zoomFactor);
	zoomFactor = 1;
}

// returns current zoom of camera
double Camera::getCameraZoom() {
	return zoomFactor;
}

bool Camera::inView(sf::FloatRect obj) {
	sf::RectangleShape cameraBoundary;
	cameraBoundary.setSize(sf::Vector2f(getSize().x, getSize().y));
	cameraBoundary.setPosition(getPosition().x - getSize().x / 2, getPosition().y - getSize().y / 2);
	if (cameraBoundary.getGlobalBounds().intersects(obj)) return true;
	return false;
}