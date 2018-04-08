#pragma once
#include <SFML/Graphics.hpp>
#include "particlesEmitter.h"

class block
{
public:
	block();
	~block();
	static sf::Vector3f blockSize;
	sf::ConvexShape top;
	sf::RectangleShape front;
	sf::ConvexShape side;
	std::unique_ptr<particlesEmitter> emitter;

	void draw(sf::RenderWindow& window) const;
	void move(const sf::Vector2f& movement);
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getCenter();

	static sf::Texture top_texture, front_texture, side_texture;

	static void loadTextures();
};

