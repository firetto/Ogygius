#pragma once
#include "Entity.h"
#include "DroppedItem.h" 
#include "Chunk.h"
#include "Collision.h"
#include "Particle.h"

class Mob;

std::map<int, Mob> mobMap;
std::vector<Mob> mobVector;
class Mob : virtual public Entity, public Collidable {
public:
	Mob() { height = 5; }
	Mob(MobType _type) {
		*this = mobMap[_type];
		type = _type;
		height = 5;
		vis.setTexture(mobTextureMap[type]);
		head.setTexture(mobTextureMap[type]);
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

		vis.setOrigin(getMiddle(vis));

		head.setOrigin(getMiddle(head));
		canMove = true;
	}
	void setHealth(int newHealth) {
		health = newHealth;
	}
	void subtractHealth(int _health) {
		health -= _health;
	}
	int getHealth() { return health; }
	void setMoveSpeed(float newMoveSpeed) {
		MOVE_SPEED = newMoveSpeed;
	}
	void setType(MobType t) {
		type = t;
	}
	void setDamage(int d) {
		damage = d;
	}
	void setSpawnRate(int spawnRateDenomin) {
		spawnRateDenominator = spawnRateDenomin;
	}
	void setHostility(bool h) {
		isHostile = h;
	}
	void setSpawnBiomes(BiomeType biomes[]) {
		for (int i = 0; i < sizeof(biomes) / sizeof(biomes[0]); i++) {
			spawnBiomes.push_back(biomes[i]);
		}
	}
	bool isRightBiome(Square &s) {
		for (BiomeType &b : spawnBiomes) {
			if (s.getType() == b) return true;
		}
		return false;
	}
	void update() {
		physics();
		isInWater();
		draw();
		head.setPosition(sf::Vector2f(getPosition().x + _head_from_middle_amount * (sin(rotation * PI / 180)), getPosition().y - _head_from_middle_amount * cos(rotation * PI / 180)));
		head.setRotation(headRotation);
		vis.setRotation(rotation);
		aiStuff();
		releaseParticles();

		drawableVector.push_back(ObjDrawable(head, height + 0.1));

		if (type == MOB_MOUSE && !isDay) {
			sf::Vector2f pos = getPosition();
			float rot = getRotation();
			float headRot = headRotation;
			*this = Mob(MOB_MOUSE_ANGRY);
			setPosition(pos);
			rotation = rot;
			headRotation = headRot;
		}
		else if (type == MOB_MOUSE_ANGRY && isDay) {
			sf::Vector2f pos = getPosition();
			float rot = getRotation();
			float headRot = headRotation;
			*this = Mob(MOB_MOUSE);
			setPosition(pos);
			rotation = rot;
			headRotation = headRot;
		}
		if (health < 1 && !isDeleted) {
			isDeleted = true;
			if (type == MOB_COW) spawnItem(Item(itemMap[ITEM_MEAT], 1), getPosition(), sf::Vector2f(0, 0));
			Particle::spawnParticles(sf::Color(128, 128, 128), 10, getPosition());
		}
	}
	sf::Vector2i getCurrSquare() { 
		sf::Vector2i currSquare(getPosition().x / GAME_SQUARE_SIZE,
			getPosition().y / GAME_SQUARE_SIZE);
		if (currSquare.x < 0) currSquare.x = 0; 
		if (currSquare.x >= GAME_SQUARES_PER_WORLD_AMOUNT.x) currSquare.x = GAME_SQUARES_PER_WORLD_AMOUNT.x - 1;
		if (currSquare.y < 0) currSquare.y = 0; 
		if (currSquare.y >= GAME_SQUARES_PER_WORLD_AMOUNT.y) currSquare.y = GAME_SQUARES_PER_WORLD_AMOUNT.y - 1;
		return currSquare;
	}
	// returns rotation (in degrees)
	double getRotation() {
		return rotation;
	}

	bool checkCollision(Collidable &collidable) {
		if (collidable.isCollidable && Collision::PixelPerfectTest(vis, collidable.vis, 254)) {
			float kbAmount = (canMove && collidable.canMove) ? GAME_ENTITY_BOUNCE_AMOUNT / 2 : GAME_ENTITY_BOUNCE_AMOUNT;
			// get angle between collidable and the player
			float angle = atan2(getPosition().y - collidable.getPosition().y, getPosition().x - collidable.getPosition().x) * 180 / PI;

			// declare new acceleration variables
			float newAccelX, newAccelY;

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
			//sf::Vector2f n((getVelocity().x > 0) ? 1 : -1, (getVelocity().y > 0) ? 1 : -1);
			/*setPosition(sf::Vector2f(getPosition().x - getVelocity().x// - getAccel().x
			, getPosition().y - getVelocity().y// - getAccel().y
			));*/
			//setVelocity(sf::Vector2f(0, 0));
			if (canMove) setAcceleration(sf::Vector2f(newAccelX, newAccelY));
			if (collidable.canMove) collidable.setAcceleration(sf::Vector2f(-newAccelX, -newAccelY));
			return true;
		}
		return false;
	}

