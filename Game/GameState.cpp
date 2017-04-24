#include "GameState.hpp"

#include "GameSingleton.hpp" // Used to store the map
#include "GameConfig.hpp"
#include "PostState.hpp" // Used to switch to

GameState::GameState(oe::StateManager& manager)
	: oe::State(manager)
	, mWorld(manager.getApplication())
{
	GameSingleton::clear();

	mTurnNumber = 0;
	mWorld.getRenderSystem().setBackgroundColor(oe::Color::DarkGray);

	// Load resources
	mGameMaskTexture.loadFromFile("Assets/gamemask.png");
	mGameHudTexture.loadFromFile("Assets/gamehud.png");

	// HUD
	mGameMask.setTexture(mGameMaskTexture);
	mButton1.setTexture(mGameHudTexture);
	mButton1.setTextureRect(sf::IntRect(0, 0, 75, 60));
	mButton1.setPosition(0.f, WINSIZEY - 60.f);
	mButton2.setTexture(mGameHudTexture);
	mButton2.setTextureRect(sf::IntRect(75, 0, 75, 60));
	mButton2.setPosition(75.f, WINSIZEY - 60.f);
	mButton3.setTexture(mGameHudTexture);
	mButton3.setTextureRect(sf::IntRect(150, 0, 75, 60));
	mButton3.setPosition(150.f, WINSIZEY - 60.f);
	mButtonNext.setTexture(mGameHudTexture);
	mButtonNext.setTextureRect(sf::IntRect(285, 0, 60, 60));
	mButtonNext.setPosition(WINSIZEX - 120.f, WINSIZEY - 60.f);
	mButtonTurn.setTexture(mGameHudTexture);
	mButtonTurn.setTextureRect(sf::IntRect(225, 0, 60, 60));
	mButtonTurn.setPosition(WINSIZEX - 60.f, WINSIZEY - 60.f);

	// Init map first because needed by other entities
    GameSingleton::map = mWorld.createEntity<GameMap>().getAs<GameMap>();
	GameSingleton::initCollisions(MAPSIZEX, MAPSIZEY);

	// Init player 1 : Player
	mPlayer1Anthill.set(6, 6);
	GameSingleton::anthill = mWorld.createEntity<Anthill>();
	Anthill* anthill = GameSingleton::anthill.getAs<Anthill>();
	anthill->setCoords(mPlayer1Anthill);
	anthill->setPlayer(1);
	GameSingleton::setCollision(mPlayer1Anthill, true);

	// Init player 2 : AI
	mPlayer2Anthill.set(23, 23);
	GameSingleton::aiAnthill = mWorld.createEntity<Anthill>();
	anthill = GameSingleton::aiAnthill.getAs<Anthill>();
	anthill->setCoords(mPlayer2Anthill);
	anthill->setPlayer(2);
	GameSingleton::setCollision(mPlayer2Anthill, true);

	// Init map & AI
	initMap();
	mAi.init();

	// Start the game
	mCurrentPlayer = 2;
	mSelectedAnt = nullptr;
	passTurn(); // Pass to player 1 and do the announce it
}

bool GameState::handleEvent(const sf::Event& event)
{
	moveView(event);

	zoomView(event);

	if (mCurrentPlayer == 1)
	{
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			bool used = false;

			used = useButtons(toSF(getApplication().getWindow().getCursorPosition()));
			
			if (!used && mTurnReady)
			{
				selectAnt();
			}
		}
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && mSelectedAnt != nullptr && mSelectedAnt->canPlay() && mTurnReady)
		{
			oe::Vector2i mc(getMouseCoords());
			oe::EntityHandle enemyAnt = GameSingleton::getAIAntHandle(mc);
			if (mc == mPlayer1Anthill)
			{
				mSelectedAnt->goToAnthill();
			}
			else if (mc == mPlayer2Anthill)
			{
				mSelectedAnt->goToTarget(GameSingleton::aiAnthill);
			}
			else if (enemyAnt.isValid())
			{
				mSelectedAnt->goToTarget(enemyAnt);
			}
			else
			{
				mSelectedAnt->goTo(mc);
			}
		}
	}
	return false;
}

