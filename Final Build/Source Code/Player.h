/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Player.h 

This file is for declarations of the player and their functions
*/

#pragma once
#include "Controls.h"
#include "Camera.h"
#include "AnimationDeclaration.h"
#include "ItemBar.h"
#include "Mob.h"
#include "GUIScreen.h"

// class declaration
class Player : public Mob {
public:
	Player() {
		// set height
		height = 6;

		// set body texture
		vis.setTexture(playerTex);
		vis.setOrigin(vis.getGlobalBounds().width / 2, vis.getGlobalBounds().height / 2);

		// set position of player
		setPosition(sf::Vector2f(GAME_TOTAL_SIZE / 2, GAME_TOTAL_SIZE / 2));

		// reset the view
		resetView();

		// restarts the attack clock
		attackClock.restart();

		// play the animation
		weaponSprite.play(*weaponAnimation);
		weaponSprite.stop();

		// set collision sprite for fists
		weaponCollisionSprite.setTexture(anim_fistTex);

		// for collision purposes, so the player gets knocked back
		canMove = true;

		// set the health
		setHealth(GAME_PLAYER_DEFAULT_HEALTH);

		// player is not dead 
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

		// draw the player
		draw();

		// place breakable objects
		placeBreakables();

		// use an item
		useItem();

		// calculate whether you are in water
		isInWater();

		// release walking particles
		releaseParticles();

		// set view dimensions
		setViewDimensions();

		// whether the last frame it had collision
		lastFrameCollision = false;

		// if the player health is 0
		if (health <= 0) {

			// dead
			isDead = true;

			// set GUI screen to dead
			currentGUIScreen = &deadScreen;

			// game is now paused
			GAME_PAUSED = true;
		}
	}

	// function to reset the view
	void resetView() {

		// set up the camera
		view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
		view.setSize(sf::Vector2f(WINDOW_DIMENSIONS));

		// set camera zoom based on window dimensions
		view.cameraZoom(float(defaultWindowDimension.y) / WINDOW_DIMENSIONS.y);
	}

	// get a hypotenuse based on player rotation (1)
	sf::Vector2f getFacing() {

		// local x and y changes
		double newXChange, newYChange;

		// if the rotation is less than 90
		if (rotation < 90) {
			newXChange = -cos(rotation*PI / 180);
			newYChange = -sin(rotation*PI / 180);
		}

		// if the rotation is less than 180
		else if (rotation < 180) {
			newXChange = sin((rotation - 90)*PI / 180);
			newYChange = -cos((rotation - 90)*PI / 180);
		}

		// otherwise
		else {
			newXChange = cos(rotation*PI / 180);
			newYChange = -sin(rotation*PI / 180);
		}

		// return the new vecor
		return sf::Vector2f(newXChange, newYChange);
	}

	// draw the player 
	void draw() {

		// if the player is dead, return
		if (isDead) return;

		// draw the weapon (fists)
		drawableVector.emplace_back(ObjDrawable(weaponCollisionSprite, height - 0.01));

		// draw player
		Object::draw();

		// if the player is placing, draw the place sprite
		if (isPlacing) drawableVector.emplace_back(ObjDrawable(placeSprite, 1000));
	}

	// event update function
	void eventUpdate() {

		// temporary fist animation
		Animation *temp = &fistAnimation;

		// get the animation type based on the type
		switch (items.getItemSelected().type) {
		case ITEM_AXE: temp = &axeAnimation; break;
		case ITEM_PICKAXE: temp = &pickAnimation; break;
		default: temp = &fistAnimation; break;
		}

		// if the current weapon animation isnt the temp
		if (weaponAnimation != temp) {

			// set the animation type
			weaponAnimation = temp;

			// set it to the first frame
			weaponSprite.play(*weaponAnimation);
			weaponSprite.stop();

			// set texture of the weapon collision sprite
			weaponCollisionSprite.setTexture(*weaponAnimation->getSpriteSheet());
		}
	}

