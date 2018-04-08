#pragma once
#include "menu_template.h"
#include "game.h"



class gameSuccess final : public menu_template
{
public:
	gameSuccess(const sf::Font&, sf::RenderWindow& window, appState& st);
	~gameSuccess();
	void run();
	void setInfo(const gameResult& res);
};

