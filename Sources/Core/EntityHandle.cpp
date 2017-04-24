#include "EntityHandle.hpp"
#include "World.hpp"

namespace oe
{

EntityHandle::EntityHandle()
	: mEntityId(0)
	, mHandleIndex(0)
	, mWorld(nullptr)
{
}

EntityHandle::EntityHandle(World* world, const Entity& entity, U32 handleIndex)
	: mEntityId(entity.getId())
	, mHandleIndex(handleIndex)
	, mWorld(world)
{
}

EntityHandle::EntityHandle(const EntityHandle& handle)
	: mEntityId(handle.mEntityId)
    , mHandleIndex(handle.mHandleIndex)
	, mWorld(handle.mWorld)
{
}

void EntityHandle::operator=(const EntityHandle& handle)
{
	mEntityId = handle.mEntityId;
	mHandleIndex = handle.mHandleIndex;
	mWorld = handle.mWorld;
}

Entity* EntityHandle::get() const
{
	Entity* entity = (mWorld != nullptr) ? mWorld->getEntityFromHandleIndex(mHandleIndex) : nullptr;
	if (entity != nullptr && entity->getId() == mEntityId)
	{
		return entity;
	}
	return nullptr;
}

bool EntityHandle::isValid() const
{
	if (mEntityId == 0)
	{
		return false;
	}
	Entity* entity = (mWorld != nullptr) ? mWorld->getEntityFromHandleIndex(mHandleIndex) : nullptr;
	return entity != nullptr && entity->getId() == mEntityId;
}

bool EntityHandle::operator==(const EntityHandle& handle) const
{
	return mEntityId == handle.mEntityId;
}

bool EntityHandle::operator!=(const EntityHandle& handle) const
{
	return mEntityId != handle.mEntityId;
}

void EntityHandle::invalidate()
{
	mEntityId = 0;
}

UID EntityHandle::getEntityId() const
{
	return mEntityId;
}

U32 EntityHandle::getHandleIndex() const
{
	return mHandleIndex;
}

} // namespace oe