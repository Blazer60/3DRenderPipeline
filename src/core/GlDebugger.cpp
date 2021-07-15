/**
 * @file glDebugger.cpp
 * @brief Functions for dealing with debugging opengl errors.
 * Project: RenderPipeline
 * Initial Version: 29/06/2021
 * @author Ryan Purse
 */

#include "GlDebugger.h"

#include <iostream>

namespace glDebugger
{
    void logSource(GLenum);
    void logType(GLenum);
    void logSeverity(GLenum);
    std::string lineEnding();

    severity throwLevel{ severity::Low };
    severity currentError{ severity::None };

    void debugMessageCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                          const GLchar *message, const void *userParam)
    {
        std::cout << "<< OPENGL ERROR " << id << " >>\n";
        logSource(source);
        logType(type);
        logSeverity(severity);
        std::cout << "\nMessage : " << message << "\n";
        if (currentError >= throwLevel) { throw std::exception("OpenGL Error, see output."); }
    }

    void setInterruptSeverity(severity level)
    {
        throwLevel = level;
    }

    void logSource(GLenum source)
    {
        std::cout << "Source: ";
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:
                std::cout << "API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cout << "Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cout << "Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cout << "Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cout << "Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cout << "Other";
                break;
            default:
                std::cout << "Unknown";
                break;
        }
        std::cout << lineEnding();
    }

    void logType(GLenum type)
    {
        std::cout << "Type: ";
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:
                std::cout << "Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cout << "Deprecated";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cout << "Undefined Behaviour";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cout << "Portability";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cout << "Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cout << "Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cout << "Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cout << "Other";
                break;
            default:
                std::cout << "Unknown";
                break;
        }
        std::cout << lineEnding();
    }

    void logSeverity(GLenum severity)
    {
        std::cout << "Severity: ";
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cout << "High";
                currentError = severity::High;
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cout << "Medium";
                currentError = severity::Medium;
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cout << "Low";
                currentError = severity::Low;
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cout << "Notification";
                currentError = severity::Notification;
                break;
            default:
                std::cout << "Unknown";
                currentError = severity::High;
                break;
        }
    }

    std::string lineEnding()
    {
        return " | ";
    }
}