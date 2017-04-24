#include "../Sources/Core/Application.hpp"

#include "GameSingleton.hpp"
#include "GameConfig.hpp"

#include "GameState.hpp"
#include "IntroState.hpp"
#include "MenuState.hpp"

int main()
{
	oe::Application application;

	// Load Resources
	GameSingleton::win = false;
	GameSingleton::loadTileset();
	GameSingleton::antTexture = application.getTextures().create("ants", "Assets/pions.png");
	GameSingleton::objectsTexture = application.getTextures().create("objects", "Assets/objects.png");
	GameSingleton::sansationFont = application.getFonts().create("sansation", "Assets/sansation.ttf");
	GameSingleton::movementSound = application.getAudio().createSound("movement", "Assets/movement.wav");
	GameSingleton::actionSound = application.getAudio().createSound("action", "Assets/action.wav");
	GameSingleton::attackSound = application.getAudio().createSound("attack", "Assets/attack.wav");

	// Load Window
	oe::Window& window = application.getWindow();
	window.create(sf::VideoMode(WINSIZEX, WINSIZEY), WINTITLE, sf::Style::Close);
	window.setMainView(sf::View(sf::FloatRect(0.0f, 0.0f, WINSIZEX, WINSIZEY)));
	window.applyMainView();

	// Load State
	application.pushState<IntroState>();
	//application.pushState<GameState>();
	application.run();

	getchar();
	return 0;
}
