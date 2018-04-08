#include "menu.h"



menu::menu(const sf::Font& f, sf::RenderWindow& wi, appState& st) : menu_template(f, wi,st)
{

	//menu_buttons
	textContainer.resize(3);
	textContainer[1].setString("Play");
	textContainer[2].setString("Exit");
	textContainer[0].setString("0x44652");
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

	
	if (!menuTheme.openFromFile("resources/sounds/x03ces.ogg")) { std::cerr << "Cannot load music." << std::endl; }
}

menu::~menu()
{
	///*ERROR: HEAP CORRUPTION*///
	backgroundTexture[0].~Texture(); 
	///*ERROR: HEAP CORRUPTION*///
	backgroundTexture[1].~Texture(); 

	//std::clog << "menu d-tor" << std::endl;
}

void menu::run()
{
	window.setMouseCursorVisible(1);
	menuTheme.play();

	while (window.isOpen() && state == appState::menu)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		update(deltaTime);

		//menu:
		if (textContainer[1].getGlobalBounds().contains(mouse))
		{
			textContainer[1].setFillColor(sf::Color(192, 192, 192));
			if (mouseLeftReleased)
			{
				//state = appState::levelMenu;
				state = appState::levelMenu;
			}
		}
		else if (textContainer[2].getGlobalBounds().contains(mouse))
		{
			textContainer[2].setFillColor(sf::Color(255, 0x66, 0x66));
			if (mouseLeftReleased)
			{
				state = appState::exit;
			}
		}

		window.setView(menuView);

		window.clear();
		draw(window);
		window.display();
	}

	menuTheme.stop();
}
