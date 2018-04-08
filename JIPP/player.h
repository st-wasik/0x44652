#pragma once
#include "SFML/Graphics.hpp"
#include "particlesEmitter.h"
#include "memory"

class player
{
private:
	float maxMoveSpeed;
	float accelerationSpeed;
	float brakeSpeed;
	float fallAcceleration;
	float currentFallSpeed;
	sf::Vector2f movementVector;
	sf::Vector2f playerPosition;
	sf::CircleShape playerShape;
	sf::Clock textureClock;
	sf::Texture playerTexture[10];
	int textureCounter;
	int emitterOnMove;
	int emitterOnStop;

	std::unique_ptr<particlesEmitter> emitter;

public:
	player();
	~player();
	bool outOfLevel;
	bool outOfLevelOld;
	sf::Vector2i positionIndex;


	void update(const double& deltaTime);
	void move_update(const double& deltaTime);
	void draw(sf::RenderWindow& window);

	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition();
	sf::Vector2f getOrigin();

	void moveOrigin(sf::Vector2f move);

	float getRadius();
	particlesEmitter& getEmitter();
	std::vector<particlesEmitter::particle>& getParticlesVector();
};

