//
// Created by ultcrt on 24-5-21.
//

#ifndef ULTENGINE_BOUNDINGINFO_H
#define ULTENGINE_BOUNDINGINFO_H

#include "glm/glm.hpp"
#include "TransformationNode.h"

namespace UltEngine {
    class BoundingInfo {
    private:
        glm::vec3 center_;
        glm::vec3 min_;
        glm::vec3 max_;

    public:
        BoundingInfo(const glm::vec3& center, float radius);

        glm::vec3 getCenter() const;
        float getRadius() const;
    };
} // UltEngine

#endif //ULTENGINE_BOUNDINGINFO_H
