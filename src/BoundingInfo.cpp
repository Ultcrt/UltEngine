//
// Created by ultcrt on 24-5-21.
//

#include "BoundingInfo.h"

namespace UltEngine {
    BoundingInfo::BoundingInfo(const glm::vec3 &center, float radius):
    center_(center),
    min_(center - glm::vec3(radius / std::sqrt(3.0f))),
    max_(center + glm::vec3(radius / std::sqrt(3.0f))) {}

    glm::vec3 BoundingInfo::getCenter() const {
        return center_;
    }

    float BoundingInfo::getRadius() const {
        return glm::length(max_ - min_);
    }
} // UltEngine