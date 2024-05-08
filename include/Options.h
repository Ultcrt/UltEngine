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
            int width  = 800;
            int height = 600;
            std::string title = "UltEngine";
        };

        struct CameraOptions {
            // Space options
            float fov    = glm::radians(45.0f);
            float aspect = 4.0f / 3.0f;
            float near   = 0.1f;
            float far    = 100.0f;

            // Movement options
            float moveSpeed = 2.5f;
            float rotateSensitivity = 0.2f;
            float zoomSensitivity = 0.2f;
        };
    } // Options
} // UltEngine

#endif //ULTENGINE_OPTIONS_H
