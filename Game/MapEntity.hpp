#ifndef MAPENTITY_HPP
#define MAPENTITY_HPP

#include "../Sources/Core/World.hpp"
#include "../Sources/Core/Entity.hpp"
#include "ResourceComponent.hpp"

class MapEntity : public oe::Entity
{
	public:
		MapEntity(oe::World& world);

		void setCoords(I32 x, I32 y);
		void setCoords(const oe::Vector2i& coords);
		const oe::Vector2i& getCoords() const;

		void setPlayer(U32 player);
		U32 getPlayer() const;

		void setLife(U32 life);
		U32 getLife() const;
		void infligeDamage(U32 damage);

		void setResources(U32 resources);
		U32 getResources() const;
		void addResources(U32 resources);
		void takeResources(U32 resources);

	protected:
		ResourceComponent mResourceComponent;

	private:
		oe::Vector2i mCoords;
		U32 mPlayer;
		U32 mLife;
		U32 mResources;
};

#endif // MAPENTITY_HPP
