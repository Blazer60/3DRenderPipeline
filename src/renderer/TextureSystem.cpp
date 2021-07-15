/**
 * @file TextureSystem.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 01/07/2021
 * @author Ryan Purse
 */

#include "TextureSystem.h"

#include <glew.h>
#include <stb_image.h>

TextureSystem::TextureSystem()
{
    createWhite();
    stbi_set_flip_vertically_on_load(1);
}

void TextureSystem::createWhite()
{
    auto &data = mTextures["White"];
    glGenTextures(1, &data.id);
    glBindTexture(GL_TEXTURE_2D, data.id);  // Bind the texture (typically will be in texture slot 1).
    glActiveTexture(GL_TEXTURE0);
    setDefaultTextureParams();
    size_t white = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
    data.useCount = 999;  // Safety in case an item try to decrement the white values.
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int TextureSystem::createTexture(const std::string &path)
{
    // Check if the texture already exists.
    if (path.empty()) { return mTextures["White"].id; }
    auto it = mTextures.find(path);
    if (it != std::end(mTextures))
    {
        it->second.useCount++;
        return it->second.id;
    }

    unsigned int id;
    int width, height;
    auto buffer = stbi_load(path.c_str(), &width, &height, nullptr, 4);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glActiveTexture(GL_TEXTURE0);
    setDefaultTextureParams();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    mTextures.insert({ path, { id, 1 }});

    if (buffer)
    {
        stbi_image_free(buffer);
    }
    return id;
}

void TextureSystem::setDefaultTextureParams()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
