//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_SHADER_H
#define ULTENGINE_SHADER_H

#include <string>
#include <filesystem>
#include "glm/glm.hpp"
#include "glad/glad.h"

namespace UltEngine {
    class Shader {
    private:
        static unsigned CreateShaderObjectFromFile_(const std::filesystem::path& path, GLenum type);
        GLuint vertexShaderID_;
        GLuint geometryShaderID_;
        GLuint fragmentShaderID_;

    public:
        Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);
        Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& geometryShaderPath, const std::filesystem::path& fragmentShaderPath);
        void use() const;
        unsigned programID;

        void set(const std::string& name, int val) const;
        void set(const std::string& name, bool val) const;
        void set(const std::string& name, float val) const;
        void set(const std::string& name, const glm::vec3& val) const;
        void set(const std::string& name, const glm::vec2& val) const;
        void set(const std::string& name, const glm::mat3& val, bool transpose = false) const;
        void set(const std::string& name, const glm::mat4& val, bool transpose = false) const;
    };
} // UltEngine

#endif //ULTENGINE_SHADER_H
