//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_MESH_H
#define ULTENGINE_MESH_H

#include <vector>
#include "Data.h"

namespace UltEngine {
    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<vec3u> triangles;
        std::vector<vec2u> lines;
        std::vector<unsigned> points;
        std::vector<Texture> textures;

        unsigned VAO = 0;
        unsigned VBO = 0;
        unsigned EBO = 0;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<vec3u>& triangles, const std::vector<vec2u>& lines, const std::vector<unsigned>& points, const std::vector<Texture>& textures);

        void update();
    };
} // UltEngine

#endif //ULTENGINE_MESH_H
