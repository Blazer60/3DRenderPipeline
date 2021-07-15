/**
 * @file CameraControllerSystem.cpp
 * @brief Camera controller for navigating the viewport.
 * Project: RenderPipeline
 * Initial Version: 30/06/2021
 * @author Ryan Purse
 */

#include <iostream>
#include "CameraControllerSystem.h"

CameraControllerSystem::CameraControllerSystem()
    : mWindow(glfwGetCurrentContext())
{
    int x, y;
    glfwGetWindowSize(mWindow, &x, &y);
    mViewportSize = glm::vec2(x, y);

    // Create and set the callback for locking and unlocking the cursor from the centre of the screen.
    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *window, int button, int action, int mods){
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, 0.0, 0.0);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    });
}

void CameraControllerSystem::update(const float tickRate)
{
    // Camera is only activated when the right mouse button is pressed.
    if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        moveFirstPerson(tickRate);
    }
}

void CameraControllerSystem::moveFirstPerson(float tickRate)
{
    double xPos{ 0.0 }, yPos{ 0.0 };
    glfwGetCursorPos(mWindow, &xPos, &yPos);

    // re-center the mouse for the next pass.
    // Glfw handles putting the mouse back to its original positions when unlocking.
    glfwSetCursorPos(mWindow, 0.0, 0.0);

    // This should only really update the main camera as you don't want to move all the camera's at once.
    for (const auto &entity : mEntities)
    {
        auto &cameraController = getComponent<CameraController>(entity);
        auto &transform = getComponent<Transform>(entity);

        cameraController.horizontalAngle += cameraController.mouseSpeed * tickRate * -xPos;
        cameraController.verticalAngle += cameraController.mouseSpeed * tickRate * yPos;

        // Find the local direction of all axes.
        glm::vec3 direction(
                glm::cos(cameraController.verticalAngle) * glm::sin(cameraController.horizontalAngle),
                glm::sin(cameraController.verticalAngle),
                glm::cos(cameraController.verticalAngle) * glm::cos(cameraController.horizontalAngle)
        );
        glm::vec3 right(
                glm::sin(cameraController.horizontalAngle - 1.57f),
                0.f,
                glm::cos(cameraController.horizontalAngle - 1.57f)
        );
        glm::vec3 up = glm::normalize(glm::cross(right, direction));

        transform.rotation = glm::angleAxis(-cameraController.verticalAngle, right) * glm::angleAxis(-cameraController.horizontalAngle, up);

        // Handle Movement Input.
        if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        {
            transform.position += direction * tickRate * cameraController.moveSpeed;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        {
            transform.position -= direction * tickRate * cameraController.moveSpeed;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        {
            transform.position += right * tickRate * cameraController.moveSpeed;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        {
            transform.position -= right * tickRate * cameraController.moveSpeed;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            transform.position.y -= tickRate * cameraController.moveSpeed;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            transform.position.y += tickRate * cameraController.moveSpeed;
        }
        // Camera System will update the view matrix for this camera.
    }
}
