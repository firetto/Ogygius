#pragma once
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "Textures.h"
Animation fistAnimation, axeAnimation, pickAnimation;
void animationLoad() {
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
	/*
	pickAnimation.setSpriteSheet(anim_pickTex);
	pickAnimation.addFrame(sf::IntRect(0, 0, 160, 160));
	pickAnimation.addFrame(sf::IntRect(160, 0, 160, 160));
	pickAnimation.addFrame(sf::IntRect(320, 0, 160, 160));
	pickAnimation.addFrame(sf::IntRect(480, 0, 160, 160));
	pickAnimation.addFrame(sf::IntRect(640, 0, 160, 160));
	pickAnimation.addFrame(sf::IntRect(800, 0, 160, 160));
	pickAnimation.addFrame(sf::IntRect(0, 160, 160, 160));
	pickAnimation.addFrame(sf::IntRect(160, 160, 160, 160));
	pickAnimation.addFrame(sf::IntRect(320, 160, 160, 160));
	pickAnimation.addFrame(sf::IntRect(480, 160, 160, 160));
	pickAnimation.addFrame(sf::IntRect(640, 160, 160, 160));
	*/
}