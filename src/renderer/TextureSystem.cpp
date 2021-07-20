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
#include <iostream>

TextureSystem::TextureSystem()
{
    createWhite();
    stbi_set_flip_vertically_on_load(1);
}

void TextureSystem::createWhite()
{
    auto &data = mTextures["White"];
    glGenTextures(1, &data.id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, data.id);  // Bind the texture (typically will be in texture slot 1).
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
    unsigned char *buffer = stbi_load(path.c_str(), &width, &height, nullptr, 4);
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    setDefaultTextureParams();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
//    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
//    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    mTextures.insert({ path, { id, 1 }});

    if (buffer)
    {
        stbi_image_free(buffer);
    }
    return id;
}

unsigned int TextureSystem::createTextureArray(const std::vector<std::string> &paths)
{
    int width   { 1 };
    int height  { 1 };
    std::vector<unsigned char *> buffers(paths.size(), nullptr);
    unsigned int id;

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id);
    setDefaultTextureParams();

    for (int i = 0; i < paths.size(); ++i)
    {
        if (paths[i].empty()) { continue; }
        int currWidth, currHeight;
        buffers[i] = stbi_load(paths[i].c_str(), &currWidth, &currHeight, nullptr, 4);
        if (width == 1)
        {
            width = currWidth;
            height = currHeight;
        }
        // All textures must be the same size. Path does not exists
        if (currWidth != width || currHeight != height || !buffers[i]) { throw std::exception(); }
    }

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, static_cast<int>(paths.size()));
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (buffers[i])
        {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffers[i]);
            stbi_image_free(buffers[i]);
        }
        else { createWhite(width, height, i); }
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    return id;
}

void TextureSystem::createWhite(int width, int height, int index)
{
    auto *white = new unsigned char[4 * width * height];
    memset(white, 0xff, 4 * width * height);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, white);
    delete[] white;
}

void TextureSystem::setDefaultTextureParams()
{
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
