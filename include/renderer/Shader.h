/**
 * @file Shader.h
 * @brief [fill in this secton]
 * Project: OpenGLTutorialCherno
 * Initial Version: 04/06/2021
 * @author Ryan Purse
 */


#pragma once

#include <string>
#include <unordered_map>
#include <glm.hpp>

struct shaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

/**
 * [Description goes here.]
 * [Initial Version: 04/06/2021]
 * @author Ryan Purse
 */
class Shader
{
private:
    std::string mFilePath;  // This is for debug purposes
    unsigned int mRendererId;
    std::unordered_map<std::string, int> mUniformLocationCache;

public:
    explicit Shader(const std::string &filepath);
    ~Shader();

    void bind() const;
    static void unBind();

    // Set uniforms
    void setUniform(const std::string &name, const glm::vec3 &values, int count);
    void setUniform(const std::string &name, const float *values, int count);
    void setUniform(const std::string &name, const int *values, int count);
    void setUniform(const std::string &name, int value);
    void setUniform(const std::string &name, float value);
    void setUniform(const std::string &name, const glm::vec2 &value);
    void setUniform(const std::string &name, const glm::vec3 &value);
    void setUniform(const std::string &name, const glm::vec4 &vec4);
    void setUniform(const std::string &name, const glm::mat4 &mat4);

private:
    static shaderProgramSource parseShader(const std::string &filepath);
    static unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader);
    static unsigned int compileShader(unsigned int type, const std::string &source);
    int getUniformLocation(const std::string &name);
};


