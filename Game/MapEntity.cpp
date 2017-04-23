#include "MapEntity.hpp"

#include "GameSingleton.hpp"

MapEntity::MapEntity(oe::World& world)
	: oe::Entity(world)
	, mResourceComponent(*this)
	, mPlayer(0)
	, mLife(0)
{
	setCoords(oe::Vector2i(0, 0));
}

void MapEntity::setCoords(I32 x, I32 y)
{
	setCoords(oe::Vector2i(x, y));
}

void MapEntity::setCoords(const oe::Vector2i& coords)
{
	mCoords = coords;
	setPosition(GameSingleton::map->coordsToWorld(coords));
}

const oe::Vector2i& MapEntity::getCoords() const
{
	return mCoords;
}

void MapEntity::setPlayer(U32 player)
{
	mPlayer = player;
}

U32 MapEntity::getPlayer() const
{
	return mPlayer;
}

void MapEntity::setLife(U32 life)
{
	mLife = life;
}

U32 MapEntity::getLife() const
{
	return mLife;
}

void MapEntity::infligeDamage(U32 damage)
{
	mLife -= damage;
}

void MapEntity::setResources(U32 resources)
{
	mResources = resources;
	mResourceComponent.setResources(mResources);
}

U32 MapEntity::getResources() const
{
	return mResources;
}

void MapEntity::addResources(U32 resources)
{
	mResources += resources;
	mResourceComponent.setResources(mResources);
}

void MapEntity::takeResources(U32 resources)
{
	mResources -= resources;
	mResourceComponent.setResources(mResources);
}
