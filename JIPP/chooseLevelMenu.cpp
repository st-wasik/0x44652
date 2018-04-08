#include "chooseLevelMenu.h"
#include "fstream"
#include "string"


chooseLevelMenu::chooseLevelMenu(const sf::Font& f, sf::RenderWindow & window, appState& state, std::string& lvl, int& lvlNo) : menu_template(f, window, state), levelName(lvl), choosenLevelNumber(lvlNo)
{
	//menu_buttons
	textContainer.resize(12);
	textContainer[0].setString("Choose Level");

	textContainer[1].setString("Level #1");
	textContainer[2].setString("Level #2");
	textContainer[3].setString("Level #3");
	textContainer[4].setString("Level #4");
	textContainer[5].setString("Level #5");

	textContainer[6].setString("-");
	textContainer[7].setString("-");
	textContainer[8].setString("-");
	textContainer[9].setString("-");
	textContainer[10].setString("-");

	std::ifstream fin("resources/settings/settings.dat");
	if (!fin)
	{
		std::ofstream fout("resources/settings/settings.dat");
		if (fout)
		{
			for (float i = 6.f; i < textContainer.size() - 1; i += 1.0f)
			{
				fout << std::string(textContainer[i].getString()) << std::endl;
			}
		}
		fout.close();
	}
	else
	{
		for (float i = 6.f; i < textContainer.size() - 1 && !fin.eof(); i += 1.0f)
		{
			std::string temp;
			fin >> temp;
			textContainer[i].setString(temp);
		}
		fin.close();
	}

	textContainer[11].setString("Back to menu");

	float i = 1.0f;
	for (float i = 1.f; i < textContainer.size() / 2; i += 1.0f)
	{
		sf::Text& t = textContainer[i];

		t.setFont(f);
		t.setCharacterSize(72);
		t.setOrigin({ t.getGlobalBounds().width / 2, t.getGlobalBounds().height / 2 });
		t.setPosition(
			sf::Vector2f{ static_cast<float>(window.getSize().x / 4),static_cast<float>(window.getSize().y) * ((i + 1) / (textContainer.size() / 2 + 2)) }
		);
	}

	for (float i = 6.f; i < textContainer.size() - 1; i += 1.0f)
	{
		sf::Text& t = textContainer[i];

		t.setFont(f);
		t.setCharacterSize(72);
		t.setOrigin({ t.getGlobalBounds().width / 2, t.getGlobalBounds().height / 2 });
		t.setPosition(
			sf::Vector2f{ static_cast<float>(window.getSize().x / 4 * 3),static_cast<float>(window.getSize().y) * ((i + 1 - 5) / (textContainer.size() / 2 + 2)) }
		);
	}

	i = 1.0f;

	textContainer[0].setFont(f);
	textContainer[0].setCharacterSize(90);
	textContainer[0].setOrigin({ textContainer[0].getGlobalBounds().width / 2, textContainer[0].getGlobalBounds().height / 2 });
	textContainer[0].setPosition(
		sf::Vector2f{ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) * (i / (textContainer.size() / 2 + 2)) }
	);

	i = 7.0f;

	textContainer.back().setFont(f);
	textContainer.back().setCharacterSize(72);
	textContainer.back().setOrigin({ textContainer.back().getGlobalBounds().width / 2, textContainer.back().getGlobalBounds().height / 2 });
	textContainer.back().setPosition(
		sf::Vector2f{ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) * (i / (textContainer.size() / 2 + 2)) }
	);
	//END_menu_buttons

	//menu_background
	{
		sf::Texture& T = backgroundTexture[0];
		sf::RectangleShape& R = background[0];

		if (!T.loadFromFile("resources/textures/menu_background_2.png")) { throw std::exception("Cannot load texture. Program will exit."); }
		T.setRepeated(1);
		R.setFillColor(sf::Color(255, 255, 255, 127));
		R.setSize({ static_cast<float>(window.getSize().x),static_cast<float>(T.getSize().y) });
		R.setTexture(&T);
		R.setTextureRect({ 0, 0,static_cast<int>(R.getSize().x), static_cast<int>(T.getSize().y) });
		R.setPosition({ 0,static_cast<float>(window.getSize().y - T.getSize().y) });
	}
	{
		sf::Texture& T = backgroundTexture[1];
		sf::RectangleShape& R = background[1];

		if (!T.loadFromFile("resources/textures/menu_background_3.png")) { throw std::exception("Cannot load texture. Program will exit."); }
		T.setRepeated(1);
		R.setSize({ static_cast<float>(window.getSize().x),static_cast<float>(window.getSize().y) });
		R.setTexture(&T);
		R.setTextureRect({ 0, 0,static_cast<int>(window.getSize().x), static_cast<int>(window.getSize().y) });
		R.setPosition({ 0,0 });
	}
	//END_menu_background
	if (!menuTheme.openFromFile("resources/sounds/x13.ogg")) { std::cerr << "Cannot load music." << std::endl; }
}

