//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_DATA_H
#define ULTENGINE_DATA_H

#include <string>
#include "glm/glm.hpp"

namespace UltEngine {
    using vec3u = glm::vec<3, unsigned>;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct Texture {
        unsigned id;
        std::string type;
    };
} // UltEngine

#endif //ULTENGINE_DATA_H
