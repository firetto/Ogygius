// This file is for global variable declarations that do not belong in a class.

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <random>

// definition for pi
#define PI 3.14159265359

// define rad to deg
#define RAD_TO_DEG PI/180

// a list of possible window dimensions
std::map<int, sf::Vector2i> windowDimensionsMap{
	{0, sf::Vector2i(800,600) },
	{1, sf::Vector2i(1024, 768) },
	{2, sf::Vector2i(1280, 720) },
	{3, sf::Vector2i(1280, 800) },
	{4, sf::Vector2i(1366, 768) },
	{5, sf::Vector2i(1440, 900) },
	{6, sf::Vector2i(1600, 900) },
	{7, sf::Vector2i(1680, 1050) },
	{8, sf::Vector2i(1920, 1080) },
};
// current window dimensions index
int currentWindowDimensionsPos = 2;

// default window dimensions
sf::Vector2i defaultWindowDimension = windowDimensionsMap[currentWindowDimensionsPos];

// is fullscreen, and a variable for tracking whether the last frame was fullsccreen
bool isFullscreen = false, wasFullscreen = isFullscreen;

// window dimensions
sf::Vector2i WINDOW_DIMENSIONS = defaultWindowDimension;

// the window name
const std::string WINDOW_NAME = "Ogygius";

// the actual window
sf::RenderWindow WINDOW(sf::VideoMode(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y), WINDOW_NAME);

// how many times a second the physics are updated
const float WINDOW_TIME_SCALE = 60.f; 

// the time to wait before the physics are updated again
const sf::Time WINDOW_TIME_PER_FRAME = sf::seconds(1.f / WINDOW_TIME_SCALE); 

// clock that counts time for frames
sf::Clock WINDOW_FRAME_CLOCK; 

// time since the last update
sf::Time WINDOW_TIME_SINCE_LAST_UPDATE; 

// to handle all events
sf::Event event; 

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
	if (denominator == -1) return false;
	if (getRandomInt(0, int(denominator)) == 0) return true;
	return false;
}

// returns a probability based on a percentage
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

// return the angle between points in a 2d plane
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


