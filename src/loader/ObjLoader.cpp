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
#include <filesystem>

/**
 * Computes the tangents and bi-tangents for all of the incoming vertices.
 * @note The function assumes that there is at least three incoming vertices
 * and that all vertices live on the same plane (flat).
 * @param uniqueIndices The vertices that you want to modify.
 */
void computeTangents(const std::vector<unsigned int> &uniqueIndices, std::vector<Vertex> &vertices);

ModelData loadObj(std::string_view path)
{
    std::filesystem::path objPath(path);
    objPath = objPath.remove_filename();

    // Information that is stored in an .obj file.
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::vec3> normals;

    // Generated on the fly.
    std::vector<unsigned int> indices;
    std::unordered_map<std::string, unsigned int> vertexLocations;
    std::vector<Vertex> vertices;

    // Information stored in an attached .mtl file.
    std::unordered_map<std::string, size_t> matMap;
    std::vector<ObjMaterial> materials;

    int currentMatIndex = 0;

    // Function name to method conversion.
    std::unordered_map<std::string_view, std::function<void(std::string_view)>> functionMap{
            {"#",      [](std::string_view args){}},                            // Comment.

            {"o",      [](std::string_view args){}},                            // Object.

            {"s",      [](std::string_view args){}},                            // Smoothing toggle.

            { "g", [](std::string_view args){}},                                // group

            {"mtllib", [&](std::string_view args){                      // Material Library.
                const std::filesystem::path materialPath = std::filesystem::path(args);
                const std::string pathString = materialPath.is_relative() ? objPath.string() + std::string(args) : std::string(args);
                auto pair = loadMat(pathString);
                matMap = std::move(pair.first);
                materials = std::move(pair.second);
            }},

            {"usemtl", [&](std::string_view args){                      // Use Material in Library.
                currentMatIndex = static_cast<int>(matMap.find(std::string(args))->second);
            }},

            {"v",      [&positions](std::string_view args){             // Vertex Position (x, y, z).
                positions.emplace_back(createVec<3>(args)); }},

            {"vt",     [&textureCoords](std::string_view args){         // Vertex Texture Coords.
                textureCoords.emplace_back(createVec<2>(args)); }},

            {"vn",     [&normals](std::string_view args){               // Vertex Normals.
                normals.emplace_back(createVec<3>(args)); }},

            {"f",      [&](std::string_view args){                      // Face [3, *] vertices.
                std::vector<std::string> vertexIdentifiers = splitArgs(args);
                std::vector<unsigned int> uniqueIndices;
                for (const auto &vertexIdentifier : vertexIdentifiers)
                {
                    auto it = vertexLocations.find(vertexIdentifier);
                    if (it != vertexLocations.end())
                    {
                        uniqueIndices.emplace_back(it->second);
                        continue;
                    }

                    vertices.emplace_back(createBaseVertex(
                            vertexIdentifier, positions,
                            textureCoords, normals,
                            currentMatIndex
                    ));
                    unsigned int lastIndex = static_cast<int>(vertices.size()) - 1;
                    vertexLocations.emplace(vertexIdentifier, lastIndex);
                    uniqueIndices.emplace_back(lastIndex);
                }
                computeTangents(uniqueIndices, vertices);
                generateIndices(uniqueIndices, indices);
            }},
    };

    convertFile(path, functionMap);

    std::vector<MaterialTexture> textures;
    std::vector<Material> outMaterials;
    textures.reserve(materials.size());
    for (const auto &material : materials)
    {
        textures.push_back({ material.mapKd, material.mapNormal });
        outMaterials.push_back({ material.ka, material.kd, 0, material.ks, material.ns });
    }

    return { vertices, indices, outMaterials, textures };
}

Vertex
createBaseVertex(std::string_view args, const std::vector<glm::vec3> &positions, const std::vector<glm::vec2> &uvs,
                 const std::vector<glm::vec3> &normals, const int materialId)
{
    auto nums = splitArgs(args, '/');
    unsigned int positionIndex  { 0 };
    unsigned int uvIndex        { 0 };
    unsigned int normalIndex    { 0 };
    switch (nums.size())
    {
        // Switch case deliberately cascades down.
        case 3:
            normalIndex = std::stoi(nums[2]);
        case 2:
            if (!nums[1].empty())
            {
                uvIndex = std::stoi(nums[1]);
            }
        case 1:
        default:
            positionIndex = std::stoi(nums[0]);
            break;
    }
    const glm::vec3 position    = positionIndex == 0 ? glm::vec3(0.f) : positions[positionIndex - 1];
    const glm::vec2 uv          = uvIndex       == 0 ? glm::vec2(0.f) : uvs[uvIndex - 1];
    const glm::vec3 normal      = normalIndex   == 0 ? glm::vec3(0.f) : normals[normalIndex - 1];
    return {
            position,
            uv,
            normal,
            // Both of these are set later on.
            glm::vec3(0.f),
            glm::vec3(0.f),
            materialId
    };
}

void computeTangents(const std::vector<unsigned int> &uniqueIndices, std::vector<Vertex> &vertices)
{
    // TB = ΔUV ^ -1 * E
    // Where:
    // - TB  is [Tangent, Bi-tangent] matrix,
    // - ΔUV is [Δuv1, Δuv2]          matrix,
    // - E   is [edge1, edge2]        matrix

    const Vertex & v0 = vertices[uniqueIndices[0]];
    const Vertex & v1 = vertices[uniqueIndices[1]];
    const Vertex & v2 = vertices[uniqueIndices[2]];

    const glm::vec3 edge1 = v1.position - v0.position;
    const glm::vec3 edge2 = v2.position - v0.position;

    const glm::vec2 uv1 = v1.uvCoord - v0.uvCoord;
    const glm::vec2 uv2 = v2.uvCoord - v0.uvCoord;

    const glm::mat2   uvMat     = { uv1, uv2 };
    const glm::mat2x3 edgeMat   = { edge1, edge2 };

    const glm::mat2x3 tbMat = edgeMat * glm::inverse(uvMat);

    const glm::vec3 tangent   = glm::normalize(tbMat[0]);
    const glm::vec3 biTangent = glm::normalize(tbMat[1]);

    // Assuming that all vertices live on single plane, all tangents and bi-tangents will be the same.
    for (unsigned int index : uniqueIndices)
    {
        vertices[index].tangent   = tangent;
        vertices[index].biTangent = biTangent;
    }
}

void generateIndices(const std::vector<unsigned int> &uniqueIndices, std::vector<unsigned int> &outIndices)
{
    const unsigned int baseVertexIndex = uniqueIndices[0];
    // Indexes into the uniqueIndices.
    unsigned int firstIndex = 1;
    unsigned int secondIndex = 2;
    for (int i = 0; i < uniqueIndices.size() - 2; ++i)
    {
        outIndices.emplace_back(baseVertexIndex);
        outIndices.emplace_back(uniqueIndices[firstIndex++]);
        outIndices.emplace_back(uniqueIndices[secondIndex++]);
    }
}


