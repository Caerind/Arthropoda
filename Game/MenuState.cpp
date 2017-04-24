#include "MenuState.hpp"

#include "GameSingleton.hpp"
#include "GameState.hpp" // Used to switch to
#include "PreState.hpp" // Used to switch to

#include "../Sources/Math/Math.hpp"
#include "../Sources/Math/Vector2.hpp"

MenuState::MenuState(oe::StateManager& manager)
	: oe::State(manager)
{
	mTextureBg.loadFromFile("Assets/menu.png");
	mBackground.setTexture(mTextureBg);

	mPion.setTexture(getApplication().getTextures().get(GameSingleton::antTexture));
	mPion.setTextureRect(sf::IntRect(0, 0, 60, 52));
	mPion.setOrigin(30, 26);

	mChoice = 0; // Start
	mIteration = 0;
	mMoving = false;
	mReady = false;

	getApplication().getAudio().setMusicVolume(10.0f);
	getApplication().getAudio().setSoundVolume(80.0f);
	mButton1Bool = true;
	mButton2Bool = true;
	mButton1Shape.setPosition(sf::Vector2f(113, 383));
	mButton2Shape.setPosition(sf::Vector2f(113, 453));
	mButton1Shape.setRadius(10.f);
	mButton2Shape.setRadius(10.f);
	mButton1Shape.setFillColor(sf::Color::Black);
	mButton2Shape.setFillColor(sf::Color::Black);
	mButton1Shape.setPointCount(100);
	mButton2Shape.setPointCount(100);
	mButton1 = mButton1Shape.getGlobalBounds();
	mButton2 = mButton2Shape.getGlobalBounds();

	mPion.setPosition(170, 150);

	mPlay1 = sf::FloatRect(561, 252, 143, 123);
	mPlay2 = sf::FloatRect(540, 408, 180, 130);

	mPath1.push_back(sf::Vector2f(265, 172));
	mPath1.push_back(sf::Vector2f(301, 288));
	mPath1.push_back(sf::Vector2f(521, 265));
	mPath1.push_back(sf::Vector2f(640, 311));
	mPath1.emplace_back(0.f, 0.f);

	mPath2.push_back(sf::Vector2f(265, 172));
	mPath2.push_back(sf::Vector2f(301, 288));
	mPath2.push_back(sf::Vector2f(336, 419));
	mPath2.push_back(sf::Vector2f(301, 288));
	mPath2.push_back(sf::Vector2f(521, 265));
	mPath2.push_back(sf::Vector2f(640, 311));
	mPath2.emplace_back(0.f, 0.f);
}

bool MenuState::handleEvent(const sf::Event& event)
{
	sf::Vector2f mpos = toSF(getApplication().getWindow().getCursorPosition());

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (!mMoving)
		{
			if (mPlay1.contains(mpos))
			{
				mMoving = true;
				mChoice = 1;
				mIteration = 0;
				mDestination = mPath1[0];
			}
			if (mPlay2.contains(mpos))
			{
				mMoving = true;
				mChoice = 2;
				mIteration = 0;
				mDestination = mPath2[0];
			}
			getApplication().getAudio().playSound(GameSingleton::actionSound);
		}
		if (mButton1.contains(mpos))
		{
			mButton1Bool = !mButton1Bool;
			if (mButton1Bool)
			{
				mButton1Shape.setFillColor(sf::Color::Black);
				getApplication().getAudio().setMusicVolume(10.0f);
			}
			else
			{
				mButton1Shape.setFillColor(sf::Color::Transparent);
				getApplication().getAudio().setMusicVolume(0.0f);
			}
			getApplication().getAudio().playSound(GameSingleton::actionSound);
		}
		else if (mButton2.contains(mpos))
		{
			mButton2Bool = !mButton2Bool;
			getApplication().getAudio().playSound(GameSingleton::actionSound);
			if (mButton2Bool)
			{
				mButton2Shape.setFillColor(sf::Color::Black);
				getApplication().getAudio().setSoundVolume(80.0f);
			}
			else
			{
				mButton2Shape.setFillColor(sf::Color::Transparent);
				getApplication().getAudio().setSoundVolume(0.0f);
			}
		}
	}

	return false;
}

bool MenuState::update(oe::Time dt)
{
	mElapsed += dt;

	if (mMoving)
	{
		if (oe::Math::equals(mPion.getPosition().x, mDestination.x, 5.f) && oe::Math::equals(mPion.getPosition().y, mDestination.y, 5.f))
		{
			if (mChoice == 1 && mIteration == mPath1.size() - 1)
			{
				mMoving = false;
				mReady = true;
				mElapsed = oe::Time::Zero;
			}
			if (mChoice == 2 && mIteration == mPath2.size() - 1)
			{
				mMoving = false;
				mReady = true;
				mElapsed = oe::Time::Zero;
			}

			if (mChoice == 1)
			{
				mDestination = mPath1[mIteration];
				mIteration++;
			}
			else
			{
				mDestination = mPath2[mIteration];
				mIteration++;
			}
		}

		oe::Vector2 offset(oe::toOE(mDestination - mPion.getPosition()));
		offset.normalize();
		offset *= 500.f * dt.asSeconds();
		mPion.move(offset.x, offset.y);
	}

	if (mReady && mElapsed > oe::seconds(0.5f))
	{
		clearStates();

		if (mChoice == 1)
		{
			pushState<GameState>();
		}
		if (mChoice == 2)
		{
			pushState<GameState>(); // TODO : MULTI
		}

		static bool preStateDone = false;
		if (!preStateDone)
		{
			preStateDone = true;
			pushState<PreState>();
		}
	}

	return false;
}

void MenuState::render(sf::RenderTarget& target)
{
	target.draw(mBackground);
	target.draw(mPion);
	target.draw(mButton1Shape);
	target.draw(mButton2Shape);
}
