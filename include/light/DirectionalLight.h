//
// Created by ultcrt on 24-5-11.
//

#ifndef ULTENGINE_DIRECTIONALLIGHT_H
#define ULTENGINE_DIRECTIONALLIGHT_H

#include "ILight.h"

namespace UltEngine {
    class DirectionalLight: public ILight {
    protected:
        glm::vec3 direction_ = {0.0f, 0.0f, -1.0f};         // Default

    public:
        explicit DirectionalLight(const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& ambient);

        void prepare(std::size_t idx, const Shader &shader, std::size_t& unitId) override;

        void prepareShadowMap(const Shader &shader, const BoundingInfo& frustumBoundingInfo) override;

        LightType type() override;

        void setDirection(const glm::vec3& direction);

        glm::vec3 getDirection();
    };
} // UltEngine

#endif //ULTENGINE_DIRECTIONALLIGHT_H
