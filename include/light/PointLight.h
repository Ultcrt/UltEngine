//
// Created by ultcrt on 24-5-11.
//

#ifndef ULTENGINE_POINTLIGHT_H
#define ULTENGINE_POINTLIGHT_H

#include "ILight.h"

namespace UltEngine {
    class PointLight: public ILight{
    public:
        float constant = 0.0f;
        float linear = 0.0f;
        float quadratic = 1.0f;

        explicit PointLight(const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& ambient);

        void prepare(std::size_t idx, const Shader &shader, std::size_t& unitId) override;

        void prepareShadowMap(const UltEngine::Shader &shader, const UltEngine::BoundingInfo &frustumBoundingInfo) override;

        LightType type() override;
    };
} // UltEngine

#endif //ULTENGINE_POINTLIGHT_H
