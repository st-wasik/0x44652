#include "gameDefeat.h"

gameDefeat::gameDefeat(const sf::Font& f, sf::RenderWindow& wi, appState& st) : menu_template(f, wi, st)
{
	//menu_buttons
	textContainer.resize(7);
	textContainer[0].setString("Defeat");
	textContainer[1].setString("");
	textContainer[2].setString("You fell from"); //levelName
	textContainer[3].setString("");
	textContainer[4].setString("Try again!"); //elapsedTime
	textContainer[6].setString("Back to menu");
	float i = 1.0f;
	for (auto& t : textContainer)
	{
		t.setFont(f);
		t.setCharacterSize(72);
		t.setOrigin({ t.getGlobalBounds().width / 2, t.getGlobalBounds().height / 2 });
		t.setPosition(
			sf::Vector2f{ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) * (i / (textContainer.size() + 2)) }
		);
		i += 1;
	}
	i = 1.0f;

	textContainer[0].setFont(f);
	textContainer[0].setCharacterSize(90);
	textContainer[0].setOrigin({ textContainer[0].getGlobalBounds().width / 2, textContainer[0].getGlobalBounds().height / 2 });
	textContainer[0].setPosition(
		sf::Vector2f{ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) * (i / (textContainer.size() + 2)) }
	);
	//END_menu_buttons

	if (!menuTheme.openFromFile("resources/sounds/x08.ogg")) { std::cerr << "Cannot load music." << std::endl; }
}

gameDefeat::~gameDefeat()
{
	///*ERROR: HEAP CORRUPTION*///
	backgroundTexture[0].~Texture();
	///*ERROR: HEAP CORRUPTION*///
	backgroundTexture[1].~Texture();
}

void gameDefeat::run()
{
	window.setMouseCursorVisible(1);
	menuTheme.play();

	while (window.isOpen() && state == appState::game)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		update(deltaTime);
		//text_color
		textContainer[3].setFillColor(sf::Color(192, 192, 192));
		//textContainer[4].setFillColor(sf::Color(192, 192, 192));

		//menu:
		if (textContainer.back().getGlobalBounds().contains(mouse))
		{
			textContainer.back().setFillColor(sf::Color(255, 0x66, 0x66));
			if (mouseLeftReleased)
			{
				//state = appState::levelMenu;
				state = appState::menu;
			}
		}

		window.setView(menuView);

		window.clear();
		draw(window);
		window.display();
	}

	menuTheme.stop();
}

void gameDefeat::setInfo(const gameResult& res)
{
	textContainer[3].setString(res.levelName);
	textContainer[3].setOrigin({ textContainer[3].getGlobalBounds().width / 2, textContainer[3].getGlobalBounds().height / 2 });
	textContainer[3].setPosition(sf::Vector2f{ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) * (4.0f / (textContainer.size() + 2)) });
}
