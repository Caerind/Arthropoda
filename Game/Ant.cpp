#include "Ant.hpp"

#include "GameSingleton.hpp"
#include "GameConfig.hpp"

const oe::Vector2i Ant::invalidDest(-1, -1);

Ant::Ant(oe::World& world)
	: MapEntity(world)
	, mSprite(*this)
	, mSelectionSprite(*this)
	, mDestination(-1, -1)
{
	// Sprite
	mSprite.setTexture(GameSingleton::antTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, 60, 52));
	//mSprite.setPosition(0.f, -5.f);
	mSelectionSprite.setTexture(GameSingleton::antTexture);
	mSelectionSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));

	mResourceComponent.move(40.f, 0.f);

	mSprite.setPositionZ(0.0f);
	mSelectionSprite.setPositionZ(1.0f);
	mResourceComponent.setPositionZ(2.0f);

	// MapEntity
	setPlayer(0);
	setLife(100);
	setResources(0);

	// Ant
	setType(Ant::Scout);
	reset();
}

void Ant::goTo(I32 x, I32 y)
{
	goTo(oe::Vector2i(x, y));
}

void Ant::goTo(const oe::Vector2i& coords)
{
	mPath.clear();
	if (AStar::run(mPath, getCoords(), coords, GameSingleton::collisions))
	{
		mDestination = coords;
		if (mPath.size() > mPM)
		{
			mPath.resize(mPM);
		}
	}
	else
	{
		mDestination.set(-1, -1); // Invalid destination
		mPath.clear();
	}
}

void Ant::goToAnthill()
{
	Anthill& anthill((getPlayer() == 1) ? GameSingleton::getAnthill() : GameSingleton::getAIAnthill());
	std::vector<oe::Vector2i> n;
	oe::MapUtility::getNeighboors(n, anthill.getCoords(), oe::MapUtility::Hexagonal);
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
		oe::Vector2i cCoords(getCoords());
		std::sort(n.begin(), n.end(), [&cCoords](const oe::Vector2i& a, const oe::Vector2i& b)
		{
			return AStar::heuristic(cCoords, a) < AStar::heuristic(cCoords, b);
		});
		goTo(n[0]);
	}
}

void Ant::goToDestination()
{
	if (mDestination != invalidDest && mDestination != getCoords())
	{
		goTo(mDestination);
	}
}

bool Ant::canPlay() const
{
	return mPM > 0;
}

bool Ant::isTurnOver() const
{
	return mPM == 0;
}

void Ant::reset()
{
	mPM = getDistance();
}

U32 Ant::getPM() const
{
	return mPM;
}

U32 Ant::getCap() const
{
	return getResourcesMax() - getResources();
}

bool Ant::isFullCap() const
{
	return getCap() <= 0;
}

bool Ant::updateAnt(oe::Time dt, bool selected)
{
	selectedOverlay(selected); // Selection (+ zone of movement if can play && selected)
	if (!mPath.empty()) // We need to move
	{
		mTime += dt; 
		static const oe::Time timeNeeded(oe::seconds(0.1f));
		if (mTime > timeNeeded)
		{
			mTime = oe::Time::Zero;
			oe::Vector2i coords(mPath.front());
			if (!GameSingleton::isCollision(coords))
			{
				GameSingleton::setCollision(getCoords(), false);
				GameSingleton::setCollision(coords, true);
				setCoords(coords);
				mPath.pop_front();
				//GameSingleton::map->invalidOverlay();
				mPM--;
			}
			else
			{
				mPath.clear();
			}
		}

		if (mPM == 0)
		{
			tryCollectResources();
			tryDeposit();
			if (getCoords() == mDestination)
			{
				mDestination.set(-1, -1);
			}
			return true;
		}

		if (getCoords() == mDestination)
		{
			tryCollectResources();
			tryDeposit();
			mDestination.set(-1, -1);
			return true;
		}
	}
	return false;
}

