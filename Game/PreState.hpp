#ifndef PRESTATE_HPP
#define PRESTATE_HPP

#include "../Sources/Core/StateManager.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class PreState : public oe::State
{
	public:
		PreState(oe::StateManager& manager);

		bool handleEvent(const sf::Event& event);
		bool update(oe::Time dt);
		void render(sf::RenderTarget& target);

	private:
		sf::Texture mTexture;
		sf::Sprite mScreen;
		sf::Text mText;
};

#endif // PRESTATE_HPP
