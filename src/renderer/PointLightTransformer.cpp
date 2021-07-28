/**
 * @file PointLightTransformer.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 14/07/2021
 * @author Ryan Purse
 */

#include "PointLightTransformer.h"

void PointLightTransformer::calculateLightPositions(Shader &shader)
{
    int i = 0;
    for (const auto &entity : mEntities)
    {
        const auto &lightTransform = getComponent<Transform>(entity);
        shader.setUniform(
                "u_lights[" + std::to_string(i) + "].position_ws",
                glm::vec4(lightTransform.position, 1.f)
        );
        ++i;
    }
}

void PointLightTransformer::setShaderLights(ecs::entity mainCamera, Shader &shader)
{
    calculateLightPositions(shader);
    int i = 0;
    const auto &cameraPosition = getComponent<Transform>(mainCamera).position;
    shader.setUniform("u_camera_position_ws", glm::vec4(-cameraPosition, 1.f));
    for (const auto &entity : mEntities)
    {
        const auto &light = getComponent<PointLight>(entity);
        const auto &lightPos = getComponent<Transform>(entity).position;
        std::string stringI = std::to_string(i);
        shader.setUniform("u_lights[" + stringI + "].colour", glm::vec4(light.kDiffuse, 1.f));
        shader.setUniform("u_lights[" + stringI + "].intensity", light.intensity);
        shader.setUniform("u_lights[" + stringI + "].fall_off", light.fallOff);
        ++i;
    }
}
