/*
------------------------------------------------
Ogygius - Survival Game
Developed by: Anatoly Zavyalov, Jonathan Sumabat
------------------------------------------------

Mob.h 

This file is for the declarations of the mobs (animals)
*/


#pragma once
#include "Entity.h"
#include "DroppedItem.h" 
#include "Chunk.h"
#include "Collision.h"
#include "Particle.h"

// mob class
class Mob;

// map of mobs
std::map<int, Mob> mobMap;

// array of all mobs
std::vector<Mob> mobVector;

// actual declaration
class Mob : virtual public Entity, public Collidable {
public:
	// empty constructor that just defines height
	Mob() { height = 5; }

	// actual constructor
	Mob(MobType _type) {

		// set the mob to the one already defined in the mob map
		*this = mobMap[_type];

		// set type
		type = _type;

		// set height
		height = 5;

		// set head and body textures
		vis	.setTexture(mobTextureMap[type]);
		head.setTexture(mobTextureMap[type]);

		// set the sprite rects of the mobs for heads and bodies, different depending on the mobs
		switch (type) {
		case MOB_COW:
			vis.setTextureRect(sf::IntRect(0, 0, 96, 192));
			head.setTextureRect(sf::IntRect(0, 192, 96, 96));
			break;
		case MOB_MOUSE: case MOB_MOUSE_ANGRY:
			vis.setTextureRect(sf::IntRect(0, 0, 72, 192));
			head.setTextureRect(sf::IntRect(0, 192, 72, 96));
			break;
		}

		// set origin of the sprite
		vis.setOrigin(getMiddle(vis));

		// set origin of the head
		head.setOrigin(getMiddle(head));

		// collidable variable (so the mob gets pushed back)
		canMove = true;
	}
	
	// set health of mob
	void setHealth(int newHealth) {
		health = newHealth;
	}

	// subtract health from mob
	void subtractHealth(int _health) {
		health -= _health;
	}

	// return health of mob
	int getHealth() { return health; }

	// set movement speed of mob
	void setMoveSpeed(float newMoveSpeed) {
		MOVE_SPEED = newMoveSpeed;
	}

	// set type
	void setType(MobType t) {
		type = t;
	}

	// set damage
	void setDamage(int d) {
		damage = d;
	}

	// set spawnrate
	void setSpawnRate(int spawnRateDenomin) {
		spawnRateDenominator = spawnRateDenomin;
	}

	// set hostility
	void setHostility(bool h) {
		isHostile = h;
	}

	// set the biomes that the mob spawns in
	void setSpawnBiomes(BiomeType biomes[]) {
		for (int i = 0; i < sizeof(biomes) / sizeof(biomes[0]); i++) {
			spawnBiomes.push_back(biomes[i]);
		}
	}

	// returns whether the mob can spawn on the square
	bool isRightBiome(Square &s) {
		for (BiomeType &b : spawnBiomes) {
			if (s.getType() == b) return true;
		}
		return false;
	}

