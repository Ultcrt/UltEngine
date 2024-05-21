//
// Created by ultcrt on 24-5-17.
//

#include <random>
#include <format>
#include "postprocessor/SSAOPostProcessor.h"

namespace UltEngine {
    SSAOPostProcessor::SSAOPostProcessor(float radius, std::size_t noiseSize):
    ssaoShader_(SHADER_DIRECTORY / std::filesystem::path("ScreenFiller.vert"),
            SHADER_DIRECTORY / std::filesystem::path("SSAO.frag")), 
    multiplyShader_(std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.vert",
        std::filesystem::path(SHADER_DIRECTORY) / "SSAOMultiply.frag"),
    radius_(radius), noiseSize_(noiseSize) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        for (std::size_t idx = 0; idx < sampleNum_; idx++) {
            glm::vec3 point = {
                2.0f * dist(mt) - 1.0f,
                2.0f * dist(mt) - 1.0f,
                dist(mt)
            };

            point = glm::normalize(point);
            point *= dist(mt);

            // Generate more point at center
            float scale = static_cast<float>(idx) / static_cast<float>(sampleNum_);
            scale = glm::mix(0.1f, 1.0f, scale * scale);
            point *= scale;

            samples_.emplace_back(point);
        }

        std::vector<glm::vec3> noise;
        for (std::size_t idx = 0; idx < noiseSize_ * noiseSize_; idx++) {
            glm::vec3 point = {
                2.0f * dist(mt) - 1.0f,
                2.0f * dist(mt) - 1.0f,
                0.0f
            };

            noise.emplace_back(point);
        }

        glGenTextures(1, &noiseCTO_);
        glBindTexture(GL_TEXTURE_2D, noiseCTO_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, noiseSize_, noiseSize_, 0, GL_RGB, GL_FLOAT, noise.data());
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void SSAOPostProcessor::process(GLuint vao, const std::array<GLuint, 2> &pingPongFBOs,
                                    const std::array<GLuint, 2> &pingPongCTOs,
                                    const std::array<GLuint, 2> &pingPongRBOs, GLuint resolvedFBO,
                                    const std::array<GLuint, 2> &resolvedCTOs, const std::array<GLuint, 4> &gbos, const glm::vec2& windowSize, const Scene& scene) {
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        ssaoShader_.use();

        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs[0]);
        glBindVertexArray(vao);

        ssaoShader_.set("windowSize", windowSize);

        const auto& pCamera = scene.getCamera();
        ssaoShader_.set("view", pCamera->getView());
        ssaoShader_.set("projection", pCamera->getProjection());

        for (std::size_t idx = 0; idx < sampleNum_; idx++) {
            ssaoShader_.set(std::format("samples[{}]", idx), samples_[idx]);
        }

        ssaoShader_.set("radius", radius_);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, noiseCTO_);
        ssaoShader_.set("noise", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbos[0]);
        ssaoShader_.set("geometryBuffers.position", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbos[1]);
        ssaoShader_.set("geometryBuffers.normal", 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, resolvedCTOs[0]);
        ssaoShader_.set("lightingPass", 3);

        glActiveTexture(GL_TEXTURE0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        multiplyShader_.use();
        glBindFramebuffer(GL_FRAMEBUFFER, resolvedFBO);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pingPongCTOs[0]);
        multiplyShader_.set("weight", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, resolvedCTOs[0]);
        multiplyShader_.set("image", 1);

        multiplyShader_.set("noiseSize", static_cast<int>(noiseSize_));

        glActiveTexture(GL_TEXTURE0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
} // UltEngine