	// attack breakables
	void attackBreakables(Breakable &breakable) {

		// if they collide
		if (getDistanceBetweenPoints(breakable.getPosition(), getPosition()) < WINDOW_DIMENSIONS.y &&
			Collision::PixelPerfectTest(breakable.vis, weaponCollisionSprite, 254) && isAttacking) {

			// get the color and darken it
			sf::Color c(BreakableColor[breakable.type].r * 0.85, BreakableColor[breakable.type].g * 0.85, BreakableColor[breakable.type].b * 0.85);
			
			// spawn particles
			Particle::spawnParticlesRadial(c, 5, 0.3, breakable.getPosition(), 3);

			// get the difference in HP
			int diffHp = (items.getItemSelected().type == breakable.breakableBy) ? items.getItemSelected().modifier : (!breakable.breakableExclusive) ? 1 : 0;
			
			// subtract health
			breakable.changeHealth(-diffHp);

			// reduce the item durability if it is a tool
			if (items.getItemSelected().isTool) items.getItemSelected().durability--;

			// not attacking
			isAttacking = false;
		}

		// whether the breakable is a crafting table
		if (breakable.type == BREAKABLE_CRAFTING_STATION || breakable.type == BREAKABLE_CRAFTING_STUMP) {

			// open the HUD
			if (getDistanceBetweenPoints(breakable.getPosition(), getPosition()) < breakable.vis.getGlobalBounds().width * 1.5 && breakable.maxHealth > 0) craftHUD.open(CraftingHUD::HUDLevel((breakable.type == BREAKABLE_CRAFTING_STUMP) ? 0 : 1));
			
			// close the hud if far away
			else craftHUD.close();
		}
	}

	// interact with mob
	void interactWithMob(Mob &mob) {

		// if the mob is really far away, delete it
		if (getDistanceBetweenPoints(getPosition(), mob.getPosition()) > GAME_CHUNK_SIZE*2) mob.isDeleted = true;

		// check the collision between it and mobs
		checkCollision(mob);

		// if attacking and you are colliding 
		if (isAttacking && Collision::PixelPerfectTest(mob.vis, weaponCollisionSprite, 254)) {

			// spawn particles
			Particle::spawnParticlesRadial(sf::Color(255, 32, 32), 3, 0.2, mob.getPosition(), 5);

			// attack mob
			attackMob(mob, items.getItemSelected().damage);

			// reduce durability
			if (items.getItemSelected().isTool) items.getItemSelected().durability--;

			// attacking is false
			isAttacking = false;
		}

		// if hostile, have it chase you
		if (mob.isHostile) mob.chaseMob(*this);
	}

	// reset the player
	void reset() {

		// clear items
		items.clear();

		// attacking false
		isAttacking = false;

		// placing false
		isPlacing = false;

		// attack ready
		attackReady = true;

		// unflip hands
		handsFlipped = false;

		// reset rotation
		rotation = 0;

		// not dead
		isDead = false;

		// reset health
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

	// how much the acceleration changes every tick that the player is moving. Cannot be declared const otherwise it freaks out
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

		// controls dont work when paused
		if (GAME_PAUSED) return;

		// set acceleration
		changeAcceleration(sf::Vector2f(
		(keyboard.press(sf::Keyboard::A)) ? -MOVE_AMOUNT : (keyboard.press(sf::Keyboard::D)) ? MOVE_AMOUNT : 0,
		(keyboard.press(sf::Keyboard::W)) ? -MOVE_AMOUNT : (keyboard.press(sf::Keyboard::S)) ? MOVE_AMOUNT : 0
		));

		// if the map is open, not allow clicking
		if (GAME_MAP_OPEN) return;

		// if clicking left button
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && attackReady && !MOUSE_IN_HUD) {

			// play the sprite
			weaponSprite.play(*weaponAnimation);

			// flip the sprite
			float s = (items.getItemSelected().isTool) ? 1.f : (handsFlipped) ? 1.f : -1.f;
			weaponSprite.setScale(s, 1.f);
			handsFlipped = (items.getItemSelected().isTool) ? false : s < 0;

			// set attack variables
			attackReady = false;
			isAttacking = true;
		}
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

