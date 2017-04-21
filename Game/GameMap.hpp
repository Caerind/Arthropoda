#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "../Sources/Core/Map.hpp"

class GameMap : public oe::Map
{
	public:
		GameMap(oe::World& world);
		virtual ~GameMap();

		virtual void onCreate(); // Init the map layer
		virtual void onSpawn(); // Do nothing yet
		virtual void onDestroy(); // Do nothing yet

		void setTileId(const oe::Vector2& worldPos, oe::TileId tile);
		void setTileId(const oe::Vector2i& coords, oe::TileId tile);

		void setTileOverlayId(const oe::Vector2& worldPos, oe::TileId tile);
		void setTileOverlayId(const oe::Vector2i& coords, oe::TileId tile);
		void cleanOverlay();

	private:
		oe::LayerComponent mLayer;
		oe::LayerComponent mOverlay;
		oe::Tileset mTilesetInstance;
		std::vector<oe::Vector2i> mCurrentOverlay;
};

#endif // GAMEMAP_HPP
