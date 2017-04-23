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
}

void AI::think(oe::Time dt)
{
	// If have money, try to buy
	if (mAnthill->getResources() > GPRICE1) // TODO : Choose the cheapest
	{
		tryBuy();
	}

	// Take an ant and think what to do with it
	if (mCurrentAnt == nullptr || mCurrentAnt->isTurnOver())
	{
		mCurrentAnt = GameSingleton::getAIAnt(mCurrentAntIndex);
		mCurrentAntIndex++;
		if (mCurrentAntIndex >= GameSingleton::aiAnts.size()) // TODO : >= or > ?
		{
			endTurn();
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

void AI::endTurn()
{
	mTurnOver = true;
}

bool AI::isTurnOver() const
{
	return mTurnOver;
}

bool AI::isDead() const
{
	return mAnthill->getLife() <= 0;
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
	// Scout farm resources if nb turn < x2 (medium)
	if (mCurrentAnt->getDestination() == Ant::invalidDest)
	{
		if (mTurnNumber < AIX2)
		{
			if (mCurrentAnt->isFullCap())
			{
				mCurrentAnt->goToAnthill();
			}
			else
			{
				findResource();
			}
		}
		else
		{
			harass();
		}
	}
	mCurrentAnt->updateAnt(dt, false);
}

void AI::playWorker(oe::Time dt)
{
	// Worker farm resources if nb turn < x3 (very high)
	if (mCurrentAnt->getDestination() == Ant::invalidDest)
	{
		if (mCurrentAnt->isFullCap())
		{
			mCurrentAnt->goToAnthill();
		}
		else if (mTurnNumber < AIX3)
		{
			findResource();
		}
		else
		{
			harass();
		}
	}
	mCurrentAnt->updateAnt(dt, false);
}

void AI::playSoldier(oe::Time dt)
{
	// Soldier farm resources if nb turn < x1 (low)
	if (mCurrentAnt->getDestination() == Ant::invalidDest)
	{
		if (mTurnNumber < AIX1)
		{
			if (mCurrentAnt->isFullCap())
			{
				mCurrentAnt->goToAnthill();
			}
			else
			{
				findResource();
			}
		}
		else
		{
			harass();
		}
	}
	mCurrentAnt->updateAnt(dt, false);
}

void AI::findResource()
{
}

void AI::harass()
{
}
