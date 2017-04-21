#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include "../Sources/Core/StateManager.hpp"

class IntroState : public oe::State
{
	public:
		IntroState(oe::StateManager& manager);

		bool handleEvent(const sf::Event& event);
		bool update(oe::Time dt);
		void render(sf::RenderTarget& target);

	private:
		oe::Time mElapsed;
};

#endif // INTROSTATE_HPP
