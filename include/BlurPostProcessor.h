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

        void process(GLuint vao, const std::array<GLuint, 2>& fbos, const std::array<GLuint, 2>& ctos, const std::array<GLuint, 2>& rbos, GLuint resolvedFBO, const std::array<GLuint, 2>& resolvedCTOs) override;
    };
} // UltEngine

#endif //ULTENGINE_BLURPOSTPROCESSOR_H
