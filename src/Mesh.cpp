//
// Created by ultcrt on 24-5-6.
//

#include <cstddef>
#include <iostream>
#include <format>
#include "Mesh.h"
#include "glad/glad.h"

namespace UltEngine {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<vec3u>& triangles, const std::vector<vec2u>& lines, const std::vector<unsigned>& points, const std::shared_ptr<Material>& pMaterial): vertices(vertices), triangles(triangles), lines(lines), points(points), pMaterial(pMaterial) {
        // Create objects (should not be done in update, because otherwise OpenGL would create multiple redundant objects)
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        // Upload to GPU
        upload();
    }

    void Mesh::upload() {
        glBindVertexArray(vao_);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(triangles.size() * sizeof(vec3u)), &triangles[0], GL_STATIC_DRAW);

        // TODO: Lines and points are ignored here

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

        glBindVertexArray(0);
    }

    void Mesh::draw() const {
        pMaterial->pShader->set("model", transformation_);

        pMaterial->prepare();

        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, static_cast<int>(triangles.size()) * 3, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
} // UltEngine