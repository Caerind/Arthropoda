#ifndef SOLDIER_HPP
#define SOLDIER_HPP

#include "HexaEntity.hpp"
#include "Pathfinding.hpp"

class Soldier : public HexaEntity
{
	public:
		Soldier(oe::World& world);

		void goTo(const oe::Vector2i& coords);

		void update(oe::Time dt);

	private:
		oe::SpriteComponent mSprite;
		std::list<oe::Vector2i> mPath;
		oe::Time mTime;
};

#endif // SOLDIER_HPP
