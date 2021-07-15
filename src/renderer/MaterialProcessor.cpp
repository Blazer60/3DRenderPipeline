/**
 * @file MaterialProcessor.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */

#include "MaterialProcessor.h"
#include <glew.h>
#include <format>


MaterialProcessor::MaterialProcessor()
{
    glGenTextures(1, &mRendererId);
    createDefaultMaterial();
}

void MaterialProcessor::init()
{
    for (const auto &entity : mEntities)
    {
        auto &mats = getComponent<std::vector<Material>>(entity);
        auto &textureMats = getComponent<std::vector<MaterialTexture>>(entity);

        std::vector<unsigned int> ids;
        ids.reserve(mats.size());

        for (int i = 0; i < mats.size(); i++)
        {
            mats[i].kDTextureId = mTextureSystem.createTexture(textureMats[i].kDPath);
            ids.emplace_back(addMaterial(mats[i]));
        }

        if (ids.empty()) { ids = { mDefaultId }; }

        auto &renderUniforms = getComponent<RendererUniforms>(entity);
        renderUniforms.materialIds = std::move(ids);
    }
}

void MaterialProcessor::createDefaultMaterial()
{
    Material defaultMat{ glm::vec3(1.f), glm::vec3(1.f), mTextureSystem.createTexture("") };
    mDefaultId = addMaterial(defaultMat);
}

unsigned int MaterialProcessor::addMaterial(const Material &material)
{
    mMaterials.insert({ mNextId, material });
    return mNextId++;
}

void MaterialProcessor::bind() const
{
    glBindTexture(GL_TEXTURE_2D, mRendererId);
    mShader.bind();
}

void MaterialProcessor::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    Shader::unBind();
}

void MaterialProcessor::setupMaterials(const std::vector<unsigned int> &materialIds)
{
    glBindTextureUnit(0, 0);
//    std::vector<int> textureIds;
//    std::vector<glm::vec3> kDiffuse;
    auto i = 0;
    for (const auto &id : materialIds)
    {
        // Find the material.
        auto material = mMaterials.at(id);
        // Set the uniforms based on that material.
        glBindTextureUnit(i, material.kDTextureId);
        mShader.setUniform(std::format("u_materials[{}].kAmbient", i), glm::vec4(material.kAmbient, 1.f));
        mShader.setUniform(std::format("u_materials[{}].kDiffuse", i), glm::vec4(material.kDiffuse, 1.f));
        mShader.setUniform(std::format("u_materials[{}].kDiffuseTexture", i), i);
        mShader.setUniform(std::format("u_materials[{}].kSpecular", i), glm::vec4(material.kSpecular, 1.f));
        mShader.setUniform(std::format("u_materials[{}].nSpecular", i), material.nSpecular);
        i++;
    }
    if (materialIds.empty())
    {
        // Bind the default Texture.
        auto defaultMat = mMaterials[mDefaultId];
        glBindTextureUnit(0, defaultMat.kDTextureId);
        mShader.setUniform(std::format("u_materials[{}].kAmbient", i), glm::vec4(defaultMat.kAmbient, 1.f));
        mShader.setUniform(std::format("u_materials[{}].kDiffuse", i), glm::vec4(defaultMat.kDiffuse, 1.f));
        mShader.setUniform(std::format("u_materials[{}].kDiffuseTexture", i), i);
        mShader.setUniform(std::format("u_materials[{}].kSpecular", i), glm::vec4(defaultMat.kSpecular, 1.f));
        mShader.setUniform(std::format("u_materials[{}].nSpecular", i), defaultMat.nSpecular);
    }
//    mShader.setUniform("u_textures", &textureIds[0], static_cast<int>(textureIds.size()));
//    mShader.setUniform("u_colours", kDiffuse[0], static_cast<int>(kDiffuse.size()));
}
