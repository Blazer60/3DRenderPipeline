#pragma once

#include "System.h"

#include <glfw3.h>

/**
 * Camera controller for navigating the viewport.
 * @author Ryan Purse
 */
class CameraControllerSystem : public System
{
public:
    CameraControllerSystem();
    void update(float tickRate);
    void moveFirstPerson(float tickRate);
protected:
    GLFWwindow *mWindow;
    glm::vec2 mViewportSize{ 0.f };
};


