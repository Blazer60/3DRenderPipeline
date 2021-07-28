/**
 * @file ObjLoader.h
 * @brief A Model loader the specifically handles .obj files.
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */


#pragma once

#include "Components.h"
#include "LoaderCommon.h"

struct vertexData;

/**
 * Loads an object from a .obj file
 * @param path Relative path to an .obj file
 * @return
 */
ModelData loadObj(std::string_view path);

/**
 * Creates a Vertex based on the incoming args in the form position[/[uvs]/[normals]].
 * @note Assumes that all indices are valid and are one based.
 * @param args typically in the form Pos/UV/Normal
 * @param positions All positions that have currently been read from a file.
 * @param uvs All Uvs that have currently been read from a file.
 * @param normals All Normals that have currently been read from a file.
 * @return A Vertex with it's Position, UV and normal being set (They can still be 0).
 */
Vertex
createBaseVertex(std::string_view args, const std::vector<glm::vec3> &positions, const std::vector<glm::vec2> &uvs,
                 const std::vector<glm::vec3> &normals, int materialId);

/**
 * Generate a repeating pattern based on the numbers of unique vertices supplied.
 * @param uniqueIndices The indices that you want to be duplicated.
 * @param outIndices The location that you want the indices to end up.
 */
void generateIndices(const std::vector<unsigned int> &uniqueIndices, std::vector<unsigned int> &outIndices);