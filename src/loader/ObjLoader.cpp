/**
 * @file ObjLoader.cpp
 * @brief A Model loader the specifically handles .obj files.
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */


#include "ObjLoader.h"
#include "LoaderCommon.h"
#include "MtlLoader.h"

#include <fstream>
#include <unordered_map>
#include <functional>
#include <iostream>


ModelData loadObj(std::string_view path)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    std::unordered_map<std::string, unsigned int> indexMap;
    std::vector<vertexData> data;

    std::unordered_map<std::string, size_t> matMap;
    std::vector<ObjMaterial> materials;

    unsigned int currentMatIndex = 0;

    // Function name to method conversion.
    std::unordered_map<std::string_view, std::function<void(std::string_view)>> functionMap{
            {"#",      [](std::string_view args){}},                            // Comment.

            {"o",      [](std::string_view args){}},                            // Object.

            {"s",      [](std::string_view args){}},                            // Smoothing toggle.

            { "g", [](std::string_view args){}},  // group

            {"mtllib", [&](std::string_view args){                      // Material Library.
                auto pair = loadMat("../res/models/" + std::string(args));
                matMap = std::move(pair.first);
                materials = std::move(pair.second);
            }},

            {"usemtl", [&](std::string_view args){                      // Use Material in Library.
                currentMatIndex = matMap.find(std::string(args))->second;
            }},

            {"v",      [&positions](std::string_view args){             // Vertex Position (x, y, z).
                positions.emplace_back(createVec<3>(args)); }},

            {"vt",     [&textureCoords](std::string_view args){         // Vertex Texture Coords.
                textureCoords.emplace_back(createVec<2>(args)); }},

            {"vn",     [&normals](std::string_view args){               // Vertex Normals.
                normals.emplace_back(createVec<3>(args)); }},

            {"f",      [&](std::string_view args){                      // Face [3, 4] vertices.
                auto argsVec = splitArgs(args);
                // Find out if it's in the map or not.
                for (const auto &item : argsVec)
                {
                    unsigned int nextIndex = 0;
                    if (!indexMap.contains(item))
                    {
                        // Create Vertex if not.
                        nextIndex = static_cast<unsigned int>(data.size());
                        indexMap.insert({item, nextIndex});
                        data.emplace_back(createVertexData(item, currentMatIndex));
                    }
                    else
                    {
                        nextIndex = indexMap[item];
                    }
                    // Append index to indices.
                    indices.emplace_back(nextIndex);
                }
            }},
    };

    convertFile(path, functionMap);

    std::vector<MaterialTexture> textures;
    std::vector<Material> outMaterials;
    textures.reserve(materials.size());
    for (const auto &material : materials)
    {
        textures.push_back({ material.mapKd });
        outMaterials.push_back({ material.ka, material.kd, 0, material.ks, material.ns });
    }

    return {{ convertVertexData(data, positions, textureCoords, normals), indices }, outMaterials, textures };
}

std::vector<Vertex> convertVertexData(const std::vector<vertexData> &data, const std::vector<glm::vec3> &positions,
                                      const std::vector<glm::vec2> &textureCoords,
                                      const std::vector<glm::vec3> &normals)
{
    std::vector<Vertex> outVertices;
    outVertices.reserve(data.size());
    for (const auto &item : data)
    {
        const glm::vec3 position = item.positionIndex == 0 ? glm::vec3(0.f) : positions[item.positionIndex - 1];
        const glm::vec2 textureCoord = item.textureIndex == 0 ? glm::vec2(0.f) : textureCoords[item.textureIndex - 1];
        const glm::vec3 normal = item.normalIndex == 0 ? glm::vec3(0.f) : normals[item.normalIndex - 1];

        outVertices.push_back({
            position,
            normal,
            textureCoord,
            static_cast<int>(item.materialIndex)
        });
    }
    return outVertices;
}

vertexData createVertexData(std::string_view args, unsigned int materialIndex)
{
    vertexData data{ 0, 0, 0, materialIndex };
    auto nums = splitArgs(args, '/');
    switch (nums.size())
    {
        // Switch case deliberately cascades down.
        case 3:
            data.normalIndex = std::stoi(nums[2]);
        case 2:
            if (!nums[1].empty())
            {
                data.textureIndex = std::stoi(nums[1]);
            }
        case 1:
        default:
            data.positionIndex = std::stoi(nums[0]);
            break;
    }
    return data;
}


