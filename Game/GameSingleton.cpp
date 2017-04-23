#include "GameSingleton.hpp"

#include "GameConfig.hpp" // Used to load the tileset

oe::Tileset GameSingleton::tileset;
GameMap* GameSingleton::map;
CollisionMatrix GameSingleton::collisions;
oe::ResourceId GameSingleton::sansationFont;
oe::ResourceId GameSingleton::antTexture;
oe::ResourceId GameSingleton::objectsTexture;
oe::EntityList GameSingleton::resources;
oe::EntityHandle GameSingleton::anthill;
oe::EntityList GameSingleton::ants;
oe::EntityHandle GameSingleton::aiAnthill;
oe::EntityList GameSingleton::aiAnts;

void GameSingleton::loadTileset()
{
	tileset.setImageSource(TILESETSOURCE);
	tileset.setTileSize(oe::Vector2i(TILESETSIZEX, TILESETSIZEY));
	tileset.setTileCount(TILESETCOUNT);
	tileset.setColumns(TILESETCOLUMNS);
	tileset.getTexture(); // Used to load the texture now
}

void GameSingleton::initCollisions(I32 sizeX, I32 sizeY)
{
	collisions.create(sizeX, sizeY);
}

void GameSingleton::setCollision(const oe::Vector2i& coords, bool value)
{
	collisions.set(coords, value);
}

bool GameSingleton::isCollision(const oe::Vector2i& coords)
{
	if (coords.x < 0 || coords.y < 0 || coords.x >= MAPSIZEX || coords.y >= MAPSIZEY)
	{
		return false;
	}
	return collisions.get(coords);
}

void GameSingleton::setCollision(I32 x, I32 y, bool value)
{
	setCollision(oe::Vector2i(x, y), value);
}

bool GameSingleton::isCollision(I32 x, I32 y)
{
	return isCollision(oe::Vector2i(x, y));
}

Resource* GameSingleton::getResource(I32 x, I32 y)
{
	return getResource(oe::Vector2i(x, y));
}

Resource* GameSingleton::getResource(const oe::Vector2i& coords)
{
	for (auto itr = resources.begin(); itr != resources.end(); ++itr)
	{
		Resource* r = itr->getAs<Resource>();
		if (r != nullptr && r->getCoords() == coords)
		{
			return r;
		}
	}
	return nullptr;
}

bool GameSingleton::hasResource(I32 x, I32 y)
{
	return getResource(oe::Vector2i(x, y)) != nullptr;
}

bool GameSingleton::hasResource(const oe::Vector2i& coords)
{
	return getResource(coords) != nullptr;
}

Anthill& GameSingleton::getAnthill()
{
	ASSERT(anthill.isValid());
	Anthill* anthillPtr = anthill.getAs<Anthill>();
	ASSERT(anthillPtr != nullptr);
	return *anthillPtr;
}

Ant* GameSingleton::getAnt(U32 index)
{
	U32 i = 0;
	for (auto itr = ants.begin(); itr != ants.end(); ++itr)
	{
		if (i == index)
		{
			return itr->getAs<Ant>();
		}
		i++;
	}
	return nullptr;
}

Ant* GameSingleton::getAnt(I32 x, I32 y)
{
	return getAnt(oe::Vector2i(x, y));
}

Ant* GameSingleton::getAnt(const oe::Vector2i& coords)
{
	for (const oe::EntityHandle& e : ants)
	{
		Ant* ant = e.getAs<Ant>();
		if (ant != nullptr && ant->getCoords() == coords)
		{
			return ant;
		}
	}
	return nullptr;
}

Anthill& GameSingleton::getAIAnthill()
{
	ASSERT(aiAnthill.isValid());
	Anthill* anthillPtr = aiAnthill.getAs<Anthill>();
	ASSERT(anthillPtr != nullptr);
	return *anthillPtr;
}

Ant* GameSingleton::getAIAnt(U32 index)
{
	U32 i = 0;
	for (auto itr = aiAnts.begin(); itr != aiAnts.end(); ++itr)
	{
		if (i == index)
		{
			return itr->getAs<Ant>();
		}
		i++;
	}
	return nullptr;
}

Ant* GameSingleton::getAIAnt(I32 x, I32 y)
{
	return getAIAnt(oe::Vector2i(x, y));
}

Ant* GameSingleton::getAIAnt(const oe::Vector2i& coords)
{
	for (const oe::EntityHandle& e : aiAnts)
	{
		Ant* ant = e.getAs<Ant>();
		if (ant != nullptr && ant->getCoords() == coords)
		{
			return ant;
		}
	}
	return nullptr;
}

void GameSingleton::update()
{
	resources.update();
	ants.update();
	aiAnts.update();
}
