#pragma once

#include "System.h"
#include "Shader.h"
#include "TextureSystem.h"

/**
 * Handles assignment of uniforms from entities before being rendered to the screen.
 * @author Ryan Purse
 */
class MaterialProcessor : public System
{
public:
    MaterialProcessor();
    void init();
    void createDefaultMaterial();

    void bind() const;
    static void unbind();
    void setupMaterials(const std::vector<unsigned int> &materialIds);
    unsigned int addMaterial(const Material &material);

    Shader mShader { "../res/shaders/Default.shader" };
protected:
    unsigned int mNextId { 0 };
    std::unordered_map<unsigned int, Material> mMaterials;
    TextureSystem mTextureSystem;
    unsigned int mRendererId { 0 };
    unsigned int mDefaultId{ 0 };
};


