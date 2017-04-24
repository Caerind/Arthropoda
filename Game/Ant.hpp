#ifndef ANT_HPP
#define ANT_HPP

#include "MapEntity.hpp"
#include "Pathfinding.hpp"

class Anthill;

class Ant : public MapEntity
{
	public:
		Ant(oe::World& world);

		static const oe::Vector2i invalidDest;

		enum Type
		{
			Scout,
			Worker,
			Soldier,
		};

		void goTo(I32 x, I32 y);
		void goTo(const oe::Vector2i& coords);
		void goToAnthill();
		void goToDestination();
		void goToTarget(oe::EntityHandle target);

		void invalidateTarget();

		bool canPlay() const;
		bool isTurnOver() const;
		void endTurn();
		void reset();
		void resetDest();
		U32 getPM() const;

		U32 getCap() const;
		bool isFullCap() const;

		bool canAttack();
		void attack();

		bool updateAnt(oe::Time dt, bool selected);

		void select();
		void unselect();

		void setType(Type type);
		Type getType() const;

		U32 getDistance() const;
		U32 getResourcesMax() const;
		U32 getDamage() const;
		const oe::Vector2i& getDestination() const;

		bool isAdjacent(const oe::Vector2i& coords) const;

		static U32 getPrice(Type antType);
		static U32 getDistance(Type antType);
		static U32 getResourcesMax(Type antType);
		static U32 getLife(Type antType);
		static U32 getDamage(Type antType);

	private:
		void selectedOverlay(bool displayMovements);
		void tryCollectResources();
		void tryDeposit();

		Anthill& getAnthill();
		Anthill& getAnthillEnemy();

	private:
		oe::SpriteComponent mSprite;
		oe::SpriteComponent mSelectionSprite;
		Type mType;

		U32 mPM;
		oe::Vector2i mDestination;
		std::list<oe::Vector2i> mPath;

		oe::Time mTime;
		bool mMoving;
		oe::Vector2 mStart;
		oe::Vector2 mEnd;

		oe::EntityHandle mTargetHandle; // Can be enemy anthill or enemy ant -> so get it has map entity
		bool mCanAttack;

};

#endif // ANT_HPP
