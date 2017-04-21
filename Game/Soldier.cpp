#include "Soldier.hpp"

#include "GameSingleton.hpp"

Soldier::Soldier(oe::World& world)
	: HexaEntity(world)
	, mSprite(*this)
{
	setPositionZ(1000);
	mSprite.setTexture(GameSingleton::soldierR);
	mSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	mSprite.setPosition(0.f, -10.f);
}

void Soldier::goTo(const oe::Vector2i& coords)
{
	GameSingleton::setCollision(getCoords(), false);
	printf("A* started\n");
	oe::Clock c;
	AStar::run(mPath, getCoords(), coords, GameSingleton::collisions);
	printf("A* ended in %d milliseconds\n", c.getElapsedTime().asMilliseconds());
	printf("Path size : %d\n\n", mPath.size());
	GameSingleton::setCollision(getCoords(), true);
}

void Soldier::update(oe::Time dt)
{
	if (!mPath.empty())
	{
		mTime += dt;
		if (mTime > oe::seconds(0.2f))
		{
			mTime = oe::Time::Zero;
			GameSingleton::setCollision(getCoords(), false);
			oe::Vector2i newCoords(mPath.front());
			GameSingleton::setCollision(newCoords, true);
			setCoords(newCoords);
			mPath.pop_front();
		}
	}
}