chooseLevelMenu::~chooseLevelMenu()
{
	std::ofstream fout("resources/settings/settings.dat");
	if (fout)
	{
		for (float i = 6.f; i < textContainer.size() - 1; i += 1.0f)
		{
			fout << std::string(textContainer[i].getString()) << std::endl;
		}
	}
	fout.close();
}

void chooseLevelMenu::run()
{
	window.setMouseCursorVisible(1);
	menuTheme.play();
	while (window.isOpen() && state == appState::levelMenu)
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

		//menu:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			levelName = "bonus.lvl";
			choosenLevelNumber = 0;
			state = appState::game;
		}

		if (textContainer[1].getGlobalBounds().contains(mouse) || textContainer[6].getGlobalBounds().contains(mouse))
		{
			textContainer[1].setFillColor(sf::Color(192, 192, 192));
			textContainer[6].setFillColor(sf::Color(192, 192, 192));
			if (mouseLeftReleased)
			{
				levelName = "Jipp1.lvl";
				choosenLevelNumber = 1;
				state = appState::game;
			}
		}
		else if (textContainer[2].getGlobalBounds().contains(mouse) || textContainer[7].getGlobalBounds().contains(mouse))
		{
			textContainer[2].setFillColor(sf::Color(192, 192, 192));
			textContainer[7].setFillColor(sf::Color(192, 192, 192));
			if (mouseLeftReleased)
			{
				levelName = "Jipp2.lvl";
				choosenLevelNumber = 2;
				state = appState::game;
			}
		}
		else if (textContainer[3].getGlobalBounds().contains(mouse) || textContainer[8].getGlobalBounds().contains(mouse))
		{
			textContainer[3].setFillColor(sf::Color(192, 192, 192));
			textContainer[8].setFillColor(sf::Color(192, 192, 192));
			if (mouseLeftReleased)
			{
				levelName = "Jipp3.lvl";
				choosenLevelNumber = 3;
				state = appState::game;
			}
		}
		else if (textContainer[4].getGlobalBounds().contains(mouse) || textContainer[9].getGlobalBounds().contains(mouse))
		{
			textContainer[4].setFillColor(sf::Color(192, 192, 192));
			textContainer[9].setFillColor(sf::Color(192, 192, 192));
			if (mouseLeftReleased)
			{
				levelName = "Jipp4.lvl";
				choosenLevelNumber = 4;
				state = appState::game;
			}
		}
		else if (textContainer[5].getGlobalBounds().contains(mouse) || textContainer[10].getGlobalBounds().contains(mouse))
		{
			textContainer[5].setFillColor(sf::Color(192, 192, 192));
			textContainer[10].setFillColor(sf::Color(192, 192, 192));
			if (mouseLeftReleased)
			{
				levelName = "Jipp5.lvl";
				choosenLevelNumber = 5;
				state = appState::game;
			}
		}

		if (textContainer.back().getGlobalBounds().contains(mouse))
		{
			textContainer.back().setFillColor(sf::Color(255, 0x66, 0x66));
			if (mouseLeftReleased)
			{
				levelName.clear();
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

sf::Text& chooseLevelMenu::getText(int index)
{
	return textContainer[index];
}
