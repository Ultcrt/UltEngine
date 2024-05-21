//
// Created by ultcrt on 24-5-13.
//

#ifndef ULTENGINE_IPOSTPROCESSOR_H
#define ULTENGINE_IPOSTPROCESSOR_H

#include <filesystem>
#include "Shader.h"
#include "Scene.h"

namespace UltEngine {
    class IPostProcessor {
    public:
        virtual void process(
                GLuint vao,
                const std::array<GLuint, 2>& pingPongFBOs, const std::array<GLuint, 2>& pingPongCTOs, const std::array<GLuint, 2>& pingPongRBOs,
                GLuint resolvedFBO, const std::array<GLuint, 2>& resolvedCTOs, const std::array<GLuint, 4>& gbos, const glm::vec2& windowSize, const Scene& scene) = 0;
    };
} // UltEngine

#endif //ULTENGINE_IPOSTPROCESSOR_H
