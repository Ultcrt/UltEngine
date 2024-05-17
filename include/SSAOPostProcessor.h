//
// Created by ultcrt on 24-5-17.
//

#ifndef ULTENGINE_SSAOPOSTPROCESSOR_H
#define ULTENGINE_SSAOPOSTPROCESSOR_H

#include <array>
#include "IPostProcessor.h"

namespace UltEngine {
    class SSAOPostProcessor : public IPostProcessor {
    private:
        std::size_t sampleNum_;
        std::size_t noiseSize_;

        Shader shader_;

        std::vector<glm::vec3> samples_ = {};

        GLuint noiseCTO_ = 0;

    public:
        SSAOPostProcessor(std::size_t sampleNum, std::size_t noiseSize);

        void process(GLuint vao, const std::array<GLuint, 2> &pingPongFBOs, const std::array<GLuint, 2> &pingPongCTOs,
                     const std::array<GLuint, 2> &pingPongRBOs, GLuint resolvedFBO,
                     const std::array<GLuint, 2> &resolvedCTOs, const std::array<GLuint, 4> &gbos) override;
    };
} // UltEngine

#endif //ULTENGINE_SSAOPOSTPROCESSOR_H
