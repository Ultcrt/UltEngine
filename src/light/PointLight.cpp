//
// Created by ultcrt on 24-5-11.
//

#include <format>
#include "light/PointLight.h"

namespace UltEngine {
    void PointLight::prepare(std::size_t idx, const Shader &shader, std::size_t& unitId) {
        std::string lightSignature = signature();
        shader.set(std::format("{}[{}].position", lightSignature, idx), translation);

        shader.set(std::format("{}[{}].diffuse", lightSignature, idx), diffuse);
        shader.set(std::format("{}[{}].specular", lightSignature, idx), specular);
        shader.set(std::format("{}[{}].ambient", lightSignature, idx), ambient);

        shader.set(std::format("{}[{}].constant", lightSignature, idx), constant);
        shader.set(std::format("{}[{}].linear", lightSignature, idx), linear);
        shader.set(std::format("{}[{}].quadratic", lightSignature, idx), quadratic);
    }

    PointLight::PointLight(const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& ambient): ILight(diffuse, specular, ambient) {
        glGenTextures(1, &shadowMap_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap_);
        for (std::size_t idx = 0; idx < 6; idx++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, GL_DEPTH_COMPONENT,
                         static_cast<int>(shadowMapWidth_), static_cast<int>(shadowMapHeight_),
                         0, GL_DEPTH_COMPONENT, GL_FLOAT,nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    LightType PointLight::type() {
        return LightType::PointLight;
    }

    void PointLight::prepareShadowMap(const Shader &shader, const BoundingInfo &frustumBoundingInfo) {

    }
} // UltEngine