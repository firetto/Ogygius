/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

GUI.h 

This file is for the declarations of GUI elements, such as buttons, etc.
*/




#pragma once
#include <SFML/Graphics.hpp>
#include "GameDeclarations.h"
#include "Textures.h"
#include <iostream>
#include <string>
//// GUI ELEMENT ////
class GUIElement {
protected:
	// default constructor for GUI element
	GUIElement();

	// visual sprite for button
	sf::Sprite m_vis;

	// hitbox for clicking button
	sf::RectangleShape m_boundary;

	// text to be displayed
	sf::Text m_text;

	// GUI element dimensions
	unsigned m_width, m_height;

	// if this is enabled, then the sprite will fill the boundary
	bool m_spriteFillEnabled;

	// amount of sprite fill
	sf::Vector2f m_spriteFillAmount;

	// if sprite filling is enabled, then the sprite will fill the boundary
	void m_spriteFill();

	// button draw function
	void m_draw();

	// if this element is hovered
	bool m_isHover();

	// if this element is clicked
	bool m_isClick();

	bool hasClicked = false;
};
GUIElement::GUIElement() {
	hasClicked = false;
}
void GUIElement::m_draw() {
	// draws button image first
	WINDOW.draw(m_vis);

	// draws button text
	WINDOW.draw(m_text);
}

void GUIElement::m_spriteFill() {
	// if sprite filling is enabled, then the sprite will set scale to
	// the boundary size divided by original vis position
	if (m_spriteFillEnabled) {
		m_spriteFillAmount = sf::Vector2f(m_boundary.getGlobalBounds().width / m_vis.getGlobalBounds().width, m_boundary.getGlobalBounds().height / m_vis.getGlobalBounds().height);
		m_vis.setScale(m_spriteFillAmount);
		std::cout << m_spriteFillAmount.x << ", " << m_spriteFillAmount.y << std::endl;
	}
}


bool GUIElement::m_isHover() {
	// mouse position based on the window
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(WINDOW).x, sf::Mouse::getPosition(WINDOW).y);

	// if mouse position is inside of the GUI element boundary, returns true
	if (m_boundary.getGlobalBounds().contains(mousePos)) return true;

	// otherwise, return false
	return false;
}

bool GUIElement::m_isClick() {
	if (event.type == sf::Event::MouseButtonPressed && // if a button is pressed
		event.mouseButton.button == sf::Mouse::Left && // if it's the left mouse button that is pressed
		GAME_CLICK_DELAY_PASSED &&
		!hasClicked
		) {
		GAME_CLICK_DELAY_PASSED = false;
		hasClicked = true;
		return true; // returns true if all of that is true
	}

	// otherwise, returns false
	return false;
}

//// BUTTON ////

class Button : public GUIElement {
public:
	// setup the button
	Button() {}
	Button(std::string label, sf::Vector2f dimensions, sf::Vector2f position, void(*function)());
	
	void *pressFunction;

	// update the button, place after window.draw(). function to execute when the button is clicked is the parameter
	void update();

	void draw();
};

Button::Button(std::string label, sf::Vector2f dimensions, sf::Vector2f position, void (*function)()) {

	pressFunction = (*function);

	// set font for the text
	m_text.setFont(gameFont);

	// set string of the button to the label
	m_text.setString(label);

	// set character size of the button label
	m_text.setCharacterSize(dimensions.y / 1.75);

	// set letter spacing for button
	m_text.setLetterSpacing(1);
	m_text.setLineSpacing(0);

	// set origin for text
	m_text.setOrigin(m_text.getGlobalBounds().width / 2, m_text.getCharacterSize() / 2 + m_text.getCharacterSize()*(1.0/7));

	// set button dimensions
	m_boundary.setSize(dimensions);

	m_boundary.setFillColor(sf::Color(100, 100, 100));
	m_boundary.setOutlineThickness(3);
	m_boundary.setOutlineColor(sf::Color(50, 50, 50));



	// set origin for button boundary
	m_boundary.setOrigin(m_boundary.getGlobalBounds().width / 2, m_boundary.getGlobalBounds().height / 2);

	// set position for text
	m_text.setPosition(position);

	// set position for button boundary
	m_boundary.setPosition(position);

}

