#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "../Sources/Core/Map.hpp"
#include "../Sources/Core/Components/SpriteComponent.hpp"

class GameMap : public oe::Map
{
	public:
		GameMap(oe::World& world);

		void setTileId(const oe::Vector2& worldPos, oe::TileId tile);
		void setTileId(const oe::Vector2i& coords, oe::TileId tile);

		void setCursorVisible(bool visible);
		void setCursorRect(const sf::IntRect& rect);
		void setCursorCoords(const oe::Vector2i& coords, U32 currentPlayer);

		void validOverlay();
		void invalidOverlay();
		bool isOverlayValid() const;

		void setTileOverlayId(const oe::Vector2& worldPos, oe::TileId tile);
		void setTileOverlayId(const oe::Vector2i& coords, oe::TileId tile);
		void cleanOverlay();

	private:
		oe::LayerComponent mLayer;
		oe::LayerComponent mOverlay;
		oe::SpriteComponent mCursor;
		std::vector<oe::Vector2i> mCurrentOverlay;
		bool mOverlayValid;
		bool mOverlayClean;
};

#endif // GAMEMAP_HPP
