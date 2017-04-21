#include "HexaEntity.hpp"

#include "GameSingleton.hpp"

HexaEntity::HexaEntity(oe::World& world)
	: oe::Entity(world)
{
	setCoords(oe::Vector2i(0, 0));
}

void HexaEntity::setCoords(I32 x, I32 y)
{
	setCoords(oe::Vector2i(x, y));
}

void HexaEntity::setCoords(const oe::Vector2i& coords)
{
	mCoords = coords;
	setPosition(GameSingleton::getMap().coordsToWorld(coords));
}

const oe::Vector2i& HexaEntity::getCoords() const
{
	return mCoords;
}
