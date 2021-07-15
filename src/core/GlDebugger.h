#pragma once

#include <glew.h>
#include <string>

/**
 * Functions for dealing with debugging opengl errors.
 * @author Ryan Purse
 */
namespace glDebugger
{
    enum class severity{ Notification, Low, Medium, High, None };

    void debugMessageCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar *message, const void *userParam);

    void setInterruptSeverity(severity level);
}