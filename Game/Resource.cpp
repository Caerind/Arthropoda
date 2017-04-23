#include "Resource.hpp"

#include "GameSingleton.hpp"
#include "GameConfig.hpp"

Resource::Resource(oe::World& world)
	: MapEntity(world)
	, mSprite(*this)
{
	// Sprite
	mSprite.setTexture(GameSingleton::objectsTexture);
	mSprite.setTextureRect(sf::IntRect(98, 0, 62, 48));
	mSprite.setPosition(0.f, -10.f);
	mSprite.setPositionZ(0.0f);
	mResourceComponent.setPositionZ(1.0f);

	// MapEntity
	setPlayer(0);
	setLife(0);
	U32 qty = oe::Random::get(GSRMIN, GSRMAX);
	setResources(qty);
	mResourceComponent.setResourcesMax(qty);
}