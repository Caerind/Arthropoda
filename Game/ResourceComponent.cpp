#include "ResourceComponent.hpp"

#include "GameSingleton.hpp"
#include "GameConfig.hpp"

ResourceComponent::ResourceComponent(oe::Entity& entity)
	: oe::RenderableComponent(entity)
	, mResourcesMax(0)
	, mResources(0)
{
	mText.setFillColor(sf::Color::White);
	mText.setOutlineColor(sf::Color::Black);
	mText.setOutlineThickness(1.f);
	mText.setFont(getWorld().getFonts().get(GameSingleton::sansationFont));
	mText.setCharacterSize(10);
}

void ResourceComponent::setResourcesMax(U32 max)
{
	mResourcesMax = max;
	updateText();
}

void ResourceComponent::setResources(U32 resources)
{
	mResources = resources;
	updateText();
}

void ResourceComponent::render(sf::RenderTarget& target)
{
	target.draw(mText, getGlobalTransform());
}

void ResourceComponent::updateText()
{
	if (mResourcesMax != 0 && mResources != 0)
	{
		mText.setString(oe::toString(mResources) + "/" + oe::toString(mResourcesMax));
	}
	else
	{
		mText.setString("");
	}
}
