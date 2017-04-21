#include "GameMap.hpp"

#include "GameSingleton.hpp" // Used to get tileset
#include "GameConfig.hpp" // Used to get data

GameMap::GameMap(oe::World& world)
	: oe::Map(world)
	, mLayer(*this)
	, mOverlay(*this)
	, mTilesetInstance()
{
	setTileset(&GameSingleton::tileset);
	setOrientation(oe::MapUtility::Hexagonal);
	setSize(oe::Vector2i(MAPSIZEX, MAPSIZEY));
	setTileSize(oe::Vector2i(MAPTILESIZEX, MAPTILESIZEY));
	setStaggerAxis(oe::MapUtility::Y);
	setStaggerIndex(oe::MapUtility::Odd);
	setHexSideLength(MAPHEXSIDE);
}

GameMap::~GameMap()
{
}

void GameMap::onCreate()
{
	mLayer.create(getTileset(), getSize(), getTileSize(), getOrientation(), getStaggerAxis(), getStaggerIndex(), getHexSideLength());
	mLayer.setPositionZ(0.f);
	mOverlay.create(getTileset(), getSize(), getTileSize(), getOrientation(), getStaggerAxis(), getStaggerIndex(), getHexSideLength());
	mOverlay.setPositionZ(10.f);

	oe::Vector2i size = getSize();
	oe::Vector2i coords;
	for (coords.x = 0; coords.x < size.x; coords.x++)
	{
		for (coords.y = 0; coords.y < size.y; coords.y++)
		{
			mLayer.setTileId(coords, TID_GRASS);
			mOverlay.setTileId(coords, TID_NONE);
		}
	}
}

void GameMap::onSpawn()
{
}

void GameMap::onDestroy()
{
}

void GameMap::setTileId(const oe::Vector2& worldPos, oe::TileId tile)
{
	setTileId(worldToCoords(worldPos), tile);
}

void GameMap::setTileId(const oe::Vector2i& coords, oe::TileId tile)
{
	mLayer.setTileId(coords, tile);
}

void GameMap::setTileOverlayId(const oe::Vector2& worldPos, oe::TileId tile)
{
	setTileOverlayId(worldToCoords(worldPos), tile);
}

void GameMap::setTileOverlayId(const oe::Vector2i& coords, oe::TileId tile)
{
	mOverlay.setTileId(coords, tile);
	mCurrentOverlay.push_back(coords);
}

void GameMap::cleanOverlay()
{
	for (const oe::Vector2i& c : mCurrentOverlay)
	{
		mOverlay.setTileId(c, TID_NONE);
	}
	mCurrentOverlay.clear();
}
