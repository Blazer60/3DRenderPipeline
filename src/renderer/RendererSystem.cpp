/**
 * @file RendererSystem.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 29/06/2021
 * @author Ryan Purse
 */

#include "RendererSystem.h"
#include "Components.h"
#include "Vertex.h"

#include <glew.h>
#include <iostream>
#include <algorithm>
#include <numeric>

RendererSystem::RendererSystem()
{
    glGenBuffers(1, &mVertexBufferId);
    glGenBuffers(1, &mIndexBufferId);
    glGenVertexArrays(1, &mVertexArrayId);
    glGenTextures(1, &mCurrentTexturesId);

    glBindVertexArray(mVertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);

    glEnableVertexAttribArray(0);  // Position
    glEnableVertexAttribArray(1);  // UV Coord
    glEnableVertexAttribArray(2);  // Normal
    glEnableVertexAttribArray(3);  // Tangent
    glEnableVertexAttribArray(4);  // BiTangent
    glEnableVertexAttribArray(5);  // Texture Id

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, uvCoord)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, normal)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, tangent)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, biTangent)));
    glVertexAttribIPointer(5, 1, GL_INT, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, textureId)));

    glClearColor(0.16f, 0.16f, 0.16f, 1.f);
}

void RendererSystem::setMainCamera(ecs::entity entity)
{
    mMainCamera = entity;
}

void RendererSystem::render()
{
    mMaterialProcessor->bind();
    computeModels();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(mVertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);  // Binding a buffer does not attach it to a VAO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);

    // Process Lights
    const auto &cameraMats = getComponent<CameraMatrices>(mMainCamera);
    mPointLightTransformer->setShaderLights(mMainCamera, mMaterialProcessor->mShader);

    for (const auto &entity : mEntities)
    {
        const auto &mesh = getComponent<PolygonalMesh>(entity);
        const auto &uniforms = getComponent<RendererUniforms>(entity);

        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * Vertex::stride(), &mesh.vertices[0], GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof (unsigned int), &mesh.indices[0], GL_DYNAMIC_DRAW);

        // Process Uniforms
        mMaterialProcessor->mShader.setUniform("u_mvp_matrix", uniforms.mvp);
        mMaterialProcessor->mShader.setUniform("u_model_matrix", uniforms.modelMat);
        mMaterialProcessor->mShader.setUniform("u_view_matrix", cameraMats.viewMatrix);
        mMaterialProcessor->setupMaterials(uniforms.materialIds, uniforms.diffuseTexturesId, uniforms.normalMapId);

        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

void RendererSystem::setTextures(const TextureIds &textures) const
{
    glBindTexture(GL_TEXTURE_2D, mCurrentTexturesId);
    for (int i = 0; i < textures.size(); i++)
    {
        glBindTextureUnit(i, textures[i]);
    }
}

void RendererSystem::computeModels()
{
    const auto &camera = getComponent<CameraMatrices>(mMainCamera);

    std::vector<glm::mat4> modelMatrices;
    for (auto &entity : mEntities)
    {
        auto &rendererMaterial = getComponent<RendererUniforms>(entity);
        const auto &transform = getComponent<Transform>(entity);

        glm::mat4 translation = glm::translate(glm::mat4(1.f), transform.position);
        glm::mat4 rotation = glm::toMat4(transform.rotation);
        glm::mat4 scale = glm::scale(glm::mat4(1.f), transform.scale);
        glm::mat4 model = translation * rotation * scale;
        rendererMaterial.mvp = camera.vpMatrix * model;
        rendererMaterial.modelMat = model;
    }
}
