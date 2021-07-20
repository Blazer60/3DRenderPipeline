/**
 * @file Scene.cpp
 * @brief Runs a specialised update and render loops
 * Project: RenderPipeline
 * Initial Version: 29/06/2021
 * @author Ryan Purse
 */

#include "Scene.h"
#include "Components.h"
#include "Primitives.h"
#include "Loader.h"
#include "MaterialProcessor.h"


Scene::Scene()
{
    // Must be in this order.
    registerComponents();
    registerSystems();
    registerEntities();

    mRendererSystem->setMainCamera(mMainCamera);
    mRendererSystem->mMaterialProcessor->init();
    mCameraSystem->init();
}

void Scene::registerComponents()
{
    mDirector.registerComponent<Transform>();
    mDirector.registerComponent<PolygonalMesh>();
    mDirector.registerComponent<Camera>();
    mDirector.registerComponent<CameraMatrices>();
    mDirector.registerComponent<CameraController>();
    mDirector.registerComponent<RendererUniforms>();
    mDirector.registerComponent<Textures>();
    mDirector.registerComponent<TextureIds>();
    mDirector.registerComponent<std::vector<Material>>();
    mDirector.registerComponent<std::vector<MaterialTexture>>();
    mDirector.registerComponent<PointLight>();
}

void Scene::registerSystems()
{
    ecs::signature rendererSignature;
    rendererSignature.set(mDirector.getComponentId<Transform>());
    rendererSignature.set(mDirector.getComponentId<PolygonalMesh>());
    rendererSignature.set(mDirector.getComponentId<RendererUniforms>());
    mRendererSystem = mDirector.registerSystem<RendererSystem>();
    mDirector.setSystemSignature<RendererSystem>(rendererSignature);

    ecs::signature materialSignature;
    materialSignature.set(mDirector.getComponentId<RendererUniforms>());
    materialSignature.set(mDirector.getComponentId<std::vector<Material>>());
    materialSignature.set(mDirector.getComponentId<std::vector<MaterialTexture>>());
    mRendererSystem->mMaterialProcessor = mDirector.registerSystem<MaterialProcessor>();
    mDirector.setSystemSignature<MaterialProcessor>(materialSignature);

    ecs::signature lightingSignature;
    lightingSignature.set(mDirector.getComponentId<PointLight>());
    lightingSignature.set(mDirector.getComponentId<Transform>());
    mRendererSystem->mPointLightTransformer = mDirector.registerSystem<PointLightTransformer>();
    mDirector.setSystemSignature<PointLightTransformer>(lightingSignature);

    ecs::signature cameraSystemSignature;
    cameraSystemSignature.set(mDirector.getComponentId<Transform>());
    cameraSystemSignature.set(mDirector.getComponentId<Camera>());
    cameraSystemSignature.set(mDirector.getComponentId<CameraMatrices>());
    mCameraSystem = mDirector.registerSystem<CameraSystem>();
    mDirector.setSystemSignature<CameraSystem>(cameraSystemSignature);

    ecs::signature camControllerSignature;
    camControllerSignature.set(mDirector.getComponentId<CameraController>());
    camControllerSignature.set(mDirector.getComponentId<Transform>());
    camControllerSignature.set(mDirector.getComponentId<CameraMatrices>());
    mCameraControllerSystem = mDirector.registerSystem<CameraControllerSystem>();
    mDirector.setSystemSignature<CameraControllerSystem>(camControllerSignature);
}

void Scene::registerEntities()
{
    auto cube = mDirector.createEntity();
    mDirector.addComponent(cube, Transform());
    mDirector.addComponent(cube, primitives::cube());
    mDirector.addComponent(cube, RendererUniforms());

    auto teapot = mDirector.createEntity();
    mDirector.addComponent(teapot, Transform{ glm::vec3(0.f, 1.f, -5.f) });
    mDirector.addComponent(teapot, loadModel("../res/models/CubeTest.obj"));
//
    auto tank = mDirector.createEntity();
    mDirector.addComponent(tank, Transform{ glm::vec3(15.f, 1.f, 0.f) });
    mDirector.addComponent(tank, loadModel("../res/models/CubesTextures.obj"));

    auto kirb = mDirector.createEntity();
    mDirector.addComponent(kirb, Transform{
            glm::vec3(-15.f, 0.f, 0.f),
            glm::quat(),
            glm::vec3(1.f)
    });
    mDirector.addComponent(kirb, loadModel("../res/models/SphereTextures.obj"));

//    auto light = mDirector.createEntity();
//    mDirector.addComponent(light, PointLight { glm::vec3(1.f) });
//    mDirector.addComponent(light, Transform {
//        glm::vec3(5.f),
//        glm::quat(),
//        glm::vec3(0.1f)
//        });
//    mDirector.addComponent(light, primitives::inverseCube());
//    mDirector.addComponent(light, RendererUniforms());

    mLight = mDirector.createEntity();
    mDirector.addComponent(mLight, PointLight { glm::vec3(1.f) });
    mDirector.addComponent(mLight, Transform {
        glm::vec3(0.f, 5.f, 0.f),
        glm::quat(),
        glm::vec3(0.1f)
    } );
    mDirector.addComponent(mLight, primitives::inverseCube());
    mDirector.addComponent(mLight, RendererUniforms());


    mMainCamera = mDirector.createEntity();
    mDirector.addComponent(mMainCamera, Transform{
            glm::vec3(0.f, -0.7f, -0.8f),
            glm::quat(glm::vec3(0.f, 0.f, 0.f)),
            glm::vec3(1.f)
    });
    mDirector.addComponent(mMainCamera, CameraMatrices());
    mDirector.addComponent(mMainCamera, Camera());
    mDirector.addComponent(mMainCamera, CameraController());
}

void Scene::update(float deltaTime)
{
    mCameraSystem->update();
    mCameraControllerSystem->update(deltaTime);
}

void Scene::render()
{
    mRendererSystem->render();
}

void Scene::renderImGui()
{
    auto &lightPos = mDirector.getComponent<Transform>(mLight);
    auto &lightValues = mDirector.getComponent<PointLight>(mLight);
    ImGui::SliderFloat3("Pos", &lightPos.position[0], -10.f, 10.f);
    ImGui::SliderFloat3("Colour", &lightValues.kDiffuse[0], 0.f, 1.f);
    ImGui::SliderFloat("Intensity", &lightValues.intensity, 0.f, 1.f);
    ImGui::SliderFloat("Fall Off", &lightValues.fallOff, 0.f, 10.f);
    static bool wireFrame;
    ImGui::Checkbox("Wireframe", &wireFrame);
    if (wireFrame) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
}
