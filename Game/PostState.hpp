#ifndef POSTSTATE_HPP
#define POSTSTATE_HPP

#include "../Sources/Core/StateManager.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class PostState : public oe::State
{
	public:
		PostState(oe::StateManager& manager);

		bool handleEvent(const sf::Event& event);
		bool update(oe::Time dt);
		void render(sf::RenderTarget& target);

	private:
		sf::Texture mTexture;
		sf::Sprite mScreen;
		sf::Text mText;
};

#endif // POSTSTATE_HPP
