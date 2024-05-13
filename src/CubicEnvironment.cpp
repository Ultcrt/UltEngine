//
// Created by ultcrt on 24-5-13.
//

#include "CubicEnvironment.h"
#include "assimp/material.h"

namespace UltEngine {
    CubicEnvironment::CubicEnvironment(const std::array<std::string, 6> &paths):
    multiCTO_(0),
    shader_(PROJECT_SOURCE_DIR + std::string("/src/shaders/Skybox.vert"),
            PROJECT_SOURCE_DIR + std::string("/src/shaders/Skybox.frag")) {
        // Create texture
        glGenTextures(1, &multiCTO_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, multiCTO_);

        auto options = Options::TextureOptions::ColorTextureOptions;
        options.generateMipmap = false;
        options.flipVertically = false;
        for (std::size_t idx = 0; idx < 6; idx++) {
            options.target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx;
            textures_[idx] = Texture(paths[idx], aiTextureType_DIFFUSE, options);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Create VAO
        glGenVertexArrays(1, &screenVAO_);
        glGenBuffers(1, &screenVBO_);
        // Skybox normals are pointing to inside
        std::array<float, 108> vertices = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        glBindVertexArray(screenVAO_);
        glBindBuffer(GL_ARRAY_BUFFER, screenVBO_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
        glBindVertexArray(0);
    }

    void CubicEnvironment::draw() {
        // Bind skybox texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, multiCTO_);
        shader_.set("skybox", 0);

        // Draw
        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(screenVAO_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
    }

    Shader *CubicEnvironment::getShader() {
        return &shader_;
    }
} // UltEngine