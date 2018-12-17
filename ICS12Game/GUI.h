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
	m_text.setCharacterSize(dimensions.y / 1.5);

	// set letter spacing for button
	m_text.setLetterSpacing(1);
	m_text.setLineSpacing(0);

	// set origin for text
	m_text.setOrigin(m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().height / 2 + m_text.getCharacterSize()*(1.0/7));

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
	WINDOW.draw(m_text);
}

void Button::update() {
	if (m_isHover()) {
		if (hasClicked) {
			m_boundary.setFillColor(sf::Color(90, 90, 90));
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == 0) {
				((void(*)(void)) pressFunction)();
				hasClicked = false;
			}
		}
		else {
			m_boundary.setFillColor(sf::Color(100, 100, 100));
			m_isClick();
		}
	}
	else {
		m_boundary.setFillColor(sf::Color(128, 128, 128));
	}
}

//// CHECKMARK ////
/*
class Checkmark : public GUIElement {
public:
	// setup the button
	Checkmark() {}
	Checkmark(sf::Vector2f dimensions, sf::Vector2f position, bool boolean, std::string hoverLabel);

	// update the button, place after window.draw()
	void update(bool &boolean);

private:
	// size multiplier when hovering
	float size_hoverMultiplier = 1.15;

	// how long it takes for the hover to zoom in
	float hoverTime = 0.2;

	// clock for hovering
	sf::Clock clock;

	// final multiplier
	double hoverMultiplier, prevHoverMultiplier;

	// has finished hover zoom
	bool finishedHoverZoom = false;

	bool justHovered = false, justReleased = false, isEnabled = false;
};

Checkmark::Checkmark(sf::Vector2f dimensions, sf::Vector2f position, bool boolean, std::string hoverLabel) {

	// set vis sprite texture
	m_vis.setTexture(checkmarkTexture);

	// set button dimensions
	m_boundary.setSize(dimensions);
	m_height = dimensions.y; m_width = dimensions.x;

	// set origin for button boundary
	m_boundary.setOrigin(m_boundary.getSize().x / 2, m_boundary.getSize().y / 2);

	// set vis for button
	m_vis.setOrigin(m_vis.getGlobalBounds().width / 2, m_vis.getGlobalBounds().height / 2);

	// set position for button boundary
	m_boundary.setPosition(position.x - m_width / 2, position.y);

	// set position for button sprite
	m_vis.setPosition(position);

	isEnabled = boolean;

	if (isEnabled) {
		m_vis.setTextureRect(sf::IntRect(16, 0, 16, 16));
	}
	else {
		m_vis.setTextureRect(sf::IntRect(0, 0, 16, 16));
	}

	// set sprite fill
	m_spriteFillEnabled = true;

	// sprite fill
	m_spriteFill();
}

void Checkmark::update(bool &boolean) {
	// if button is hovered by mouse
	if (m_isHover()) {
		if (justReleased) {
			finishedHoverZoom = false;
			justReleased = false;
			clock.restart();
			prevHoverMultiplier = hoverMultiplier;
		}
		if (clock.getElapsedTime().asSeconds() <= hoverTime && !finishedHoverZoom) hoverMultiplier = (size_hoverMultiplier - prevHoverMultiplier) * pow(1 / hoverTime, 2) * pow(clock.getElapsedTime().asSeconds(), 2) + prevHoverMultiplier;
		else if (clock.getElapsedTime().asSeconds() > hoverTime) {
			hoverMultiplier = size_hoverMultiplier;
			clock.restart();
			finishedHoverZoom = true;
		}
		// set scale of text when hovering
		if (m_isClick()) {
			if (isEnabled) {
				isEnabled = false;
				boolean = false;
			}
			else {
				isEnabled = true;
				boolean = true;
			}
			playVitalSound(buttonPressBuff);
		}
		justHovered = true;
	}
	else {
		if (justHovered) {
			finishedHoverZoom = false;
			justHovered = false;
			prevHoverMultiplier = hoverMultiplier;
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() <= hoverTime && !finishedHoverZoom) hoverMultiplier = -((prevHoverMultiplier - 1) * 100) * pow(1 / hoverTime, 2) * pow(clock.getElapsedTime().asSeconds(), 2) + prevHoverMultiplier;
		else if (clock.getElapsedTime().asSeconds() > hoverTime) {
			hoverMultiplier = 1;
			clock.restart();
			finishedHoverZoom = true;
		}
		if (hoverMultiplier < 1) hoverMultiplier = 1;
		if (hoverMultiplier > size_hoverMultiplier) hoverMultiplier = size_hoverMultiplier;
		justReleased = true;
	}
	if (isEnabled) {
		m_vis.setTextureRect(sf::IntRect(16, 0, 16, 16));
	}
	else {
		m_vis.setTextureRect(sf::IntRect(0, 0, 16, 16));
	}
	m_draw();
}*/