void Ant::select()
{
	mSelectionSprite.setTextureRect(sf::IntRect(240, 0, 60, 52));
	GameSingleton::map->invalidOverlay();
}

void Ant::unselect()
{
	mSelectionSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	GameSingleton::map->invalidOverlay();
}

void Ant::setType(Type type)
{
	mType = type;
	setLife(getLife(type));
	mPM = getDistance();
	I32 x;
	switch (type)
	{
		case Scout: x = 0; break;
		case Worker: x = 1; break;
		case Soldier: x = 2; break;
		default: x = 0; break;
	}
	I32 y = (getPlayer() == 1) ? 0 : 1;
	mResourceComponent.setResourcesMax(getResourcesMax());
	mSprite.setTextureRect(sf::IntRect(60 * x, 52 * y, 60, 52));
}

Ant::Type Ant::getType() const
{
	return mType;
}

U32 Ant::getDistance() const
{
	return getDistance(mType);
}

U32 Ant::getResourcesMax() const
{
	return getResourcesMax(mType);
}

U32 Ant::getDamage() const
{
	return getDamage(mType);
}

const oe::Vector2i& Ant::getDestination() const
{
	return mDestination;
}

U32 Ant::getPrice(Type antType)
{
	switch (antType)
	{
		case Scout: return GPRICE1; break;
		case Worker: return GPRICE2; break;
		case Soldier: return GPRICE3; break;
		default: break;
	}
	return 0;
}

U32 Ant::getDistance(Type antType)
{
	switch (antType)
	{
		case Scout: return GDISTANCE1; break;
		case Worker: return GDISTANCE2; break;
		case Soldier: return GDISTANCE3; break;
		default: break;
	}
	return 0;
}

U32 Ant::getResourcesMax(Type antType)
{
	switch (antType)
	{
		case Scout: return GRMAX1; break;
		case Worker: return GRMAX2; break;
		case Soldier: return GRMAX3; break;
		default: break;
	}
	return 0;
}

U32 Ant::getLife(Type antType)
{
	switch (antType)
	{
		case Scout: return GLIFE1; break;
		case Worker: return GLIFE2; break;
		case Soldier: return GLIFE3; break;
		default: break;
	}
	return 0;
}

U32 Ant::getDamage(Type antType)
{
	switch (antType)
	{
		case Scout: return GDMG1; break;
		case Worker: return GDMG2; break;
		case Soldier: return GDMG3; break;
		default: break;
	}
	return 0;
}

void Ant::selectedOverlay(bool displayMovements)
{
	if (canPlay() && displayMovements && !GameSingleton::map->isOverlayValid())
	{
		std::vector<oe::Vector2i> dist;
		Distance::run(dist, getCoords(), mPM, GameSingleton::collisions);
		for (const oe::Vector2i& coords : dist)
		{
			GameSingleton::map->setTileOverlayId(coords, (getPlayer() == 1) ? TILE_OVER1 : TILE_OVER2);
		}
		GameSingleton::map->validOverlay();
	}
}

void Ant::tryCollectResources()
{
	Resource* r = GameSingleton::getResource(getCoords());
	if (r != nullptr)
	{
		U32 antCap = getCap();
		U32 resources = r->getResources();
		if (antCap > resources)
		{
			addResources(resources);
			r->setResources(0);
			r->kill();
		}
		else // resource >= antCap
		{
			addResources(antCap);
			r->takeResources(antCap);
			if (r->getResources() <= 0)
			{
				r->kill();
			}
		}
	}
}

void Ant::tryDeposit()
{
	Anthill& anthill((getPlayer() == 1) ? GameSingleton::getAnthill() : GameSingleton::getAIAnthill());
	std::vector<oe::Vector2i> n;
	oe::MapUtility::getNeighboors(n, anthill.getCoords(), oe::MapUtility::Hexagonal);
	for (const oe::Vector2i& c : n)
	{
		if (c == getCoords())
		{
			anthill.addResources(getResources());
			setResources(0);
		}
	}
}
