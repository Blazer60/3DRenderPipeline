/**
 * @file LoaderCommon.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 11/07/2021
 * @author Ryan Purse
 */

#include <vector>
#include <fstream>
#include "LoaderCommon.h"

std::vector<std::string> splitArgs(std::string_view args, char delim)
{
    std::vector<std::string> argVec;

    auto start = std::begin(args);
    auto end = std::find(start, std::end(args), delim);
    while (end != std::end(args))
    {
        argVec.emplace_back(start, end);
        start = end + 1;  // Skip over th delim
        end = std::find(start, std::end(args), delim);
    }
    argVec.emplace_back(start, std::end(args));  // Always add the last element as a single arg.
    return argVec;
}

float getFloat(std::string_view arg)
{
    float value;
    auto [_, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), value);
    if (ec != std::errc())  // An error was produced by the args list.
    {
        throw std::exception(std::format("Invalid Argument ({}) for float construction.", arg).c_str());
    }
    return value;
}

void convertFile(std::string_view path,
                 const std::unordered_map<std::string_view, std::function<void(std::string_view)>> &functionMap)
{
    std::ifstream fileStream(path.data());
    if (fileStream.bad()) { throw std::exception("Path could not be found to file."); }

    // Perform the actual conversion per line (Only one function on each line).
    std::string line;
    while (std::getline(fileStream, line))
    {
        if (line.empty()) { continue; }  // Do nothing if there is a blank line.
        // Find where the "function" name ends and where the args start.
        auto separator = std::find(std::begin(line), std::end(line), ' ');

        // Split the function name and args into separate views.
        std::string_view functionName(std::begin(line), separator);
        std::string_view args(separator + 1, std::end(line));

        // Find the function name within the map.
        auto it = functionMap.find(functionName);
        if (it != std::end(functionMap)) { it->second(args); } // Process the function.
        else
        {
            throw std::exception(std::format(
                    "Function ({}) could not be found.",
                    functionName
            ).c_str());
        }
    }
}
