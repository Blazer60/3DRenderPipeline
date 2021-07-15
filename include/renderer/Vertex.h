/**
 * @file Vertex.h
 * @brief Defines the specification for a single default vertex.
 * Project: RenderPipeline
 * Initial Version: 29/06/2021
 * @author Ryan Purse
 */


#pragma once

#include <glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uvCoord{ 1.f };
    int textureId{ 0 };
    static int stride();
};