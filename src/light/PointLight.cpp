//
// Created by ultcrt on 24-5-11.
//

#include <format>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
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

        if (castShadows) {
            glActiveTexture(GL_TEXTURE0 + unitId);
            glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap_);
            shader.set(std::format("{}[{}].shadowMap", lightSignature, idx), static_cast<int>(unitId++));
        }
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
        glViewport(0, 0, static_cast<GLint>(shadowMapWidth_), static_cast<GLint>(shadowMapHeight_));

        glm::vec3 xAxis = { 1.0f, 0.0f, 0.0f };
        glm::vec3 yAxis = { 0.0f, 1.0f, 0.0f };
        glm::vec3 zAxis = { 0.0f, 0.0f, 1.0f };

        std::vector<glm::mat4> views;

        views.emplace_back(glm::lookAt(
            translation,
            translation + xAxis,
            -yAxis
        ));
        views.emplace_back(glm::lookAt(
            translation,
            translation - xAxis,
            -yAxis
        ));
        views.emplace_back(glm::lookAt(
            translation,
            translation + yAxis,
            zAxis
        ));
        views.emplace_back(glm::lookAt(
            translation,
            translation - yAxis,
            -zAxis
        ));
        views.emplace_back(glm::lookAt(
            translation,
            translation + zAxis,
            -yAxis
        ));
        views.emplace_back(glm::lookAt(
            translation,
            translation - zAxis,
            -yAxis
        ));

        // TODO: Attenuation can be used as far plane
        float far = 10.0f;
        projection_ = glm::perspective(glm::radians(90.0f), static_cast<float>(shadowMapWidth_) / static_cast<float>(shadowMapHeight_), 0.1f, far);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap_, 0);

        for (std::size_t idx = 0; idx < 6; idx++) {
            shader.set(std::format("views[{}]", idx), views[idx]);
        }
        shader.set("projection", projection_);
        shader.set("lightPosition", translation);
        shader.set("far", far);
    }
} // UltEngine