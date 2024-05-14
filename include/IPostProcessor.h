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

        virtual void process(GLuint fbo, GLuint cto, GLuint rbo) = 0;

    protected:
        Shader shader_;
    };
} // UltEngine

#endif //ULTENGINE_IPOSTPROCESSOR_H