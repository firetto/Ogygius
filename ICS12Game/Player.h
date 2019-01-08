#pragma once
#include "Controls.h"
#include "Camera.h"
#include "AnimationDeclaration.h"
#include "ItemBar.h"
#include "Mob.h"
#include "GUIScreen.h"

class Player : public Mob {
public:
	Player() {

		height = 6;

		vis.setTexture(playerTex);
		vis.setOrigin(vis.getGlobalBounds().width / 2, vis.getGlobalBounds().height / 2);
		setPosition(sf::Vector2f(GAME_TOTAL_SIZE / 2, GAME_TOTAL_SIZE / 2));

		resetView();

		// restarts the attack clock
		attackClock.restart();

		// play the animation
		weaponSprite.play(*weaponAnimation);
		weaponSprite.stop();

		// set collision sprite for fists
		weaponCollisionSprite.setTexture(anim_fistTex);
		canMove = true;
		setHealth(GAME_PLAYER_DEFAULT_HEALTH);
		isDead = false;
	}

	// update the player physics, etc.
	void update() {
		// update controls
		controls();

		// update physics
		physics();

		// rotate the player sprite
		if (!MOUSE_IN_HUD && !GAME_PAUSED) rotatePlayer();

		// updates camera
		view.cameraPhysics();

		// sets center of camera
		view.setPosition(getPosition());

		// handle animations
		handleAnimations();

		// handle clocks
		handleClocks();

		// update player boundaries
		playerBoundaries();

		// if dropping item, drop item
		dropItem();

		setCurrSquare();

		draw();

		placeBreakables();

		useItem();

		isInWater();

		releaseParticles();

		lastFrameCollision = false;

		if (health <= 0) {
			isDead = true;
			currentGUIScreen = &deadScreen;
			GAME_PAUSED = true;
		}


	}

	void resetView() {
		view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
		view.setSize(sf::Vector2f(WINDOW_DIMENSIONS));
		view.cameraZoom(float(defaultWindowDimension.y) / WINDOW_DIMENSIONS.y);
	}

	// get a hypotenuse based on player rotation
	sf::Vector2f getFacing() {
		double newXChange, newYChange;
		if (rotation < 90) {
			newXChange = -cos(rotation*PI / 180);
			newYChange = -sin(rotation*PI / 180);
		}
		else if (rotation < 180) {
			newXChange = sin((rotation - 90)*PI / 180);
			newYChange = -cos((rotation - 90)*PI / 180);
		}
		else {
			newXChange = cos(rotation*PI / 180);
			newYChange = -sin(rotation*PI / 180);
		}
		return sf::Vector2f(newXChange, newYChange);
	}

	// draw the player 
	void draw() {
		if (isDead) return;
		drawableVector.emplace_back(ObjDrawable(weaponCollisionSprite, height - 0.01));
		Object::draw();
		if (isPlacing) drawableVector.emplace_back(ObjDrawable(placeSprite, 1000));
	}

	void eventUpdate() {
		Animation *temp = &fistAnimation;
		switch (items.getItemSelected().type) {
		case ITEM_AXE: temp = &axeAnimation; break;
		case ITEM_PICKAXE: temp = &pickAnimation; break;
		default: temp = &fistAnimation; break;
		}
		if (weaponAnimation != temp) {
			weaponAnimation = temp;
			weaponSprite.play(*weaponAnimation);
			weaponSprite.stop();
			weaponCollisionSprite.setTexture(*weaponAnimation->getSpriteSheet());
		}
	}

	void attackBreakables(Breakable &breakable) {
		if (getDistanceBetweenPoints(breakable.getPosition(), getPosition()) < WINDOW_DIMENSIONS.y &&
			Collision::PixelPerfectTest(breakable.vis, weaponCollisionSprite, 254) && isAttacking) {
			int diffHp = (items.getItemSelected().type == breakable.breakableBy) ? items.getItemSelected().modifier : (!breakable.breakableExclusive) ? 1 : 0;
			breakable.changeHealth(-diffHp);
			if (items.getItemSelected().isTool) items.getItemSelected().durability--;
			isAttacking = false;
		}
		if (breakable.type == BREAKABLE_CRAFTING_STATION || breakable.type == BREAKABLE_CRAFTING_STUMP) {
			if (getDistanceBetweenPoints(breakable.getPosition(), getPosition()) < breakable.vis.getGlobalBounds().width * 1.25 && breakable.maxHealth > 0) craftHUD.open(CraftingHUD::HUDLevel((breakable.type == BREAKABLE_CRAFTING_STUMP) ? 0 : 1));
			else craftHUD.close();
		}
	}

