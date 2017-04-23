#ifndef GAMESINGLETON_HPP
#define GAMESINGLETON_HPP

#include "../Sources/System/Tileset.hpp"
#include "../Sources/Core/EntityHandle.hpp"
#include "../Sources/Core/EntityList.hpp"

#include "GameMap.hpp"
#include "Ant.hpp"
#include "Anthill.hpp"
#include "Pathfinding.hpp"
#include "Resource.hpp"

class GameSingleton
{
	public:
		// Tileset
		static oe::Tileset tileset;
		static void loadTileset();

		// Fonts
		static oe::ResourceId sansationFont;

		// Map
		static GameMap* map;

		// Collisions
		static CollisionMatrix collisions;
		static void initCollisions(I32 sizeX, I32 sizeY);
		static void setCollision(const oe::Vector2i& coords, bool value);
		static bool isCollision(const oe::Vector2i& coords);
		static void setCollision(I32 x, I32 y, bool value);
		static bool isCollision(I32 x, I32 y);

		// Resources
		static oe::ResourceId antTexture;
		static oe::ResourceId objectsTexture;

		// Game Resources
		static oe::EntityList resources;
		static Resource* getResource(I32 x, I32 y);
		static Resource* getResource(const oe::Vector2i& coords);
		static bool hasResource(I32 x, I32 y);
		static bool hasResource(const oe::Vector2i& coords);

		// Player 1 : Player
		static oe::EntityHandle anthill;
		static Anthill& getAnthill();
		static oe::EntityList ants;
		static Ant* getAnt(U32 index);
		static Ant* getAnt(I32 x, I32 y);
		static Ant* getAnt(const oe::Vector2i& coords);

		// Player 2 : AI
		static oe::EntityHandle aiAnthill;
		static Anthill& getAIAnthill();
		static oe::EntityList aiAnts;
		static Ant* getAIAnt(U32 index);
		static Ant* getAIAnt(I32 x, I32 y);
		static Ant* getAIAnt(const oe::Vector2i& coords);

		// Update all lists
		static void update();
};

#endif // GAMESINGLETON_HPP