bool GameState::update(oe::Time dt)
{
	mWorld.update(dt);

	GameSingleton::update();

	moveView(dt);

	if (GameSingleton::getAnthill().getLife() == 0)
	{
		GameSingleton::win = false;
		popState();
		pushState<PostState>();
	}
	if (GameSingleton::getAIAnthill().getLife() == 0)
	{
		GameSingleton::win = true;
		popState();
		pushState<PostState>();
	}

	if (!GameSingleton::map->isOverlayValid())
	{
		GameSingleton::map->cleanOverlay();
	}

	Anthill& anthill = GameSingleton::getAnthill();

	if (mCurrentPlayer == 1)
	{
		GameSingleton::map->setCursorCoords(getMouseCoords(), mCurrentPlayer);
		mButton1.setTextureRect(sf::IntRect(0, 60 * ((anthill.canSpawn(Ant::Scout)) ? 0 : 1), 75, 60));
		mButton2.setTextureRect(sf::IntRect(75, 60 * ((anthill.canSpawn(Ant::Worker)) ? 0 : 1), 75, 60));
		mButton3.setTextureRect(sf::IntRect(150, 60 * ((anthill.canSpawn(Ant::Soldier)) ? 0 : 1), 75, 60));

		if (mSelectedAnt == nullptr && !mTurnReady)
		{
			if (mAntUpdateIterator < GameSingleton::ants.size())
			{
				mSelectedAnt = GameSingleton::getAnt(mAntUpdateIterator);
				if (mSelectedAnt != nullptr && mSelectedAnt->getDestination() != Ant::invalidDest && mSelectedAnt->getDestination() != mSelectedAnt->getCoords())
				{
					GameSingleton::map->setCursorVisible(true);
					mSelectedAnt->select();
					mSelectedAnt->goToDestination();
				}
				else
				{
					mSelectedAnt = nullptr;
				}
				mAntUpdateIterator++;
			}
			else
			{
				mTurnReady = true;
				mAntUpdateIterator = 0;
			}
		}

		if (mSelectedAnt != nullptr)
		{
			if (mSelectedAnt->updateAnt(dt, true))
			{
				GameSingleton::map->setCursorVisible(false);
				mSelectedAnt->unselect();
				mSelectedAnt = nullptr;
			}
		} 
	}
	else if (mCurrentPlayer == 2)
	{
		mButton1.setTextureRect(sf::IntRect(0, 60, 75, 60));
		mButton2.setTextureRect(sf::IntRect(75, 60, 75, 60));
		mButton3.setTextureRect(sf::IntRect(150, 60, 75, 60));
		mAi.think(dt);
		if (mAi.isTurnOver())
		{
			passTurn();
		}
	}

	return false;
}

void GameState::render(sf::RenderTarget& target)
{
	mWorld.render(target);
	target.draw(mGameMask);
	target.draw(mButton1);
	target.draw(mButton2);
	target.draw(mButton3);
	target.draw(mButtonNext);
	target.draw(mButtonTurn);
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
			if (view.getZoom() < 3.0f)
			{
				view.zoom(2.0f);
			}
		}
		else
		{
			if (view.getZoom() > 1.5f)
			{
				view.zoom(0.5f);
			}
		}
	}
}

void GameState::moveView(oe::Time dt)
{
	oe::Vector2 mvt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		mvt.y -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		mvt.y += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		mvt.x -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		mvt.x += 1.0f;
	}
	getView().move(mvt * dt.asSeconds() * 500.f);

	oe::Vector2 v = getView().getCenter();
	if (v.x < 350.f)
	{
		v.x = 350.f;
	}
	if (v.y < 225.f)
	{
		v.y = 225.f;
	}
	if (v.x > 1500.f)
	{
		v.x = 1500.f;
	}
	if (v.y > 1400.f)
	{
		v.y = 1400.f;
	}
	getView().setCenter(v);
}

oe::Vector2i GameState::getMouseCoords()
{
	return GameSingleton::map->worldToCoords(getApplication().getWindow().getCursorPositionView(mWorld.getRenderSystem().getView()));
}

