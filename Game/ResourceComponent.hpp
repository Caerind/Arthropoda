#ifndef RESOURCECOMPONENT_HPP
#define RESOURCECOMPONENT_HPP

#include "../Sources/Core/RenderableComponent.hpp"
#include <SFML/Graphics/Text.hpp>

class ResourceComponent : public oe::RenderableComponent
{
	public:
		ResourceComponent(oe::Entity& entity);

		void setResourcesMax(U32 max);
		void setResources(U32 resources);

		virtual void render(sf::RenderTarget& target);

		void updateText();

	private:
		U32 mResourcesMax;
		U32 mResources;
		sf::Text mText;
};

#endif // RESOURCE_HPP
