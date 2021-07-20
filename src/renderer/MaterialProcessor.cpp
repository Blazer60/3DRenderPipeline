/**
 * @file MaterialProcessor.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */

#include "MaterialProcessor.h"
#include <glew.h>


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

        std::vector<std::string> kDPaths;
        kDPaths.reserve(textureMats.size());
        for (const auto &textureMat : textureMats)
        {
            kDPaths.emplace_back(textureMat.kDPath);
        }

        unsigned int kDiffusesId = TextureSystem::createTextureArray(kDPaths);

        std::vector<unsigned int> ids;
        ids.reserve(mats.size());

        for (int i = 0; i < mats.size(); i++)
        {
            mats[i].kDTextureIndex = i;
            ids.emplace_back(addMaterial(mats[i]));
        }

//        if (ids.empty()) { ids = { mDefaultId }; }

        auto &renderUniforms = getComponent<RendererUniforms>(entity);
        renderUniforms.materialIds = std::move(ids);
        renderUniforms.diffuseTexturesId = kDiffusesId;
    }
}

void MaterialProcessor::createDefaultMaterial()
{
    Material defaultMat{ glm::vec3(1.f), glm::vec3(1.f), mTextureSystem.createTexture("") };
    mDefaultId = addMaterial(defaultMat);
    mDefaultKDiffuseTextureId = TextureSystem::createTextureArray({""});
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

void MaterialProcessor::setupMaterials(const std::vector<unsigned int> &materialIds, unsigned int diffuseTextureIds)
{
    glActiveTexture(GL_TEXTURE0);
    if (diffuseTextureIds == 0) { glBindTexture(GL_TEXTURE_2D_ARRAY, mDefaultKDiffuseTextureId); }
    else { glBindTexture(GL_TEXTURE_2D_ARRAY, diffuseTextureIds); }
    mShader.setUniform("u_kDiffuseTextures", 0);

    auto i = 0;
    for (const auto &id : materialIds)
    {
        // Find the material.
        auto material = mMaterials.at(id);
        // Set the uniforms based on that material.
        std::string stringI = std::to_string(i);
        mShader.setUniform("u_materials[" + stringI + "].kAmbient", glm::vec4(material.kAmbient, 1.f));
        mShader.setUniform("u_materials[" + stringI + "].kDiffuse", glm::vec4(material.kDiffuse, 1.f));
        mShader.setUniform("u_materials[" + stringI + "].kSpecular", glm::vec4(material.kSpecular, 1.f));
        mShader.setUniform("u_materials[" + stringI + "].nSpecular", material.nSpecular);
        i++;
    }
    if (materialIds.empty())
    {
        // Bind the default Texture.
        auto defaultMat = mMaterials[mDefaultId];
        mShader.setUniform("u_materials[0].kAmbient", glm::vec4(defaultMat.kAmbient, 1.f));
        mShader.setUniform("u_materials[0].kDiffuse", glm::vec4(defaultMat.kDiffuse, 1.f));
        mShader.setUniform("u_materials[0].kSpecular", glm::vec4(defaultMat.kSpecular, 1.f));
        mShader.setUniform("u_materials[0].nSpecular", defaultMat.nSpecular);
    }
}
