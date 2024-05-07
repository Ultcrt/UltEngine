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
} // UltEngine