#include "AI.hpp"

#include "GameConfig.hpp"

AI::AI()
{
	mTurnNumber = 0;
}

void AI::init()
{
	mAnthill = &GameSingleton::getAIAnthill();
	mAnthillPlayer = &GameSingleton::getAnthill();
	mAnthillPos = mAnthill->getCoords();
	mAnthillPosPlayer = mAnthillPlayer->getCoords();
}

void AI::startTurn()
{
	mCurrentAnt = nullptr;
	mCurrentAntIndex = 0;
	mTurnNumber++;
	mTurnOver = false;
	mEnemies = GameSingleton::ants;

	if (mTurnNumber == 1)
	{
		tryBuyScout();
		tryBuyWorker();
		tryBuyWorker();
		tryBuySoldier();
	}

	// Remove empty resources
	Resource* r = nullptr;
	U32 rSize = mResourcesPos.size();
	for (U32 i = 0; i < rSize; )
	{
		r = GameSingleton::getResource(mResourcesPos[i]);
		if (r == nullptr)
		{
			mResourcesPos.erase(mResourcesPos.begin() + i);
			rSize--;
		}
		else
		{
			i++;
		}
	}
}

void AI::think(oe::Time dt)
{
	dt *= 2.f;

	if (mAnthill->getResources() > GPRICE1) // Choose the cheapest
	{
		tryBuy();
	}

	if (mCurrentAnt == nullptr || mCurrentAnt->isTurnOver())
	{
		mCurrentAnt = GameSingleton::getAIAnt(mCurrentAntIndex);

		if (mCurrentAnt != nullptr)
		{
			GameSingleton::setCollision(mAnthill->getCoords(), false);
			if (!AStar::canGo(mCurrentAnt->getCoords(), mAnthill->getCoords(), GameSingleton::collisions))
			{
				mCurrentAnt = nullptr;
			}
			GameSingleton::setCollision(mAnthill->getCoords(), true);
		}

		mCurrentAntIndex++;
		if (mCurrentAntIndex > GameSingleton::aiAnts.size())
		{
			mTurnOver = true;
		}
	}
	else
	{
		switch (mCurrentAnt->getType())
		{
			case Ant::Scout: playScout(dt); break;
			case Ant::Worker: playWorker(dt); break;
			case Ant::Soldier: playSoldier(dt); break;
			default: break;
		}
	}
}

bool AI::isTurnOver() const
{
	return mTurnOver;
}

void AI::addResource(const oe::Vector2i& coords)
{
	mResourcesPos.push_back(coords);
}

void AI::tryBuy()
{
	I32 r = oe::Random::get(0, 2);
	if (r == 0)
	{
		tryBuyScout();
		tryBuyWorker();
		tryBuySoldier();
	}
	if (r == 1)
	{
		tryBuyWorker();
		tryBuySoldier();
		tryBuyScout();
	}
	if (r == 2)
	{
		tryBuySoldier();
		tryBuyScout();
		tryBuyWorker();
	}
}

void AI::tryBuyScout()
{
	if (mAnthill->canSpawn(Ant::Scout))
	{
		mAnthill->spawn(Ant::Scout);
	}
}

void AI::tryBuyWorker()
{
	if (mAnthill->canSpawn(Ant::Worker))
	{
		mAnthill->spawn(Ant::Worker);
	}
}

void AI::tryBuySoldier()
{
	if (mAnthill->canSpawn(Ant::Soldier))
	{
		mAnthill->spawn(Ant::Soldier);
	}
}

void AI::playScout(oe::Time dt)
{
	if (mCurrentAnt->getDestination() == Ant::invalidDest)
	{
		if (mCurrentAnt->getResources() > 0)
		{
			mCurrentAnt->goToAnthill();
		}
		else
		{
			if (mTurnNumber < 15)
			{
				goToResource();
			}
			else
			{
				harass();
			}
		}
	}
	else
	{
		mCurrentAnt->goToDestination();
	}
	mCurrentAnt->updateAnt(dt, false);
}

void AI::playWorker(oe::Time dt)
{
	if (mCurrentAnt->getDestination() == Ant::invalidDest)
	{
		if (mCurrentAnt->getResources() > 0)
		{
			mCurrentAnt->goToAnthill();
		}
		else
		{
			goToResource();
		}
	}
	else
	{
		mCurrentAnt->goToDestination();
	}
	mCurrentAnt->updateAnt(dt, false);
}

void AI::playSoldier(oe::Time dt)
{
	if (mCurrentAnt->getDestination() == Ant::invalidDest)
	{
		harass();
	}
	else
	{
		mCurrentAnt->goToDestination();
	}
	mCurrentAnt->updateAnt(dt, false);
}

void AI::goToResource()
{
	std::vector<oe::Vector2i> res; // Best res
	oe::Vector2i coords = mCurrentAnt->getCoords();
	I32 minDistance = 9999;
	U32 rSize = mResourcesPos.size();
	for (U32 i = 0; i < rSize; i++)
	{
		I32 heur = AStar::heuristic(coords, mResourcesPos[i]);
		bool col = GameSingleton::isCollision(mResourcesPos[i]);
		if (heur < minDistance && heur >= 0 && !col)
		{
			res.clear();
			res.push_back(mResourcesPos[i]);
			minDistance = heur;
		}
		else if (heur == minDistance && heur >= 0 && !col)
		{
			res.push_back(mResourcesPos[i]);
		}
	}
	if (!res.empty())
	{
		U32 index = oe::Random::get(0u, res.size() - 1);
		if (res[index] != mCurrentAnt->getCoords())
		{
			mCurrentAnt->goTo(res[index]);
		}
		else
		{
			mCurrentAnt->goToAnthill();
		}
	}
	else
	{
		mCurrentAnt->goToAnthill();
	}
}

void AI::harass()
{
	mEnemies.update();

	std::vector<oe::EntityHandle> res; // Best res
	oe::Vector2i coords = mCurrentAnt->getCoords();
	I32 minDistance = 9999;
	Ant* enemy = nullptr;
	for (auto itr = mEnemies.begin(); itr != mEnemies.end(); ++itr)
	{
		enemy = itr->getAs<Ant>();
		if (enemy != nullptr)
		{
			I32 heur = AStar::heuristic(coords, enemy->getCoords());
			if (heur < minDistance && heur >= 0)
			{
				res.clear();
				res.push_back(*itr);
				minDistance = heur;
			}
			else if (heur == minDistance && heur >= 0)
			{
				res.push_back(*itr);
			}
		}
	}
	if (!res.empty())
	{
		U32 index = oe::Random::get(0u, res.size() - 1);
		mCurrentAnt->goToTarget(res[index]);
	}
	else
	{
		mCurrentAnt->goToTarget(GameSingleton::anthill);
	}
}
