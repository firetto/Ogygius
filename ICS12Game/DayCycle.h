#pragma once
#include <SFML/Graphics.hpp>
#include "GameDeclarations.h"
#include <iostream>

sf::Clock dayClock;
int daysElapsed = 0;
bool isDay = true;
int rectAlpha = 0;
int rectMaxAlpha = 128;


void processDayCycle() {
	dayOverlayRect.setFillColor(sf::Color(0, 0, 50, rectAlpha));
	if (dayClock.getElapsedTime().asSeconds() > GAME_DAY_LENGTH) {
		isDay = !isDay;
		dayClock.restart();
	}
	if (dayClock.getElapsedTime().asSeconds() <= GAME_DAY_LENGTH / 4) {
		if (isDay) {
			rectAlpha = (rectMaxAlpha / 2) - (dayClock.getElapsedTime().asSeconds() / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2));
			
		}
		else {
			rectAlpha = (dayClock.getElapsedTime().asSeconds() / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2)) + rectMaxAlpha / 2;
			
		}
	}
	else if (dayClock.getElapsedTime().asSeconds() >= 3*GAME_DAY_LENGTH/4) {
		if (isDay) {
			rectAlpha = ((dayClock.getElapsedTime().asSeconds() - 3 * GAME_DAY_LENGTH / 4) / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2));
			
		}
		else {
			rectAlpha = rectMaxAlpha - ((dayClock.getElapsedTime().asSeconds() - 3 * GAME_DAY_LENGTH / 4) / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2));
			
		}
	}
}

