#include "level.h"
#include "iostream"
#include "fstream"
#include "exception"

using uint = unsigned int;

template <typename type>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2<type> vect)
{
	return stream << vect.x << ":" << vect.y;
}

level::level(sf::RenderWindow& window, sf::Font& f) : emitterDensity(20), background(), end(false), endOld(false), closeLevel(false)
{
	particlesEmitterVector.resize(window.getSize().x / emitterDensity + 1);

	if (!backgroundTexture[0].loadFromFile("resources/textures/background1.png")) { throw std::exception("Cannot load texture. Program will exit."); }
	if (!backgroundTexture[1].loadFromFile("resources/textures/background2.png")) { throw std::exception("Cannot load texture. Program will exit."); }
	if (!backgroundTexture[2].loadFromFile("resources/textures/background3.png")) { throw std::exception("Cannot load texture. Program will exit."); }
	if (!backgroundTexture[3].loadFromFile("resources/textures/background4.png")) { throw std::exception("Cannot load texture. Program will exit."); }
	if (!backgroundTexture[4].loadFromFile("resources/textures/background5.png")) { throw std::exception("Cannot load texture. Program will exit."); }

	for (auto& texture : backgroundTexture)
		texture.setRepeated(1);

	background.setSize(static_cast<sf::Vector2f>(window.getSize()));
	background.setTexture(&backgroundTexture[random(0,backgroundTexture.size()-1)]);
	background.setTextureRect({ 0, 0,static_cast<int>(background.getSize().x), static_cast<int>(background.getSize().y) });
	background.setOrigin(background.getSize().x / 2, background.getSize().y / 2);
	background.setPosition(window.getView().getCenter());
	levelClock.restart();

	int j = 0;
	for (auto& emitter : particlesEmitterVector)
	{
		emitter.setLifeTimeInterval(8000, 12000);
		emitter.setRunSpeedInterval(10, 25);
		emitter.setInitialRotationInterval(0, 355);
		emitter.setRunAngleInterval(170, 190);
		emitter.setRotateSpeedInterval(1, 3);
		emitter.setCreatingTimePeriod(static_cast<float>(random(650, 2000)));
		//emitter.setCreatingTimePeriod(0);
		emitter.setParticlesCreator([](sf::RectangleShape& part) {part.setSize(sf::Vector2f{ 3,3 }); int rand_int = random(224, 255); part.setFillColor(sf::Color(rand_int, rand_int, 255)); part.setOrigin(part.getSize().x / 2, part.getSize().y / 2); });

		emitter.setPosition({ static_cast<float>(j), -20 });
		j += emitterDensity;
	}

	endEmitter.setLifeTimeInterval(2000, 4000);
	endEmitter.setRunSpeedInterval(10, 15);
	endEmitter.setInitialRotationInterval(0, 355);
	endEmitter.setRunAngleInterval(-15, 15);
	endEmitter.setRotateSpeedInterval(1, 3);
	endEmitter.setCreatingTimePeriod(0.f);
	endEmitter.setParticlesCreator([](sf::RectangleShape& part) {part.setSize(sf::Vector2f{ 5,5 }); int rand_int = random(192, 255); part.setFillColor(sf::Color(rand_int, 0, 0)); part.setOrigin(part.getSize().x / 2, part.getSize().y / 2); });

	levelNameText.setFont(f);
	levelNameText.setCharacterSize(90);
	levelNameText.setFillColor(sf::Color::Yellow);

	timeText.setFont(f);
	timeText.setCharacterSize(36);
	timeText.setFillColor(sf::Color::Yellow);
	
}

level::~level()
{
}

void level::update(float deltaTime, sf::RenderWindow& window)
{
	endOld = end;
	int j = 0;
	this->background.setPosition(window.getView().getCenter());
	auto backgroundPosition = background.getPosition() - (background.getSize() / 2.f);


	float emitterPositionY = backgroundPosition.y - 20;
	for (auto& emitter : particlesEmitterVector)
	{
		emitter.setPosition({ static_cast<float>(backgroundPosition.x + j), emitterPositionY });
		j += emitterDensity;
		emitter.update(deltaTime);
	}

	for (auto& p : globalParticlesVector)
	{
		p.update(deltaTime);
	}

	endEmitter.update(deltaTime);
}