	// update the mob
	void update() {

		// update physics
		physics();

		// update whether it is in water
		isInWater();

		// draw
		draw();

		// set head position and rotation
		head.setPosition(sf::Vector2f(getPosition().x + _head_from_middle_amount * (sin(rotation * PI / 180)), getPosition().y - _head_from_middle_amount * cos(rotation * PI / 180)));
	
		head.setRotation(headRotation);
		
		// set sprite rotation
		vis.setRotation(rotation);

		// mob ai
		aiStuff();

		// release particles when walking
		releaseParticles();

		// push back the head
		drawableVector.push_back(ObjDrawable(head, height + 0.1));

		// if its a mouse and its night, change the mob to a hostile mouse
		if (type == MOB_MOUSE && !isDay) {
			sf::Vector2f pos = getPosition();
			float rot = getRotation();
			float headRot = headRotation;
			*this = Mob(MOB_MOUSE_ANGRY);
			setPosition(pos);
			rotation = rot;
			headRotation = headRot;
			Particle::spawnParticlesRadial(sf::Color(255, 100, 100), 30, 0.75, getPosition(), 3);
		}

		// vice versa
		else if (type == MOB_MOUSE_ANGRY && isDay) {
			sf::Vector2f pos = getPosition();
			float rot = getRotation();
			float headRot = headRotation;
			*this = Mob(MOB_MOUSE);
			setPosition(pos);
			rotation = rot;
			headRotation = headRot;
			Particle::spawnParticlesRadial(sf::Color(200, 200, 200), 30, 0.75, getPosition(), 3);
		}

		// TODO: put into its own function
		// if the mob dies
		if (health < 1 && !isDeleted) {
			// set deleted to true
			isDeleted = true;

			// drop something depending on cow
			switch (type) {
			case MOB_COW:
				spawnItem(Item(itemMap[ITEM_MEAT], getRandomInt(1, 4)), getPosition(), sf::Vector2f(0, 0));
				break;
			case MOB_MOUSE:
				spawnItem(Item(itemMap[ITEM_MEAT], getRandomInt(0, 2)), getPosition(), sf::Vector2f(0, 0));
				break;
			}

			// spawn particles
			Particle::spawnParticlesRadial(sf::Color(128, 128, 128), 15, 0.75, getPosition(), 3);
		}
	}

	// return the current square
	sf::Vector2i getCurrSquare() { 

		// find the current square
		sf::Vector2i currSquare(getPosition().x / GAME_SQUARE_SIZE,
			getPosition().y / GAME_SQUARE_SIZE);

		// just in case, make sure that the current square isn't out of bounds
		if (currSquare.x < 0) currSquare.x = 0; 
		if (currSquare.x >= GAME_SQUARES_PER_WORLD_AMOUNT.x) currSquare.x = GAME_SQUARES_PER_WORLD_AMOUNT.x - 1;
		if (currSquare.y < 0) currSquare.y = 0; 
		if (currSquare.y >= GAME_SQUARES_PER_WORLD_AMOUNT.y) currSquare.y = GAME_SQUARES_PER_WORLD_AMOUNT.y - 1;

		// return the square
		return currSquare;
	}

	// returns rotation (in degrees)
	double getRotation() {
		return rotation;
	}

	// check collision
	bool checkCollision(Collidable &collidable) {

		// if the other collidable can be collided with and they are colliding
		if (collidable.isCollidable && Collision::PixelPerfectTest(vis, collidable.vis, 254)) {

			// calculate the knockback amount
			float kbAmount = (canMove && collidable.canMove) ? GAME_ENTITY_BOUNCE_AMOUNT / 2 : GAME_ENTITY_BOUNCE_AMOUNT;

			// get angle between collidable and the mob
			float angle = atan2(getPosition().y - collidable.getPosition().y, getPosition().x - collidable.getPosition().x) * 180 / PI;

			// declare new acceleration variables
			float newAccelX, newAccelY;

			// if they are at the same position
			if (getPosition().x == collidable.getPosition().x && getPosition().y == collidable.getPosition().y) {
				newAccelX = kbAmount * cos(getRandomInt(0, 360)*PI / 180);
				newAccelY = kbAmount * sin(getRandomInt(0, 360)*PI / 180);
			}
			else {
				// if the angle is <90
				if (angle < 90) {
					newAccelX = kbAmount * cos(angle*PI / 180);
					newAccelY = kbAmount * sin(angle*PI / 180);
				}
				// if angle is between 90 and 180
				else if (angle < 180) {
					newAccelX = -kbAmount * sin((angle - 90)*PI / 180);
					newAccelY = kbAmount * cos((angle - 90)*PI / 180);
				}
				// otherwise
				else {
					newAccelX = -kbAmount * cos(angle*PI / 180);
					newAccelY = kbAmount * sin(angle*PI / 180);
				}
			}
			// if the mob can move, push it back
			if (canMove) setAcceleration(sf::Vector2f(newAccelX, newAccelY));

			// if the other collidable can move, push that back too
			if (collidable.canMove) collidable.setAcceleration(sf::Vector2f(-newAccelX, -newAccelY));

			// return true
			return true;
		}

		// return false
		return false;
	}

