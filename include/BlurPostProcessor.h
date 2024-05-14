//
// Created by ultcrt on 24-5-14.
//

#ifndef ULTENGINE_BLURPOSTPROCESSOR_H
#define ULTENGINE_BLURPOSTPROCESSOR_H

#include <array>
#include "IPostProcessor.h"

namespace UltEngine {
    class BlurPostProcessor: public IPostProcessor{
    public:
        std::size_t amount;

        explicit BlurPostProcessor(std::size_t amount = 5);

        std::size_t process(GLuint vao, const std::array<GLuint, 2>& fbos, const std::array<GLuint, 2>& ctos, const std::array<GLuint, 2>& rbos, std::size_t inputIdx) override;
    };
} // UltEngine

#endif //ULTENGINE_BLURPOSTPROCESSOR_H
