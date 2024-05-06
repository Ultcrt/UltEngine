//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_DATA_H
#define ULTENGINE_DATA_H

#include <string>
#include "glm/glm.hpp"

namespace UltEngine {
    using vec3u = glm::vec<3, unsigned>;
    using vec2u = glm::vec<2, unsigned>;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords = { 0.0f, 0.0f };
    };

    struct Texture {
        unsigned id;
        int type;
    };
} // UltEngine

#endif //ULTENGINE_DATA_H