	// attack another mob
	void attackMob(Mob &mob, int damage) {

		// subtract health
		mob.subtractHealth(damage);

		// calculate angle
		float angle = getAngleBetweenPoints(getPosition(), mob.getPosition());

		// new acceleration, knockback amounts
		float newAccelX, newAccelY, kbAmount = GAME_ENTITY_BOUNCE_AMOUNT * damage;

		// if angle is less than 90
		if (angle < 90) {
			newAccelX = kbAmount * cos(angle*PI / 180);
			newAccelY = kbAmount * sin(angle*PI / 180);
		}
		// if angle is between 90 and 180
		else if (angle < 180) {
			newAccelX = -kbAmount * sin((angle - 90)*PI / 180);
			newAccelY = kbAmount * cos((angle - 90)*PI / 180);
		}
		// otherwise
		else {
			newAccelX = -kbAmount * cos(angle*PI / 180);
			newAccelY = kbAmount * sin(angle*PI / 180);
		}

		// set the acceleration of the other mob
		mob.setAcceleration(sf::Vector2f(-newAccelX, -newAccelY));

		// set that mobs run pointer to this
		mob.mobToRun = this;

		// make the mob run away from that position
		mob.runFromPosition(getPosition());
	}

	// chase mob
	void chaseMob(Mob &mob) {

		// if the distance between the two mobs is less than 500 pixels
		if (getDistanceBetweenPoints(getPosition(), mob.getPosition()) <= 500) {

			// set the mob run pointer to that mob
			mobToRun = &mob;

			// make it run toward 
			runningTo = true;

			// run to that position
			runToPosition(mob.getPosition());

			// if they collide
			if (checkCollision(mob)) {

				// if it hasn't attacked yet (so it doesn't attack multiple times during one collision
				if (!hasAttacked) {

					// attack that mob
					attackMob(mob, damage);

					// set attacked bool to true
					hasAttacked = true;
				}
			}
			// if they don't collide
			else {

				// set it to false
				hasAttacked = false;
			}
		}

		// if stopped chasing
		else if (mobToRun != NULL) {

			// reset mob to run
			mobToRun = NULL;

			// reset AI state
			aiState = AIState::STATE_IDLE;
			aiStateLength = aiStateLengthMin;
		}
	}

	// spawn mob (static function)
	static void spawnMob(Square &square) {

		// for every mob
		for (int i = 0; i < mobMap.size(); i++) {

			// checks if there is a chance and there is space in the mob array
			if (getChance(mobMap[i].spawnRateDenominator) && mobVector.size() < GAME_MOB_MAX_COUNT) {

				// local done boolean
				bool done = false;

				// for every biome that the mob cna spawn in
				for (BiomeType &biome : mobMap[i].spawnBiomes) {
					if (done) break;

					// check if that squares biome is one of these biomes
					if (square.getType() == biome) {

						// add the mob
						mobVector.push_back(Mob(mobMap[i].type));
						mobVector.back().setPosition(square.ground.getPosition());
						done = true;
					}
				}
			}
		}
	}

