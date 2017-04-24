#include "PreState.hpp"

#include "GameSingleton.hpp"

PreState::PreState(oe::StateManager& manager)
	: oe::State(manager)
{
	mTexture.loadFromFile("Assets/screen.png");
	mScreen.setTexture(mTexture);
	mText.setFont(getApplication().getFonts().get(GameSingleton::sansationFont));
	mText.setCharacterSize(24);
	mText.setFillColor(sf::Color::White);
	mText.setOutlineThickness(2.0f);
	mText.setOutlineColor(sf::Color::Black);
	mText.setString("Controls : \n - Use LEFT click to select your ant\n - Use RIGHT click to make an action with your ant\n - You can move the view with the mouse or the ARROWS\n\nUsing the buttons, you can spawn ants, next to your anthill\nBring resources to your anthill to spawn more ants\n\nTo win, you have to destroy the adverse anthill");
	mText.setPosition(90, 90);
}

bool PreState::handleEvent(const sf::Event& event)
{
	sf::Vector2f mpos(oe::toSF(getApplication().getWindow().getCursorPosition()));
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (sf::FloatRect(330, 400, 140, 100).contains(mpos))
		{
			getApplication().getAudio().playSound(GameSingleton::actionSound);
			popState();
		}
	}
	return false;
}

bool PreState::update(oe::Time dt)
{
	return false;
}

void PreState::render(sf::RenderTarget& target)
{
	target.draw(mScreen);
	target.draw(mText);
}