void Button::draw() {
	// draw rect
	WINDOW.draw(m_boundary);

	// draw text
	WINDOW.draw(m_text);
}

void Button::update() {

	// if hovering
	if (m_isHover()) {
		// if the mouse is being held
		if (hasClicked) {
			// set fill color
			m_boundary.setFillColor(sf::Color(90, 90, 90));

			// if release
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == 0) {
				
				// perform function
				((void(*)(void)) pressFunction)();

				// no longer being held
				hasClicked = false;
			}
		}
		// if just being hovered
		else {
			// fill color
			m_boundary.setFillColor(sf::Color(100, 100, 100));

			// update click
			m_isClick();
		}
	}
	// if not being hovered, set to normal color
	else m_boundary.setFillColor(sf::Color(128, 128, 128));
}

//// CHECKMARK ////

class Checkmark : public GUIElement {
public:
	// setup the checkmark
	Checkmark() {}
	Checkmark(int width, sf::Vector2f position, bool * boolean);

	// update the checkmark
	void update();

	// draw
	void draw();

private:
	// the rectangle inside of the checkmark (when it's on)
	sf::RectangleShape insideRect;

	// how thick the outline is compared to the size of the checkmark (multiplier)
	float outlineThicknessFactor = 0.2;

	// whether the checkmark is enabled (turned on)
	bool isEnabled = false;

	// the boolean that the checkmark triggers (pointer)
	bool *boolToHold;
};

Checkmark::Checkmark(int width, sf::Vector2f position, bool * boolean) {

	// set checkmark dimensions
	m_boundary.setSize(sf::Vector2f(width*(1.0-outlineThicknessFactor), width*(1.0 - outlineThicknessFactor)));

	// set origin for checkmark boundary
	m_boundary.setOrigin(m_boundary.getSize().x / 2, m_boundary.getSize().y / 2);

	// set outline params
	m_boundary.setOutlineThickness(width*outlineThicknessFactor);
	m_boundary.setOutlineColor(sf::Color::White);

	// inside of the outline is transparent
	m_boundary.setFillColor(sf::Color::Transparent);

	// set position for button boundary
	m_boundary.setPosition(position);

	// set params for the inside rect
	insideRect.setSize(sf::Vector2f(width*(1.0 - outlineThicknessFactor * 3), width*(1.0 - outlineThicknessFactor * 3)));
	insideRect.setOrigin(insideRect.getSize().x / 2, insideRect.getSize().y / 2);
	insideRect.setPosition(position);

	// set booleans
	boolToHold = boolean;
	isEnabled = *boolToHold;

	// set fill color depending on boolean
	if (isEnabled) insideRect.setFillColor(sf::Color::White);
	else insideRect.setFillColor(sf::Color::Transparent);
}

// update function
void Checkmark::update() {
	// if hovering
	if (m_isHover()) {

		// if holding
		if (hasClicked) {

			// set fill color
			m_boundary.setFillColor(sf::Color(60, 60, 60, 64));

			// once released
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == 0) {

				// invert boolean
				isEnabled = !isEnabled;
				*boolToHold = isEnabled;

				// change colors
				if (isEnabled) insideRect.setFillColor(sf::Color::White);
				else insideRect.setFillColor(sf::Color::Transparent);

				// set holding variable to false
				hasClicked = false;
			}
		}

		// if just hovering
		else {

			// set color
			m_boundary.setFillColor(sf::Color(30, 30, 30, 64));

			// update click
			m_isClick();
		}
	}

	// if not hovering, set color of inside rect to be transparent
	else m_boundary.setFillColor(sf::Color::Transparent);
}

// draw checkmark
void Checkmark::draw() {

	// draw boundary
	WINDOW.draw(m_boundary);

	// draw inside rectangle
	WINDOW.draw(insideRect);
}