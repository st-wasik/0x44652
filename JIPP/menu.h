#pragma once
#include "menu_template.h"

class menu final : public menu_template
{
public:
	menu(const sf::Font&, sf::RenderWindow& window, appState& st);
	~menu();

	void run();
};

