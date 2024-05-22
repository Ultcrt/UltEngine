//
// Created by ultcrt on 24-5-11.
//

#include <format>
#include <glm/gtc/quaternion.hpp>
#include "light/DirectionalLight.h"

namespace UltEngine {
    DirectionalLight::DirectionalLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient):
    ILight(diffuse, specular, ambient) {
        glGenTextures(1, &shadowMap_);
        glBindTexture(GL_TEXTURE_2D, shadowMap_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     static_cast<int>(shadowMapWidth_), static_cast<int>(shadowMapHeight_), 0,
                     GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void DirectionalLight::setDirection(const glm::vec3 &direction) {
        direction_ = direction;

        setRotationWithDirection(direction);
    }

    glm::vec3 DirectionalLight::getDirection() {
        return direction_;
    }

    void DirectionalLight::prepare(std::size_t idx, const Shader &shader, std::size_t& unitId) {
        std::string lightSignature = signature();
        shader.set(std::format("{}[{}].direction", lightSignature, idx), direction_);

        shader.set(std::format("{}[{}].diffuse", lightSignature, idx), diffuse);
        shader.set(std::format("{}[{}].specular", lightSignature, idx), specular);
        shader.set(std::format("{}[{}].ambient", lightSignature, idx), ambient);

        if (castShadows) {
            glActiveTexture(GL_TEXTURE0 + unitId);
            glBindTexture(GL_TEXTURE_2D, shadowMap_);
            shader.set(std::format("{}[{}].shadowMap", lightSignature, idx), static_cast<int>(unitId++));
            shader.set(std::format("{}[{}].view", lightSignature, idx), view_);
            shader.set(std::format("{}[{}].projection", lightSignature, idx), projection_);
        }
    }

    LightType DirectionalLight::type() {
        return LightType::DirectionalLight;
    }

    void DirectionalLight::prepareShadowMap(const Shader &shader, const BoundingInfo& frustumBoundingInfo) {
        glViewport(0, 0, static_cast<GLint>(shadowMapWidth_), static_cast<GLint>(shadowMapHeight_));

        glm::vec3 xAxis = {1.0f, 0.0f, 0.0f};
        glm::vec3 yAxis = {0.0f, 1.0f, 0.0f};

        float radius = glm::min(frustumBoundingInfo.getRadius(), 5.0f);
        // TODO: Light position is fixed
        auto  center = glm::vec3(0.0f, 0.0f, 0.0f);

        view_ = glm::lookAt(
                center,
                center + direction_,
                glm::dot(-direction_, yAxis) != 0.0f ? yAxis : xAxis
                );

        projection_ = glm::ortho(-radius, radius, -radius, radius, -radius, radius);

        glBindTexture(GL_TEXTURE_2D, shadowMap_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap_, 0);

        shader.set("view", view_);
        shader.set("projection", projection_);
    }
} // UltEngine