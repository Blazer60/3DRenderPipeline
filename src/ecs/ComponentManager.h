#pragma once

#include "EcsCommon.h"
#include "ComponentArray.h"

#include <unordered_map>
#include <memory>

/**
 * Handles all of the component arrays.
 * @author Ryan Purse
 */
class ComponentManager
{
public:
    template<typename Component>
    void registerComponent()
    {
        auto typeId = typeid(Component).hash_code();
        if (mComponentIds.find(typeId) != std::end(mComponentIds))
        {
            debug::log("Component " + ecs::toString<Component>() + " has already been registered.",
                       debug::severity::Fatal);
        }

        // Add the component to the component Id map.
        mComponentIds.insert({ typeId, mNextComponentId++ });

        // Create a component array pointer and add it to the component array map.
        mComponentArrays.insert({ typeId, std::make_shared<ComponentArray<Component>>() });
    }

    template<typename Component>
    ecs::componentId getComponentId()
    {
        auto typeId = typeid(Component).hash_code();
        validateComponent(typeId);
        return mComponentIds[typeId];
    }

    template<typename Component>
    void addComponent(ecs::entity entity, Component component)
    {
        getComponentArray<Component>()->insetData(entity, component);
    }

    template<typename Component>
    void removeComponent(ecs::entity entity)
    {
        getComponentArray<Component>()->removeData(entity);
    }

    template<typename Component>
    Component &getComponent(ecs::entity entity)
    {
        return getComponentArray<Component>()->getData(entity);
    }

    void entityDestroyed(ecs::entity entity)
    {
        for (auto &[_, component] : mComponentArrays) { component->entityDestroyed(entity); }
    }

protected:
    template<typename Component>
    std::shared_ptr<ComponentArray<Component>> getComponentArray()
    {
        auto typeId = typeid(Component).hash_code();
        validateComponent(typeId);
        return std::static_pointer_cast<ComponentArray<Component>>(mComponentArrays[typeId]);
    }

    void validateComponent(size_t typeId)
    {
        if (mComponentIds.find(typeId) == std::end(mComponentIds))
        {
            debug::log("Component has not been registered before use.", debug::severity::Fatal);
        }
    }

    std::unordered_map<size_t, ecs::componentId> mComponentIds;
    std::unordered_map<size_t, std::shared_ptr<IComponentArray>> mComponentArrays;
    ecs::componentId mNextComponentId{ 0 };
};


