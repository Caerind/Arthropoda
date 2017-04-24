#include "GameMap.hpp"

#include "GameSingleton.hpp" // Used to get tileset
#include "GameConfig.hpp" // Used to get data

GameMap::GameMap(oe::World& world)
	: oe::Map(world)
	, mLayer(*this)
	, mOverlay(*this)
	, mCursor(*this)
	, mOverlayClean(true)
{
	setTileset(&GameSingleton::tileset);
	setOrientation(oe::MapUtility::Hexagonal);
	setSize(oe::Vector2i(MAPSIZEX, MAPSIZEY));
	setTileSize(oe::Vector2i(MAPTILESIZEX, MAPTILESIZEY));
	setStaggerAxis(oe::MapUtility::Y);
	setStaggerIndex(oe::MapUtility::Odd);
	setHexSideLength(MAPHEXSIDE);

	mCursor.setTexture(GameSingleton::tileset.getTexture());
	mCursor.setTextureRect(sf::IntRect(0, 0, 0, 0));
	mCursor.setVisible(false);

	mLayer.create(getTileset(), getSize(), getTileSize(), getOrientation(), getStaggerAxis(), getStaggerIndex(), getHexSideLength());
	mOverlay.create(getTileset(), getSize(), getTileSize(), getOrientation(), getStaggerAxis(), getStaggerIndex(), getHexSideLength());

	setPositionZ(-100.f);
	mLayer.setPositionZ(0.f); 
	mOverlay.setPositionZ(10.f);
	mCursor.setPositionZ(20.f);

	oe::Vector2i size = getSize();
	oe::Vector2i coords;
	for (coords.x = 0; coords.x < size.x; coords.x++)
	{
		for (coords.y = 0; coords.y < size.y; coords.y++)
		{
			mLayer.setTileId(coords, TILE_GROUND);
			mOverlay.setTileId(coords, TILE_GRID);
		}
	}
}

void GameMap::setTileId(const oe::Vector2& worldPos, oe::TileId tile)
{
	setTileId(worldToCoords(worldPos), tile);
}

void GameMap::setTileId(const oe::Vector2i& coords, oe::TileId tile)
{
	mLayer.setTileId(coords, tile);
}

void GameMap::setCursorVisible(bool visible)
{
	mCursor.setVisible(visible);
}

void GameMap::setCursorRect(const sf::IntRect& rect)
{
	mCursor.setTextureRect(rect);
}

void GameMap::setCursorCoords(const oe::Vector2i& coords, U32 currentPlayer)
{
	mCursor.setPosition(coordsToWorld(coords));
	mCursor.setTextureRect(sf::IntRect(420, 0, 60, 80)); // Default color
	Ant* enemyAnt = (currentPlayer == 1) ? GameSingleton::getAIAnt(coords) : GameSingleton::getAnt(coords);
	Anthill& enemyAnthill = (currentPlayer == 1) ? GameSingleton::getAIAnthill() : GameSingleton::getAnthill();
	if (enemyAnt != nullptr || coords == enemyAnthill.getCoords())
	{
		mCursor.setTextureRect(sf::IntRect(540, 0, 60, 80)); // Color enemy
	}
	else if (GameSingleton::isCollision(coords))
	{
		mCursor.setTextureRect(sf::IntRect(0, 0, 0, 0));
	}
}

void GameMap::validOverlay()
{
	mOverlayValid = true;
}

void GameMap::invalidOverlay()
{
	mOverlayValid = false;
}

bool GameMap::isOverlayValid() const
{
	return mOverlayValid;
}

void GameMap::setTileOverlayId(const oe::Vector2& worldPos, oe::TileId tile)
{
	setTileOverlayId(worldToCoords(worldPos), tile);
}

void GameMap::setTileOverlayId(const oe::Vector2i& coords, oe::TileId tile)
{
	mOverlay.setTileId(coords, tile);
	mCurrentOverlay.emplace_back(coords);
	mOverlayClean = false;
}

void GameMap::cleanOverlay()
{
	if (!mOverlayClean)
	{
		for (const oe::Vector2i& coords : mCurrentOverlay)
		{
			mOverlay.setTileId(coords, TILE_GRID);
		}
		mCurrentOverlay.clear();
		mOverlayClean = true;
	}
}
