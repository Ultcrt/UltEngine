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
        float radius_;
        std::size_t sampleNum_ = 64;
        std::size_t noiseSize_;
        std::size_t amount_;

        Shader ssaoShader_;
        Shader multiplyShader_;

        std::vector<glm::vec3> samples_;
        GLuint noiseCTO_ = 0;

    public:
        // TODO: sampleNum not working right now
        SSAOPostProcessor(float radius = 0.5, std::size_t noiseSize = 4, std::size_t amount = 5);

        void process(GLuint vao, const std::array<GLuint, 2> &pingPongFBOs, const std::array<GLuint, 2> &pingPongCTOs,
                     const std::array<GLuint, 2> &pingPongRBOs, GLuint resolvedFBO,
                     const std::array<GLuint, 2> &resolvedCTOs, const std::array<GLuint, 4> &gbos, const glm::vec2& windowSize, const Scene& scene) override;
    };
} // UltEngine

#endif //ULTENGINE_SSAOPOSTPROCESSOR_H
