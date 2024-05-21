//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_OPTIONS_H
#define ULTENGINE_OPTIONS_H

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glad/glad.h"

namespace UltEngine {
    namespace Options {
        struct EngineOptions {
            int width  = 640;
            int height = 360;
            std::string title = "UltEngine";

            bool deferRendering = true;
        };

        struct CameraOptions {
            // Space options
            float fov    = glm::radians(45.0f);
            float aspect = 640.0f / 360.0f;
            float near   = 0.1f;
            float far    = 5.0f;

            // Movement options
            float moveSpeed = 2.5f;
            float rotateSensitivity = 0.5f;
            float zoomSensitivity = 5.0f;
        };

        struct TextureOptions {
            static TextureOptions ColorTextureOptions;
            static TextureOptions NonColorTextureOptions;

            bool srgb = false;
            bool generateMipmap = true;
            bool flipVertically = true;
            GLenum target = GL_TEXTURE_2D;
            GLenum dataType = GL_UNSIGNED_BYTE;
            std::unordered_map<GLenum, GLint> params = {};
        };
    } // Options
} // UltEngine

#endif //ULTENGINE_OPTIONS_H
