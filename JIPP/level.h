#pragma once
#include "vector"
#include "block.h"
#include "string"
#include "memory"
#include "particlesEmitter.h"
#include "iostream"
#include "array"

class level
{
private:
	
	int emitterDensity;
public:
	level(sf::RenderWindow& window, sf::Font& f);
	~level();

	bool end;
	bool endOld;

	bool closeLevel;

	std::string levelName;
	particlesEmitter endEmitter;

	sf::Vector2u beginBlockCoords;
	sf::Vector2u endBlockCoords;

	sf::Vector2u levelSize;
	sf::Clock levelClock;

	std::array<sf::Texture, 5> backgroundTexture;
	sf::RectangleShape background;

	std::vector<std::vector<std::unique_ptr<block>>> levelArray;
	std::vector<particlesEmitter::particle> globalParticlesVector;
	std::vector<particlesEmitter> particlesEmitterVector;



	void update(float deltaTime, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window) ;
	void draw(sf::RenderWindow& window, sf::Vector2u begin, sf::Vector2u end);
	void loadFromFile(std::string filename);
	sf::Vector2f getBeginBlockPosition();
	sf::Vector2f getEndBlockPosition();

	std::vector<particlesEmitter::particle>& getGlobalParticlesVector();
	float levelEndTime;

	sf::Text levelNameText;
	sf::Text timeText;
};

