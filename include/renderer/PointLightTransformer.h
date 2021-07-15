#pragma once

#include "System.h"
#include "Shader.h"

#include <glm.hpp>
#include <vector>

/**
 * [Description goes here.]
 * @author Ryan Purse
 */
class PointLightTransformer : public System
{
public:
    void setShaderLights(ecs::entity mainCamera, Shader &shader);
    void calculateLightPositions(Shader &shader);
protected:

};


