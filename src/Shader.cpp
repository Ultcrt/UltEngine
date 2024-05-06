//
// Created by ultcrt on 24-5-6.
//

#include <fstream>
#include <sstream>
#include <format>
#include "Shader.h"
#include "glad/glad.h"

namespace UltEngine {
    Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
        // Create shader object
        vertexShaderID = CreateShaderObjectFromFile_(vertexShaderPath);
        fragmentShaderID = CreateShaderObjectFromFile_(fragmentShaderPath);

        // Create and link program
        programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);
        int success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            char info[512];
            glGetProgramInfoLog(programID, 512, nullptr, info);
            throw std::runtime_error(std::format("Cannot link program:\n{}", info));
        }
    }

    unsigned Shader::CreateShaderObjectFromFile_(const std::string &path) {
        // Create file streams that throws error when encountering failbit and badbit
        std::ifstream fileStream;
        fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        // Open files
        fileStream.open(path);

        // Convert file streams into string streams
        std::stringstream strStream;
        strStream << fileStream.rdbuf();

        // Close file streams
        fileStream.close();

        // Read strings from string streams
        std::string shaderCode = strStream.str();

        const char* cShaderCode = shaderCode.c_str();

        // Load shaders
        unsigned shaderID;
        int success;

        shaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shaderID, 1, &cShaderCode, nullptr);
        glCompileShader(shaderID);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info[512];
            glGetShaderInfoLog(shaderID, 512, nullptr, info);
            throw std::runtime_error(std::format("Cannot compile shader:\n{}", info));
        }

        return shaderID;
    }

    void Shader::use() const {
        glUseProgram(programID);
    }

    void Shader::set(const std::string &name, int val) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), val);
    }

    void Shader::set(const std::string &name, bool val) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), static_cast<int>(val));

    }

    void Shader::set(const std::string &name, float val) const {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), val);
    }
} // UltEngine