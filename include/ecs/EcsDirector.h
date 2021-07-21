#pragma once

#include "EcsCommon.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include "Loader.h"


/**
 * A function list for all of the managers.
 * @author Ryan Purse
 */
class EcsDirector
{
public:
    // Entity Methods //
    ecs::entity createEntity()
    {
        return mEntityManager.createEntity();
    }

    void destroyEntity(ecs::entity entity)
    {
        mEntityManager.destroyEntity(entity);
        mComponentManager.entityDestroyed(entity);
        mSystemManager.entityDestroyed(entity);
    }

    // Component Methods //
    template<typename Component>
    void registerComponent()
    {
        mComponentManager.registerComponent<Component>();
    }

    template<typename Component>
    void addComponent(ecs::entity entity, Component component)
    {
        mComponentManager.addComponent(entity, component);
        auto signature = mEntityManager.getSignature(entity);
        signature.set(mComponentManager.getComponentId<Component>());
        mEntityManager.setSignature(entity, signature);
        mSystemManager.entitySignatureChanged(entity, signature);
    }

    template<>
    void addComponent<ModelData>(ecs::entity entity, ModelData component)
    {
        auto &[mesh, materials, matTextures] = component;
        if (mesh.vertices.empty()) { return; }  // Object failed to load.
        addComponent(entity, mesh);
        addComponent(entity, RendererUniforms());
        addComponent(entity, materials);
        addComponent(entity, matTextures);
    }

    template<typename Component>
    void removeComponent(ecs::entity entity)
    {
        mComponentManager.removeComponent<Component>(entity);
        auto signature = mEntityManager.getSignature(entity);
        signature.set(mComponentManager.getComponentId<Component>(), false);
        mEntityManager.setSignature(entity, signature);
        mSystemManager.entitySignatureChanged(entity, signature);
    }

    template<typename Component>
    Component &getComponent(ecs::entity entity)
    {
        return mComponentManager.getComponent<Component>(entity);
    }

    template<typename Component>
    ecs::componentId getComponentId()
    {
        return mComponentManager.getComponentId<Component>();
    }

    // System Methods //
    template<typename sys>
    std::shared_ptr<sys> registerSystem()
    {
        auto system = mSystemManager.registerSystem<sys>();
        system->mComponentManager = &mComponentManager;
        return system;
    }

    template<typename sys>
    void setSystemSignature(ecs::signature signature)
    {
        mSystemManager.setSignature<sys>(signature);
    }

protected:
    ComponentManager    mComponentManager;
    EntityManager       mEntityManager;
    SystemManager       mSystemManager;
};


