/**
 * @file Loader.h
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 08/07/2021
 * @author Ryan Purse
 */


#pragma once

#include "Components.h"
#include "LoaderCommon.h"
#include <string_view>



/**
 * Loads a model from disk. Currently supports only .obj files. All other files will not do anything.
 * @param path
 * @return Mesh
 */
ModelData loadModel(std::string_view path);
