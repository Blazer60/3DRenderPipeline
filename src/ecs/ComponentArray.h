#pragma once

#include "EcsCommon.h"

#include <unordered_map>
#include <vector>

/**
 * An interface for the manager since in doesn't know the component type explicitly.
 * @author Ryan Purse
 */
class IComponentArray
{
public:
    virtual void entityDestroyed(ecs::entity entity) = 0;
};

/**
 * Holds an array of components Component.
 * @author Ryan Purse
 */
template<class Component>
class ComponentArray : public IComponentArray
{
    typedef size_t index;
public:
    void insetData(ecs::entity entity, Component component)
    {
        if (mEntityToIndexMap.find(entity) != std::end(mEntityToIndexMap))
        {
            debug::log(ecs::toString<Component>() + " already exists for this component.",
                       debug::severity::Fatal);
        }

        index newIndex = mComponents.size();
        mComponents.push_back(component);
        mEntityToIndexMap[entity] = newIndex;
        mIndexToEntityMap[newIndex] = entity;
    }

    void removeData(ecs::entity entity)
    {
        validateEntity(entity);

        // Copy the elements at the end of the array into the deleted slot.
        index indexOfRemovedElement = mEntityToIndexMap[entity];
        index indexOfLastElement = mComponents.size() - 1;
        std::swap(mComponents[indexOfRemovedElement], mComponents[indexOfLastElement]);
        mComponents.pop_back();

        // Update the maps to point to the new spot.
        auto entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
        mEntityToIndexMap[entityOfLastElement] = indexOfRemovedElement;
        mIndexToEntityMap[indexOfRemovedElement] = entityOfLastElement;

        mEntityToIndexMap.erase(entity);
        mIndexToEntityMap.erase(indexOfLastElement);
    }

    [[nodiscard]] Component &getData(ecs::entity entity)
    {
        validateEntity(entity);
        return mComponents[mEntityToIndexMap[entity]];
    }

    void entityDestroyed(ecs::entity entity) override
    {
        removeData(entity);
    }

protected:
    void validateEntity(ecs::entity entity)
    {
        if (mEntityToIndexMap.find(entity) == std::end(mEntityToIndexMap))
        {
            debug::log("An entity with this component does not exist.", debug::severity::Fatal);
        }
    }

    std::vector<Component> mComponents;  // Components must be contiguous to help cache lines.
    std::unordered_map<ecs::entity, index> mEntityToIndexMap;
    std::unordered_map<index, ecs::entity> mIndexToEntityMap;
};


