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
 * Converts incoming data into vertices.
 * @param data The data you want to convert.
 * @param positions All valid vertex positions.
 * @param textureCoords All valid texture coords (uvs).
 * @param normals All normals (not used).
 * @return
 */
std::vector<Vertex> convertVertexData(const std::vector<vertexData> &data, const std::vector<glm::vec3> &positions,
                                      const std::vector<glm::vec2> &textureCoords,
                                      const std::vector<glm::vec3> &normals);

/**
 * Creates vertex data based on an args strings.
 * @param args
 * @return
 */
vertexData createVertexData(std::string_view args, unsigned int materialIndex);
