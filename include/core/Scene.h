#pragma once

#include "RendererSystem.h"
#include "CameraControllerSystem.h"
#include "CameraSystem.h"
#include "TextureSystem.h"
#include "EcsCommon.h"
#include "EcsDirector.h"

#include <imgui.h>
#include <memory>

/**
 * Runs a specialised update and render loops
 * @author Ryan Purse
 */
class Scene
{
public:
    Scene();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void renderImGui();

protected:
    void registerComponents();
    void registerSystems();
    void registerEntities();

    EcsDirector mDirector;
    ecs::entity mMainCamera{};
    ecs::entity mLight;
    std::shared_ptr<RendererSystem> mRendererSystem;
    std::shared_ptr<CameraSystem> mCameraSystem;
    std::shared_ptr<CameraControllerSystem> mCameraControllerSystem;
};


