//
// Created by ultcrt on 24-5-11.
//

#include <stdexcept>
#include "light/ILight.h"

namespace UltEngine {
    ILight::ILight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient, std::size_t shadowMapWidth, std::size_t shadowMapHeight):
    diffuse(diffuse), specular(specular), ambient(ambient), shadowMapWidth_(shadowMapWidth), shadowMapHeight_(shadowMapHeight)  {}

    std::string ILight::signature() {
        LightType lightType = type();

        std::string signature;
        switch (lightType) {
            case LightType::PointLight:
                signature = "pointLights";
                break;
            case LightType::DirectionalLight:
                signature = "directionalLights";
                break;
            case LightType::SpotLight:
                signature = "spotLights";
                break;
            default:
                throw std::runtime_error("Unknown light dataType");
        }
        return signature;
    }
} // UltEngine