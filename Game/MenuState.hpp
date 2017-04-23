#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "../Sources/Core/StateManager.hpp"

class MenuState : public oe::State
{
	public:
		MenuState(oe::StateManager& manager);

		bool handleEvent(const sf::Event& event);
		bool update(oe::Time dt);
		void render(sf::RenderTarget& target);

	private:
		oe::Time mElapsed;
};

#endif // MENUSTATE_HPP
