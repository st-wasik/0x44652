#include "block.h"
#include "iostream"

sf::Vector3f block::blockSize(64, 32, 24);
//sf::Vector3f block::blockSize(128, 64, 256);

sf::Texture block::top_texture, block::front_texture, block::side_texture;

block::block() : emitter (nullptr)
{
	top.setPointCount(4);
	top.setPoint(0, sf::Vector2f{ 0,0 });
	top.setPoint(1, sf::Vector2f{ blockSize.x,0 });
	top.setPoint(2, sf::Vector2f{ blockSize.x*3/2,blockSize.y });
	top.setPoint(3, sf::Vector2f{ blockSize.x/2,blockSize.y });
	
	top.setTexture(&top_texture);

	front.setSize(sf::Vector2f{ blockSize.x, blockSize.z });

	side.setPointCount(4);
	side.setPoint(0, sf::Vector2f{ 0,0 });
	side.setPoint(1, sf::Vector2f{ blockSize.x / 2 ,blockSize.y });
	side.setPoint(2, sf::Vector2f{ blockSize.x/2 ,blockSize.z + blockSize.y });
	side.setPoint(3, sf::Vector2f{0 ,blockSize.z});

	top.setPosition(0, 0);
	front.setPosition(blockSize.x / 2, blockSize.y);
	front.setFillColor(sf::Color::White);
	side.setPosition(0,0);

	top.setOutlineColor(sf::Color::Yellow);
	top.setOutlineThickness(-0.f);
	front.setOutlineColor(sf::Color::Blue);
	front.setOutlineThickness(-0.f);
	front.setTexture(&front_texture);
	side.setTexture(&side_texture);

}

block::~block()
{
}

void block::draw(sf::RenderWindow & window) const
{
	window.draw(top);
	window.draw(front);
	window.draw(side);
}

void block::move(const sf::Vector2f & movement)
{
	top.move(movement);
	front.move(movement);
	side.move(movement);
}

void block::setPosition(const sf::Vector2f & position)
{
	auto movement = - top.getPosition() + position;
	move(movement);
}

sf::Vector2f block::getCenter()
{
	return{ top.getGlobalBounds().left + top.getGlobalBounds().width/2,  top.getGlobalBounds().top + top.getGlobalBounds().height / 2 };
}

void block::loadTextures()
{
	top_texture.loadFromFile("resources/textures/top.png");
	front_texture.loadFromFile("resources/textures/front.png", sf::IntRect(0, 0, blockSize.x, blockSize.z));
	side_texture.loadFromFile("resources/textures/front.png");
	top_texture.setSmooth(1);
	front_texture.setSmooth(1);
	side_texture.setSmooth(1);
}
