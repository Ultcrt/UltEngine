//
// Created by ultcrt on 24-5-15.
//

#include <array>
#include "BloomPostProcessor.h"
#include <iostream>

namespace UltEngine {
    BloomPostProcessor::BloomPostProcessor(std::size_t amount, float bloomIntensity):
    amount(amount),
    intensity(bloomIntensity),
    blurShader_(std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.vert",
        std::filesystem::path(SHADER_DIRECTORY) / "GaussianBlur.frag"),
    blendShader_(std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.vert",
         std::filesystem::path(SHADER_DIRECTORY) / "Blend.frag") {}

    void
    BloomPostProcessor::process(GLuint vao,
                                const std::array<GLuint, 2>& pingPongFBOs, const std::array<GLuint, 2>& pingPongCTOs, const std::array<GLuint, 2>& pingPongRBOs,
                                GLuint resolvedFBO, const std::array<GLuint, 2>& resolvedCTOs, const std::array<GLuint, 4>& gbos, const glm::vec2& windowSize, const Scene& scene) {
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        blurShader_.use();
        glBindVertexArray(vao);

        std::size_t inputIdx = 0;
        std::size_t outputIdx = 1;
        for (std::size_t idx = 0; idx < amount * 2; idx++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs[outputIdx]);
            glBindTexture(GL_TEXTURE_2D, idx == 0 ? resolvedCTOs[1] : pingPongCTOs[inputIdx]);

            blurShader_.set("direction", inputIdx == 0);
            blurShader_.set("screen", 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            std::swap(inputIdx, outputIdx);
        }

        blendShader_.use();
        glBindFramebuffer(GL_FRAMEBUFFER, resolvedFBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pingPongCTOs[inputIdx]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, resolvedCTOs[0]);
        glActiveTexture(GL_TEXTURE0);

        blendShader_.set("imageA", 0);
        blendShader_.set("imageB", 1);
        blendShader_.set("weightA", intensity);
        blendShader_.set("weightB", 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
} // UltEngine