#include "menu_template.h"

menu_template::menu_template(const sf::Font &f, sf::RenderWindow & window, appState & st) : window(window), state(st), emitterDensity(20),
particlesVector(window.getSize().x / emitterDensity + 1), mouseLeftPressed(0), mouseLeftPressedOld(0), mouseLeftReleased(0)
{
	menuView.setSize(static_cast<sf::Vector2f>(window.getSize()));
	menuView.setCenter(static_cast<sf::Vector2f>(window.getSize() / 2u));
	//menu_emitter
	int j = 0;
	for (auto& emitter : particlesVector)
	{
		emitter.setLifeTimeInterval(8000, 12000);
		emitter.setRunSpeedInterval(1, 5);
		emitter.setInitialRotationInterval(0, 355);
		emitter.setRunAngleInterval(170, 190);
		emitter.setRotateSpeedInterval(1, 3);
		emitter.setCreatingTimePeriod(random(500, 6000));
		//emitter.setCreatingTimePeriod(random(50, 600));
		emitter.setParticlesCreator([](sf::RectangleShape& part) {part.setSize(sf::Vector2f{ 3,3 }); int rand_int = random(192, 255); sf::Color randColor = { (sf::Uint8)random(230,255), (sf::Uint8)random(230,240), (sf::Uint8)random(0,0) }; part.setFillColor(randColor); part.setOrigin(part.getSize().x / 2, part.getSize().y / 2); });

		emitter.setPosition({ static_cast<float>(j),-20 });
		j += emitterDensity;
	}
	//END_menu_emitter

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
}

menu_template::~menu_template()
{
}

void menu_template::update(float deltaTime) //v
{
	//deltaTime
	deltaTime = deltaClock.getElapsedTime().asMilliseconds();
	deltaClock.restart();

	//MouseButtonEvent
	mouseLeftPressedOld = mouseLeftPressed;
	mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	mouseLeftReleased = (mouseLeftPressedOld && !mouseLeftPressed);
	mouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

	//Text_&_Buttons_Color
	for (auto& t : textContainer)
	{
		t.setFillColor(sf::Color(127, 127, 127));
	}
	textContainer.front().setFillColor(sf::Color::Yellow);

	//emitter_update
	for (auto& emmiter : particlesVector)
	{
		emmiter.update(deltaTime);
	}
}

void menu_template::draw(sf::RenderWindow & window) //v
{
	window.draw(background[1]);
	window.draw(background[0]);

	for (auto& emitter : particlesVector)
	{
		emitter.draw(window);
	}

	for (auto& obj : textContainer)
	{
		window.draw(obj);
	}
}
