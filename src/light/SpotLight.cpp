//
// Created by ultcrt on 24-5-11.
//

#include <format>
#include <iostream>
#include "light/SpotLight.h"

namespace UltEngine {
    SpotLight::SpotLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient) : DirectionalLight(diffuse, specular, ambient) {}

    void SpotLight::prepare(std::size_t idx, const Shader &shader, std::size_t& unitId) {
        std::string lightSignature = signature();

        DirectionalLight::prepare(idx, shader, unitId);

        shader.set(std::format("{}[{}].position", lightSignature, idx), translation);

        shader.set(std::format("{}[{}].innerCutoff", lightSignature, idx), innerCutoff);
        shader.set(std::format("{}[{}].outerCutoff", lightSignature, idx), outerCutoff);
    }

    LightType SpotLight::type() {
        return LightType::SpotLight;
    }
} // UltEngine