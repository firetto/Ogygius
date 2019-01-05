#pragma once
#include "GUI.h"
#include <vector>
class GUIScreen {
public:
	void setTitleText(std::string label, sf::Vector2f position, float textSizeMultiplier) {
		titleText = sf::Text(label, gameFont, WINDOW_DIMENSIONS.x / 20 * textSizeMultiplier);
		titleText.setOrigin(titleText.getGlobalBounds().width / 2, titleText.getCharacterSize() / 2 + titleText.getCharacterSize()*(1.0 / 7));
		if (position.y == -1) position.y = titleText.getGlobalBounds().height / 1.5;
		titleText.setPosition(position);
		titleText.setOutlineThickness(3 * ((textSizeMultiplier > 1) ? textSizeMultiplier / 2 : 1));
	}
	void setTitleText(std::string label, sf::Vector2f position) {
		setTitleText(label, position, 1);
	}
	void setTitleText(std::string label) {
		setTitleText(label, sf::Vector2f(WINDOW_DIMENSIONS.x / 2, -1), 1);
	}
	void setTitleText(std::string label, float textSizeMultiplier) {
		setTitleText(label, sf::Vector2f(WINDOW_DIMENSIONS.x / 2, -1), textSizeMultiplier);
	}

	void setBodyText(std::string label, sf::Vector2f position, float textSizeMultiplier) {
		float size = 0;
		std::istringstream iss(label);
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
					  std::istream_iterator<std::string>{} };
		bodyText = sf::Text("", gameFont, WINDOW_DIMENSIONS.y / 15 * textSizeMultiplier);
		bodyText.setLineSpacing(1.1);
		for (int i = 0; i < tokens.size(); i++) {
			std::string prevString = bodyText.getString();
			if (tokens[i] == "\n") bodyText.setString(prevString + "\n ");
			if (prevString == "") bodyText.setString(tokens[i]);
			else bodyText.setString(prevString + " " + tokens[i]);
			if (bodyText.getPosition().x + bodyText.getGlobalBounds().width > WINDOW_DIMENSIONS.x) {
				bodyText.setString(prevString + "\n" + tokens[i]);
			}
		}
		bodyText.setPosition(position);

		bodyText.setOutlineThickness(2);
	}
	void setBodyText(std::string label, sf::Vector2f position) {
		setBodyText(label, position, 1);
	}
	void setBodyText(std::string label) {
		setBodyText(label, sf::Vector2f(4, WINDOW_DIMENSIONS.y / 5), 1);
	}
	void addButton(std::string label, sf::Vector2f size, sf::Vector2f position, void(*function)()) {
		buttonVector.push_back(Button(label, size, position, function));
	}
	void addCheckmark(int width, sf::Vector2f position, bool * boolean) {
		checkmarkVector.push_back(Checkmark(width, position, boolean));
	}
	void addText(std::string label, sf::Vector2f position, bool middleAlign) {
		textVector.push_back(sf::Text(label, gameFont, WINDOW_DIMENSIONS.y / 16));
		textVector.back().setOrigin((middleAlign) ? textVector.back().getGlobalBounds().width / 2 : 0, textVector.back().getCharacterSize() / 2 + titleText.getCharacterSize()*(1.0 / 7));
		std::cout << textVector.back().getOrigin().y << std::endl;
		textVector.back().setPosition(position);
	}
	void addText(std::string label, sf::Vector2f position) {
		addText(label, position, true);
	}
	sf::Text &getTextByNumber(int n) {
		return textVector[n];
	}
	void update() {
		for (sf::Text &text : textVector) {
			WINDOW.draw(text);
		}
		for (Button &button : buttonVector) {
			button.update();
			button.draw();
		}
		for (Checkmark &checkmark : checkmarkVector) {
			checkmark.update();
			checkmark.draw();
		}
		WINDOW.draw(bodyText);
		WINDOW.draw(titleText);
	}
	void clear() {
		titleText.setString("");
		bodyText.setString("");
		buttonVector.clear();
		textVector.clear();
		checkmarkVector.clear();
	}
private:
	sf::Text titleText;
	std::vector<Button> buttonVector;
	std::vector<sf::Text> textVector;
	std::vector<Checkmark> checkmarkVector;
	sf::Text bodyText;
};
GUIScreen *currentGUIScreen; // the current GUI screen.
GUIScreen mainMenuScreen;
GUIScreen pauseMenuScreen;
GUIScreen gameScreen;
GUIScreen infoScreen;
GUIScreen settingsScreen;
GUIScreen deadScreen;