	void attackMob(Mob &mob, int damage) {
		mob.subtractHealth(damage);
		float angle = getAngleBetweenPoints(getPosition(), mob.getPosition());
		float newAccelX, newAccelY, kbAmount = GAME_ENTITY_BOUNCE_AMOUNT * damage;
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
		mob.setAcceleration(sf::Vector2f(-newAccelX, -newAccelY));
		mob.mobToRun = this;
		mob.runFromPosition(getPosition());
	}

	void chaseMob(Mob &mob) {
		if (getDistanceBetweenPoints(getPosition(), mob.getPosition()) <= 500) {
			mobToRun = &mob;
			runningTo = true;
			runToPosition(mob.getPosition());
			if (checkCollision(mob)) {
				if (!hasAttacked) {
					attackMob(mob, damage);
					hasAttacked = true;
				}
			}
			else {
				hasAttacked = false;
			}
		}
	}

	static void spawnMob(Square &square) {
		for (int i = 0; i < mobMap.size(); i++) {
			if (getChance(mobMap[i].spawnRateDenominator) && mobVector.size() < GAME_MOB_MAX_COUNT) {
				bool done = false;
				for (BiomeType &biome : mobMap[i].spawnBiomes) {
					if (done) break;
					if (square.getType() == biome) {
						mobVector.push_back(Mob(mobMap[i].type));
						mobVector.back().setPosition(square.ground.getPosition());
						std::cout << "Spawned mob " << mobMap[i].type << " at " << square.ground.getPosition().x << ", " << square.ground.getPosition().y << std::endl;
						done = true;
					}
				}
			}
		}
	}

	// initial mob spawn
	static void spawnMob(int denominator, sf::IntRect chunks) {
		int squareLeft = chunks.left * GAME_SQUARE_PER_CHUNK_AMOUNT.x, squareRight = squareLeft + chunks.width * GAME_SQUARE_PER_CHUNK_AMOUNT.x,
			squareTop = chunks.top * GAME_SQUARE_PER_CHUNK_AMOUNT.y, squareBot = squareTop + chunks.height * GAME_SQUARE_PER_CHUNK_AMOUNT.y;
		
		int amount = getRandomInt(5, 20);
		std::cout << squareLeft << ", " << squareRight << ", " << squareTop << ", " << squareBot << " | " << amount << std::endl;
		for (int i = 0; i < amount; i++) {
			bool rightBiome = false; int mobType = -1;
			sf::Vector2i pos;
			
			for (int i = 0; !rightBiome && i < 10; i++) {
				pos = sf::Vector2i(getRandomInt(squareLeft, squareRight), getRandomInt(squareTop, squareBot));
				for (int a = 0; a < mobMap.size(); a++) {
					if (mobMap[a].isRightBiome(squareVector[pos.y][pos.x])) {
						mobType = a;
						rightBiome = true;
						break;
					}
				}
			}
			if (mobType < 0) return;
			Square *square = &squareVector[pos.y][pos.x];
			mobVector.push_back(Mob(MobType(mobType)));
			std::cout << "Spawned mob at " << pos.x << ", " << pos.y << std::endl;
			mobVector.back().setPosition(square->ground.getPosition());
		}
		/*
		for (int i = squareLeft - 1; i < squareRight; i++) {
			for (int a = squareTop - 1; a < squareBot; a++) {
				if (placed) break;
				Square *square = &squareVector[a][i];
				switch (square->getType()) {
				case BIOME_GRASSLANDS: case BIOME_FOREST:
					for (int b = 0; b < amount; b++) {
						mobVector.push_back(Mob(MOB_COW));
						std::cout << "Spawned cow at " << a << ", " << i << std::endl;
						mobVector.back().setPosition(square->ground.getPosition()); break;
					}
				}
				placed = true;
				if (getChance(denominator)) {
					
				}
			}
		}*/
	}

	bool isHostile = false;
	bool isDead = false;
private:
	MobType type;
	sf::Sprite head;
	float _head_from_middle_amount = 64;
	float headRotation = 180 + rotation;
	float toRotation;
	float aiStateLengthMin = 2, aiStateLengthMax = 5;
	sf::Clock aiClock, particleClock;
	float aiStateLength = getRandomFloat(aiStateLengthMin, aiStateLengthMax);
	Mob *mobToRun;
	bool runningTo = false;

	enum AIState {
		STATE_IDLE, STATE_WANDER, STATE_RUN,

		STATETYPE_SIZE
	};
	AIState aiState = AIState(0);

