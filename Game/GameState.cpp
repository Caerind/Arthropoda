#include "GameState.hpp"

#include "GameSingleton.hpp" // Used to store the map
#include "GameConfig.hpp" // Used to move the view

#include "Soldier.hpp"

GameState::GameState(oe::StateManager& manager)
	: oe::State(manager)
	, mWorld(manager.getApplication())
{
	// Load resources
	GameSingleton::soldierR = mWorld.getTextures().create("soldier", "../Assets/soldier.png");

	// Init map first because needed by other entities
    GameSingleton::map = mWorld.createEntity<GameMap>();
	GameSingleton::initCollisions(MAPSIZEX, MAPSIZEY);

	// Init entities
	GameSingleton::soldier = mWorld.createEntity<Soldier>();
	GameSingleton::getSoldier().setCoords(2, 2);
}

bool GameState::handleEvent(const sf::Event& event)
{
	mWorld.handleEvent(event);

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		oe::Vector2 pos = getApplication().getWindow().getCursorPositionView(mWorld.getRenderSystem().getView());
		oe::Vector2i coords = GameSingleton::getMap().worldToCoords(pos);
		GameSingleton::getSoldier().goTo(coords);
	}

	//zoomView(event);

	return false;
}

bool GameState::update(oe::Time dt)
{
	mWorld.update(dt);

	moveView(dt);

	GameSingleton::getSoldier().update(dt);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		oe::Vector2i coords = GameSingleton::getMap().worldToCoords(getApplication().getWindow().getCursorPositionView(mWorld.getRenderSystem().getView()));
		GameSingleton::getMap().setTileId(coords, TID_CITY);
		GameSingleton::setCollision(coords, true); // Add collisions
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		oe::Vector2i coords = GameSingleton::getMap().worldToCoords(getApplication().getWindow().getCursorPositionView(mWorld.getRenderSystem().getView()));
		std::vector<oe::Vector2i> r;
		Distance::run(r, coords, 3, GameSingleton::collisions);
		printf("size : %d\n", r.size());
		for (const oe::Vector2i& t : r)
		{
			GameSingleton::getMap().setTileOverlayId(t, TID_WATER);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		GameSingleton::getMap().cleanOverlay();
	}

	getWindow().setTitle("LD38 - FPS : " + oe::toString(getApplication().getFPSCount()));

	return false;
}

void GameState::render(sf::RenderTarget& target)
{
	mWorld.render(target);
}

oe::Window& GameState::getWindow()
{
	return getApplication().getWindow();
}

oe::View& GameState::getView()
{
	return mWorld.getRenderSystem().getView();
}

void GameState::zoomView(const sf::Event& event)
{
	if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
	{
		oe::Vector2 wSize = getWindow().getSize();
		oe::View& view = getView();
		if (event.mouseWheelScroll.delta < 1)
		{
			if (view.getSize().x < 2 * WINSIZEX)
			{
				view.zoom(1.2f);
			}
		}
		else
		{
			if (view.getSize().x > 0.5f * WINSIZEX)
			{
				view.zoom(0.8f);
			}
		}
	}
}

void GameState::moveView(oe::Time dt)
{
	oe::Vector2 mvt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		mvt.y -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		mvt.y += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		mvt.x -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		mvt.x += 1.0f;
	}
	getView().move(mvt * dt.asSeconds() * VIEWSPEED);
}
