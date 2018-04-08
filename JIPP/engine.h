#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include "level.h"
#include "player.h"
#include "menu.h"
#include "chooseLevelMenu.h"
#include "appState.h"
#include "game.h"
#include "gameSuccess.h"
#include "gameDefeat.h"

class engine
{
	std::string levelToLoad;
	int currentLevelNumber;
	sf::RenderWindow window;
	sf::Font mainFont;
	sf::Event event;
	sf::Clock mainClock;
	appState state;

	std::unique_ptr<game> game_ptr;
	std::unique_ptr<menu> menu_ptr;
	std::unique_ptr<gameSuccess> success_ptr;
	std::unique_ptr<chooseLevelMenu> level_menu_ptr;
	std::unique_ptr<gameDefeat> defeat_ptr ;

	void runLevelMenu();

public:
	engine();
	~engine();

	void run();
};

