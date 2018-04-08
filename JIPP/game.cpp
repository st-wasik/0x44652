#include "game.h"

template <typename T>
T abs(const sf::Vector2<T>& vector)
{
	return{ abs(vector.x), abs(vector.y) };
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& vect)
{
	return stream << vect.x << ":" << vect.y;
}

////////////////////////////////////

game::game(sf::Font& f, sf::RenderWindow & wn, appState & st) : window(wn), state(st), timeToClose(7), introVectorDistance(0),
introInProgress(1), mainFont(f)
{
	gameView.setSize(static_cast<sf::Vector2f>(window.getSize()));
	gameView.setCenter(static_cast<sf::Vector2f>(window.getSize() / 2u));
	window.setView(gameView);
	try {
		player_ptr = std::make_unique<player>();
		//level_ptr = std::make_unique<level>(window, mainFont);
	}
	catch (std::exception& e) { std::clog << e.what() << std::endl; wn.close(); }
	if (!successTheme.openFromFile("resources/sounds/x04.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!defeatTheme.openFromFile("resources/sounds/x06.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!startSignal.openFromFile("resources/sounds/start.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!musicArray[0].openFromFile("resources/sounds/x03ces.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!musicArray[1].openFromFile("resources/sounds/x10.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!musicArray[2].openFromFile("resources/sounds/x07.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!musicArray[3].openFromFile("resources/sounds/x05.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!musicArray[4].openFromFile("resources/sounds/x12.ogg")) { std::cerr << "Cannot load music." << std::endl; }
	if (!musicArray[5].openFromFile("resources/sounds/x13.ogg")) { std::cerr << "Cannot load music." << std::endl; }
}

game::~game()
{
}

gameResult game::run()
{
	window.setMouseCursorVisible(0);
	gameClock.restart();

	currentMusic = &musicArray[random(0, musicArray.size() - 1)];
	currentMusic->play();


	while (window.isOpen() && state == appState::game)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				currentMusic->stop();
				state = appState::menu;
				return gameResult{ gameMode::pause, 0, level_ptr->levelName };
			}
		}

		if (!(currentMusic->getStatus() == sf::SoundSource::Status::Playing))
		{
			if(!(successTheme.getStatus() == sf::SoundSource::Status::Playing)
				&& !(defeatTheme.getStatus() == sf::SoundSource::Status::Playing)
				&& !(startSignal.getStatus() == sf::SoundSource::Status::Playing))
			{currentMusic = &musicArray[random(0, musicArray.size() - 1)]; currentMusic->play(); }
		}

		introInProgressOld = introInProgress;
		if (introInProgress)
		{
			introInProgress = levelIntrodution(deltaTime);
		}

		if (!introInProgress && introInProgressOld) { gameClock.restart(); startSignal.setVolume(150); startSignal.play(); }

		if (!introInProgress)
		{
			/*check_player_collision_with_blocks*/
			if (!player_ptr->outOfLevel) {
				int	Y = int((player_ptr->getPosition().y + player_ptr->getRadius()) / block::blockSize.y),
					X = int(player_ptr->getPosition().x - int((Y + 1)*block::blockSize.x / 2)) / block::blockSize.x;

				int arrX = level_ptr->levelArray.size(),
					arrY = level_ptr->levelArray[0].size();

				bool FELL = true;
				for (int i = ((X > 1) ? X - 2 : 0); i < ((X + 1 < arrX) ? X + 2 : arrX); i++)
				{
					for (int j = ((Y > 1) ? Y - 2 : 0); j < ((Y + 1 < arrY) ? Y + 2 : arrY); j++)
					{
						if (level_ptr->levelArray[i][j])
						{
							if (isCollision(level_ptr->levelArray[i][j]->top, { player_ptr->getPosition().x, player_ptr->getPosition().y + player_ptr->getRadius() }))
							{
								FELL = false;
								player_ptr->positionIndex = { static_cast<int>(i),static_cast<int>(j) };
							}
						}
					}
				}
				player_ptr->outOfLevelOld = player_ptr->outOfLevel;
				player_ptr->outOfLevel = FELL;

				if (!player_ptr->outOfLevelOld && player_ptr->outOfLevel)
				{
					outOfLevelClock.restart();
					if (!(successTheme.getStatus() == sf::SoundSource::Status::Playing))
					{
						currentMusic->stop();
						defeatTheme.play();
					}
				}
			}
		}
		/*END_check_player_collision_with_blocks*/

		/*updates*/
		player_ptr->update(deltaTime);
		if (!introInProgress) 
		{
			player_ptr->move_update(deltaTime);
			gameView.setCenter(player_ptr->getPosition());
		}
		window.setView(gameView);

		move(level_ptr->getGlobalParticlesVector(), player_ptr->getParticlesVector());

		level_ptr->update(deltaTime, window);
		/*END_updates*/

		/*outOfLevel_events*/
		if (player_ptr->outOfLevel)
		{
			if (level_ptr->levelArray[player_ptr->positionIndex.x][player_ptr->positionIndex.y])
			{
				switch (whereLostCollision(level_ptr->levelArray[player_ptr->positionIndex.x][player_ptr->positionIndex.y]->top, { player_ptr->getPosition().x, player_ptr->getPosition().y + player_ptr->getRadius() }))
				{
				case where::Up: player_ptr->positionIndex = { player_ptr->positionIndex.x, player_ptr->positionIndex.y - 1 }; break;
				case where::Down: player_ptr->positionIndex = { player_ptr->positionIndex.x, player_ptr->positionIndex.y + 1 }; break;
				case where::Left: player_ptr->positionIndex = { player_ptr->positionIndex.x - 1, player_ptr->positionIndex.y }; break;
				case where::Right: player_ptr->positionIndex = { player_ptr->positionIndex.x + 1, player_ptr->positionIndex.y }; break;
				}
			}

			if (player_ptr->positionIndex.x < 0) player_ptr->positionIndex.x = 0;
			if (player_ptr->positionIndex.y < 0) player_ptr->positionIndex.y = 0;
			if (player_ptr->positionIndex.y >= level_ptr->levelSize.x) player_ptr->positionIndex.x = level_ptr->levelSize.x - 1;
			if (player_ptr->positionIndex.y >= level_ptr->levelSize.y) player_ptr->positionIndex.y = level_ptr->levelSize.y - 1;
		}
		/*END_outOfLevel_events*/

		/*end_block_reached*/
		if(player_ptr->positionIndex == static_cast<sf::Vector2i>( level_ptr->endBlockCoords))
		//if (isCollision(level_ptr->levelArray[level_ptr->endBlockCoords.x][level_ptr->endBlockCoords.y]->top, player_ptr->getPosition()) && !player_ptr->outOfLevel)
		{
			level_ptr->levelArray[level_ptr->endBlockCoords.x][level_ptr->endBlockCoords.y]->top.setFillColor(sf::Color::Yellow);
			level_ptr->endEmitter.setParticlesCreator([](sf::RectangleShape& part) {part.setSize(sf::Vector2f{ 5,5 }); int rand_int = random(192, 255); part.setFillColor(sf::Color(rand_int, rand_int, 0)); part.setOrigin(part.getSize().x / 2, part.getSize().y / 2); });
			level_ptr->end = true;
			if (!level_ptr->endOld)
			{
				if (!(defeatTheme.getStatus() == sf::SoundSource::Status::Playing))
				{
					currentMusic->stop();
					successTheme.play();
				}
				endClock.restart();
				level_ptr->levelEndTime = gameClock.getElapsedTime().asSeconds();
			}
		}
		/*END_end_block_reached*/

		/*end_clocks*/
		if (player_ptr->outOfLevel && !player_ptr->outOfLevelOld)
		{
			outOfLevelClock.restart();
		}
		if (!level_ptr->endOld && level_ptr->end)
		{
			endClock.restart();
		}

		if (player_ptr->outOfLevel && outOfLevelClock.getElapsedTime().asSeconds() > timeToClose)
		{
			level_ptr->closeLevel = true;
		}
		if (level_ptr->end && endClock.getElapsedTime().asSeconds() > timeToClose)
		{
			level_ptr->closeLevel = true;
		}
		/*END_end_clocks*/


		window.clear(sf::Color(60, 50, 60));
		/*draw*/
		if (player_ptr->outOfLevel)
		{
			auto player_index = sf::Vector2u{ static_cast<unsigned int>(player_ptr->positionIndex.x) ,static_cast<unsigned int>(player_ptr->positionIndex.y) };
			level_ptr->background.setPosition(window.getView().getCenter());

			window.draw(level_ptr->background);

			for (auto& x : level_ptr->particlesEmitterVector)
				x.draw(window);

			for (int i = 0; i < level_ptr->levelSize.y; i++)
			{
				for (int j = level_ptr->levelSize.x - 1; j > -1; j--)
				{
					if (level_ptr->levelArray[j][i])
					{
						level_ptr->levelArray[j][i]->draw(window);
					}
					if (j == player_ptr->positionIndex.x && i == player_ptr->positionIndex.y)
					{
						for (auto& x : level_ptr->globalParticlesVector)
							x.draw(window);
						player_ptr->draw(window);
					}
				}
			}

			level_ptr->endEmitter.draw(window);

		}
		else
		{
			level_ptr->draw(window);
			player_ptr->draw(window);
			level_ptr->endEmitter.draw(window);
		}

		if (introInProgress)
		{
			level_ptr->levelNameText.setPosition({ gameView.getCenter().x , gameView.getCenter().y - window.getSize().y /4 });
			window.draw(level_ptr->levelNameText);
		}
		else 
		{
			if (level_ptr->timeText.getFillColor().a > 0)
			{
				level_ptr->levelNameText.setPosition({ gameView.getCenter().x , gameView.getCenter().y - window.getSize().y / 4 });
				auto color = level_ptr->levelNameText.getFillColor();
				static int colorStep = 7;
				if (color.a >= colorStep)
					color.a -= colorStep;
				else color.a = 0;
				level_ptr->levelNameText.setFillColor(color);
				window.draw(level_ptr->levelNameText);
			}

			if (!level_ptr->end)
			{
				level_ptr->timeText.setString("Time: " + timeToStr(gameClock.getElapsedTime().asSeconds()));
			}
			else
			{
				level_ptr->timeText.setString("Time: " + timeToStr(level_ptr->levelEndTime));
			}

			level_ptr->timeText.setPosition({ gameView.getCenter().x - window.getSize().x / 2 + 20 , gameView.getCenter().y - window.getSize().y / 2 + 20 });
			window.draw(level_ptr->timeText);
		}
		/*END_draw*/

		window.display();
		if (level_ptr->closeLevel)
		{
			if (level_ptr->end)
			{
				currentMusic->stop();
				return gameResult{ gameMode::success, level_ptr->levelEndTime, level_ptr->levelName };
			}

			if (player_ptr->outOfLevel)
			{
				currentMusic->stop();
				return gameResult{ gameMode::defeat, 0 , level_ptr->levelName };
			}

		}
		deltaTime = deltaClock.getElapsedTime().asMilliseconds();
		deltaClock.restart();
	}
	currentMusic->stop();
	return gameResult{ gameMode::pause, 0 , level_ptr->levelName };
}

void game::setLevel(std::string s)
{
	try
	{
		level_ptr = std::make_unique<level>(window, mainFont);
		level_ptr->loadFromFile(s);
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; window.close(); }
	player_ptr->setPosition(level_ptr->getBeginBlockPosition());
	gameView.setCenter(level_ptr->getEndBlockPosition());

	introVector = sf::Vector2f{ level_ptr->getEndBlockPosition() - level_ptr->getBeginBlockPosition() };
	introVectorLength = (sqrt(static_cast<long double>(pow(introVector.x, 2) + pow(introVector.y, 2))));
	introVector /= introVectorLength;
}

bool game::isCollision(const sf::ConvexShape & shape, const sf::Vector2f& point) const
{
	sf::Vector2f position = shape.getPosition();

	sf::Vector2f A = shape.getPoint(0) + position,
		B = shape.getPoint(1) + position,
		C = shape.getPoint(2) + position,
		D = shape.getPoint(3) + position;

	return (point.y <= C.y) && (point.y >= A.y) &&
		((((A.y - D.y) / (A.x - D.x)) * (point.x - D.x) + D.y) >= point.y) &&
		((((B.y - C.y) / (B.x - C.x)) * (point.x - C.x) + C.y) <= point.y);
}

where game::whereLostCollision(const sf::ConvexShape & shape, const sf::Vector2f & point) const
{
	sf::Vector2f position = shape.getPosition();

	sf::Vector2f A = shape.getPoint(0) + position,
		B = shape.getPoint(1) + position,
		C = shape.getPoint(2) + position,
		D = shape.getPoint(3) + position;
	if (point.y > C.y) return where::Down;
	if ((((B.y - C.y) / (B.x - C.x)) * (point.x - C.x) + C.y) > point.y) return where::Right;
	if (point.y < A.y) return where::Up;
	if ((((A.y - D.y) / (A.x - D.x)) * (point.x - D.x) + D.y) < point.y) return where::Left;
}

bool game::levelIntrodution(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { gameView.setCenter(player_ptr->getPosition()); return false; }
	if (gameClock.getElapsedTime().asSeconds() < 1.0f) return true;
	auto move = introVector*deltaTime*0.5f; //*prec
	introVectorDistance += (sqrt(pow(move.x, 2) + pow(move.y, 2)));
	if (introVectorDistance > introVectorLength) { gameView.setCenter(player_ptr->getPosition()); return false; }
	gameView.move(-move);
	return true;
}

std::string timeToStr(int time)
{
	std::string result;
	if (time / 60 < 10) result += std::to_string(0);
	result += std::to_string(time / 60) + "m"; time %= 60;
	if (time >= 10) { result += std::to_string(time) + "s"; }
	else if (time > 0) { result += "0" + std::to_string(time) + "s"; }
	else if (time == 0) { result += "00s"; }
	return result;
}
