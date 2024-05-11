//
// Created by ultcrt on 24-5-8.
//

#include <iostream>
#include <format>
#include "Material.h"

namespace UltEngine {
	Material::Material(const std::vector<Texture>& textures, const std::shared_ptr<Shader>& pShader): textures(textures), pShader(pShader) {}

	void Material::prepare() {
        pShader->set("material.shininess", shininess);

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
            glBindTexture(GL_TEXTURE_2D, textures[i].id);

            pShader->set(std::string("material.").append(name).append(number), static_cast<int>(i));
        }
        // Set texture unit back to default
        glActiveTexture(GL_TEXTURE0);
    }
} // UltEngine
