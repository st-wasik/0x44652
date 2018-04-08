#include "engine.h"
#include "particlesEmitter.h"
#include "iostream"
#include "exception"
#include "iostream"

#include "time.h"

using uint = unsigned int;

bool BTimeBetter(const std::string& a, const std::string& b)
{
	auto A = a, B = b;
	if (A.front() == '-') return false;
	for (uint i = 0; i < A.size(); i++)
	{
		if (A[i] > B[i]) return true;
		if (A[i] < B[i]) return false;
	}
	return false;
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& vect)
{
	return stream << vect.x << ":" << vect.y;
}

engine::engine()
{
	srand(static_cast<uint>(time(NULL)));
	mainClock.restart();

	if (!mainFont.loadFromFile("resources/fonts/DPSDbeyond.otf"))
	{
		std::cerr << "Cannot load font. Program will exit. " << std::endl; return;
	}

	block::loadTextures();
	window.create(sf::VideoMode::getDesktopMode(), "0x44652", sf::Style::Fullscreen);
	window.setFramerateLimit(120);
	window.setVerticalSyncEnabled(true);

	try {
		game_ptr = std::make_unique<game>(mainFont, window, state);
		menu_ptr = std::make_unique<menu>(mainFont, window, state);
		success_ptr = std::make_unique<gameSuccess>(mainFont, window, state);
		defeat_ptr = std::make_unique<gameDefeat>(mainFont, window, state);
		level_menu_ptr = std::make_unique<chooseLevelMenu>(mainFont, window, state, levelToLoad, currentLevelNumber);
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; window.close(); }
	state = appState::menu;
}

engine::~engine()
{
}

void engine::run()
{
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		gameResult result;
		switch (state)
		{
		case appState::menu:
			menu_ptr->run();
			break;

		case appState::levelMenu:
			level_menu_ptr->run();
			if (state == appState::game)
			{
				game_ptr->setLevel(levelToLoad);
			}
			break;

		case appState::game:
			result = game_ptr->run();
			switch (result.mode)
			{
			case gameMode::success:
				success_ptr->setInfo(result);
				success_ptr->run();
				
				if (BTimeBetter(level_menu_ptr->getText((currentLevelNumber + 5)).getString(),
					timeToStr(result.elapsedTime)))
				{
					level_menu_ptr->getText(currentLevelNumber + 5).setString(timeToStr(result.elapsedTime));
				}

				game_ptr = std::make_unique<game>(mainFont, window, state);
				break;

			case gameMode::defeat:
				defeat_ptr->setInfo(result);
				defeat_ptr->run();
				game_ptr = std::make_unique<game>(mainFont, window, state);
				break;

			case gameMode::pause:
				game_ptr = std::make_unique<game>(mainFont, window, state);
				break;
			}
			break;

		case appState::exit:
			window.close();
			break;
		}
	}
	//std::cin.ignore(2);
	//exit(-1); //////////////////////////!!!!!!
}