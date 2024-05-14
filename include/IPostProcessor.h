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
        explicit IPostProcessor(const std::filesystem::path& path);

        virtual std::size_t process(GLuint vao, const std::array<GLuint, 2>& fbos, const std::array<GLuint, 2>& ctos, const std::array<GLuint, 2>& rbos, std::size_t inputIdx) = 0;

    protected:
        Shader shader_;
    };
} // UltEngine

#endif //ULTENGINE_IPOSTPROCESSOR_H
