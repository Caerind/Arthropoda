#include "IntroState.hpp"

#include "GameState.hpp" // Used to switch to

IntroState::IntroState(oe::StateManager& manager)
	: oe::State(manager)
{
}

bool IntroState::handleEvent(const sf::Event& event)
{
	return false;
}

bool IntroState::update(oe::Time dt)
{
	mElapsed += dt;
	if (mElapsed > oe::seconds(0.5f))
	{
		popState();
		pushState<GameState>();
	}
	return false;
}

void IntroState::render(sf::RenderTarget& target)
{
}
