#include "Ant.hpp"

#include "GameSingleton.hpp"
#include "GameConfig.hpp"
#include "Anthill.hpp"

const oe::Vector2i Ant::invalidDest(-1, -1);

Ant::Ant(oe::World& world)
	: MapEntity(world)
	, mSprite(*this)
	, mSelectionSprite(*this)
	, mDestination(invalidDest)
{
	// Sprite
	mSprite.setTexture(GameSingleton::antTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, 60, 52));
	mSprite.move(0.0f, 10.0f);
	mSelectionSprite.setTexture(GameSingleton::antTexture);
	mSelectionSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	mSelectionSprite.move(0.0f, 10.0f);

	mResourceComponent.move(40.f, 0.f);
	mLifeComponent.move(5.f, 10.f);

	setPositionZ(0.0f);
	mSprite.setPositionZ(0.0f);
	mSelectionSprite.setPositionZ(1.0f);
	mResourceComponent.setPositionZ(5.0f);
	mLifeComponent.setPositionZ(10.0f);

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
	invalidateTarget();
	mPath.clear();

	if (coords.x >= 30 || coords.x < 0 || coords.y >= 30 || coords.y < 0)
	{
		return;
	}

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
		mDestination.set(invalidDest);
		mPath.clear();
	}
}

void Ant::goToAnthill()
{
	invalidateTarget();
	oe::Vector2i coords(getAnthill().getCoords());

	mPath.clear();

	GameSingleton::setCollision(coords, false);
	bool a = AStar::run(mPath, getCoords(), coords, GameSingleton::collisions);
	GameSingleton::setCollision(coords, true);

	if (a)
	{
		if (mPath.back() == coords)
		{
			mPath.pop_back();
		}

		if (!mPath.empty())
		{
			mDestination = mPath.back();
		}

		if (mPath.size() > mPM)
		{
			mPath.resize(mPM);
		}
	}
	else
	{
		mDestination.set(invalidDest);
		mPath.clear();
	}
}

void Ant::goToDestination()
{
	invalidateTarget();
	if (mDestination != invalidDest && mDestination != getCoords())
	{
		if (mDestination == getAnthill().getCoords())
		{
			goToAnthill();
		}
		else
		{
			goTo(mDestination);
		}
	}
}

void Ant::goToTarget(oe::EntityHandle target)
{
	mTargetHandle = target;
	if (!mTargetHandle.isValid())
	{
		return;
	}

	mPath.clear();
	mDestination.set(invalidDest);

	// Adjacent
	MapEntity* ent = mTargetHandle.getAs<MapEntity>();

	if (canAttack())
	{
		attack();
		return;
	}

	oe::Vector2i coords(ent->getCoords());

	GameSingleton::setCollision(coords, false);
	bool a = AStar::run(mPath, getCoords(), coords, GameSingleton::collisions);
	GameSingleton::setCollision(coords, true);

	if (a)
	{
		if (mPath.back() == coords)
		{
			mPath.pop_back();
		}
		if (mPath.size() > mPM)
		{
			mPath.resize(mPM);
		}
	}
	else
	{
		mPath.clear();
	}
}

void Ant::invalidateTarget()
{
	mTargetHandle.invalidate();
}

bool Ant::canPlay() const
{
	return mPM > 0;
}

bool Ant::isTurnOver() const
{
	return mPM == 0 && !mMoving;
}

void Ant::endTurn()
{
	mPM = 0;
	mMoving = false;
}

void Ant::reset()
{
	mPM = getDistance();
	mMoving = false;
	mCanAttack = true;
	mTargetHandle.invalidate();
}

