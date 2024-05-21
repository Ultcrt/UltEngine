//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_CAMERA_H
#define ULTENGINE_CAMERA_H

#include <glm/glm.hpp>
#include "Options.h"
#include "TransformationNode.h"
#include "Observer.h"
#include "BoundingInfo.h"

namespace UltEngine {
    class Camera: public TransformationNode, public RenderObserver {
    private:
        glm::mat4 projection_ = glm::mat4(1.0f);
        glm::mat4 view_ = glm::mat4(1.0f);

        double lastCursorPoseX_ = 0.0;
        double lastCursorPoseY_ = 0.0;

    public:
        float fov;
        float aspect;
        float near;
        float far;

        float moveSpeed;
        float rotateSensitivity;
        float zoomSensitivity;

        explicit Camera(const glm::vec3& position = {0.0f, 0.0f, 0.0f},
                        const glm::vec3& target = {0.0f, 0.0f, -1.0f},
                        const glm::vec3& up = {0.0f, 1.0f, 0.0f},
                        const Options::CameraOptions& options = {});

        void updateView();
        void updateProjection();

        void update(GLFWwindow* pWindow) override;

        [[nodiscard]] glm::mat4 getProjection() const;
        [[nodiscard]] glm::mat4 getView() const;

        [[nodiscard]] BoundingInfo getBoundingInfo() const;
    };
} // UltEngine

#endif //ULTENGINE_CAMERA_H
