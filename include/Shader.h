//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_SHADER_H
#define ULTENGINE_SHADER_H

#include <string>
#include <glm/glm.hpp>
#include "glad/glad.h"

namespace UltEngine {
    class Shader {
    private:
        static unsigned CreateShaderObjectFromFile_(const std::string& path, GLenum type);
        unsigned vertexShaderID;
        unsigned fragmentShaderID;
        unsigned programID;

    public:
        Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        void use() const;

        void set(const std::string& name, int val) const;
        void set(const std::string& name, bool val) const;
        void set(const std::string& name, float val) const;
        void set(const std::string& name, const glm::mat4& val, bool transpose = false) const;
    };
} // UltEngine

#endif //ULTENGINE_SHADER_H