	void aiStuff() {
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
		if (aiClock.getElapsedTime().asSeconds() >= aiStateLength) {
			aiClock.restart();
			aiStateLength = getRandomFloat(aiStateLengthMin, aiStateLengthMax);
			aiState = AIState(getRandomInt(0, STATETYPE_SIZE - 1));
			rotation = toRotation;
		}

		else {
			switch (aiState) {
			case STATE_WANDER:	move(); break;
			case STATE_RUN:		move(MOVE_SPEED * 3); break;
			}
			if (aiState != STATE_RUN) {
				if (aiClock.getElapsedTime().asSeconds() >= aiStateLength - 1) {
					//headRotation = 
					rotation = (int(toRotation) != int(rotation)) ? (toRotation - rotation)*(aiClock.getElapsedTime().asSeconds() - (aiStateLength - 1)) + rotation : toRotation;
					//headRotation = rotation+180;
				}
				else if (aiClock.getElapsedTime().asSeconds() >= aiStateLength - 1.5) {
					int r = int(headRotation) - 180;
					headRotation = (int(toRotation) != r) ? ((toRotation - r) * 2)*(aiClock.getElapsedTime().asSeconds() - (aiStateLength - 1.5)) + headRotation : toRotation + 180;
				}

				else toRotation = getRandomFloat(rotation - 85, rotation + 85);
			}
			else {
				float angle = getAngleBetweenPoints(getPosition(), mobToRun->getPosition());
				if (runningTo) {
					rotation = angle - 90;
					headRotation = angle + 90;
				}
				else {
					rotation = angle + 90;
					headRotation = angle - 90;
				}
				toRotation = rotation;
			}
		}
	}
	void move() {
		move(MOVE_SPEED);
	}
	void move(double speed) {
		setAcceleration(sf::Vector2f(speed*sin(rotation*PI / 180), -speed * cos(rotation*PI / 180)));
	}
	void runFromPosition(sf::Vector2f position) {
		float angle = getAngleBetweenPoints(getPosition(), position);
		aiState = STATE_RUN;
		rotation = angle + 90;
		headRotation = angle - 90;
		aiClock.restart();
		aiStateLength = aiStateLengthMax;
		runningTo = false;
	}
	void runToPosition(sf::Vector2f position) {
		float angle = getAngleBetweenPoints(getPosition(), position);
		aiState = STATE_RUN;
		rotation = angle - 90;
		headRotation = angle + 90;
		aiClock.restart();
		aiStateLength = aiStateLengthMax;
		runningTo = true;
	}
protected:
	void isInWater() {
		if (getPosition().x <= GAME_TOTAL_SIZE - 10 && Entity::getPosition().y <= GAME_TOTAL_SIZE - 10 &&
			squareVector[getCurrSquare().y][getCurrSquare().x].getType() == BIOME_WATER) {
			inWater = true;
		}
		else inWater = false;
	};
	void releaseParticles() {
		if (particleClock.getElapsedTime().asSeconds() > 0.25 && (int(getVelocity().x) != 0 || int(getVelocity().y) != 0)) {
			sf::Color currBiomeColor = squareVector[getCurrSquare().y][getCurrSquare().x].ground.getFillColor();
			currBiomeColor = sf::Color(currBiomeColor.r * 0.85, currBiomeColor.g * 0.85, currBiomeColor.b * 0.85);
			int particlesCount = std::max(fabs(getVelocity().x), fabs(getVelocity().y));
			if (inWater) {
				Particle::spawnParticlesRadial(currBiomeColor, particlesCount, 0.85, getPosition(), 1.5);
				if (!wasInWater) {
					sf::Color c(BiomeColor[BIOME_WATER].r*1.3, BiomeColor[BIOME_WATER].g*1.3, 255);
					Particle::spawnParticlesRadial(c, 30, 0.4, getPosition(), particlesCount);
				}
				wasInWater = true;
			}
			else {
				Particle::spawnParticlesRadial(currBiomeColor, particlesCount, 0.85, getPosition(), 0.3);
				wasInWater = false;
			}
			particleClock.restart();
		}
	}
	int health;

	int damage;

	float MOVE_SPEED;

	int spawnRateDenominator;

	std::vector<BiomeType> spawnBiomes;

	bool hasAttacked = false;

	bool wasInWater = false;

	// rotation of mob
	float rotation = 0;
};
void declareMobs() {
	for (int i = 0; i < MOBTYPE_COUNT; i++) {
		mobMap.insert(std::pair<int, Mob>(i, Mob(MobType(i))));
		mobMap[i].setType(MobType(i));
	}

	BiomeType t[2] = { BIOME_GRASSLANDS, BIOME_FOREST };
	mobMap[MOB_COW].setHealth(10);
	mobMap[MOB_COW].setMoveSpeed(0.3);
	mobMap[MOB_COW].setSpawnRate(100000);
	mobMap[MOB_COW].setSpawnBiomes(t);

	BiomeType t2[4] = { BIOME_GRASSLANDS, BIOME_FOREST, BIOME_SAVANNAH, BIOME_DESERT };
	mobMap[MOB_MOUSE].setHealth(10);
	mobMap[MOB_MOUSE].setMoveSpeed(0.4);
	mobMap[MOB_MOUSE].setSpawnRate(75000);
	mobMap[MOB_MOUSE].setSpawnBiomes(t2);
	
	mobMap[MOB_MOUSE_ANGRY].setHealth(15);
	mobMap[MOB_MOUSE_ANGRY].setMoveSpeed(0.5);
	mobMap[MOB_MOUSE_ANGRY].setHostility(true);
	mobMap[MOB_MOUSE_ANGRY].setDamage(1);
}