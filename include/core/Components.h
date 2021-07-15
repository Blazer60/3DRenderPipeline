/**
 * @file Components.h
 * @brief Core Components that are needed for the renderer.
 * Project: RenderPipeline
 * Initial Version: 29/06/2021
 * @author Ryan Purse
 */


#pragma once

#include "Vertex.h"

#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <utility>
#include <vector>
#include <string>

struct Transform
{
    glm::vec3 position  { 0.f };
    glm::quat rotation  { glm::vec3(0.f, 0.f, 0.f) };
    glm::vec3 scale     { 1.f };
};

struct PolygonalMesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

struct CameraMatrices
{
    glm::mat4 vpMatrix          { 1.f };
    glm::mat4 projectionMatrix  { 1.f };
    glm::mat4 viewMatrix        { 1.f };
};

struct Camera
{
    float fovY  { 0.46f };
    float zNear { 0.1f };
    float zFar  { 50.f };
};

struct CameraController
{
    float horizontalAngle   { 0.f };
    float verticalAngle     { 0.f };
    float mouseSpeed        { 0.005f };
    float moveSpeed         { 1.f };
};

struct RendererUniforms
{
    glm::mat4 mvp           { 1.f };
    glm::mat4 modelMat { 1.f };
    std::vector <unsigned int> materialIds{};  // Not related to the uv index in vertices.
};

struct Material  // Materials are not needed for rendering. RenderUniforms are however.
{
    glm::vec3 kAmbient          { 0.f };
    glm::vec3 kDiffuse          { 1.f };
    unsigned int kDTextureId    { 0 };
    glm::vec3 kSpecular         { 1.f };
    float nSpecular             { 225 };
};

struct MaterialTexture
{
    std::string kDPath;
};

struct Textures
{
    std::vector<std::string> filePaths;
    std::vector<int> widths;
    std::vector<int> heights;

    Textures() = default;
    explicit Textures(std::vector<std::string> paths) : filePaths(std::move(paths)){}
};

struct Light
{
    glm::vec3 kDiffuse { 1.f };
};

struct PointLight : Light
{
    float intensity { 1.f };
    float fallOff { 5.f };
};

typedef std::vector<unsigned int> TextureIds;
