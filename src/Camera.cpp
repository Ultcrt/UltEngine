//
// Created by ultcrt on 24-5-7.
//

#include "glm/gtc/quaternion.hpp"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace UltEngine {
    Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, const Options::CameraOptions &options):
    fov(options.fov),
    aspect(options.aspect),
    near(options.near),
    far(options.far) {
        view_ = glm::lookAt(position, target, up);
        projection_ = glm::perspective(fov, aspect, near, far);

        translation = position;
        rotation = glm::eulerAngles(conjugate(glm::quat(view_)));

        updateTransformation();
    }

    void Camera::updateView() {
        updateTransformation();
        view_ = glm::transpose(transformation_);
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

    void Camera::registerInput(const Engine &engine) {
        // Register cursor move input callback
        glfwSetCursorPosCallback(engine.pWindow, [](GLFWwindow* window, double x, double y){

        });
    }
} // UltEngine