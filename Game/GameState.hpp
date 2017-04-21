#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../Sources/Core/StateManager.hpp"
#include "../Sources/Core/World.hpp"

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

	private:
		oe::World mWorld;
};

#endif // GAMESTATE_HPP
