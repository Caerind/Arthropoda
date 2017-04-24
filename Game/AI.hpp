#ifndef AI_HPP
#define AI_HPP

#include "Ant.hpp"
#include "GameSingleton.hpp"

class AI
{
	public:
		AI();

		void init();

		void startTurn();
		void think(oe::Time dt);

		bool isTurnOver() const;

		void addResource(const oe::Vector2i& coords);

	private:
		void tryBuy();
		void tryBuyScout();
		void tryBuyWorker();
		void tryBuySoldier();

		void playScout(oe::Time dt);
		void playWorker(oe::Time dt);
		void playSoldier(oe::Time dt);

		void goToResource();
		void harass();

	private:
		Anthill* mAnthill;
		Anthill* mAnthillPlayer;
		oe::Vector2i mAnthillPos;
		oe::Vector2i mAnthillPosPlayer;
		U32 mTurnNumber;
		bool mTurnOver;
		Ant* mCurrentAnt;
		U32 mCurrentAntIndex;

		std::vector<oe::Vector2i> mResourcesPos;
		oe::EntityList mEnemies;
};

#endif // AI_HPP