void Ant::resetDest()
{
	mDestination.set(invalidDest);
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

bool Ant::canAttack()
{
	MapEntity* ent = mTargetHandle.getAs<MapEntity>();
	return (mPM > 0 && mCanAttack && ent != nullptr && isAdjacent(ent->getCoords()));
}

void Ant::attack()
{
	ASSERT(canAttack());

	setCoords(mCoords);

	MapEntity* ent = mTargetHandle.getAs<MapEntity>();

	ent->infligeDamage(getDamage());
	mPM--;
	mCanAttack = false;

	// Attack sound
	if (getWorld().getRenderSystem().getView().getBounds().contains(toSF(getPosition().toVector2())))
	{
		getWorld().getApplication().getAudio().playSound(GameSingleton::attackSound);
	}

	// If ant : kill it
	if (ent->getLife() == 0 && ent->getCoords() != getAnthillEnemy().getCoords())
	{
		ent->kill();
		GameSingleton::setCollision(ent->getCoords(), false);
		if (getPlayer() == 1)
		{
			GameSingleton::map->setTileOverlayId(ent->getCoords(), TILE_OVER1);
			GameSingleton::map->validOverlay(); // Needed ?
		}
	}

	if (getPlayer() == 2) // TODO : Temp
	{
		mPM = 0;
		mMoving = false;
	}
}

bool Ant::updateAnt(oe::Time dt, bool selected)
{
	selectedOverlay(selected); // Selection (+ zone of movement if can play && selected)
	if (!mPath.empty() || mMoving) // We need to move
	{
		if (!mMoving)
		{
			mTime = oe::Time::Zero;
			oe::Vector2i coords(mPath.front());
			if (!GameSingleton::isCollision(coords))
			{
				GameSingleton::setCollision(getCoords(), false);
				GameSingleton::setCollision(coords, true);
				mCoords.set(coords);
				mPM--;
				mPath.pop_front();

				// If ant is visible : play sound
				if (getWorld().getRenderSystem().getView().getBounds().contains(toSF(getPosition().toVector2())))
				{
					getWorld().getApplication().getAudio().playSound(GameSingleton::movementSound);
				}

				mStart.set(getPosition());
				mEnd.set(GameSingleton::map->coordsToWorld(coords));
				mMoving = true;
			}
			else
			{
				mPath.clear();
				mMoving = false;
			}
		}
		else
		{
			mTime += dt;
			static const oe::Time time1(oe::seconds(0.10f));
			static const oe::Time time2(oe::seconds(0.15f));
			if (mTime < time1)
			{
				setPosition(oe::Vector2::lerp(mStart, mEnd, mTime.asSeconds() * 10.f));
			}
			else if (mTime > time2)
			{
				mMoving = false;

				setCoords(mCoords);

				if (mPM == 0)
				{
					tryCollectResources();
					tryDeposit();
					if (getCoords() == mDestination)
					{
						mDestination.set(invalidDest);
					}
					return true;
				}

				if (canAttack())
				{
					attack();
				}

				if (getCoords() == mDestination)
				{
					tryCollectResources();
					tryDeposit();
					mDestination.set(invalidDest);
					return true;
				}
			}
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

	mMoving = false;
	setCoords(getCoords());
}

void Ant::setType(Type type)
{
	mType = type;
	setLife(getLife(type));
	mPM = getDistance();
	I32 x;
	switch (type)
	{
		case Scout: x = 3; break;
		case Worker: x = 1; break;
		case Soldier: x = 2; break;
		default: x = 0; break;
	}
	I32 y = (getPlayer() == 1) ? 0 : 1;
	mResourceComponent.setResourcesMax(getResourcesMax());
	mLifeComponent.setLifeMax(getLife(mType));
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

bool Ant::isAdjacent(const oe::Vector2i& coords) const
{
	std::vector<oe::Vector2i> n;
	oe::MapUtility::getNeighboors(n, getCoords(), oe::MapUtility::Hexagonal);
	for (const oe::Vector2i& c : n)
	{
		if (c == coords)
		{
			return true;
		}
	}
	return false;
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
			GameSingleton::map->setTileOverlayId(coords, TILE_OVER1);
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
	Anthill& anthill(getAnthill());
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

Anthill& Ant::getAnthill()
{
	return (getPlayer() == 1) ? GameSingleton::getAnthill() : GameSingleton::getAIAnthill();
}

Anthill& Ant::getAnthillEnemy()
{
	return (getPlayer() == 2) ? GameSingleton::getAnthill() : GameSingleton::getAIAnthill();
}
