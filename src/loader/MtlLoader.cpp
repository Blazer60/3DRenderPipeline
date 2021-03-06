/**
 * @file MtlLoader.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */

#include "MtlLoader.h"
#include "LoaderCommon.h"
#include <fstream>
#include <vector>
#include <functional>

std::pair<std::unordered_map<std::string, size_t>, std::vector<ObjMaterial>> loadMat(std::string_view path)
{
    std::filesystem::path materialPath(path);
    materialPath = materialPath.remove_filename();

    std::unordered_map<std::string, size_t> matMap;
    std::vector<ObjMaterial> materials;

    ObjMaterial *currentMat;
    unsigned int nextIndex = 0;

    std::unordered_map<std::string_view, std::function<void(std::string_view)>> functionMap{
            { "#", [](std::string_view args){} },  // Comment
            { "Tf", [](std::string_view args){} },  //
            { "map_Bump", [&](std::string_view args){ currentMat->mapNormal = convertPath(args, materialPath); } },  //
            { "Ns", [&](std::string_view args){ currentMat->ns = getFloat(args); } },  //
            { "Ka", [&](std::string_view args){ currentMat->ka = createVec<3>(args); } },  //
            { "Kd", [&](std::string_view args){ currentMat->kd = createVec<3>(args); } },  //
            { "Ks", [&](std::string_view args){ currentMat->ks = createVec<3>(args); } },  //
            { "Ke", [&](std::string_view args){ currentMat->ke = createVec<3>(args); } },  //
            { "Ni", [&](std::string_view args){ currentMat->ni = getFloat(args); } },  //
            { "d", [&](std::string_view args){ currentMat->d = getFloat(args); } },  //
            { "map_Ka", [&](std::string_view args){ currentMat->mapKa = convertPath(args, materialPath); } },  //
            { "map_Kd", [&](std::string_view args){ currentMat->mapKd = convertPath(args, materialPath); } },  //
            { "map_Ks", [&](std::string_view args){ currentMat->mapKs = convertPath(args, materialPath); } },  //
            { "map_Ke", [&](std::string_view args){ currentMat->mapKe = convertPath(args, materialPath); } },  //
            { "illum", [&](std::string_view args){
                currentMat->illum = static_cast<int>(getFloat(args));
            } },  //
            { "newmtl", [&](std::string_view args){
                matMap.insert({ std::string(args), nextIndex++ });
                materials.emplace_back(ObjMaterial());
                currentMat = &materials[materials.size() - 1];
            } },  //
    };

    convertFile(path, functionMap);

    return { matMap, materials };
}

std::string convertPath(const std::string_view &path, const std::filesystem::path &materialPath)
{
    std::filesystem::path p(path);
    return p.is_relative() ? materialPath.string() + std::string(path) : std::string(path);
}
