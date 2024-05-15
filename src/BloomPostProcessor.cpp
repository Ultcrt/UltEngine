//
// Created by ultcrt on 24-5-15.
//

#include <array>
#include "BloomPostProcessor.h"

namespace UltEngine {
    BloomPostProcessor::BloomPostProcessor(std::size_t amount, float bloomIntensity):
    amount(amount),
    intensity(bloomIntensity),
    blurShader_(std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.vert",
        std::filesystem::path(SHADER_DIRECTORY) / "GaussianBlur.frag"),
    blendShader_(std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.vert",
         std::filesystem::path(SHADER_DIRECTORY) / "Blend.frag") {}

    void
    BloomPostProcessor::process(GLuint vao, const std::array<GLuint, 2> &fbos, const std::array<GLuint, 2> &ctos,
                                const std::array<GLuint, 2> &rbos, GLuint resolvedFBO, const std::array<GLuint, 2> &resolvedCTOs) {
        blurShader_.use();
        glBindVertexArray(vao);

        std::size_t inputIdx = 0;
        std::size_t outputIdx = 1;
        for (std::size_t idx = 0; idx < amount * 2; idx++) {
            glBindFramebuffer(GL_FRAMEBUFFER, fbos[outputIdx]);
            glBindTexture(GL_TEXTURE_2D, idx == 0 ? resolvedCTOs[1] : ctos[inputIdx]);

            blurShader_.set("direction", inputIdx == 0);
            blurShader_.set("screen", 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            std::swap(inputIdx, outputIdx);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, resolvedFBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ctos[inputIdx]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, resolvedCTOs[0]);

        blendShader_.use();
        blendShader_.set("imageA", 0);
        blendShader_.set("imageB", 1);
        blendShader_.set("weightA", intensity);
        blendShader_.set("weightB", 1.0f);
    }
} // UltEngine