	void interactWithMob(Mob &mob) {
		if (getDistanceBetweenPoints(getPosition(), mob.getPosition()) > GAME_CHUNK_SIZE*2) mob.isDeleted = true;
		checkCollision(mob);
		if (isAttacking && Collision::PixelPerfectTest(mob.vis, weaponCollisionSprite, 254)) {
			attackMob(mob, items.getItemSelected().damage);
			if (items.getItemSelected().isTool) items.getItemSelected().durability--;
			isAttacking = false;
		}
		if (mob.isHostile) mob.chaseMob(*this);
	}

	// reset the player
	void reset() {
		items.clear();
		isAttacking = false;
		isPlacing = false;
		attackReady = true;
		handsFlipped = false;
		rotation = 0;
		isDead = false;
		health = GAME_PLAYER_DEFAULT_HEALTH;
	}
	// return player view
	Camera &getView() { return view; }

	// the actual SPRITE to check for collision, animated sprite and sprite are different
	sf::Sprite weaponCollisionSprite;

	// if the player is attacking
	bool isAttacking = false;

	// the item bar
	ItemBar items = ItemBar(10);

private:
	bool isPlacing = false;
	// semi transparent sprite for placing stuff
	sf::Sprite placeSprite;

	// how much the acceleration changes every tick that the player is moving. Cannot be declared const
	float MOVE_AMOUNT = 0.25;

	// attack speed times per second. cannot be declared const.
	float attackSpeed = 2;

	// animation of fist
	Animation* weaponAnimation = &fistAnimation;

	// the animated sprite of the weapon held
	AnimatedSprite weaponSprite = AnimatedSprite(sf::seconds(0.025), true, false);

	// clock for handling weapon attacking
	sf::Clock attackClock, frameClock;

	// player view
	Camera view;

	// if hit is ready
	bool attackReady = true;

	// if hands model is flipped
	bool handsFlipped = false;

	// whether the player has collided with something last frame
	bool lastFrameCollision = false, lastFrameCollisionTriggered = false;

