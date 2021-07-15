/**
 * @file PointLightTransformer.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 14/07/2021
 * @author Ryan Purse
 */

#include "PointLightTransformer.h"
#include <format>

void PointLightTransformer::calculateLightPositions(Shader &shader)
{
    int i = 0;
    for (const auto &entity : mEntities)
    {
        const auto &lightTransform = getComponent<Transform>(entity);
        shader.setUniform(
                std::format("u_lights[{}].position", i),
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
    shader.setUniform("u_cameraPosition", glm::vec4(-cameraPosition, 1.f));
    for (const auto &entity : mEntities)
    {
        const auto &light = getComponent<PointLight>(entity);
        const auto &lightPos = getComponent<Transform>(entity).position;
        shader.setUniform(
                std::format("u_lights[{}].colour", i),
                glm::vec4(light.kDiffuse, 1.f)
                );
        shader.setUniform(
                std::format("u_lights[{}].intensity", i),
                light.intensity
                );
        shader.setUniform(
                std::format("u_lights[{}].fallOff", i),
                light.fallOff
                );
        ++i;
    }
}
