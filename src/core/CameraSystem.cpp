/**
 * @file CameraSystem.cpp
 * @brief Creates view matrices out of camera transforms.
 * Project: RenderPipeline
 * Initial Version: 30/06/2021
 * @author Ryan Purse
 */

#include "CameraSystem.h"

#include <glfw3.h>

void CameraSystem::init()
{
    GLFWwindow *window = glfwGetCurrentContext();
    glm::ivec2 viewPortSize;
    glfwGetWindowSize(window, &viewPortSize.x, &viewPortSize.y);
    for (const auto &entity : mEntities)
    {
        const auto &camera = getComponent<Camera>(entity);
        auto &mat = getComponent<CameraMatrices>(entity);
        mat.projectionMatrix = glm::perspective(
                camera.fovY,
                static_cast<float>(viewPortSize.x) / static_cast<float>(viewPortSize.y),
                camera.zNear,
                camera.zFar);
    }
}

void CameraSystem::update()
{
    for (const auto &entity : mEntities)
    {
        const auto &camera = getComponent<Camera>(entity);
        const auto transform = getComponent<Transform>(entity);
        auto &mat = getComponent<CameraMatrices>(entity);

        const glm::mat4 translation = glm::translate(glm::mat4(1.f), transform.position);
        const glm::mat4 rotation = glm::toMat4(transform.rotation);
        const glm::mat4 scale = glm::scale(glm::mat4(1.f), transform.scale);

        // View Matrices are from world space to Camera space. (Therefore the opposite of a model Matrix)
        mat.viewMatrix = scale * rotation * translation;

        mat.projectionMatrix = glm::perspective(
                camera.fovY,
                1920.f / 1080.f,
                camera.zNear,
                camera.zFar);

        mat.vpMatrix = mat.projectionMatrix * mat.viewMatrix;
    }
}
