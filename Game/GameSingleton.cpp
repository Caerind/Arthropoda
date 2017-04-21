#include "GameSingleton.hpp"

#include "GameConfig.hpp" // Used to load the tileset

oe::Tileset GameSingleton::tileset;
oe::EntityHandle GameSingleton::map;
oe::ResourceId GameSingleton::soldierR;
oe::EntityHandle GameSingleton::soldier;
Matrix<I32> GameSingleton::collisions;

void GameSingleton::loadTileset()
{
	tileset.setImageSource(TILESETSOURCE);
	tileset.setTileSize(oe::Vector2i(TILESETSIZEX, TILESETSIZEY));
	tileset.setTileCount(TILESETCOUNT);
	tileset.setColumns(TILESETCOLUMNS);
	tileset.getTexture(); // Used to load the texture now
}

GameMap& GameSingleton::getMap()
{
	ASSERT(map.isValid());
	GameMap* mapPtr = map.getAs<GameMap>();
	ASSERT(mapPtr != nullptr);
	return *mapPtr;
}

Soldier& GameSingleton::getSoldier()
{
	ASSERT(soldier.isValid());
	Soldier* soldierPtr = soldier.getAs<Soldier>();
	ASSERT(soldierPtr != nullptr);
	return *soldierPtr;
}

void GameSingleton::initCollisions(I32 sizeX, I32 sizeY)
{
	collisions.create(sizeX, sizeY, 0);
}

void GameSingleton::setCollision(const oe::Vector2i& coords, bool value)
{
	collisions.set(coords, ((value == true) ? 1 : 0));
}

bool GameSingleton::isCollision(const oe::Vector2i& coords)
{
	return collisions.get(coords) != 0;
}

void GameSingleton::setCollision(I32 x, I32 y, bool value)
{
	setCollision(oe::Vector2i(x, y), value);
}

bool GameSingleton::isCollision(I32 x, I32 y)
{
	return isCollision(oe::Vector2i(x, y));
}
