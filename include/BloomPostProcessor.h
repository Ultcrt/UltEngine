//
// Created by ultcrt on 24-5-15.
//

#ifndef ULTENGINE_BLOOMPOSTPROCESSOR_H
#define ULTENGINE_BLOOMPOSTPROCESSOR_H

#include "IPostProcessor.h"

namespace UltEngine {
    class BloomPostProcessor: public IPostProcessor {
    private:
        Shader blurShader_;
        Shader blendShader_;

    public:
        float intensity;
        std::size_t amount;

        explicit BloomPostProcessor(std::size_t amount, float bloomIntensity = 1.0f);

        void process(GLuint vao, const std::array<GLuint, 2> &fbos, const std::array<GLuint, 2> &ctos, const std::array<GLuint, 2> &rbos, GLuint resolvedFBO, const std::array<GLuint, 2> &resolvedCTOs) override;
    };
} // UltEngine

#endif //ULTENGINE_BLOOMPOSTPROCESSOR_H
