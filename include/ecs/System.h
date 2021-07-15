#pragma once

#include "EcsCommon.h"
#include "ComponentManager.h"
#include "Components.h"

#include <set>

/**
 * A system base class that handles common functionality.
 * @author Ryan Purse
 */
class System
{
public:

    std::set<ecs::entity> mEntities;
protected:
    template<typename Component>
    Component &getComponent(ecs::entity entity)
    {
        return mComponentManager->getComponent<Component>(entity);
    }
private:
    friend class EcsDirector;
    ComponentManager *mComponentManager;
};


