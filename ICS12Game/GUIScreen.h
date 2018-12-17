#pragma once
#include "GUI.h"
#include <vector>
class GUIScreen {
public:
	void setTitleText(std::string label, sf::Vector2f position, float textSizeMultiplier) {
		titleText = sf::Text(label, gameFont, WINDOW_DIMENSIONS.y / 10 * textSizeMultiplier);
		titleText.setOrigin(titleText.getGlobalBounds().width / 2, titleText.getGlobalBounds().height / 2 + titleText.getCharacterSize()*(1.0 / 7));
		if (position.y == -1) position.y = titleText.getGlobalBounds().height / 1.5;
		titleText.setPosition(position);
		titleText.setOutlineThickness(3 * ((textSizeMultiplier>1) ? textSizeMultiplier/2 : 1));
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
	void update() {
		for (Button &button : buttonVector) {
			button.update();
			button.draw();
		}
		WINDOW.draw(bodyText);
		WINDOW.draw(titleText);
	}
private:
	sf::Text titleText;
	std::vector<Button> buttonVector;
	sf::Text bodyText;
};
GUIScreen *currentGUIScreen; // the current GUI screen.
GUIScreen mainMenuScreen;
GUIScreen pauseMenuScreen;
GUIScreen gameScreen;
GUIScreen infoScreen;