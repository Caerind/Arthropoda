#ifndef ANTHILL_HPP
#define ANTHILL_HPP

#include "MapEntity.hpp"
#include "Pathfinding.hpp"
#include "Ant.hpp"

class Anthill : public MapEntity
{
	public:
		Anthill(oe::World& world);

		void getNeighbors(std::vector<oe::Vector2i>& n);

		bool canSpawn(Ant::Type antType);

		bool spawn(Ant::Type antType);

	private:
		oe::SpriteComponent mSprite;
};

#endif // SOLDIER_HPP