		// set weapon sprite origin
		weaponSprite.setOrigin(98, 145);

		// set origin
		weaponSprite.setPosition(getPosition());

		// set texture rectangle
		weaponCollisionSprite.setTextureRect(weaponSprite.getAnimation()->getFrame(weaponSprite.getCurrentFrame()));

		// setup collision sprite
		weaponCollisionSprite.setOrigin(weaponSprite.getOrigin());
		weaponCollisionSprite.setPosition(getPosition());
		weaponCollisionSprite.setScale(weaponSprite.getScale());
	}
	
	// handle clocks
	void handleClocks() {
		// if attack isn't ready
		if (!attackReady) {

			// if the attack clock has elapsed
			if (attackClock.getElapsedTime().asSeconds() > 0.3) {

				// set attack variables
				attackReady = true;
				isAttacking = false;

				// restart attack clock
				attackClock.restart();
			}
		}

		// restart attack clock if ready
		else attackClock.restart();
	}

	// update player boundaries (if the position is out of bounds)
	void playerBoundaries() {
		if (getPosition().x < 0 || getPosition().x > GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE || getPosition().y < 0 || getPosition().y > GAME_CHUNKS_PER_WORLD_AMOUNT.y * GAME_CHUNK_SIZE) {
			setPosition(sf::Vector2f((getPosition().x < 0) ? 0 : (getPosition().x > GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE) ? GAME_CHUNKS_PER_WORLD_AMOUNT.x * GAME_CHUNK_SIZE : getPosition().x, (getPosition().y < 0) ? 0 : (getPosition().y > GAME_CHUNKS_PER_WORLD_AMOUNT.y * GAME_CHUNK_SIZE) ? GAME_CHUNKS_PER_WORLD_AMOUNT.y * GAME_CHUNK_SIZE : getPosition().y));
		}
	}

	// drop item
	void dropItem() {

		// if the item type isn't none
		if (items.itemToDrop.type != ITEM_NONE) {

			// spawn a dropped item
			spawnItem(items.itemToDrop, getPosition(), sf::Vector2f(-getFacing().x * 5.5, -getFacing().y * 5.5));

			// reset item to drop
			items.itemToDrop.type = ITEM_NONE;
		}
	}

	// use an item
	void useItem() {

		// if the item is usable
		if (items.getItemSelected().isUsable) {

			// if they rightclick
			if ((sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && GAME_CLICK_DELAY_PASSED)) {

				// set heal amount
				int healAmount = items.getItemSelected().healAmount;

				// if the health is less than 0
				if (health < 10) {

					// update health
					health = (health + healAmount > 10) ? 10 : health + healAmount;

					// subtract item
					items.subtractItems(items.getItemLocation(items.getItemSelected()), 1);
				}
			}
		}
	}

	// place breakables
	void placeBreakables() {

		// if the item can be placed
		if (items.getItemSelected().placeType != BREAKABLE_NONE) {

			// item is placing
			isPlacing = true;

			float rot = round(getRotation() / 22.5) * 22.5;

			// get position
			sf::Vector2f pos;
			if (breakableMap[items.getItemSelected().placeType].placePrecise) {
				rot = round(getRotation() / 45) * 45;
				if ((int)rot % 10 != 0) pos = sf::Vector2f(
					int((getPosition().x + getFacing().x * 125) / GAME_SQUARE_SIZE)*GAME_SQUARE_SIZE + GAME_SQUARE_SIZE / 2,
					int((getPosition().y + getFacing().y * 125) / GAME_SQUARE_SIZE)*GAME_SQUARE_SIZE + GAME_SQUARE_SIZE / 2);
				else {
					pos = sf::Vector2f(
						int((getPosition().x + getFacing().x * 125) / (GAME_SQUARE_SIZE / 4))*(GAME_SQUARE_SIZE / 4) + ((rot == 0) ? GAME_SQUARE_SIZE / 8 : (rot == -180) ? -GAME_SQUARE_SIZE / 8 : 0),
						int((getPosition().y + getFacing().y * 125) / (GAME_SQUARE_SIZE / 4))*(GAME_SQUARE_SIZE / 4) + ((rot == 90) ? GAME_SQUARE_SIZE / 8 : (rot == -90) ? -GAME_SQUARE_SIZE / 8 : 0));
				}
			}
			else {
				pos = sf::Vector2f(
					int((getPosition().x + getFacing().x * 125) / GAME_SQUARE_SIZE)*GAME_SQUARE_SIZE + GAME_SQUARE_SIZE / 2,
					int((getPosition().y + getFacing().y * 125) / GAME_SQUARE_SIZE)*GAME_SQUARE_SIZE + GAME_SQUARE_SIZE / 2);
			}
			
			// get the square index
			sf::Vector2i squareIndx = sf::Vector2i(pos.x / GAME_SQUARE_SIZE, pos.y / GAME_SQUARE_SIZE);

			// rotation
			

			// set place sprite 
			placeSprite = sf::Sprite(breakableTextureMap[items.getItemSelected().placeType]);
			placeSprite.setOrigin(getMiddle(placeSprite));
			placeSprite.setRotation(rot);
			placeSprite.setPosition(pos);

			// set square vector
			if (squareVector[squareIndx.y][squareIndx.x].obj.type == BREAKABLE_NONE &&
				squareVector[squareIndx.y][squareIndx.x].getType() != BIOME_WATER) {

				// set place sprite color
				placeSprite.setColor(sf::Color(255, 255, 255, 64));

				// if rightclicked
				if ((sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && GAME_CLICK_DELAY_PASSED)) {

					// set that square object
					squareVector[squareIndx.y][squareIndx.x].obj = Breakable(items.getItemSelected().placeType);
					squareVector[squareIndx.y][squareIndx.x].obj.setPosition(pos);
					squareVector[squareIndx.y][squareIndx.x].obj.vis.setRotation(rot);

					// subtract items
					items.subtractItems(items.getItemLocation(items.getItemSelected()), 1);

					// reset click delay
					GAME_CLICK_DELAY_PASSED = false;
				}
			}

			// set color
			else placeSprite.setColor(sf::Color(255, 64, 64, 64));
		}

		// not placing
		else isPlacing = false;
	}
	
	// set the view dimensions
	void setViewDimensions() {
		GAME_VIEW_LEFTSQUARE = getCurrSquare().x - getView().getSize().x / 2 / GAME_SQUARE_SIZE - 1;
		GAME_VIEW_RIGHTSQUARE = getCurrSquare().x + getView().getSize().x / 2 / GAME_SQUARE_SIZE + 2;
		GAME_VIEW_TOPSQUARE = getCurrSquare().y - getView().getSize().y / 2 / GAME_SQUARE_SIZE - 1;
		GAME_VIEW_BOTSQUARE = getCurrSquare().y + getView().getSize().y / 2 / GAME_SQUARE_SIZE + 2;

		// make sure that the squares are not out of bounds
		if (GAME_VIEW_LEFTSQUARE < 0) GAME_VIEW_LEFTSQUARE = 0;
		if (GAME_VIEW_RIGHTSQUARE >= GAME_SQUARES_PER_WORLD_AMOUNT.x) GAME_VIEW_RIGHTSQUARE = GAME_SQUARES_PER_WORLD_AMOUNT.x - 1;
		if (GAME_VIEW_TOPSQUARE < 0) GAME_VIEW_TOPSQUARE = 0;
		if (GAME_VIEW_BOTSQUARE >= GAME_SQUARES_PER_WORLD_AMOUNT.y) GAME_VIEW_BOTSQUARE = GAME_SQUARES_PER_WORLD_AMOUNT.y - 1;
	}
};

// a player variable to be accessed basically only by the lambda functions in GameSetup...
Player *globalPlayer; 