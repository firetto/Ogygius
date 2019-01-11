// loads the sprite sheets

#pragma once
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "Textures.h"

// declares a fist, axe, and pickaxe animation
Animation fistAnimation, axeAnimation, pickAnimation;
void animationLoad() {

	// loads sprite sheet for fist animation
	fistAnimation.setSpriteSheet(anim_fistTex);
	fistAnimation.addFrame(sf::IntRect(0, 0, 192, 192));
	fistAnimation.addFrame(sf::IntRect(192, 0, 192, 192));
	fistAnimation.addFrame(sf::IntRect(384, 0, 192, 192));
	fistAnimation.addFrame(sf::IntRect(576, 0, 192, 192));
	fistAnimation.addFrame(sf::IntRect(768, 0, 192, 192));
	fistAnimation.addFrame(sf::IntRect(960, 0, 192, 192));
	fistAnimation.addFrame(sf::IntRect(0, 192, 192, 192));
	fistAnimation.addFrame(sf::IntRect(192, 192, 192, 192));
	fistAnimation.addFrame(sf::IntRect(384, 192, 192, 192));
	fistAnimation.addFrame(sf::IntRect(576, 192, 192, 192));
	fistAnimation.addFrame(sf::IntRect(768, 192, 192, 192));

	// loads sprite sheet for axe animation
	axeAnimation.setSpriteSheet(anim_axeTex);
	axeAnimation.addFrame(sf::IntRect(0, 0, 192, 192));
	axeAnimation.addFrame(sf::IntRect(192, 0, 192, 192));
	axeAnimation.addFrame(sf::IntRect(384, 0, 192, 192));
	axeAnimation.addFrame(sf::IntRect(576, 0, 192, 192));
	axeAnimation.addFrame(sf::IntRect(768, 0, 192, 192));
	axeAnimation.addFrame(sf::IntRect(960, 0, 192, 192));
	axeAnimation.addFrame(sf::IntRect(0, 192, 192, 192));
	axeAnimation.addFrame(sf::IntRect(192, 192, 192, 192));
	axeAnimation.addFrame(sf::IntRect(384, 192, 192, 192));
	axeAnimation.addFrame(sf::IntRect(576, 192, 192, 192));
	axeAnimation.addFrame(sf::IntRect(768, 192, 192, 192));

	// loads sprite sheet for pickaxe animation
	pickAnimation.setSpriteSheet(anim_pickTex);
	pickAnimation.addFrame(sf::IntRect(0, 0, 192, 192));
	pickAnimation.addFrame(sf::IntRect(192, 0, 192, 192));
	pickAnimation.addFrame(sf::IntRect(384, 0, 192, 192));
	pickAnimation.addFrame(sf::IntRect(576, 0, 192, 192));
	pickAnimation.addFrame(sf::IntRect(768, 0, 192, 192));
	pickAnimation.addFrame(sf::IntRect(960, 0, 192, 192));
	pickAnimation.addFrame(sf::IntRect(0, 192, 192, 192));
	pickAnimation.addFrame(sf::IntRect(192, 192, 192, 192));
	pickAnimation.addFrame(sf::IntRect(384, 192, 192, 192));
	pickAnimation.addFrame(sf::IntRect(576, 192, 192, 192));
	pickAnimation.addFrame(sf::IntRect(768, 192, 192, 192));
}