#pragma once
#include "Entity.h"
#include "DroppedItem.h" 
#include "Chunk.h"
#include "Collision.h"

class Mob;

std::map<int, Mob> mobMap;
std::vector<Mob> mobVector;
class Mob : virtual public Entity, public Collidable{
public:
	Mob() { height = 5; }
	Mob(MobType _type) {
		*this = mobMap[_type];
		type = _type;
		height = 5;
		vis.setTexture(mobTextureMap[type]);
		head.setTexture(mobTextureMap[type]);
		vis.setTextureRect(sf::IntRect(0, 0, 96, 192));
		head.setTextureRect(sf::IntRect(0, 192, 96, 96));
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
	void setMoveSpeed(float newMoveSpeed) {
		MOVE_SPEED = newMoveSpeed;
	}
	void update() {
		physics();
		isInWater();
		draw();
		setCurrChunk();
		head.setPosition(sf::Vector2f(getPosition().x + _head_from_middle_amount*(sin(rotation * PI/180)), getPosition().y - _head_from_middle_amount*cos(rotation * PI/180)));
		head.setRotation(headRotation);
		vis.setRotation(rotation);
		aiStuff();
		if (health < 1) isDeleted = true;
		drawableVector.push_back(ObjDrawable(head, height + 0.1));
	}
	sf::Vector2i getCurrChunk() { return currChunk; }
	sf::Vector2i getCurrSquare() { return currSquare; }
	// returns rotation (in degrees)
	double getRotation() {
		return rotation;
	}

	void checkCollision(Collidable &collidable) {
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

		}
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
	}

	static void spawnMob(Square &square) {
		if (getChance(GAME_MOB_SPAWN_FREQUENCY) && mobVector.size() < GAME_MOB_MAX_COUNT) {
			switch (square.getType()) {
			case BIOME_GRASSLANDS:
				mobVector.push_back(Mob(MOB_COW));
				mobVector.back().setPosition(square.ground.getPosition()); break;
			}
		}
	}
	
	// initial mob spawn
	static void spawnMob(int denominator, sf::IntRect chunks) {
		for (int i = chunks.left-1; i < chunks.width; i++) {
			for (int a = chunks.top-1; a < chunks.height; a++) {
				bool placed = false; int amount = getRandomInt(1, 5);
				for (std::vector<Square> squares : chunkVector[a][i].squareVector) {
					if (placed) break;
					for (Square square : squares) {
						if (placed) break;
						if (getChance(denominator*GAME_SQUARE_PER_CHUNK_AMOUNT.x*GAME_SQUARE_PER_CHUNK_AMOUNT.y)) {
							switch (square.getType()) {
							case BIOME_GRASSLANDS: case BIOME_FOREST:
								for (int b = 0; b < amount; b++) {
									mobVector.push_back(Mob(MOB_COW));
									mobVector.back().setPosition(square.ground.getPosition()); break;
								}
							}
							placed = true;
						}
					}
				}
			}
		}
	}
private:
	MobType type;
	sf::Sprite head;
	sf::Vector2i currChunk, currSquare;
	float _head_from_middle_amount = 64;
	float headRotation = 180 + rotation;
	float toRotation;
	float aiStateLengthMin = 2, aiStateLengthMax = 5;
	sf::Clock aiClock;
	float aiStateLength = getRandomFloat(aiStateLengthMin, aiStateLengthMax);

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
			aiState = AIState(getRandomInt(0, STATETYPE_SIZE));
			rotation = toRotation;
		}

		else {
			switch (aiState) {
			case STATE_WANDER:	move(); break;
			case STATE_RUN:		move(MOVE_SPEED * 2); break;
			}
			if (aiClock.getElapsedTime().asSeconds() >= aiStateLength - 1) {
				//headRotation = 
				rotation = (int(toRotation) != int(rotation)) ? (toRotation - rotation)*(aiClock.getElapsedTime().asSeconds() - (aiStateLength - 1)) + rotation : toRotation;
				//headRotation = rotation+180;
			}
			else if (aiClock.getElapsedTime().asSeconds() >= aiStateLength - 1.5) {
				int r = int(headRotation)-180;
				headRotation = (int(toRotation) != r) ? ((toRotation - r)*2)*(aiClock.getElapsedTime().asSeconds()-(aiStateLength-1.5)) + headRotation: toRotation+180;
			}	
				
			else toRotation = getRandomFloat(rotation - 85, rotation + 85);

		}
	}
	void move() {
		move(MOVE_SPEED);
	}
	void move(double speed) {
		setAcceleration(sf::Vector2f(speed*sin(rotation*PI / 180), -speed * cos(rotation*PI / 180)));
	}
	
protected:
	void setCurrChunk() {
		currChunk = sf::Vector2i(getPosition().x / GAME_CHUNK_SIZE, getPosition().y / GAME_CHUNK_SIZE);

		currSquare = sf::Vector2i(
			((getPosition().x - (GAME_CHUNK_SIZE * currChunk.x))) / GAME_SQUARE_SIZE,
			((getPosition().y - (GAME_CHUNK_SIZE * currChunk.y))) / GAME_SQUARE_SIZE);

		
	}

	void isInWater() {
		if (getPosition().x <= GAME_TOTAL_SIZE - 10 && Entity::getPosition().y <= GAME_TOTAL_SIZE - 10 &&
			chunkVector[currChunk.y][currChunk.x].squareVector[currSquare.y][currSquare.x].getType() == BIOME_WATER) inWater = true;
		else inWater = false;
	}

	int health;

	float MOVE_SPEED;

	// rotation of mob
	float rotation = 0;
};
void declareMobs() {
	for (int i = 0; i < MOBTYPE_COUNT - 1; i++) {
		mobMap.insert(std::pair<int, Mob>(i, Mob(MobType(i))));
	}
	mobMap[MOB_COW].setHealth(10);
	mobMap[MOB_COW].setMoveSpeed(0.3);
}