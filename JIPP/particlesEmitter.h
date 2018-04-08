#pragma once
#include "SFML/Graphics.hpp"
#include "vector"
#include "memory"
#include "algorithm"

int random(int begin, int end);

class particlesEmitter
{
public:
	particlesEmitter();
	~particlesEmitter();

	void setRunSpeedInterval(int begin, int end);
	void setLifeTimeInterval(int begin, int end);
	void setRunAngleInterval(int begin, int end);
	void setRotateSpeedInterval(int begin, int end);
	void setInitialRotationInterval(int begin, int end);
	void setCreatingTimePeriod(float frequency);
	
	void update(const double& deltaTime);
	void draw(sf::RenderWindow& window);

	void move(sf::Vector2f& movementVector);
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition();

	void setParticlesCreator(void (*creator)(sf::RectangleShape& part));

	class particle
	{
	public:
		particle();
		particle(const particle& part);
		particle(void (*creator)(sf::RectangleShape& part), float lifeTime, float runSpeed, float rotateSpeed, float initialRotation, int runAngle);
		~particle();
		
		particle& operator=(const particle& source);

		void update(const double& deltaTime);
		void draw(sf::RenderWindow& window);

		sf::Clock partClock;

		float lifeTime;
		float runSpeed;
		float runAngle; 
		float rotateSpeed;

		sf::Vector2f moveVector;
		sf::RectangleShape particleShape;

	};

	std::vector<particle>& getparticlesEmitterVector();

private:
	void (*particlesCreator)(sf::RectangleShape& part);
	sf::Clock emitterClock;
	sf::Vector2f position;
	sf::Vector2i runSpeedInterval;
	sf::Vector2i runAngleInterval;
	sf::Vector2i lifeTimeInterval;
	sf::Vector2i rotateSpeedInterval;
	sf::Vector2i initialRotationInterval;
	double frequency;
	std::vector<particle> particlesEmitterVector;

};

template <typename T>
void move(std::vector<T>& dest, std::vector<T>& source)
{
	while (!source.empty())
	{
		dest.push_back(source.back());
		source.pop_back();
	}
}
