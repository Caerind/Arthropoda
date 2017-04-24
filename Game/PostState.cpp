#include "PostState.hpp"

#include "GameSingleton.hpp"

#include "MenuState.hpp" // Used to switch to

PostState::PostState(oe::StateManager& manager)
	: oe::State(manager)
{
	mTexture.loadFromFile("Assets/screen.png");
	mScreen.setTexture(mTexture);
	mText.setFont(getApplication().getFonts().get(GameSingleton::sansationFont));
	mText.setCharacterSize(40);
	mText.setFillColor(sf::Color::White);
	mText.setOutlineThickness(1.f);
	mText.setOutlineColor(sf::Color::Black);
	if (GameSingleton::win)
	{
		mText.setString("       Win !\n\n Play again :)");
	}
	else
	{
		mText.setString("     Loose...\n\n Try again :)");
	}
	mText.setPosition(270, 140);
}

bool PostState::handleEvent(const sf::Event& event)
{
	sf::Vector2f mpos(oe::toSF(getApplication().getWindow().getCursorPosition()));
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (sf::FloatRect(330, 400, 140, 100).contains(mpos))
		{
			getApplication().getAudio().playSound(GameSingleton::actionSound);
			popState();
			pushState<MenuState>();
		}
	}
	return false;
}

bool PostState::update(oe::Time dt)
{
	return false;
}

void PostState::render(sf::RenderTarget& target)
{
	target.draw(mScreen);
	target.draw(mText);
}
