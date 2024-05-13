//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_MESH_H
#define ULTENGINE_MESH_H

#include <vector>
#include "Data.h"
#include "Material.h"
#include "TransformationNode.h"

namespace UltEngine {
    class Mesh: public TransformationNode {
    public:
        std::vector<Vertex> vertices;
        std::vector<vec3u> triangles;
        std::vector<vec2u> lines;
        std::vector<unsigned> points;

        std::shared_ptr<Material> pMaterial;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<vec3u>& triangles, const std::vector<vec2u>& lines, const std::vector<unsigned>& points, const std::shared_ptr<Material>& pMaterial);

        void upload();

        void draw() const;

    private:
        unsigned screenVAO_ = 0;
        unsigned screenVBO_ = 0;
        unsigned ebo_ = 0;
    };
} // UltEngine

#endif //ULTENGINE_MESH_H
