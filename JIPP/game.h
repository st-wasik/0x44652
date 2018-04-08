#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "appState.h"
#include "memory"
#include "level.h"
#include "player.h"
#include "math.h"

enum class gameMode {success, defeat, pause};

std::string timeToStr(int time);

class gameResult
{
public:
	gameMode mode;
	float elapsedTime;
	std::string levelName;
};

enum class where {Up, Down, Left, Right};

class game
{
	sf::RenderWindow&		window;
	appState&				state;
	sf::Font& mainFont;
	std::unique_ptr<level>	level_ptr;
	std::unique_ptr<player> player_ptr;

	double deltaTime;
	sf::Clock gameClock;
	sf::Clock deltaClock;
	sf::Clock outOfLevelClock;
	sf::Clock endClock;
	sf::Event event;
	sf::View gameView;

	sf::Music successTheme;
	sf::Music defeatTheme;
	sf::Music startSignal;

	int timeToClose;
	sf::Vector2f introVector;
	float introVectorLength;
	float introVectorDistance;
	bool introInProgress;
	bool introInProgressOld;

	bool	isCollision(const sf::ConvexShape& shape, const sf::Vector2f& point) const;
	where	whereLostCollision(const sf::ConvexShape& shape, const sf::Vector2f& point) const;
	bool levelIntrodution(float deltaTime);

	std::array<sf::Music, 6> musicArray;
	sf::Music* currentMusic;

public:
	game(sf::Font& f, sf::RenderWindow& wn, appState& st);
	~game();
	gameResult	run();
	void setLevel(std::string s);

};

