/**
 * @file DebugLogger.h
 * @brief Logs info to a file/terminal upon users request. Depending on the severity, the logger will also abort the program.
 * Project: RenderPipeline
 * Initial Version: 20/07/2021
 * @author Ryan Purse
 */


#pragma once

#include <source_location>

namespace debug
{
    /**
     * Severity types for when you want to log something.
     * @enum Notification - Used for logging miscellaneous information.
     * @enum Warning - Used when the program can still continue but said thing should be avoided.
     * @enum Minor - Used when an error could occur or something has an easy fix. Such as changing an enum to the correct state.
     * @enum Major - Used when ar error will occur when an action is performed. E.g.: Run-time behaviour is ill defined.
     * @enum Fatal - Used when the program cannot continue regardless of whats happened.
     * @enum Unknown - An incoming message has no known type attached to it.
     */
    enum class severity : unsigned char { Notification, Warning, Minor, Major, Fatal, Unknown };

    /** Sets the severity level in which the program will crash. */
    void setThrowLevel(severity level);

    /** Gets the severity level in which the program will crash. */
    [[nodiscard]] severity getThrowLevel();

    /**
     * Logs a message, force crashing if the severity level is above a threshold.
     * @param message The message that you want to be outputted.
     * @param level The severe the message is.
     */
    void log(std::string_view message, severity level);

    /**
     * Logs a message, force crashing if the severity level is above a threshold.
     * @param message The message that you want to be outputted.
     * @param level The severe the message is.
     */
    void log(const unsigned char *message, severity level);

    /** Call back to attach to opengl when in debug mode. */
    void openglCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

    /** Clears the log file. */
    void clearLogs();

    /** Used when a log exception occurs. */
    class LogException : std::exception
    {
        [[nodiscard]] const char *what() const override
        {
            return "Log message exceed the maximum throw level threshold. Check the logs for more information.";
        }
    };
}