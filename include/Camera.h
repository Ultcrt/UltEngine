//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_CAMERA_H
#define ULTENGINE_CAMERA_H

#include <glm/glm.hpp>
#include "Options.h"
#include "TransformationNode.h"

namespace UltEngine {
    class Camera: public TransformationNode {
    private:
        glm::mat4 projection_ = glm::mat4(1.0f);
        glm::mat4 view_ = glm::mat4(1.0f);

    public:
        float fov;
        float aspect;
        float near;
        float far;

        explicit Camera(const glm::vec3& position = {0.0f, 0.0f, 0.0f},
                        const glm::vec3& target = {0.0f, 0.0f, -1.0f},
                        const glm::vec3& up = {0.0f, 1.0f, 0.0f},
                        const Options::CameraOptions& options = {});

        void updateView();
        void updateProjection();

        [[nodiscard]] glm::mat4 getProjection() const;
        [[nodiscard]] glm::mat4 getView() const;
    };
} // UltEngine

#endif //ULTENGINE_CAMERA_H
