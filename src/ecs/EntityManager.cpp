/**
 * @file EntityManager.cpp
 * @brief Records all Alive Entities and handles all UUID's.
 * Project: RenderPipeline
 * Initial Version: 28/06/2021
 * @author Ryan Purse
 */

#include "EntityManager.h"

#include <exception>

ecs::entity EntityManager::createEntity()
{
    if (mNextId == ecs::maxEntities)
    {
        throw std::exception("Maximum amount of entities have already been created. Cannot deduce that next UUID for this Entity.");
    }
    mEntityMap[mNextId] = ecs::signature();
    return mNextId++;
}

void EntityManager::destroyEntity(ecs::entity entity)
{
    validateEntity(entity);
    mEntityMap.erase(entity);
}

void EntityManager::setSignature(ecs::entity entity, ecs::signature signature)
{
    validateEntity(entity);
    mEntityMap[entity] = signature;
}

ecs::signature EntityManager::getSignature(ecs::entity entity)
{
    validateEntity(entity);
    return mEntityMap[entity];
}

void EntityManager::validateEntity(ecs::entity entity)
{
    if (mEntityMap.find(entity) == std::end(mEntityMap))
    {
        throw std::exception("Entity does not exist.");
    }
}
