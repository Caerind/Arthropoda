#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "../Sources/Core/StateManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class MenuState : public oe::State
{
	public:
		MenuState(oe::StateManager& manager);

		bool handleEvent(const sf::Event& event);
		bool update(oe::Time dt);
		void render(sf::RenderTarget& target);

	private:
		sf::Texture mTextureBg;
		sf::Sprite mBackground;

		sf::Texture mTexturePions;
		sf::Sprite mPion;

		bool mReady;
		bool mMoving;
		U32 mIteration;
		U32 mChoice;
		sf::Vector2f mDestination;
		std::vector<sf::Vector2f> mPath1;
		std::vector<sf::Vector2f> mPath2;

		sf::FloatRect mPlay1;
		sf::FloatRect mPlay2;

		sf::FloatRect mButton1;
		sf::FloatRect mButton2;
		bool mButton1Bool;
		bool mButton2Bool;
		sf::CircleShape mButton1Shape;
		sf::CircleShape mButton2Shape;

		oe::Time mElapsed;
};

#endif // MENUSTATE_HPP
