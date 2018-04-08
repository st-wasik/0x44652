#include "player.h"
#include "iostream"
#include "cmath"

sf::Vector2f operator/(sf::Vector2f vect, int x)
{
	return{ vect.x / 2, vect.y / 2 };
}

player::player() : maxMoveSpeed(0.4), outOfLevel(0), accelerationSpeed(0.009), brakeSpeed(0.009), movementVector(0, 0),
fallAcceleration(0.02), currentFallSpeed(0), emitter(nullptr), textureCounter(0), emitterOnMove(100), emitterOnStop(750)
{
	int radius = 16;
	playerShape.setRadius(radius);
	playerShape.setOrigin(radius, radius);

	emitter = std::make_unique<particlesEmitter>();
	emitter->setInitialRotationInterval(0, 355);
	emitter->setLifeTimeInterval(2000, 3000);
	emitter->setRotateSpeedInterval(1, 5);
	emitter->setRunSpeedInterval(3, 5);
	emitter->setRunAngleInterval(-20, 20);

	emitter->setParticlesCreator([](sf::RectangleShape& part) {part.setSize(sf::Vector2f{ 10,10 }); part.setOrigin(part.getSize().x / 2, part.getSize().y / 2); int r = random(160, 255);  part.setFillColor(sf::Color(r, r, 0)); });
	emitter->setCreatingTimePeriod(emitterOnStop);

	if (!(
		playerTexture[0].loadFromFile("resources/textures/player.png", sf::IntRect(0, 0, 32, 32)) &&
		playerTexture[1].loadFromFile("resources/textures/player.png", sf::IntRect(32, 0, 64, 64)) &&
		playerTexture[2].loadFromFile("resources/textures/player.png", sf::IntRect(0, 32, 32, 32)) &&
		playerTexture[3].loadFromFile("resources/textures/player.png", sf::IntRect(32, 32, 64, 64)) &&
		playerTexture[4].loadFromFile("resources/textures/player.png", sf::IntRect(0, 64, 32, 32)) &&
		playerTexture[5].loadFromFile("resources/textures/player.png", sf::IntRect(32, 64, 64, 64))
		))
	{
		throw std::exception("Cannot load texture. Program will exit.");
	}

	playerTexture[6] = playerTexture[4];
	playerTexture[7] = playerTexture[3];
	playerTexture[8] = playerTexture[2];
	playerTexture[9] = playerTexture[1];

	playerShape.setTexture(&playerTexture[0]);
	textureClock.restart();
}

void player::update(const double& deltaTime)
{
	outOfLevelOld = outOfLevel;
	float fall = fallAcceleration*deltaTime;

	if (outOfLevel)
	{
		currentFallSpeed += fall;
		moveOrigin({ 0,currentFallSpeed });
		emitter->setCreatingTimePeriod(0);
		emitter->setRunSpeedInterval(50, 50);
		emitter->setParticlesCreator([](sf::RectangleShape& part) {part.setSize(sf::Vector2f{ 20,20 }); part.setOrigin(part.getSize() / 2); int rand_int = random(192, 255); part.setFillColor(sf::Color(rand_int, rand_int, 0)); });
	}

	if (emitter)
	{
		emitter->setPosition(playerShape.getPosition() - playerShape.getOrigin() + sf::Vector2f{ playerShape.getRadius(),playerShape.getRadius() });
		emitter->update(deltaTime);
	}

	if (textureClock.getElapsedTime().asMilliseconds() > 50)
	{
		if (textureCounter > 9) textureCounter = 0;
		playerShape.setTexture(&playerTexture[textureCounter++]);
		textureClock.restart();
	}
}

void player::move_update(const double & deltaTime)
{
	float acc = accelerationSpeed;
	float brake = brakeSpeed;
	float fall = fallAcceleration;
	bool move = false;

	if(!outOfLevel)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				movementVector.y -= acc;
				if (movementVector.y < -maxMoveSpeed) movementVector.y = -maxMoveSpeed;
				move = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				movementVector.y += acc;
				if (movementVector.y > maxMoveSpeed) movementVector.y = maxMoveSpeed;
				move = true;
			}
		}
		else
		{
			if (abs(movementVector.y) < 0.01) movementVector.y = 0;
			else if (movementVector.y > 0) movementVector.y -= brake;
			else if (movementVector.y < 0) movementVector.y += brake;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				movementVector.x -= acc;
				if (movementVector.x < -maxMoveSpeed) movementVector.x = -maxMoveSpeed;
				move = true;

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				movementVector.x += acc;
				if (movementVector.x > maxMoveSpeed) movementVector.x = maxMoveSpeed;
				move = true;
			}
		}
		else
		{
			if (abs(movementVector.x) <  0.01) movementVector.x = 0;
			else if (movementVector.x > 0) movementVector.x -= brake;
			else if (movementVector.x < 0) movementVector.x += brake;
		}

		if (move)
		{
			emitter->setCreatingTimePeriod(emitterOnMove);
			emitter->setRunSpeedInterval(3, 5);
		}
		else
		{
			emitter->setCreatingTimePeriod(emitterOnStop);
			emitter->setRunSpeedInterval(3, 5);
		}
	}
	else
	{
		emitter->setCreatingTimePeriod(0);
		emitter->setRunSpeedInterval(50, 50);
		emitter->setParticlesCreator([](sf::RectangleShape& part) {part.setSize(sf::Vector2f{ 20,20 }); part.setOrigin(part.getSize() / 2); int rand_int = random(192, 255); part.setFillColor(sf::Color(rand_int, rand_int, 0)); });

		emitter->setCreatingTimePeriod(emitterOnStop);
		emitter->setRunSpeedInterval(3, 5);
	}
	playerShape.move(movementVector*static_cast<float>(deltaTime));
	
	if (emitter)
	{
		emitter->setPosition(playerShape.getPosition() - playerShape.getOrigin() + sf::Vector2f{ playerShape.getRadius(),playerShape.getRadius() });
		emitter->update(deltaTime);
	}
}

void player::draw(sf::RenderWindow & window)
{
	window.draw(playerShape);
}

void player::setPosition(const sf::Vector2f & position)
{
	playerShape.setPosition(position - sf::Vector2f{ 0, playerShape.getRadius() });
}

sf::Vector2f player::getPosition()
{
	return playerShape.getPosition();
}

sf::Vector2f player::getOrigin()
{
	return playerShape.getOrigin();
}

void player::moveOrigin(sf::Vector2f move)
{
	playerShape.setOrigin(playerShape.getOrigin() - move);
}

float player::getRadius()
{
	return playerShape.getRadius();
}

particlesEmitter & player::getEmitter()
{
	return *emitter;
}

std::vector<particlesEmitter::particle>& player::getParticlesVector()
{
	return emitter->getparticlesEmitterVector();
}

player::~player()
{
}

