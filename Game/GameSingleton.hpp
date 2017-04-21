#ifndef GAMESINGLETON_HPP
#define GAMESINGLETON_HPP

#include "../Sources/System/Tileset.hpp"
#include "../Sources/Core/EntityHandle.hpp"
#include "GameMap.hpp"
#include "Soldier.hpp"
#include "Pathfinding.hpp"

class GameSingleton
{
	public:
		// Tileset
		static oe::Tileset tileset;
		static void loadTileset();

		// Map
		static oe::EntityHandle map;
		static GameMap& getMap();

		// Soldier
		static oe::ResourceId soldierR;
		static oe::EntityHandle soldier;
		static Soldier& getSoldier();

		// Collisions
		static Matrix<I32> collisions;
		static void initCollisions(I32 sizeX, I32 sizeY);
		static void setCollision(const oe::Vector2i& coords, bool value);
		static bool isCollision(const oe::Vector2i& coords);
		static void setCollision(I32 x, I32 y, bool value);
		static bool isCollision(I32 x, I32 y);

};

#endif // GAMESINGLETON_HPP
