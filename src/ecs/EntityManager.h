#pragma once

#include "EcsCommon.h"

#include <vector>
#include <unordered_map>

/**
 * Records all Alive Entities and handles all UUID's.
 * @author Ryan Purse
 */
class EntityManager
{
public:
    ecs::entity createEntity();
    void destroyEntity(ecs::entity);

    void setSignature(ecs::entity entity, ecs::signature signature);
    ecs::signature getSignature(ecs::entity entity);

protected:
    void validateEntity(ecs::entity entity);
    size_t mNextId{ 0 };
    std::unordered_map<ecs::entity, ecs::signature> mEntityMap;
    size_t mCount{ 0 };
};


