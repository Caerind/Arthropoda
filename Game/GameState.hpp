#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../Sources/Core/StateManager.hpp"
#include "../Sources/Core/World.hpp"

#include "Ant.hpp"

class GameState : public oe::State
{
	public:
		GameState(oe::StateManager& manager);

		bool handleEvent(const sf::Event& event);
		bool update(oe::Time dt);
		void render(sf::RenderTarget& target);

	private:
		inline oe::Window& getWindow();
		inline oe::View& getView();
		void zoomView(const sf::Event& event);
		void moveView(oe::Time dt);

		void initResources();
		void initCollisions();
		oe::Vector2i getMouseCoords();
		void moveView(const sf::Event& event);
		bool useButtons(const sf::Vector2f& mouse);
		void selectAnt();
		void passTurn();
		void switchToNextAnt();

	private:
		oe::World mWorld;
		oe::Clock mClock;

		bool mMousePressedBool;
		oe::Vector2 mMousePressed;

		U32 mTurnNumber;
		U32 mCurrentPlayer;
		Ant* mSelectedAnt;
		U32 mAntUpdateIterator;
		bool mTurnReady;

		oe::Vector2i mPlayer1Anthill;
		oe::Vector2i mPlayer2Anthill;

		sf::Texture mGameHudTexture;
		sf::Sprite mButton1;
		sf::Sprite mButton2;
		sf::Sprite mButton3;
		sf::Sprite mButtonNext;
		sf::Sprite mButtonTurn;
};

#endif // GAMESTATE_HPP
