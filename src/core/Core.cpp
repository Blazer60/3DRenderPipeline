/**
 * @file Core.cpp
 * @brief Runs the main loop of the program. Handles all scenes that are currently loaded.
 * Project: RenderPipeline
 * Initial Version: 28/06/2021
 * @author Ryan Purse
 */

#include "Core.h"
#include "Scene.h"
#include "GlDebugger.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

Core::Core(const glm::ivec2 &resolution)
    : mResolution(resolution)
{
    if (!(initGlfw() && initOpenGl() && initImGui()))
    {
        mIsRunning = false;
        return;
    }
    mScenes.push_back(std::make_unique<Scene>());
}

Core::~Core()
{
    glfwTerminate();
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
}

bool Core::initGlfw()
{
    if (!glfwInit()) { return false; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Version of opengl you want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);  // For debugging

    mWindow = glfwCreateWindow(mResolution.x, mResolution.y, "Rendering System", nullptr, nullptr);
    if (!mWindow)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    return true;
}

bool Core::initOpenGl()
{
    if (glewInit() != GLEW_OK) { return false; }

    // Debug Messaging.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugger::debugMessageCallBack, nullptr);

    std::cout << glGetString(GL_VERSION) << "\n";  // Version of openGL

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) { return false; }

    // Blending texture data / enabling lerping.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

bool Core::initImGui()
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    return true;
}

void Core::run()
{
    while (mIsRunning)
    {
        update();
        render();
        renderImGui();

        glfwSwapBuffers(mWindow);

        glfwPollEvents();
        mIsRunning = !glfwWindowShouldClose(mWindow);
    }
}

void Core::update()
{
    for (auto &scene : mScenes)
    {
        scene->update(mTickRate);
    }
}

void Core::render()
{
    for (auto &scene : mScenes)
    {
        scene->render();
    }
}

void Core::renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    for (auto &scene : mScenes)
    {
        scene->renderImGui();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
