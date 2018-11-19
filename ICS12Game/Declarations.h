// This file is for global variable declarations that do not belong in a class.

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <random>

#define PI 3.14159265359

#define RAD_TO_DEG PI/180

sf::Vector2i WINDOW_DIMENSIONS = sf::Vector2i(1600, 900);

const std::string WINDOW_NAME = "Ogygius";

sf::RenderWindow WINDOW(sf::VideoMode(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y), WINDOW_NAME); // the actual window

const float WINDOW_TIME_SCALE = 60.f; // how many times a second the physics are updated

const sf::Time WINDOW_TIME_PER_FRAME = sf::seconds(1.f / WINDOW_TIME_SCALE); // the time to wait before the physics are updated again

sf::Clock WINDOW_FRAME_CLOCK; // clock that counts time for frames

sf::Time WINDOW_TIME_SINCE_LAST_UPDATE; // time since the last update

sf::Event event; // to handle all events

// returns a random float

float getRandomFloat(double range1, double range2) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(range1, range2);
	return dist(mt);
}

// returns a random int
int getRandomInt(int range1, int range2) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(range1, range2);
	return dist(mt);
}

// returns a random boolean
bool getRandomBool() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0, 2);
	return int(dist(mt));
}

// returns a probability based on a denominator (denominator of 2 means 50%, of 5 means 20%, etc.)
bool getChance(float denominator) {
	if (getRandomInt(0, int(denominator)) == 0) return true;
	return false;
}

bool getPercentage(float percentage) {
	if (getRandomInt(0, 100 - int(percentage)) == 0) return true;
	return false;
}

// get middle of a sprite
sf::Vector2f getMiddle(sf::Sprite vis) {
	return sf::Vector2f(vis.getGlobalBounds().width / 2, vis.getGlobalBounds().height / 2);
}

// return the distance between points in a 2d plane
float getDistanceBetweenPoints(sf::Vector2f point1, sf::Vector2f point2) {
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}
float getAngleBetweenPoints(sf::Vector2f point1, sf::Vector2f point2) {
	return atan2(point1.y - point2.y, point1.x - point2.x) * 180 / PI;
}

// output an error message
void outputError(std::string str) {
	std::cout << "ERROR: " << str << "\n";
}

// output an info mesage
void outputInfo(std::string str) {
	std::cout << "INFO: " << str << "\n";
}