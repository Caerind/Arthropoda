#ifndef LIFECOMPONENT_HPP
#define LIFECOMPONENT_HPP

#include "../Sources/Core/RenderableComponent.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class LifeComponent : public oe::RenderableComponent
{
	public:
		LifeComponent(oe::Entity& entity);

		void setLifeMax(U32 lifeMax);
		void setLife(U32 life);

		virtual void render(sf::RenderTarget& target);

		void updateLife();

	private:
		F32 mLifeMax;
		F32 mLife;
		sf::RectangleShape mBack;
		sf::RectangleShape mBar;
};

#endif // RESOURCE_HPP
