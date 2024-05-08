//
// Created by ultcrt on 24-5-7.
//

#include <algorithm>
#include "glm/gtc/quaternion.hpp"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Engine.h"

namespace UltEngine {
    Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, const Options::CameraOptions& options):
    fov(options.fov),
    aspect(options.aspect),
    near(options.near),
    far(options.far),
    moveSpeed(options.moveSpeed),
    rotateSensitivity(options.rotateSensitivity),
    zoomSensitivity(options.zoomSensitivity) {
        view_ = glm::lookAt(position, target, up);
        projection_ = glm::perspective(fov, aspect, near, far);

        translation = position;
        rotation = glm::eulerAngles(conjugate(glm::quat(view_)));

        updateTransformation();
    }

    void Camera::updateView() {
        updateTransformation();
        view_ = glm::inverse(transformation_);
    }

    void Camera::updateProjection() {
        projection_ = glm::perspective(fov, aspect, near, far);
    }

    glm::mat4 Camera::getProjection() const {
        return projection_;
    }

    glm::mat4 Camera::getView() const {
        return view_;
    }

    void Camera::update(GLFWwindow* pWindow) {
        // Get engine pointer
        auto* pEngine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(pWindow));
        double deltaTime = pEngine->deltaTime;

        // Moving
        auto moveOffset = static_cast<float>(deltaTime * moveSpeed);
        glm::vec3 front = transformation_ * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        glm::vec3 right = transformation_ * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);

        if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
            translation += moveOffset * front;
        if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
            translation += moveOffset * right;
        if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
            translation -= moveOffset * front;
        if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
            translation -= moveOffset * right;

        // Rotate
        float threshold = (89.0f) * glm::pi<float>() / 180.0f;
        double cursorPoseX, cursorPoseY;
        glfwGetCursorPos(pWindow, &cursorPoseX, &cursorPoseY);
        double rotateOffsetX = (lastCursorPoseX_ - cursorPoseX) * rotateSensitivity * deltaTime;
        double rotateOffsetY = (lastCursorPoseY_ - cursorPoseY) * rotateSensitivity * deltaTime;

        rotation.y += static_cast<float>(rotateOffsetX);
        rotation.x += static_cast<float>(rotateOffsetY);
        rotation.x = std::clamp(rotation.x, -threshold, threshold);                        // Clamp to avoid flip
        // Update cursor position
        lastCursorPoseX_ = cursorPoseX;
        lastCursorPoseY_ = cursorPoseY;

        // Zoom
        float scrollOffsetY = pEngine->scrollOffset.y;
        fov += scrollOffsetY * zoomSensitivity * static_cast<float>(deltaTime);

        // Update view and projection matrix
        updateView();
        updateProjection();
    }
} // UltEngine