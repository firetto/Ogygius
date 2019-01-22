/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

GUIScreen.h 

This file is for declarations of GUI screens, which are screens that contain GUI elements from GUI.h
*/




#pragma once
#include "GUI.h"
#include <vector>

// class declaration for a GUI screen
class GUIScreen {
public:

	// main title text function, takes in a label, position, and a text size multiplier
	void setTitleText(std::string label, sf::Vector2f position, float textSizeMultiplier) {

		// create text
		titleText = sf::Text(label, gameFont, WINDOW_DIMENSIONS.x / 20 * textSizeMultiplier);

		// set origin
		titleText.setOrigin(titleText.getGlobalBounds().width / 2, titleText.getCharacterSize() / 2 + titleText.getCharacterSize()*(1.0 / 7));

		// if the position is -1, set it to default value
		if (position.y == -1) position.y = titleText.getGlobalBounds().height / 1.5;

		// set position
		titleText.setPosition(position);

		// set outline thickness
		titleText.setOutlineThickness(3 * ((textSizeMultiplier > 1) ? textSizeMultiplier / 2 : 1));
	}

	//title text function without text size multiplier
	void setTitleText(std::string label, sf::Vector2f position) {
		setTitleText(label, position, 1);
	}

	// title text function wihout position and text size multiplier
	void setTitleText(std::string label) {
		setTitleText(label, sf::Vector2f(WINDOW_DIMENSIONS.x / 2, -1), 1);
	}

	// title text function without position
	void setTitleText(std::string label, float textSizeMultiplier) {
		setTitleText(label, sf::Vector2f(WINDOW_DIMENSIONS.x / 2, -1), textSizeMultiplier);
	}

	// main body text function
	void setBodyText(std::string label, sf::Vector2f position, float textSizeMultiplier) {

		// local size variable
		float size = 0;

		// string stream
		std::istringstream iss(label);

		// separating the label into separate words
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
					  std::istream_iterator<std::string>{} };

		// create body text
		bodyText = sf::Text("", gameFont, WINDOW_DIMENSIONS.y / 15 * textSizeMultiplier);

		// set line spacing
		bodyText.setLineSpacing(1.1);

		// loops through every word
		for (int i = 0; i < tokens.size(); i++) {

			// stores previous string
			std::string prevString = bodyText.getString();

			// if the word is a line separator
			if (tokens[i] == "_NEWLINE_") bodyText.setString(prevString + "\n");

			// if the previous string is blank
			if (prevString == "") bodyText.setString(tokens[i]);

			// otherwise, add on the word
			else if (tokens[i] != "_NEWLINE_") bodyText.setString(prevString + ((i != 0 && tokens[i - 1] == "_NEWLINE_") ? "" : " ") + tokens[i]);

			// if the text overflows
			if (bodyText.getPosition().x + bodyText.getGlobalBounds().width > WINDOW_DIMENSIONS.x) {

				// new line
				bodyText.setString(prevString + "\n" + tokens[i]);
			}
		}

		// set position
		bodyText.setPosition(position);

		// set outline thickness of text
		bodyText.setOutlineThickness(2);
	}

	// body text without text size multiplier
	void setBodyText(std::string label, sf::Vector2f position) {
		setBodyText(label, position, 1);
	}

	// body text without position
	void setBodyText(std::string label) {
		setBodyText(label, sf::Vector2f(4, WINDOW_DIMENSIONS.y / 5), 1);
	}

	// add button
	void addButton(std::string label, sf::Vector2f size, sf::Vector2f position, void(*function)()) {
		buttonVector.push_back(Button(label, size, position, function));
	}

	// add checkmark
	void addCheckmark(int width, sf::Vector2f position, bool * boolean) {
		checkmarkVector.push_back(Checkmark(width, position, boolean));
	}

	// main add text function
	void addText(std::string label, sf::Vector2f position, bool middleAlign, bool outline) {

		// add text to array
		textVector.push_back(sf::Text(label, gameFont, WINDOW_DIMENSIONS.y / 16));

		// set the origin
		textVector.back().setOrigin((middleAlign) ? textVector.back().getGlobalBounds().width / 2 : 0, textVector.back().getCharacterSize() / 2 + titleText.getCharacterSize()*(1.0 / 7));
		
		// set outline
		if (outline) textVector.back().setOutlineThickness(2);
		textVector.back().setPosition(position);
	}

	// add text without outline
	void addText(std::string label, sf::Vector2f position, bool middleAlign) {
		addText(label, position, middleAlign, false);
	}

	// add text without option for no middle align
	void addText(std::string label, sf::Vector2f position) {
		addText(label, position, true);
	}

	void changeText(int index, std::string label) {
		textVector[index].setString(label);
		textVector[index].setOrigin(textVector[index].getGlobalBounds().width / 2, textVector[index].getCharacterSize() / 2 + textVector[index].getCharacterSize()*(1.0 / 7));
	}

	// get a text element by the n
	sf::Text &getTextByNumber(int n) {
		return textVector[n];
	}

	// update all
	void update() {

		// draw all text
		for (sf::Text &text : textVector) {
			WINDOW.draw(text);
		}

		// update and draw all buttons
		for (Button &button : buttonVector) {
			button.update();
			button.draw();
		}

		// update and draw all checkmarks
		for (Checkmark &checkmark : checkmarkVector) {
			checkmark.update();
			checkmark.draw();
		}

		// draw body and title text
		WINDOW.draw(bodyText);
		WINDOW.draw(titleText);
	}

	// clear everything
	void clear() {
		titleText.setString("");
		bodyText.setString("");
		buttonVector.clear();
		textVector.clear();
		checkmarkVector.clear();
	}
private:

	// title text
	sf::Text titleText;

	// array of buttons
	std::vector<Button> buttonVector;

	// array of text
	std::vector<sf::Text> textVector;

	// array of checkmarks
	std::vector<Checkmark> checkmarkVector;

	// body text
	sf::Text bodyText;
};

// the current GUI screen
GUIScreen *currentGUIScreen; 

// gui screen for main menu
GUIScreen mainMenuScreen;

// gui screen for pause menu
GUIScreen pauseMenuScreen;

// gui screen for game screen
GUIScreen gameScreen;

// gui screen for info
GUIScreen infoScreen;

// gui screen for settings
GUIScreen settingsScreen;

// gui screen for dead screen
GUIScreen deadScreen;

// gui screen for controls
GUIScreen controlsScreen;

// gui screen for credits
GUIScreen creditScreen;

// gui screen for how to play
GUIScreen h2pScreen;