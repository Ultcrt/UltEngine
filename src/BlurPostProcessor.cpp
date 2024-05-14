//
// Created by ultcrt on 24-5-14.
//

#include "BlurPostProcessor.h"

namespace UltEngine {
    BlurPostProcessor::BlurPostProcessor(std::size_t amount):
            IPostProcessor(std::filesystem::path(SHADER_DIRECTORY) / "GaussianBlur.frag"), amount(amount) {
        glGenFramebuffers(1, &fbo_);
        glGenTextures(1, &cto_);
    }

    void BlurPostProcessor::process(GLuint fbo, GLuint cto, GLuint rbo) {
        shader_.use();

        std::array<GLuint, 2> pingPongFBO = {fbo, fbo_};
        std::array<GLuint, 2> pingPongCTO = {cto, cto_};

        std::size_t inputIdx  = 0;
        std::size_t outputIdx = 1;
        for (std::size_t idx = 0; idx < amount * 2; idx++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[outputIdx]);
            glBindTexture(GL_TEXTURE_2D, pingPongCTO[inputIdx]);

            shader_.set("direction", inputIdx == 0);
            shader_.set("screen", 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            std::swap(inputIdx, outputIdx);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // UltEngine