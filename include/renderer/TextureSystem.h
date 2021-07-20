#pragma once

#include "System.h"

#include <array>
#include <string>
#include <unordered_map>

/**
 * Holds all of the textures for the renderer.
 * @author Ryan Purse
 */
class TextureSystem
{
    struct textureData
    {
        unsigned int id { 0 };
        int useCount    { 0 };
    };
public:
    TextureSystem();
    unsigned int createTexture(const std::string &path);
    static unsigned int createTextureArray(const std::vector<std::string> &paths);
    static void createWhite(int width, int height, int index);
    void createWhite();
    static void setDefaultTextureParams();
protected:
    std::unordered_map<std::string, textureData> mTextures;
};


