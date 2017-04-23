#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "MapEntity.hpp"

class Resource : public MapEntity
{
	public:
		Resource(oe::World& world);

	private:
		oe::SpriteComponent mSprite;
};

#endif // RESOURCE_HPP
