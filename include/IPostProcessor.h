//
// Created by ultcrt on 24-5-13.
//

#ifndef ULTENGINE_IPOSTPROCESSOR_H
#define ULTENGINE_IPOSTPROCESSOR_H

#include <filesystem>
#include "Shader.h"

namespace UltEngine {
    class IPostProcessor {
    public:
        virtual void process(GLuint vao, const std::array<GLuint, 2>& fbos, const std::array<GLuint, 2>& ctos, const std::array<GLuint, 2>& rbos, GLuint resolvedFBO, const std::array<GLuint, 2>& resolvedCTOs) = 0;
    };
} // UltEngine

#endif //ULTENGINE_IPOSTPROCESSOR_H