void level::draw(sf::RenderWindow & window)
{
	window.draw(background);
	for (auto& emitter : particlesEmitterVector)
	{
		emitter.draw(window);
	}
	//static int blockSizeX = block::blockSize.x*3/2;
	static int blockSizeY = block::blockSize.y ;
	int
		Xbegin = 0,
		Xend = levelSize.x,
		Ybegin = (window.getView().getCenter().y - window.getView().getSize().y) / blockSizeY -1,
		Yend = (window.getView().getCenter().y + window.getView().getSize().y) / blockSizeY + 1;
	if (Xbegin < 0) Xbegin = 0;
	if (Xend > levelSize.x) Xend = levelSize.x;
	if (Ybegin < 0) Ybegin = 0;
	if (Yend > levelSize.y) Yend = levelSize.y;
	//std::clog << "X:" << Xbegin << "," << Xend << "   Y:" << Ybegin << "," << Yend << std::endl;

	for (int i = Ybegin; i < Yend; i++)
	{
		for (int j = Xend - 1; j > Xbegin-1; j--)
		{
			if (levelArray[j][i])
			{
				levelArray[j][i]->draw(window);
			}
		}
	}

	for (auto& p : globalParticlesVector)
	{
		p.draw(window);
	}

	endEmitter.draw(window);
}

void level::draw(sf::RenderWindow & window, sf::Vector2u begin, sf::Vector2u end)
{
	if (begin.x < 0) begin.x = 0;
	else if (end.x >= levelArray.size()) end.x = levelArray.size() - 1;

	if (begin.y < 0) begin.y = 0;
	else if (end.y >= levelArray[0].size()) end.y = levelArray[0].size() - 1;

	for (uint i = begin.y; i < end.y; i++)
	{
		for (uint j = end.x; j > begin.x - 1; j--)
		{
			if (levelArray[j][i])
			{
				levelArray[j][i]->draw(window);
			}
		}
	}

	if (end == sf::Vector2u{ static_cast<unsigned int>(levelArray.size()), static_cast<unsigned int>(levelArray[0].size()) })
	{
		endEmitter.draw(window);
	}
}

void level::loadFromFile(std::string filename)
{
	std::fstream loadLevelStream;
	filename = "resources/levels/" + filename;
	loadLevelStream.open(filename.c_str(), std::ios::in);

	if (!loadLevelStream) { throw std::exception(("Cannot open level file: " + filename + ". Program will exit.").c_str()); }
	if (!loadLevelStream.eof())
	{
		std::getline(loadLevelStream, levelName);
		loadLevelStream >> levelSize.x >> levelSize.y;
		loadLevelStream >> beginBlockCoords.x >> beginBlockCoords.y;
		loadLevelStream >> endBlockCoords.x >> endBlockCoords.y;

		levelArray.resize(levelSize.x);

		for (auto& cell : levelArray)
		{
			cell.resize(levelSize.y);
		}

		unsigned int temp;
		for (uint i = 0; i < levelSize.y; i++)
		{
			for (uint j = 0; j < levelSize.x; j++)
			{
				loadLevelStream >> temp;
				if (temp)
				{
					levelArray[j][i] = std::make_unique<block>();
					if (temp == 1)
					{
						int r = random(192, 224), r2 = random(192, 224), r3 = random(192, 224), r4 = random(0, 4), r5 = random(0, 9);

						if (!r4)
							levelArray[j][i]->top.setFillColor(sf::Color(0xff, 0xff, r3));
						else if (!r5)
							levelArray[j][i]->top.setFillColor(sf::Color(0xff, r2, r2));
						else levelArray[j][i]->top.setFillColor(sf::Color(r, r, r));
					}
					else if (temp == 2)
					{
							levelArray[j][i]->top.setFillColor(sf::Color(0xff, 0x7f,0));
					}
					else if (temp == 3)
					{
						levelArray[j][i]->top.setFillColor(sf::Color(0xff, 0x2f, 0x4A));
					}
					//set block;
				}
			}
		}
	}

	levelNameText.setCharacterSize(90);
	levelNameText.setFillColor(sf::Color::Yellow);
	levelNameText.setString(levelName);
	levelNameText.setOrigin({ levelNameText.getGlobalBounds().width / 2, levelNameText.getGlobalBounds().height / 2 });

	for (uint i = 0; i < levelSize.x; i++)
	{
		for (uint j = 0; j < levelSize.y; j++)
		{
			if (levelArray[i][j])
			{
				levelArray[i][j]->setPosition(sf::Vector2f{ block::blockSize.x*i,block::blockSize.y*j });
				levelArray[i][j]->move(sf::Vector2f{ block::blockSize.x*j / 2, 0 });
			}
		}

	}

	endEmitter.setPosition(levelArray[endBlockCoords.x][endBlockCoords.y]->getCenter());
	levelArray[beginBlockCoords.x][beginBlockCoords.y]->top.setFillColor(sf::Color::Yellow);
	levelArray[endBlockCoords.x][endBlockCoords.y]->top.setFillColor(sf::Color::Red);
}

std::vector<particlesEmitter::particle>& level::getGlobalParticlesVector()
{
	return globalParticlesVector;
}

sf::Vector2f level::getBeginBlockPosition()
{
	return levelArray[beginBlockCoords.x][beginBlockCoords.y]->getCenter();
}

sf::Vector2f level::getEndBlockPosition()
{
	return levelArray[endBlockCoords.x][endBlockCoords.y]->getCenter();
}
