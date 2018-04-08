#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "vector"
#include "array"
#include "map"
#include "iostream"
#include "algorithm"
#include "particlesEmitter.h"
#include "appState.h"

class menu_template
{
protected:
	sf::RenderWindow&		window;
	appState&				state;

	double deltaTime;
	sf::Clock deltaClock;
	sf::Event event;
	sf::View menuView;

	int emitterDensity;
	std::vector<sf::Text> textContainer;
	std::vector<particlesEmitter> particlesVector;
	std::array<sf::Texture, 2> backgroundTexture;
	std::array<sf::RectangleShape, 2> background;
	sf::Music menuTheme;

	bool mouseLeftPressed;
	bool mouseLeftPressedOld;
	bool mouseLeftReleased;
	sf::Vector2f mouse;

public:
	menu_template(const sf::Font&, sf::RenderWindow& window, appState& st);
	~menu_template();
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	virtual void run() = 0;
};