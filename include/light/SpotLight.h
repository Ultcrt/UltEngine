//
// Created by ultcrt on 24-5-11.
//

#ifndef ULTENGINE_SPOTLIGHT_H
#define ULTENGINE_SPOTLIGHT_H

#include "DirectionalLight.h"

namespace UltEngine {
    class SpotLight: public DirectionalLight {
    public:
        float innerCutoff = glm::radians(30.0f);
        float outerCutoff = glm::radians(45.0f);

        SpotLight(const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& ambient);

        void prepare(std::size_t idx, const UltEngine::Shader &shader, std::size_t& unitId) override;

        void prepareShadowMap(const Shader &shader, const BoundingInfo& frustumBoundingInfo) override;

        LightType type() override;
    };
} // UltEngine

#endif //ULTENGINE_SPOTLIGHT_H
