//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_DATA_H
#define ULTENGINE_DATA_H

#include <string>
#include <assimp/scene.h>
#include "glm/glm.hpp"

namespace UltEngine {
    using vec3u = glm::vec<3, unsigned>;
    using vec2u = glm::vec<2, unsigned>;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texCoord = {0.0f, 0.0f };
    };

    struct Texture {
        unsigned id;
        int type;
    };

    enum class LightType {
        PointLight,
        DirectionalLight,
        SpotLight
    };
} // UltEngine

#endif //ULTENGINE_DATA_H
