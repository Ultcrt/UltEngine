//
// Created by ultcrt on 24-5-13.
//

#include <format>
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace UltEngine {
    Texture::Texture(const std::filesystem::path &path, int type, const Options::TextureOptions &options): id(0), type(type) {
        stbi_set_flip_vertically_on_load(options.flipVertically);

        int width, height, channels;
        unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

        if (!data) {
            throw std::runtime_error(std::format("Cannot load texture '{}'", path.string()));
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        GLint internalformat;
        GLenum format;
        switch (channels) {
            case 1:
                internalformat = GL_RED;
                format = GL_RED;
                break;
            case 3:
                internalformat = options.srgb ? GL_SRGB : GL_RGB;
                format = GL_RGB;
                break;
            case 4:
                internalformat = options.srgb ? GL_SRGB_ALPHA : GL_RGBA;
                format = GL_RGBA;
                break;
            default:
                throw std::runtime_error(std::format("Unexpected texture channels {}", channels));
        }

        // Load based on options
        glTexImage2D(options.target, 0, internalformat, width, height, 0, format, options.dataType, data);
        if (options.generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
        for (const auto [key, val]: options.params) {
            glTexParameteri(GL_TEXTURE_2D, key, val);
        }

        // Free up memory
        stbi_image_free(data);
    }

    Texture::Texture(unsigned id, int type): id(id), type(type) {}

    Texture::Texture(): id(0), type(-1) {}
} // UltEngine