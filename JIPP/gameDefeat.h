#pragma once
#include "menu_template.h"
#include "game.h"

class gameDefeat final : public menu_template
{
public:
	gameDefeat(const sf::Font&, sf::RenderWindow& window, appState& st);
	~gameDefeat();
	void run();
	void setInfo(const gameResult& res);
};

