//
// Created by ultcrt on 24-5-11.
//

#include <format>
#include <glm/gtc/quaternion.hpp>
#include "light/DirectionalLight.h"

namespace UltEngine {
    DirectionalLight::DirectionalLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient): ILight(diffuse, specular, ambient) {}

    void DirectionalLight::setDirection(const glm::vec3 &direction) {
        direction_ = direction;

        setRotationWithDirection(direction);
    }

    glm::vec3 DirectionalLight::getDirection() {
        return direction_;
    }

    void DirectionalLight::prepare(std::size_t idx, const Shader &shader) {
        std::string lightSignature = signature();
        shader.set(std::format("{}[{}].direction", lightSignature, idx), direction_);

        shader.set(std::format("{}[{}].diffuse", lightSignature, idx), diffuse);
        shader.set(std::format("{}[{}].specular", lightSignature, idx), specular);
        shader.set(std::format("{}[{}].ambient", lightSignature, idx), ambient);
    }

    LightType DirectionalLight::type() {
        return LightType::DirectionalLight;
    }
} // UltEngine