void GameState::moveView(const sf::Event& event)
{
	static bool mousePressedBool = false;
	static oe::Vector2 mousePressed;
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f m(toSF(getWindow().getCursorPosition()));
		if (!mButton1.getGlobalBounds().contains(m)
			&& !mButton2.getGlobalBounds().contains(m)
			&& !mButton3.getGlobalBounds().contains(m)
			&& !mButtonNext.getGlobalBounds().contains(m)
			&& !mButtonTurn.getGlobalBounds().contains(m))
		{
			mousePressedBool = true;
			mousePressed.set(getWindow().getCursorPositionView(getView()));
		}
	}
	if (event.type == sf::Event::MouseMoved && mousePressedBool)
	{
		getView().move(mousePressed - getWindow().getCursorPositionView(getView()));
	}
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		mousePressedBool = false;
	}
}

void GameState::initMap()
{
	U32 zone0r = 0;
	for (U32 zone = 0; zone < 4; zone++)
	{
		U32 nWater = (zone == 0 || zone == 3) ? oe::Random::get(0u, 1u) : oe::Random::get(1u, 2u);
		for (U32 i = 0; i < nWater; i++)
		{
			oe::Vector2i c(getCoordsZone(zone));
			std::vector<oe::Vector2i> n;
			oe::MapUtility::getNeighboors(n, c, oe::MapUtility::Hexagonal);
			n.push_back(c);
			for (const oe::Vector2i& w : n)
			{
				GameSingleton::map->setTileId(w, TILE_WATER);
				GameSingleton::setCollision(w, true);
			}
		}

		nWater = (zone == 0 || zone == 3) ? oe::Random::get(3u, 5u) : oe::Random::get(4u, 7u);
		for (U32 i = 0; i < nWater; i++)
		{
			oe::Vector2i c(getCoordsZone(zone));
			GameSingleton::map->setTileId(c, TILE_WATER);
			GameSingleton::setCollision(c, true);
		}

		U32 nRocks = (zone == 0 || zone == 3) ? oe::Random::get(3u, 5u) : oe::Random::get(4u, 7u);
		for (U32 i = 0; i < nRocks; i++)
		{
			oe::Vector2i c(getCoordsZone(zone));
			GameSingleton::map->setTileId(c, TILE_ROCK);
			GameSingleton::setCollision(c, true);
		}

		U32 nResources = oe::Random::get(3u, 5u);
		if (zone == 0)
		{
			zone0r = nResources;
		}
		if (zone == 3)
		{
			nResources = zone0r;
		}
		for (U32 i = 0; i < nResources; i++)
		{
			oe::Vector2i c(getCoordsZone(zone));
			if (!GameSingleton::isCollision(c))
			{
				bool testPassed = true;
				if (zone == 0 || zone == 3)
				{
					// Try adjacent anthill
					std::vector<oe::Vector2i> neighbors;
					oe::MapUtility::getNeighboors(neighbors, c, oe::MapUtility::Hexagonal);
					for (const oe::Vector2i& n : neighbors)
					{
						if (n == GameSingleton::getAnthill().getCoords() || n == GameSingleton::getAIAnthill().getCoords())
						{
							testPassed = false;
						}
					}
				}
				if (testPassed)
				{
					oe::EntityHandle r = mWorld.createEntity<Resource>();
					Resource* res = r.getAs<Resource>();
					if (res != nullptr)
					{
						res->setCoords(c);
						GameSingleton::resources.insert(r);
						mAi.addResource(c);
					}
				}
			}
		}
	}
}

oe::Vector2i GameState::getCoordsZone(U32 zone)
{
	oe::Vector2i c;
	switch (zone)
	{
		case 0: c.set(oe::Random::get(0, MAPSIZEX / 2), oe::Random::get(0, MAPSIZEY / 2)); break;
		case 1: c.set(oe::Random::get(MAPSIZEX / 2, MAPSIZEX - 1), oe::Random::get(0, MAPSIZEY / 2)); break;
		case 2: c.set(oe::Random::get(0, MAPSIZEX / 2), oe::Random::get(MAPSIZEY / 2, MAPSIZEY - 1)); break;
		case 3: c.set(oe::Random::get(MAPSIZEX / 2, MAPSIZEX - 1), oe::Random::get(MAPSIZEY / 2, MAPSIZEY - 1)); break;
		default: break;
	}
	return c;
}