	// controls for movement, etc
	void controls() {
		if (GAME_PAUSED) return;
		changeAcceleration(sf::Vector2f(
		(keyboard.press(sf::Keyboard::A)) ? -MOVE_AMOUNT : (keyboard.press(sf::Keyboard::D)) ? MOVE_AMOUNT : 0,
		(keyboard.press(sf::Keyboard::W)) ? -MOVE_AMOUNT : (keyboard.press(sf::Keyboard::S)) ? MOVE_AMOUNT : 0
		));
		if (GAME_MAP_OPEN) return;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && attackReady && !MOUSE_IN_HUD) {
			weaponSprite.play(*weaponAnimation);
			float s = (items.getItemSelected().isTool) ? 1.f : (handsFlipped) ? 1.f : -1.f;
			weaponSprite.setScale(s, 1.f);
			handsFlipped = (items.getItemSelected().isTool) ? false : s < 0;
			attackReady = false;
			isAttacking = true;
		}
		if (keyboard.press(sf::Keyboard::B)) std::cout << "Player current square: " << getCurrSquare().x << ", " << getCurrSquare().y << std::endl;
	}
	
	// rotates player based on mouse position on screen
	void rotatePlayer() {
		if (GAME_MAP_OPEN) return;
		// set the view to be player view so that mouse position converts its window coordinates to world coordinates, letting the player turn
		WINDOW.setView(view);

		// get mouse position
		sf::Vector2f mousePos = WINDOW.mapPixelToCoords(sf::Mouse::getPosition(WINDOW));

		// get rotation
		rotation = atan2((getPosition().y - mousePos.y), (getPosition().x - mousePos.x)) * 180 / PI;

		// set rotation of sprite 
		vis.setRotation(rotation);

		// rotate fists
		weaponSprite.setRotation(rotation - 90);

		weaponCollisionSprite.setRotation(rotation - 90);

		// reset the window view
		WINDOW.setView(WINDOW.getDefaultView());
	}

	// update animation stuff
	void handleAnimations() {
		// create a time that gets the time since last restart
		sf::Time frameTime = frameClock.restart();

		// update sprite
		weaponSprite.update(frameTime);

		weaponSprite.setOrigin(98, 145);

		weaponSprite.setPosition(getPosition());

		weaponCollisionSprite.setTextureRect(weaponSprite.getAnimation()->getFrame(weaponSprite.getCurrentFrame()));

		weaponCollisionSprite.setOrigin(weaponSprite.getOrigin());

		weaponCollisionSprite.setPosition(getPosition());
		weaponCollisionSprite.setScale(weaponSprite.getScale());
	}
	
	// handle clocks
	void handleClocks() {
		if (!attackReady) {
			if (attackClock.getElapsedTime().asSeconds() > 0.3) {
				attackReady = true;
				isAttacking = false;
				attackClock.restart();
			}
		}
		else attackClock.restart();
	}

	void playerBoundaries() {
		if (getPosition().x < 0 || getPosition().x > GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE || getPosition().y < 0 || getPosition().y > GAME_CHUNKS_PER_WORLD_AMOUNT.y * GAME_CHUNK_SIZE) {
			setPosition(sf::Vector2f((getPosition().x < 0) ? 0 : (getPosition().x > GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE) ? GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE : getPosition().x, (getPosition().y < 0) ? 0 : (getPosition().y > GAME_CHUNKS_PER_WORLD_AMOUNT.y * GAME_CHUNK_SIZE) ? GAME_CHUNKS_PER_WORLD_AMOUNT.y * GAME_CHUNK_SIZE : getPosition().y));
		}
	}

	void dropItem() {
		if (items.itemToDrop.type != ITEM_NONE) {
			spawnItem(items.itemToDrop, getPosition(), sf::Vector2f(-getFacing().x * 5.5, -getFacing().y * 5.5));
			items.itemToDrop.type = ITEM_NONE;

		}
	}

	void useItem() {
		if (items.getItemSelected().isUsable) {
			if ((sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && GAME_CLICK_DELAY_PASSED)) {
				int healAmount = items.getItemSelected().healAmount;
				if (health < 10) {
					health = (health + healAmount > 10) ? 10 : health + healAmount;
					items.subtractItems(items.getItemLocation(items.getItemSelected()), 1);
				}
			}
		}
	}

	void placeBreakables() {
		if (items.getItemSelected().placeType != BREAKABLE_NONE) {
			isPlacing = true;

			sf::Vector2f pos = sf::Vector2f(
				int((getPosition().x + getFacing().x * 150) / GAME_SQUARE_SIZE)*GAME_SQUARE_SIZE + GAME_SQUARE_SIZE / 2,
				int((getPosition().y + getFacing().y * 150) / GAME_SQUARE_SIZE)*GAME_SQUARE_SIZE + GAME_SQUARE_SIZE / 2);
			sf::Vector2i squareIndx = sf::Vector2i(pos.x / GAME_SQUARE_SIZE, pos.y / GAME_SQUARE_SIZE);
			float rot = round(getRotation() / 22.5) * 22.5;

			placeSprite = sf::Sprite(breakableTextureMap[items.getItemSelected().placeType]);
			placeSprite.setOrigin(getMiddle(placeSprite));
			placeSprite.setRotation(rot);
			placeSprite.setPosition(pos);

			if (squareVector[squareIndx.y][squareIndx.x].obj.type == BREAKABLE_NONE &&
				squareVector[squareIndx.y][squareIndx.x].getType() != BIOME_WATER) {
				placeSprite.setColor(sf::Color(255, 255, 255, 64));
				if ((sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && GAME_CLICK_DELAY_PASSED)) {
					squareVector[squareIndx.y][squareIndx.x].obj = Breakable(items.getItemSelected().placeType);
					squareVector[squareIndx.y][squareIndx.x].obj.setPosition(pos);
					squareVector[squareIndx.y][squareIndx.x].obj.vis.setRotation(rot);
					items.subtractItems(items.getItemLocation(items.getItemSelected()), 1);
					GAME_CLICK_DELAY_PASSED = false;
				}
			}
			else placeSprite.setColor(sf::Color(255, 64, 64, 64));
		}
		else isPlacing = false;
	}
	
	
};
Player *globalPlayer; // a player variable to be accessed basically only by the lambda functions in GameSetup...
// MATTHEW 