	// initial mob spawn
	static void spawnMob(int denominator, sf::IntRect chunks) {

		// gets the square values of those chunks
		int squareLeft = chunks.left * GAME_SQUARE_PER_CHUNK_AMOUNT.x, squareRight = squareLeft + chunks.width * GAME_SQUARE_PER_CHUNK_AMOUNT.x,
			squareTop = chunks.top * GAME_SQUARE_PER_CHUNK_AMOUNT.y, squareBot = squareTop + chunks.height * GAME_SQUARE_PER_CHUNK_AMOUNT.y;
		
		// random amount of mobs that it spawns, from 5 to 20
		int amount = getRandomInt(5, 21);

		// for every amount
		for (int i = 0; i < amount; i++) {
			
			// local right biome variables and mob type
			bool rightBiome = false; int mobType = -1;

			// local position variable
			sf::Vector2i pos;
			
			// tries 10 times to find a square with the right biome, after that it just quits :P
			for (int i = 0; !rightBiome && i < 10; i++) {

				// set the position
				pos = sf::Vector2i(getRandomInt(squareLeft, squareRight), getRandomInt(squareTop, squareBot));

				// for every mob
				for (int a = 0; a < mobMap.size(); a++) {

					// if that's the right biome
					if (mobMap[a].isRightBiome(squareVector[pos.y][pos.x])) {

						// sets mob type and breaks out
						mobType = a;
						rightBiome = true;
						break;
					}
				}
			}

			// if it hasn't found a mob, keep looking
			if (mobType < 0) continue;

			// get the square pointer
			Square *square = &squareVector[pos.y][pos.x];

			// new mob
			mobVector.push_back(Mob(MobType(mobType)));

			// set the position of the new mob
			mobVector.back().setPosition(square->ground.getPosition());
		}
	}

	// whether the mob is hostile or not
	bool isHostile = false;

	// is the mob dead
	bool isDead = false;
private:

	// type of mob
	MobType type;

	// head sprite
	sf::Sprite head;

	// how far the head is away from the middle of the body
	float _head_from_middle_amount = 64;

	// head rotation
	float headRotation = 180 + rotation;

	// where the mob will rotate to
	float toRotation;

	// how long each ai state will last
	float aiStateLengthMin = 2, aiStateLengthMax = 5;

	// clocks
	sf::Clock aiClock, particleClock;

	// gets a random number between the two
	float aiStateLength = getRandomFloat(aiStateLengthMin, aiStateLengthMax);

	// a pointer of the mob to run away from
	Mob *mobToRun;

	// if its running to a mob or away from it
	bool runningTo = false;

	// the different types of ai states
	enum AIState {
		STATE_IDLE, STATE_WANDER, STATE_RUN,

		STATETYPE_SIZE
	};

	// ai state variable, set to idle
	AIState aiState = AIState(0);

