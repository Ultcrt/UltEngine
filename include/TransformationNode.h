//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_TRANSFORMATIONNODE_H
#define ULTENGINE_TRANSFORMATIONNODE_H

#include "glm/glm.hpp"

namespace UltEngine {
    class TransformationNode {
    public:
        glm::vec3 scaling;
        glm::vec3 rotation;
        glm::vec3 translation;

        TransformationNode();

        TransformationNode(const glm::vec3& scaling, const glm::vec3& rotation, const glm::vec3& translation);

        void setRotationWithDirection(const glm::vec3& direction, const glm::vec3& up = {0.0f, 0.0f, 0.0f});

        void updateTransformation();

    protected:
        glm::mat4 transformation_ = glm::mat4(1.0f);
    };
} // UltEngine

#endif //ULTENGINE_TRANSFORMATIONNODE_H
