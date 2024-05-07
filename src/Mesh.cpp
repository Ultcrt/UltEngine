//
// Created by ultcrt on 24-5-6.
//

#include <cstddef>
#include <iostream>
#include <format>
#include "Mesh.h"
#include "glad/glad.h"

namespace UltEngine {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<vec3u>& triangles, const std::vector<vec2u>& lines, const std::vector<unsigned>& points, const std::vector<Texture>& textures) : vertices(vertices), triangles(triangles), lines(lines), points(points), textures(textures) {
        // Create objects (should not be done in update, because otherwise OpenGL would create multiple redundant objects)
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Update buffer data
        update();
    }

    void Mesh::update() {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(triangles.size() * sizeof(vec3u)), &triangles[0], GL_STATIC_DRAW);

        // TODO: Lines and points are ignored here

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));

        glBindVertexArray(0);
    }

    void Mesh::draw(const Shader &shader) const {
        unsigned diffuseNum = 0;
        unsigned specularNum = 0;

        for (unsigned i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string name;
            std::string number;
            switch (textures[i].type) {
                case aiTextureType_DIFFUSE:
                    name = "diffuse";
                    number = std::to_string(diffuseNum++);
                    break;
                case aiTextureType_SPECULAR:
                    name = "specular";
                    number = std::to_string(specularNum++);
                    break;
                default:
                    std::cerr << std::format("Unsupported texture type {}", textures[i].type);
                    break;
            }

            shader.set(std::string("material.").append(name).append(number), static_cast<int>(i));
        }
        // Set back to default
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<int>(triangles.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
} // UltEngine