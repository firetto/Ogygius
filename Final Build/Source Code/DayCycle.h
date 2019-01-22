/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

DayCycle.h

Functions to process the day cycle
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "GameDeclarations.h"
#include <iostream>

// a day clock
sf::Clock dayClock;

// number of how many days elapsed
int daysElapsed = 0;

// rectangle alpha variable
int rectAlpha = 0;

// max rect alpha
int rectMaxAlpha = 155;

// resets the day cycle
void resetDay() {
	isDay = true;
	dayClock.restart();
	daysElapsed = 0;
}

// processes day cycle
void processDayCycle() {

	// sets the fill color of the day overlay rectangle
	dayOverlayRect.setFillColor(sf::Color(0, 0, 50, rectAlpha));

	// if day time is greater than the length, invert day variable and restart the clock.
	if (dayClock.getElapsedTime().asSeconds() > GAME_DAY_LENGTH) {
		isDay = !isDay;
		dayClock.restart();
	}

	// if the elapsed time is less than a quarter
	if (dayClock.getElapsedTime().asSeconds() <= GAME_DAY_LENGTH / 4) {
		// set rect alpha if it's day
		if (isDay) rectAlpha = (rectMaxAlpha / 2) - (dayClock.getElapsedTime().asSeconds() / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2));

		// set rect alpha if its night
		else rectAlpha = (dayClock.getElapsedTime().asSeconds() / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2)) + rectMaxAlpha / 2;
	}

	// if the elapsed time is greater than three quarters of the day length
	else if (dayClock.getElapsedTime().asSeconds() >= 3*GAME_DAY_LENGTH/4) {

		// if its day
		if (isDay) rectAlpha = ((dayClock.getElapsedTime().asSeconds() - 3 * GAME_DAY_LENGTH / 4) / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2));

		// if its night
		else rectAlpha = rectMaxAlpha - ((dayClock.getElapsedTime().asSeconds() - 3 * GAME_DAY_LENGTH / 4) / (GAME_DAY_LENGTH / 4)*(rectMaxAlpha / 2));
	}
}

