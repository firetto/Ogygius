/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Controls.h

Contains the class definitions for the joystick and keyboard class
*/

#pragma once
#include "Declarations.h"
#include <SFML/Graphics.hpp>
namespace ctrl {
	class Joystick;
	class Keyboard;
}

class ctrl::Joystick {
public:
	enum StickDirection { UP, DOWN, LEFT, RIGHT };
	//normal joystick button press, outside of event loop
	bool press(unsigned int button, unsigned int player) {
		if (sf::Joystick::isConnected(player - 1) &&
			sf::Joystick::isButtonPressed(player - 1, button)) return true;
		else return false;
	}

	//normal joystick button release
	bool release(unsigned int button, unsigned int player) {
		if (sf::Joystick::isConnected(player - 1) &&
			(event.type == sf::Event::JoystickButtonReleased &&
				event.joystickButton.button == button &&
				event.joystickButton.joystickId == player - 1)) return true;
		else return false;
	}

	//event joystick button press, inside of event loop
	bool eventPress(unsigned int button, unsigned int player) {
		if (sf::Joystick::isConnected(player - 1) &&
			(event.type == sf::Event::JoystickButtonPressed &&
				event.joystickButton.button == button &&
				event.joystickButton.joystickId == player - 1)) return true;
		else return false;
	}
	class Stick {
	public:
		float dirX, dirY;
		enum Direction { UP, DOWN, LEFT, RIGHT };
		bool inDirection(Direction stickDir) {
			switch (stickDir) {
			case UP:	if (dirY <= -75) return true; break;
			case DOWN:	if (dirY >= 75)	 return true; break;
			case LEFT:	if (dirX <= -75) return true; break;
			case RIGHT: if (dirX >= 75)  return true; break;
			}
			return false;
		}
	};
	class DPAD : public Stick {
	public:
		void update(int playerNo) {
			dirX = sf::Joystick::getAxisPosition(playerNo - 1, sf::Joystick::PovX);
			dirY = sf::Joystick::getAxisPosition(playerNo - 1, sf::Joystick::PovY);
		}
	};
	class LTMB : public Stick {
	public:
		void update(int playerNo) {
			dirX = sf::Joystick::getAxisPosition(playerNo - 1, sf::Joystick::X);
			dirY = sf::Joystick::getAxisPosition(playerNo - 1, sf::Joystick::Y);
		}
	};
	DPAD dPad;
	LTMB lTmb;

	void update(int playerNo) {
		//updates thumbstick movement
		dPad.update(playerNo);
		lTmb.update(playerNo);
	}
};

//KEYBOARD CLASS, if you want to use a keyboard.
class ctrl::Keyboard {
public:
	//normal key press, outside of event loop
	bool press(sf::Keyboard::Key key) {
		if (sf::Keyboard::isKeyPressed(key)) return true;
		else return false;
	}

	//key release
	bool release(sf::Keyboard::Key key) {
		if (event.type == sf::Event::KeyReleased &&
			event.key.code == key) return true;
		else return false;
	}

	//event key release, place in event loop
	bool eventPress(sf::Keyboard::Key key) {
		if (event.type == sf::Event::KeyPressed &&
			event.key.code == key) return true;
		else return false;
	}
};
ctrl::Keyboard keyboard; ctrl::Joystick joystick;