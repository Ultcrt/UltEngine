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

        explicit BloomPostProcessor(std::size_t amount = 5.0f, float bloomIntensity = 1.0f);

    void process(
            GLuint vao,
            const std::array<GLuint, 2>& pingPongFBOs, const std::array<GLuint, 2>& pingPongCTOs, const std::array<GLuint, 2>& pingPongRBOs,
            GLuint resolvedFBO, const std::array<GLuint, 2>& resolvedCTOs, const std::array<GLuint, 4>& gbos) override;
    };
} // UltEngine

#endif //ULTENGINE_BLOOMPOSTPROCESSOR_H
