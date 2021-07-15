/**
 * @file Loader.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 08/07/2021
 * @author Ryan Purse
 */

#include "Loader.h"
#include "ObjLoader.h"

ModelData loadModel(std::string_view path)
{
    if (path.ends_with(".obj"))
    {
        return loadObj(path);
    }
    throw std::exception("Model type is not supported.");
}
