//
// Created by ultcrt on 24-5-11.
//

#include <format>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "light/SpotLight.h"

namespace UltEngine {
    SpotLight::SpotLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient) : DirectionalLight(diffuse, specular, ambient) {}

    void SpotLight::prepare(std::size_t idx, const Shader &shader, std::size_t& unitId) {
        std::string lightSignature = signature();

        DirectionalLight::prepare(idx, shader, unitId);

        shader.set(std::format("{}[{}].position", lightSignature, idx), translation);

        shader.set(std::format("{}[{}].innerCutoff", lightSignature, idx), glm::cos(innerCutoff));
        shader.set(std::format("{}[{}].outerCutoff", lightSignature, idx), glm::cos(outerCutoff));
    }

    LightType SpotLight::type() {
        return LightType::SpotLight;
    }

    void SpotLight::prepareShadowMap(const Shader &shader, const BoundingInfo& frustumBoundingInfo) {
        glViewport(0, 0, static_cast<GLint>(shadowMapWidth_), static_cast<GLint>(shadowMapHeight_));

        glm::vec3 xAxis = {1.0f, 0.0f, 0.0f};
        glm::vec3 yAxis = {0.0f, 1.0f, 0.0f};

        view_ = glm::lookAt(
                translation,
                translation + direction_,
                glm::dot(direction_, yAxis) != 0.0f ? yAxis : xAxis
        );

        // TODO: Spot light should have attenuation, and attenuation can be used as far plane
        projection_ = glm::perspective(outerCutoff, static_cast<float>(shadowMapWidth_) / static_cast<float>(shadowMapHeight_), 0.1f, 100.0f);

        glBindTexture(GL_TEXTURE_2D, shadowMap_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap_, 0);

        shader.set("view", view_);
        shader.set("projection", projection_);
    }
} // UltEngine