	// main AI function
	void aiStuff() {
		// checks for whether the mob has left map boundaries
		if (getPosition().x - vis.getGlobalBounds().width / 2 < 0 ||
			getPosition().x + vis.getGlobalBounds().width / 2 >= GAME_TOTAL_SIZE ||
			getPosition().y - vis.getGlobalBounds().height / 2 < 0 ||
			getPosition().y + vis.getGlobalBounds().height / 2 >= GAME_TOTAL_SIZE) {
			float newX = getPosition().x, newY = getPosition().y;
			if (getPosition().x - vis.getGlobalBounds().width / 2 < 0) newX = vis.getGlobalBounds().width / 2;
			else if (getPosition().x + vis.getGlobalBounds().width / 2 >= GAME_TOTAL_SIZE) newX = GAME_TOTAL_SIZE - vis.getGlobalBounds().width / 2;
			if (getPosition().y - vis.getGlobalBounds().height / 2 < 0) newY = vis.getGlobalBounds().height / 2;
			else if (getPosition().y + vis.getGlobalBounds().height / 2 >= GAME_TOTAL_SIZE) newY = GAME_TOTAL_SIZE - vis.getGlobalBounds().height / 2;
			setPosition(sf::Vector2f(newX, newY));
		}

		// if the ai clock has elapsed, get a new AI state and length
		if (aiClock.getElapsedTime().asSeconds() >= aiStateLength) {
			aiClock.restart();

			// get new length
			aiStateLength = getRandomFloat(aiStateLengthMin, aiStateLengthMax);

			// get new state
			aiState = AIState(getRandomInt(0, STATETYPE_SIZE - 1));

			// get rotation
			rotation = toRotation;
		}

		// if hasn't elapsed
		else {
			// if wandering or running, make it move
			switch (aiState) {
			case STATE_WANDER:	move(); break;
			case STATE_RUN:		move(MOVE_SPEED * 3); break;
			}

			// if its not running
			if (aiState != STATE_RUN) {
				
				wasRunning = false;
				if (isnan(rotation)) rotation = 0;
				if (isnan(headRotation)) headRotation = 180;
				if (isnan(toRotation)) toRotation = 0;

				if (rotation < 0 || rotation > 360) rotation = 0;
				if (headRotation < 0 || headRotation > 360) headRotation = 0;
				if (toRotation < 0 || toRotation > 360) toRotation = 0;


				// if the clock is 1 second away from elapsing, rotate the body
				if (aiClock.getElapsedTime().asSeconds() >= aiStateLength - 1) {
					rotation = (int(toRotation) != int(rotation)) ? (toRotation - rotation)*(aiClock.getElapsedTime().asSeconds() - (aiStateLength - 1)) + rotation : toRotation;
				}

				// if its 1.5 seconds away from elapsing, rotate the head
				else if (aiClock.getElapsedTime().asSeconds() >= aiStateLength - 1.5) {

					// get head rotation compared to normal rotation
					float r = headRotation - 180;

					// rotate head
					headRotation = (int(toRotation) != int(r)) ? ((toRotation - r) * 2)*(aiClock.getElapsedTime().asSeconds() - (aiStateLength - 1.5)) + headRotation : toRotation + 180;
				}

				// otehrwise, just calculate a random angle that it will rotate to
				else toRotation = getRandomFloat(rotation - 85, rotation + 85);
			}
			else {
				float angle = getAngleBetweenPoints(getPosition(), mobToRun->getPosition());
				/*
				//  gets the angle between itself and the other mob
				
				if (!wasRunning && (((runningTo && int(angle - 90) != int(rotation)) || (!runningTo && int(angle + 90) != int(rotation))))) {
					if (runningTo) rotation = (int(angle - 90) != int(rotation)) ? (int(angle - 90) > int(rotation)) ? rotation + (rotation - angle + 90)*0.05 : rotation - (rotation - angle + 90)*0.05 : angle - 90;
					else rotation = (int(angle + 90) != int(rotation)) ? (int(angle + 90) > int(rotation)) ? rotation + (rotation - angle - 90)*0.05 : rotation - (rotation - angle - 90)*0.05 : angle + 90;
				}
				else {
					wasRunning = true;
					
				}*/
				// if running to the mob
				if (runningTo) {
					rotation = angle - 90;
					headRotation = angle + 90;
				}

				// if running away from the mob
				else {
					rotation = angle + 90;
					headRotation = angle - 90;
				}

				// set torotation to rotation
				toRotation = rotation;
			}
		}
	}

	// move at normal speed
	void move() {
		move(MOVE_SPEED);
	}

	// move at custom speed
	void move(double speed) {
		setAcceleration(sf::Vector2f(speed*sin(rotation*PI / 180), -speed * cos(rotation*PI / 180)));
	}

	// run from a position
	void runFromPosition(sf::Vector2f position) {

		// calculate the angle between the 2 points
		float angle = getAngleBetweenPoints(getPosition(), position);

		// make mob run
		aiState = STATE_RUN;

		// set rotation
		rotation = angle + 90;
		headRotation = angle - 90;

		// restart ai clock
		aiClock.restart();

		// set ai state length to the max
		aiStateLength = aiStateLengthMax;

		// not running to
		runningTo = false;
	}
	
