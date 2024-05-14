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
        GLuint fbo_ = 0;
        GLuint cto_ = 0;

    public:
        std::size_t amount;

        explicit BlurPostProcessor(std::size_t amount = 5);

        void process(GLuint fbo, GLuint cto, GLuint rbo) override;
    };
} // UltEngine

#endif //ULTENGINE_BLURPOSTPROCESSOR_H
