//
// Created by ultcrt on 24-5-7.
//

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "TransformationNode.h"

namespace UltEngine {
    TransformationNode::TransformationNode(const glm::vec3 &scaling, const glm::vec3 &rotation,
                                           const glm::vec3 &translation): scaling(scaling), rotation(rotation), translation(translation) {
        updateTransformation();
    }

    void TransformationNode::updateTransformation() {
        const auto identity = glm::mat4(1.0f);
        const auto s = glm::scale(identity, scaling);
        const auto r = glm::mat4(glm::quat(rotation));
        const auto t = glm::translate(identity, translation);
        transformation_ = t * r * s;
    }

    TransformationNode::TransformationNode(): scaling(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), translation(glm::vec3(0.0f)) {
        updateTransformation();
    }

    void TransformationNode::setRotationWithDirection(const glm::vec3 &direction, const glm::vec3 &up) {
        const glm::vec3 worldZ = glm::vec3{0.0f, 0.0f, 1.0f};
        const glm::vec3 worldY = glm::vec3{0.0f, 1.0f, 0.0f};

        glm::vec3 upVec = up;
        if (glm::length(upVec) == 0.0f) {
            upVec = direction == worldY ? worldZ : worldY;
        }
        const glm::vec3 normalizedDirection = glm::normalize(direction);

        const glm::vec3 axisX = glm::normalize(glm::cross(direction, upVec));
        const glm::vec3 axisZ = -normalizedDirection;
        const glm::vec3 axisY = glm::normalize(glm::cross(axisX, axisZ));

        rotation = glm::eulerAngles(glm::quat(glm::mat3(axisX, axisY, axisZ)));

        updateTransformation();
    }
} // UltEngine