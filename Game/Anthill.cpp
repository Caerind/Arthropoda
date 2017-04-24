#include "Anthill.hpp"

#include "GameConfig.hpp"
#include "GameSingleton.hpp"

Anthill::Anthill(oe::World& world)
	: MapEntity(world)
	, mSprite(*this)
{
	// Sprite
	mSprite.setTexture(GameSingleton::objectsTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, 98, 81));
	mSprite.setPosition(-20.f, -10.f);

	mResourceComponent.setResourcesMax(999);
	mResourceComponent.move(10.f, 0.f);

	// MapEntity
	mLifeComponent.setLifeMax(GSTARTLIFE);
	setPlayer(0);
	setLife(GSTARTLIFE);
	setResources(GSTARTR);

	setPositionZ(0.f);
	mSprite.setPositionZ(-10.f);
	mLifeComponent.setPositionZ(10.f);

	mLifeComponent.move(5.f, 10.f);
}

void Anthill::getNeighbors(std::vector<oe::Vector2i>& n)
{
	oe::MapUtility::getNeighboors(n, getCoords(), oe::MapUtility::Hexagonal);
}

bool Anthill::canSpawn(Ant::Type antType)
{
	return Ant::getPrice(antType) <= getResources();
}

bool Anthill::spawn(Ant::Type antType)
{
	std::vector<oe::Vector2i> n;
	oe::MapUtility::getNeighboors(n, getCoords(), oe::MapUtility::Hexagonal);
	U32 size = n.size();
	for (U32 i = 0; i < size;)
	{
		if (GameSingleton::isCollision(n[i]))
		{
			n.erase(n.begin() + i);
			size--;
		}
		else
		{
			i++;
		}
	}
	if (n.size() > 0) 
	{
		oe::Vector2i coords(n[oe::Random::get(0u, n.size() - 1)]);
		oe::EntityHandle ant(getWorld().createEntity<Ant>());
		Ant* a = ant.getAs<Ant>();
		if (a != nullptr)
		{
			a->setCoords(coords);
			a->setPlayer(getPlayer());
			a->setType(antType);
			GameSingleton::setCollision(coords, true);
			if (getPlayer() == 1)
			{
				GameSingleton::ants.insert(ant);
			}
			else
			{
				GameSingleton::aiAnts.insert(ant);
			}
			takeResources(Ant::getPrice(antType));
			return true;
		}
	}
	return false;
}