bool GameState::useButtons(const sf::Vector2f& mouse)
{
	Anthill& anthill = GameSingleton::getAnthill();
	if (mButton1.getGlobalBounds().contains(mouse) && anthill.canSpawn(Ant::Scout))
	{
		getApplication().getAudio().playSound(GameSingleton::actionSound);
		anthill.spawn(Ant::Scout);
		return true;
	}
	if (mButton2.getGlobalBounds().contains(mouse) && anthill.canSpawn(Ant::Worker))
	{
		getApplication().getAudio().playSound(GameSingleton::actionSound);
		anthill.spawn(Ant::Worker);
		return true;
	}
	if (mButton3.getGlobalBounds().contains(mouse) && anthill.canSpawn(Ant::Soldier))
	{
		getApplication().getAudio().playSound(GameSingleton::actionSound);
		anthill.spawn(Ant::Soldier);
		return true;
	}
	if (mButtonNext.getGlobalBounds().contains(mouse))
	{
		getApplication().getAudio().playSound(GameSingleton::actionSound);
		switchToNextAnt();
		return true;
	}
	if (mButtonTurn.getGlobalBounds().contains(mouse))
	{
		getApplication().getAudio().playSound(GameSingleton::actionSound);
		passTurn();
		return true;
	}
	return false;
}

void GameState::selectAnt()
{
	if (mSelectedAnt != nullptr)
	{
		GameSingleton::map->setCursorVisible(false);
		mSelectedAnt->unselect();
	}
	mSelectedAnt = GameSingleton::getAnt(getMouseCoords());
	if (mSelectedAnt != nullptr)
	{
		GameSingleton::map->setCursorVisible(true);
		mSelectedAnt->select();
		if (!mSelectedAnt->canPlay())
		{
			GameSingleton::map->setCursorVisible(false);
			mSelectedAnt->unselect();
			mSelectedAnt = nullptr;
		}
	}
}

void GameState::passTurn()
{
	if (mSelectedAnt != nullptr)
	{
		GameSingleton::map->setCursorVisible(false);
		mSelectedAnt->unselect();
		mSelectedAnt = nullptr;
	}
	if (mCurrentPlayer == 1)
	{
		GameSingleton::map->setCursorRect(sf::IntRect(0,0,0,0)); // CURSOR 2
		for (const oe::EntityHandle& e : GameSingleton::ants)
		{
			ASSERT(e.isValid());
			Ant* ant = e.getAs<Ant>();
			if (ant != nullptr)
			{
				ant->reset();
			}
		}
		mCurrentPlayer = 2;
		mAi.startTurn();
	}
	else if (mCurrentPlayer == 2 || mCurrentPlayer == 0)
	{
		if (mCurrentPlayer == 2)
		{
			for (const oe::EntityHandle& e : GameSingleton::aiAnts)
			{
				ASSERT(e.isValid());
				Ant* ant = e.getAs<Ant>();
				if (ant != nullptr)
				{
					ant->reset();
					bool aa = true; // IF AI
					if (aa)
					{
						ant->resetDest();
						ant->invalidateTarget();
					}
				}
			}
		}
		mCurrentPlayer = 1;
		mTurnNumber++;
	}
	mButtonTurn.setTextureRect(sf::IntRect(225, (mCurrentPlayer == 1) ? 0 : 60, 60, 60));
	mTurnReady = false;
	mSelectedAnt = nullptr;
	mAntUpdateIterator = 0;
}

void GameState::switchToNextAnt()
{
	if (mSelectedAnt != nullptr)
	{
		GameSingleton::map->setCursorVisible(false);
		mSelectedAnt->unselect();
	}
	if (mAntUpdateIterator >= GameSingleton::ants.size())
	{
		mAntUpdateIterator = 0;
	}
	mSelectedAnt = GameSingleton::getAnt(mAntUpdateIterator);
	mAntUpdateIterator++;
	if (mSelectedAnt != nullptr)
	{
		GameSingleton::map->setCursorVisible(true);
		mSelectedAnt->select();
		static oe::Vector2 offset(30.f, 26.f);
		mWorld.getRenderSystem().getView().setCenter(mSelectedAnt->getPosition() + offset);
	}
}
