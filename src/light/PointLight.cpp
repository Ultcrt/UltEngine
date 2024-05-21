//
// Created by ultcrt on 24-5-11.
//

#include <format>
#include "light/PointLight.h"

namespace UltEngine {
    void PointLight::prepare(std::size_t idx, const Shader &shader) {
        std::string lightSignature = signature();
        shader.set(std::format("{}[{}].position", lightSignature, idx), translation);

        shader.set(std::format("{}[{}].diffuse", lightSignature, idx), diffuse);
        shader.set(std::format("{}[{}].specular", lightSignature, idx), specular);
        shader.set(std::format("{}[{}].ambient", lightSignature, idx), ambient);

        shader.set(std::format("{}[{}].constant", lightSignature, idx), constant);
        shader.set(std::format("{}[{}].linear", lightSignature, idx), linear);
        shader.set(std::format("{}[{}].quadratic", lightSignature, idx), quadratic);
    }

    PointLight::PointLight(const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& ambient): ILight(diffuse, specular, ambient) {}

    LightType PointLight::type() {
        return LightType::PointLight;
    }
} // UltEngine