/**
 * @file MtlLoader.h
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */


#pragma once

#include "LoaderCommon.h"
#include <unordered_map>
#include <string>
#include <filesystem>

std::pair<std::unordered_map<std::string, size_t>, std::vector<ObjMaterial>> loadMat(std::string_view path);

std::string convertPath(const std::string_view &path, const std::filesystem::path& materialPath);
