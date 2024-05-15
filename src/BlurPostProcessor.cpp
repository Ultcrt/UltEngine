//
// Created by ultcrt on 24-5-14.
//

#include "BlurPostProcessor.h"

namespace UltEngine {
    BlurPostProcessor::BlurPostProcessor(std::size_t amount):
    amount(amount),
    shader_(std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.vert",
            std::filesystem::path(SHADER_DIRECTORY) / "GaussianBlur.frag") {}

    void BlurPostProcessor::process(GLuint vao, const std::array<GLuint, 2>& fbos, const std::array<GLuint, 2>& ctos, const std::array<GLuint, 2>& rbos, GLuint resolvedFBO, const std::array<GLuint, 2>& resolvedCTOs) {
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader_.use();
        glBindVertexArray(vao);

        std::size_t inputIdx = 0;
        std::size_t outputIdx = 1;
        for (std::size_t idx = 0; idx < amount * 2; idx++) {
            glBindFramebuffer(GL_FRAMEBUFFER, (idx == amount * 2 - 1) ? resolvedFBO : fbos[outputIdx]);
            glBindTexture(GL_TEXTURE_2D, idx == 0 ? resolvedCTOs[0] : ctos[inputIdx]);

            shader_.set("direction", inputIdx == 0);
            shader_.set("screen", 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            std::swap(inputIdx, outputIdx);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // UltEngine