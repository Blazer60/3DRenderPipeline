/**
 * @file EcsCommon.h
 * @brief Common ECS definitions that are shared across multiple ECS Managers.
 * Project: RenderPipeline
 * Initial Version: 28/06/2021
 * @author Ryan Purse
 */


#pragma once

#include <bitset>

namespace ecs
{
    typedef size_t entity;
    const size_t maxEntities = 5'000;

    typedef size_t componentId;
    const size_t maxComponents = 32;

    typedef std::bitset<maxComponents> signature;

    template<typename Component>
    std::string toString()
    {
        return std::string(typeid(Component).name());
    }
}

