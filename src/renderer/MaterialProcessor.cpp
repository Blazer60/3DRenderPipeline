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
        std::vector<std::string> normalsMapPaths;

        kDPaths.reserve(textureMats.size());
        normalsMapPaths.reserve(textureMats.size());

        for (const auto &textureMat : textureMats)
        {
            kDPaths.emplace_back(textureMat.kDPath);
            normalsMapPaths.emplace_back(textureMat.normalMapPath);
        }

        unsigned int kDiffusesId = TextureSystem::createTextureArray(kDPaths, TextureSystem::Diffuse);
        unsigned int normalMapsId = TextureSystem::createTextureArray(normalsMapPaths, TextureSystem::Diffuse);

        std::vector<unsigned int> ids;
        ids.reserve(mats.size());

        for (int i = 0; i < mats.size(); i++)
        {
            mats[i].kDTextureIndex = i;
            mats[i].normalMapIndex = i;
            ids.emplace_back(addMaterial(mats[i]));
        }

//        if (ids.empty()) { ids = { mDefaultId }; }

        auto &renderUniforms = getComponent<RendererUniforms>(entity);
        renderUniforms.materialIds = std::move(ids);
        renderUniforms.diffuseTexturesId = kDiffusesId;
        renderUniforms.normalMapId = normalMapsId;
    }
}

void MaterialProcessor::createDefaultMaterial()
{
    Material defaultMat{ glm::vec3(1.f), glm::vec3(1.f), mTextureSystem.createTexture("") };
    mDefaultId = addMaterial(defaultMat);

    mDefaultKDiffuseTextureId = TextureSystem::createTextureArray({""}, TextureSystem::Diffuse);
    mDefaultNormalTextureId   = TextureSystem::createTextureArray({""}, TextureSystem::Normal);
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

void MaterialProcessor::setupMaterials(const std::vector<unsigned int> &materialIds, unsigned int diffuseTextureIds,
                                       unsigned int normalMapIds)
{
    glActiveTexture(GL_TEXTURE0);
    if (diffuseTextureIds == 0) { glBindTexture(GL_TEXTURE_2D_ARRAY, mDefaultKDiffuseTextureId); }
    else { glBindTexture(GL_TEXTURE_2D_ARRAY, diffuseTextureIds); }
    mShader.setUniform("u_diffuse_map_textures", 0);

    glActiveTexture(GL_TEXTURE0 + 1);
    if (normalMapIds == 0) { glBindTexture(GL_TEXTURE_2D_ARRAY, mDefaultNormalTextureId); }
    else { glBindTexture(GL_TEXTURE_2D_ARRAY, normalMapIds); }
    mShader.setUniform("u_normal_map_textures", 1);

    auto i = 0;
    for (const auto &id : materialIds)
    {
        // Find the material.
        auto material = mMaterials.at(id);
        // Set the uniforms based on that material.
        std::string stringI = std::to_string(i);
        mShader.setUniform("u_materials[" + stringI + "].k_ambient", glm::vec4(material.kAmbient, 1.f));
        mShader.setUniform("u_materials[" + stringI + "].k_diffuse", glm::vec4(material.kDiffuse, 1.f));
        mShader.setUniform("u_materials[" + stringI + "].k_specular", glm::vec4(material.kSpecular, 1.f));
        mShader.setUniform("u_materials[" + stringI + "].n_specular", material.nSpecular);
        i++;
    }
    if (materialIds.empty())
    {
        // Bind the default Texture.
        auto defaultMat = mMaterials[mDefaultId];
        mShader.setUniform("u_materials[0].k_ambient", glm::vec4(defaultMat.kAmbient, 1.f));
        mShader.setUniform("u_materials[0].k_diffuse", glm::vec4(defaultMat.kDiffuse, 1.f));
        mShader.setUniform("u_materials[0].k_specular", glm::vec4(defaultMat.kSpecular, 1.f));
        mShader.setUniform("u_materials[0].n_specular", defaultMat.nSpecular);
    }
}
