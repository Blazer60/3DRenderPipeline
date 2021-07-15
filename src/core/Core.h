#pragma once

#include <glew.h>  // This needs to be the first include before using any opengl libs
#include <glfw3.h>

#include "core/Scene.h"

#include <vector>
#include <memory>
#include <glm.hpp>


/**
 * Runs the main loop of the program. Handles all scenes that are currently loaded.
 * @author Ryan Purse
 */
class Core
{
public:
    explicit Core(const glm::ivec2 &resolution={ 1920, 1080 });
    ~Core();
    void run();

protected:
    bool initGlfw();
    bool initOpenGl();
    bool initImGui();

    void update();
    void render();
    void renderImGui();


    glm::ivec2 mResolution;
    GLFWwindow *mWindow{ nullptr };

    float mTickRate{ 0.16f };


    std::vector<std::unique_ptr<Scene>> mScenes;
    bool mIsRunning{ true };
};


