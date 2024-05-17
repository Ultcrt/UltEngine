//
// Created by ultcrt on 24-5-17.
//

#include <random>
#include "SSAOPostProcessor.h"

namespace UltEngine {
    SSAOPostProcessor::SSAOPostProcessor(std::size_t sampleNum, std::size_t noiseSize):
    shader_(SHADER_DIRECTORY / std::filesystem::path("ScreenFiller.vert"),
            SHADER_DIRECTORY / std::filesystem::path("SSAO.frag")), sampleNum_(sampleNum), noiseSize_(noiseSize) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        for (auto& point: samples_) {
            point.x = dist(mt);
            point.y = dist(mt);
            point.z = dist(mt);

            point = glm::normalize(point);
            point *= dist(mt);
        }
    }

    void SSAOPostProcessor::process(GLuint vao, const std::array<GLuint, 2> &pingPongFBOs,
                                    const std::array<GLuint, 2> &pingPongCTOs,
                                    const std::array<GLuint, 2> &pingPongRBOs, GLuint resolvedFBO,
                                    const std::array<GLuint, 2> &resolvedCTOs, const std::array<GLuint, 4> &gbos) {

    }
} // UltEngine