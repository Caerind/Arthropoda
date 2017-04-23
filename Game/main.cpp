#include "../Sources/Core/Application.hpp"

#include "GameSingleton.hpp"
#include "GameConfig.hpp"

#include "GameState.hpp"
#include "IntroState.hpp"

int main()
{
	oe::Application application;

	// Load Resources
	GameSingleton::loadTileset();
	GameSingleton::sansationFont = application.getFonts().create("sansation", "../Assets/sansation.ttf");

	// Load Window
	oe::Window& window = application.getWindow();
	window.create(sf::VideoMode(WINSIZEX, WINSIZEY), WINTITLE);
	window.setMainView(sf::View(sf::FloatRect(0.0f, 0.0f, WINSIZEX, WINSIZEY)));
	window.applyMainView();

	// Load State
	//application.pushState<IntroState>();
	application.pushState<GameState>();
	application.run();

	getchar();
	return 0;
}