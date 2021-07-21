#pragma once

#include "EcsCommon.h"
#include "System.h"

#include <unordered_map>
#include <memory>

/**
 * Handles all of the systems that are assigned to it.
 * @author Ryan Purse
 */
class SystemManager
{
public:
    template<typename sys>
    std::shared_ptr<sys> registerSystem()
    {
        auto typeId = typeid(sys).hash_code();
        if (mSystems.find(typeId) != std::end(mSystems))
        {
            debug::log("System " + ecs::toString<sys>() + " has already been registered.",
                    debug::severity::Fatal);
        }
        auto system = std::make_shared<sys>();
        mSystems.insert({ typeId, system });
        return system;
    }

    template<typename sys>
    void setSignature(ecs::signature signature)
    {
        auto typeId = typeid(sys).hash_code();
        if (mSystems.find(typeId) == std::end(mSystems))
        {
            debug::log("System " + ecs::toString<sys>() + " has not been registered.", debug::severity::Fatal);
        }
        mSignatures.insert({ typeId, signature });
    }

    void entityDestroyed(ecs::entity entity)
    {
        // Erase a destroyed entity from all system lists.
        for (const auto &[_, system] : mSystems) { system->mEntities.erase(entity); }
    }

    void entitySignatureChanged(ecs::entity entity, ecs::signature entitySignature)
    {
        // Notify each system that an entity's signature changed.
        for (const auto &[type, system] : mSystems)
        {
            const auto &systemSignature = mSignatures[type];
            if ((entitySignature & systemSignature) == systemSignature)
            {
                system->mEntities.insert(entity);
            }
            else
            {
                system->mEntities.erase(entity);
            }
        }
    }

protected:
    // Map from system type id to a signature.
    std::unordered_map<size_t, ecs::signature> mSignatures;

    // Map from system type id to a system pointer.
    std::unordered_map<size_t, std::shared_ptr<System>> mSystems;
};


