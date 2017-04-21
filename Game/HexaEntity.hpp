#ifndef HEXAENTITY_HPP
#define HEXAENTITY_HPP

#include "../Sources/Core/World.hpp"
#include "../Sources/Core/Entity.hpp"

class HexaEntity : public oe::Entity
{
	public:
		HexaEntity(oe::World& world);

		void setCoords(I32 x, I32 y);
		void setCoords(const oe::Vector2i& coords);
		const oe::Vector2i& getCoords() const;

	protected:
		oe::Vector2i mCoords;
};

#endif // HEXAENTITY_HPP
