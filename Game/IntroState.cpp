#include "IntroState.hpp"

#include "../Sources/Core/Application.hpp"

#include "MenuState.hpp" // Used to switch to

IntroState::IntroState(oe::StateManager& manager)
	: oe::State(manager)
{
	mAtmogTexture.loadFromFile("Assets/atmog.png");
	mAtmogSprite.setTexture(mAtmogTexture);
}

bool IntroState::handleEvent(const sf::Event& event)
{
	return false;
}

bool IntroState::update(oe::Time dt)
{
	mElapsed += dt;
	if (mElapsed > oe::seconds(1.0f))
	{
		auto music = getApplication().getAudio().createMusic("music", "Assets/music.ogg");
		getApplication().getAudio().playMusic(music, true);

		popState();
		pushState<MenuState>();
	}
	return false;
}

void IntroState::render(sf::RenderTarget& target)
{
	target.draw(mAtmogSprite);
}