	// run to a position
	void runToPosition(sf::Vector2f position) {

		// calculate angle
		float angle = getAngleBetweenPoints(getPosition(), position);

		// maje mob run
		aiState = STATE_RUN;

		// set rotation
		rotation = angle - 90;
		headRotation = angle + 90;

		// restart ai clock
		aiClock.restart();

		// set ai state length to max
		aiStateLength = aiStateLengthMax;

		// run to
		runningTo = true;
	}
protected:

	// set whether the current square its at is water
	void isInWater() {
		if (squareVector[getCurrSquare().y][getCurrSquare().x].getType() == BIOME_WATER) inWater = true;
		else inWater = false;
	}

	// release particles
	void releaseParticles() {

		// when walking
		if (particleClock.getElapsedTime().asSeconds() > 0.25 && (int(getVelocity().x) != 0 || int(getVelocity().y) != 0)) {

			// get current biome color
			sf::Color currBiomeColor = squareVector[getCurrSquare().y][getCurrSquare().x].ground.getFillColor();

			// darken it by 15%
			currBiomeColor = sf::Color(currBiomeColor.r * 0.85, currBiomeColor.g * 0.85, currBiomeColor.b * 0.85);

			// calculate number of particles to release
			int particlesCount = std::max(fabs(getVelocity().x), fabs(getVelocity().y));

			// if in water
			if (inWater) {

				// spawn particles radial
				Particle::spawnParticlesRadial(currBiomeColor, particlesCount, 0.85, getPosition(), 1.5);

				// if the last frame wasn't in water
				if (!wasInWater) {

					// make a splash
					sf::Color c(BiomeColor[BIOME_WATER].r*1.3, BiomeColor[BIOME_WATER].g*1.3, 255);
					Particle::spawnParticlesRadial(c, 30, 0.4, getPosition(), particlesCount);
				}

				// this frame was in water
				wasInWater = true;
			}
			else {
				// spawn radial particles
				Particle::spawnParticlesRadial(currBiomeColor, particlesCount, 0.85, getPosition(), 0.3);

				// not in water
				wasInWater = false;
			}
			// restart particle clock
			particleClock.restart();
		}
	}

	// health
	int health;

	// damage that the mob does
	int damage;

	// movement speed
	float MOVE_SPEED;

	// denominator spawn chance
	int spawnRateDenominator;

	// the different biomes that it can spawn in
	std::vector<BiomeType> spawnBiomes;

	// whether the mob has attacked
	bool hasAttacked = false;

	// whether last frame was in water
	bool wasInWater = false;

	// whether it was already running
	bool wasRunning = false;

	// rotation of mob
	float rotation = 0;
};

// declare all mobs
void declareMobs() {

	// insert all mobs into array
	for (int i = 0; i < MOBTYPE_COUNT; i++) {
		mobMap.insert(std::pair<int, Mob>(i, Mob(MobType(i))));
		mobMap[i].setType(MobType(i));
	}

	// COW
	BiomeType t[2] = { BIOME_GRASSLANDS, BIOME_FOREST };
	mobMap[MOB_COW].setHealth(10);
	mobMap[MOB_COW].setMoveSpeed(0.3);
	mobMap[MOB_COW].setSpawnRate(3000);
	mobMap[MOB_COW].setSpawnBiomes(t);

	// MOUSE
	BiomeType t2[4] = { BIOME_GRASSLANDS, BIOME_FOREST, BIOME_SAVANNAH, BIOME_DESERT };
	mobMap[MOB_MOUSE].setHealth(10);
	mobMap[MOB_MOUSE].setMoveSpeed(0.33);
	mobMap[MOB_MOUSE].setSpawnRate(2000);
	mobMap[MOB_MOUSE].setSpawnBiomes(t2);
	
	// ANGRY MOUSE
	mobMap[MOB_MOUSE_ANGRY].setHealth(10);
	mobMap[MOB_MOUSE_ANGRY].setMoveSpeed(0.375);
	mobMap[MOB_MOUSE_ANGRY].setHostility(true);
	mobMap[MOB_MOUSE_ANGRY].setDamage(1);
}