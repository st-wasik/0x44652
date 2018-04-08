#pragma once
#include "menu_template.h"

class chooseLevelMenu final : public menu_template
{
	std::string& levelName;
	int& choosenLevelNumber;
public:
	chooseLevelMenu(const sf::Font&,sf::RenderWindow& window, appState& state, std::string& levelName, int& lvlNo);
	~chooseLevelMenu();
	void run();
	sf::Text& getText(int index);
};

