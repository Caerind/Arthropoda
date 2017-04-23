#include "MenuState.hpp"

#include "GameState.hpp" // Used to switch to

MenuState::MenuState(oe::StateManager& manager)
	: oe::State(manager)
{
}

bool MenuState::handleEvent(const sf::Event& event)
{
	return false;
}

bool MenuState::update(oe::Time dt)
{
	mElapsed += dt;
	if (mElapsed > oe::seconds(0.5f))
	{
		popState();
		pushState<GameState>();
	}
	return false;
}

void MenuState::render(sf::RenderTarget& target)
{
}
