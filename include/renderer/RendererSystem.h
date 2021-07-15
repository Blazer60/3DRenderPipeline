#pragma once

#include "System.h"
#include "Shader.h"
#include "MaterialProcessor.h"
#include "PointLightTransformer.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <memory>


/**
 * Handles rendering entities who have a mesh and transform component.
 * @author Ryan Purse
 */
class RendererSystem : public System
{
public:
    RendererSystem();
    void setMainCamera(ecs::entity entity);
    void render();
    std::shared_ptr<MaterialProcessor> mMaterialProcessor;
    std::shared_ptr<PointLightTransformer> mPointLightTransformer;
protected:
    void computeModels();
    void setTextures(const TextureIds& textures) const;
    unsigned int mVertexBufferId{};
    unsigned int mVertexArrayId{};
    unsigned int mIndexBufferId{};
    unsigned int mCurrentTexturesId{};

    ecs::entity mMainCamera{};
};


