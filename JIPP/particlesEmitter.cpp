#include "particlesEmitter.h"
#include "time.h"
#include "iostream"
#include "cmath"

#define M_PI 3.14159265358979323846f

using uint = unsigned int;

namespace JIPP
{
	template <typename T>
	void remove_if(std::vector<T>& vect, bool(*predicate)(T obj))
	{
		for (uint i = 0; i < vect.size(); i++)
		{
			if (predicate(vect[i])) { vect.erase(vect.begin() + i); i--; }
		}
	}
}

int random(int begin, int end)
{
	return rand() % (end - begin + 1) + begin;
}

float asRadians(float angle)
{
	return angle * M_PI / 180;
}

float asDegrees(float radians)
{
	return radians * 180 / M_PI;
}


particlesEmitter::particlesEmitter() : frequency(0.00), particlesCreator(nullptr)
{
	particlesEmitterVector.reserve(512);
}

particlesEmitter::~particlesEmitter()
{
}

void particlesEmitter::update(const double& deltaTime)
{
	static const float precission = 0.01f;
	if (emitterClock.getElapsedTime().asMilliseconds() > frequency)
	{
		particlesEmitterVector.push_back(particle{ particlesCreator,static_cast<float>(random(lifeTimeInterval.x, lifeTimeInterval.y)) ,random(runSpeedInterval.x, runSpeedInterval.y)*precission , random(rotateSpeedInterval.x, rotateSpeedInterval.y)*precission, random(initialRotationInterval.x, initialRotationInterval.y)*precission, random(runAngleInterval.x, runAngleInterval.y) });
		particlesEmitterVector.back().particleShape.move(position);
		emitterClock.restart();
	}

	for (auto& part : particlesEmitterVector)
	{
		part.update(deltaTime);
	}

	JIPP::remove_if<particlesEmitter::particle>(particlesEmitterVector, [](particle p)->bool {return p.particleShape.getFillColor().a == 0; });
}

void particlesEmitter::draw(sf::RenderWindow & window)
{
	for (auto& x : particlesEmitterVector) x.draw(window);
}

void particlesEmitter::move(sf::Vector2f & movementVector)
{
	position = { position.x + movementVector.x, position.y + movementVector.y };
}

void particlesEmitter::setPosition(const sf::Vector2f & position)
{
	this->position = position;
}

sf::Vector2f particlesEmitter::getPosition()
{
	return position;
}

void particlesEmitter::setParticlesCreator(void(*creator)(sf::RectangleShape& part))
{
	/// creator must create new part, set Size and Origin 
	particlesCreator = creator;
}

std::vector<particlesEmitter::particle>& particlesEmitter::getparticlesEmitterVector()
{
	return particlesEmitterVector;
}

void particlesEmitter::setRunSpeedInterval(int begin, int end)
{
	runSpeedInterval = { begin, end };
}

void particlesEmitter::setLifeTimeInterval(int begin, int end)
{
	lifeTimeInterval = { begin, end };
}

void particlesEmitter::setRunAngleInterval(int begin, int end)
{
	runAngleInterval = { begin, end };
}

void particlesEmitter::setRotateSpeedInterval(int begin, int end)
{
	rotateSpeedInterval = { begin, end };
}

void particlesEmitter::setInitialRotationInterval(int begin, int end)
{
	initialRotationInterval = {begin, end};
}

void particlesEmitter::setCreatingTimePeriod(float frequency)
{
	this->frequency = frequency;
}

/*******************************************************************************************************************/
/*******************************************************************************************************************/
/**particlesEmitter::particle**/

particlesEmitter::particle::particle() 
	: lifeTime(0), runSpeed(0), runAngle(0), rotateSpeed(0), moveVector{0,0}
{
	particleShape.setSize(sf::Vector2f{ 0,0 });
}

particlesEmitter::particle::particle(const particle & part)
	: lifeTime(part.lifeTime), runSpeed(part.runSpeed), runAngle(part.runAngle), rotateSpeed(part.rotateSpeed)
{
	moveVector = { sin(asRadians(runAngle))*runSpeed , -cos(asRadians(runAngle))*runSpeed };
	this->moveVector = part.moveVector;
	this->partClock = part.partClock;
	particleShape = part.particleShape;
}

particlesEmitter::particle::particle(void(*creator)(sf::RectangleShape& part), float lifeTime, float runSpeed, float rotateSpeed, float initialRotation, int runAngle)
	: lifeTime(lifeTime), runSpeed(runSpeed), runAngle(static_cast<float>(runAngle)), rotateSpeed(rotateSpeed)
{
	moveVector = {sin(asRadians(static_cast<float>(runAngle)))*runSpeed ,  -cos(asRadians(static_cast<float>(runAngle)))*runSpeed };
	if (creator)
	{
		creator(particleShape);
	}
}

particlesEmitter::particle::~particle()
{
}

particlesEmitter::particle& particlesEmitter::particle::operator=(const particle& source)
{
	this->lifeTime = source.lifeTime;
	this->runSpeed = source.runSpeed;
	this->rotateSpeed = source.rotateSpeed;
	this->runAngle = source.runAngle;

	this->partClock = source.partClock;
	this->moveVector = source.moveVector;

	particleShape = source.particleShape;

	return *this;
}

void particlesEmitter::particle::update(const double& deltaTime)
{
	particleShape.move(sf::Vector2f{static_cast<float>(moveVector.x*deltaTime), static_cast<float>(moveVector.y*deltaTime)});
	particleShape.rotate(static_cast<float>(rotateSpeed*deltaTime));

	if (partClock.getElapsedTime().asMilliseconds() > lifeTime)
	{
		sf::Color color = particleShape.getFillColor();
		if (static_cast<float>(color.a) + runSpeed*deltaTime < 0) color.a = 0;
		else if (static_cast<float>(color.a) - runSpeed*deltaTime < 0) color.a = 0;
			else color.a -= static_cast<sf::Uint8>(runSpeed*deltaTime);
		
		particleShape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	}
}

void particlesEmitter::particle::draw(sf::RenderWindow & window)
{
	window.draw(particleShape);
}

