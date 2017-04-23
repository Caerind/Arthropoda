#include "GameState.hpp"

#include "GameSingleton.hpp" // Used to store the map
#include "GameConfig.hpp" // Used to move the view

GameState::GameState(oe::StateManager& manager)
	: oe::State(manager)
	, mWorld(manager.getApplication())
{
	mTurnNumber = 0;
	mWorld.getRenderSystem().setBackgroundColor(oe::Color::DarkGray);

	// Load resources
	GameSingleton::antTexture = mWorld.getTextures().create("ants", "../Assets/pions.png");
	GameSingleton::objectsTexture = mWorld.getTextures().create("objects", "../Assets/objects.png");
	mGameHudTexture.loadFromFile("../Assets/gamehud.png");

	// Buttons
	mButton1.setTexture(mGameHudTexture);
	mButton1.setTextureRect(sf::IntRect(0, 0, 115, 60));
	mButton1.setPosition(0.f, WINSIZEY - 60.f);
	mButton2.setTexture(mGameHudTexture);
	mButton2.setTextureRect(sf::IntRect(115, 0, 115, 60));
	mButton2.setPosition(115.f, WINSIZEY - 60.f);
	mButton3.setTexture(mGameHudTexture);
	mButton3.setTextureRect(sf::IntRect(115, 0, 115, 60));
	mButton3.setPosition(230.f, WINSIZEY - 60.f);
	mButtonNext.setTexture(mGameHudTexture);
	mButtonNext.setTextureRect(sf::IntRect(345, 0, 60, 60));
	mButtonNext.setPosition(WINSIZEX - 120.f, WINSIZEY - 60.f);
	mButtonTurn.setTexture(mGameHudTexture);
	mButtonTurn.setTextureRect(sf::IntRect(405, 0, 60, 60));
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
	mPlayer2Anthill.set(20, 20);
	GameSingleton::aiAnthill = mWorld.createEntity<Anthill>();
	anthill = GameSingleton::aiAnthill.getAs<Anthill>();
	anthill->setCoords(mPlayer2Anthill);
	anthill->setPlayer(2);
	GameSingleton::setCollision(mPlayer2Anthill, true);

	// Init map
	initResources();
	initCollisions();

	// Start the game
	mCurrentPlayer = 2;
	mSelectedAnt = nullptr;
	mMousePressedBool = false;
	passTurn(); // Pass to player 1 and do the announce it

	mClock.restart();
}

bool GameState::handleEvent(const sf::Event& event)
{
	moveView(event);

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
			if (mc == mPlayer1Anthill)
			{
				mSelectedAnt->goToAnthill();
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

	getWindow().setTitle("LD38 - FPS : " + oe::toString(getApplication().getFPSCount()));

	if (!GameSingleton::map->isOverlayValid())
	{
		GameSingleton::map->cleanOverlay();
	}

	Anthill& anthill = GameSingleton::getAnthill();

	if (mCurrentPlayer == 1)
	{
		GameSingleton::map->setCursorCoords(getMouseCoords());
		mButton1.setTextureRect(sf::IntRect(0, 60 * ((anthill.canSpawn(Ant::Scout)) ? 0 : 1), 115, 60));
		mButton2.setTextureRect(sf::IntRect(115, 60 * ((anthill.canSpawn(Ant::Worker)) ? 0 : 1), 115, 60));
		mButton3.setTextureRect(sf::IntRect(230, 60 * ((anthill.canSpawn(Ant::Soldier)) ? 0 : 1), 115, 60));

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
		mButton1.setTextureRect(sf::IntRect(0, 60, 115, 60));
		mButton2.setTextureRect(sf::IntRect(115, 60, 115, 60));
		mButton3.setTextureRect(sf::IntRect(230, 60, 115, 60));
		// TODO : AI
	}

	return false;
}

void GameState::render(sf::RenderTarget& target)
{
	mWorld.render(target);
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
			view.zoom(1.2f);
		}
		else
		{
			view.zoom(0.8f);
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
}

oe::Vector2i GameState::getMouseCoords()
{
	return GameSingleton::map->worldToCoords(getApplication().getWindow().getCursorPositionView(mWorld.getRenderSystem().getView()));
}

void GameState::moveView(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		mMousePressedBool = true;
		mMousePressed.set(getWindow().getCursorPositionView(getView()));
	}
	if (event.type == sf::Event::MouseMoved && mMousePressedBool)
	{
		oe::Vector2 mp(getWindow().getCursorPositionView(getView()));
		getView().move(mMousePressed - mp);
	}
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		mMousePressedBool = false;
	}
}

void GameState::initResources()
{
	U32 rCount = oe::Random::get(10u, 15u);
	for (U32 i = 0; i < rCount; i++)
	{
		bool placed = false;
		while (!placed)
		{
			oe::Vector2i coords(oe::Random::get(4, MAPSIZEX - 4), oe::Random::get(4, MAPSIZEY - 4));
			if (!GameSingleton::isCollision(coords))
			{
				oe::EntityHandle r = mWorld.createEntity<Resource>();
				Resource* res = r.getAs<Resource>();
				if (res != nullptr)
				{
					res->setCoords(coords);
					GameSingleton::resources.insert(r);
					placed = true;
				}
			}
		}
	}
}

void GameState::initCollisions()
{
}

bool GameState::useButtons(const sf::Vector2f& mouse)
{
	Anthill& anthill = GameSingleton::getAnthill();
	if (mButton1.getGlobalBounds().contains(mouse) && anthill.canSpawn(Ant::Scout))
	{
		anthill.spawn(Ant::Scout);
		return true;
	}
	if (mButton2.getGlobalBounds().contains(mouse) && anthill.canSpawn(Ant::Worker))
	{
		anthill.spawn(Ant::Worker);
		return true;
	}
	if (mButton3.getGlobalBounds().contains(mouse) && anthill.canSpawn(Ant::Soldier))
	{
		anthill.spawn(Ant::Soldier);
		return true;
	}
	if (mButtonNext.getGlobalBounds().contains(mouse))
	{
		switchToNextAnt();
		return true;
	}
	if (mButtonTurn.getGlobalBounds().contains(mouse))
	{
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
		mCurrentPlayer = 1; // TODO : Change
		mTurnNumber++; // TODO : Change
		// TODO : Change
		GameSingleton::map->setCursorRect(sf::IntRect(420, 0, 60, 80)); // CURSOR 1
	}
	else if (mCurrentPlayer == 2 || mCurrentPlayer == 0)
	{
		GameSingleton::map->setCursorRect(sf::IntRect(420, 0, 60, 80)); // CURSOR 1
		if (mCurrentPlayer == 2)
		{
			for (const oe::EntityHandle& e : GameSingleton::ants)
			{
				ASSERT(e.isValid());
				Ant* ant = e.getAs<Ant>();
				if (ant != nullptr)
				{
					ant->reset();
				}
			}
		}
		mCurrentPlayer = 1;
		mButtonTurn.setTextureRect(sf::IntRect(345, 0, 60, 60));
		mTurnNumber++;
	}
	printf("Player %d turn %d started\n", mCurrentPlayer, mTurnNumber);
	mButtonTurn.setTextureRect(sf::IntRect(345, (mCurrentPlayer == 1) ? 0 : 60, 60, 60));
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
