/**
 * @file LoaderCommon.h
 * @brief Common Functions shared across all of the types of loaders
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */


#pragma once

#include "Components.h"

#include <glm.hpp>
#include <string>
#include <charconv>
#include <unordered_map>
#include <functional>

/**
 * Material provided by a .mtl file.
 */
struct ObjMaterial
{
    float ns{ 225.f };
    glm::vec3 ka{ 0.f };
    glm::vec3 kd{ 1.f };
    glm::vec3 ks{ 0.f };
    glm::vec3 ke{ 0.f };
    float ni{ 1.45f };
    float d{ 1.f };
    int illum{ 2 };
    std::string mapKa;
    std::string mapKd;
    std::string mapKs;
    std::string mapKe;
    std::string mapNormal;
};

/**
 * Indexes to each piece of data a vertex uses.
 */
struct vertexData
{
    size_t positionIndex;
    size_t textureIndex;
    size_t normalIndex;
    size_t materialIndex;
};

struct ModelData
{
    PolygonalMesh mesh;
    std::vector<Material> materials;
    std::vector<MaterialTexture> matTextures;
};

/**
 * Splits incoming arguments by a deliminator. e.g.: "65 23 33" becomes "65", "23", "33"
 * @param args
 * @param delim
 * @return Arguments split into individual strings.
 */
std::vector<std::string> splitArgs(std::string_view args, char delim=' ');

/**
 * Gets the first float at the start of a string. The first character must comply to a number. E.g.: 64, -32, 3.14f.
 * @param arg
 * @return
 */
float getFloat(std::string_view arg);

/**
 * Extracts a vector of size count from a string. An error if thrown if not enough numbers are provided.
 * Additional numbers are ignored.
 * @tparam count Numbers of components that vector has.
 * @param args Where the incoming numbers are from.
 * @return glm::vec[count]
 */
template<size_t count>
glm::vec<count, float, glm::defaultp> createVec(std::string_view args)
{
    const char* nextNumber = args.data();
    glm::vec<count, float, glm::defaultp> position(0.f);
    for (int i = 0; i < count; ++i)
    {
        // Pointer to the end of the last number, Error code.
        auto [pointer, ec] = std::from_chars(nextNumber, nextNumber + args.size(), position[i]);
        if (ec != std::errc())  // An error was produced by the args list.
        {
            debug::log("Invalid args (" + std::string(args) + ") for vector construction",
                       debug::severity::Major);
        }
        nextNumber = pointer + 1;  // There is always a space between numbers.
    }

    return position;
}

void convertFile(std::string_view path, const std::unordered_map<std::string_view, std::function<void(std::string_view)>> &functionMap);
