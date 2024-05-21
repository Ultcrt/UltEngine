//
// Created by ultcrt on 24-5-14.
//

#ifndef ULTENGINE_BLURPOSTPROCESSOR_H
#define ULTENGINE_BLURPOSTPROCESSOR_H

#include <array>
#include "IPostProcessor.h"

namespace UltEngine {
    class BlurPostProcessor: public IPostProcessor{
    private:
        Shader shader_;

    public:
        std::size_t amount;

        explicit BlurPostProcessor(std::size_t amount = 5);

        void process(
                GLuint vao,
                const std::array<GLuint, 2>& pingPongFBOs, const std::array<GLuint, 2>& pingPongCTOs, const std::array<GLuint, 2>& pingPongRBOs,
                GLuint resolvedFBO, const std::array<GLuint, 2>& resolvedCTOs, const std::array<GLuint, 4>& gbos, const glm::vec2& windowSize, const Scene& scene) override;
    };
} // UltEngine

#endif //ULTENGINE_BLURPOSTPROCESSOR_H
