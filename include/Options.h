//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_OPTIONS_H
#define ULTENGINE_OPTIONS_H

#include <string>
#include <glm/glm.hpp>

namespace UltEngine {
    namespace Options {
        struct EngineOptions {
            int width  = 640;
            int height = 360;
            std::string title = "UltEngine";
        };

        struct CameraOptions {
            // Space options
            float fov    = glm::radians(45.0f);
            float aspect = 640.0f / 360.0f;
            float near   = 0.1f;
            float far    = 100.0f;

            // Movement options
            float moveSpeed = 2.5f;
            float rotateSensitivity = 0.5f;
            float zoomSensitivity = 5.0f;
        };
    } // Options
} // UltEngine

#endif //ULTENGINE_OPTIONS_H
