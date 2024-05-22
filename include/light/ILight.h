//
// Created by ultcrt on 24-5-11.
//

#ifndef ULTENGINE_ILIGHT_H
#define ULTENGINE_ILIGHT_H

#include "TransformationNode.h"
#include "Shader.h"
#include "Data.h"
#include "BoundingInfo.h"

namespace UltEngine {
    class ILight: public TransformationNode {
    protected:
        GLuint shadowMap_ = 0;

        std::size_t shadowMapWidth_;
        std::size_t shadowMapHeight_;

        glm::mat4 view_;
        glm::mat4 projection_;

    public:
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 ambient;

        bool castShadows = false;

        explicit ILight(const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& ambient,
                        std::size_t shadowMapWidth = 1024, std::size_t shadowMapHeight = 1024);

        std::string signature();

        virtual LightType type() = 0;

        virtual void prepare(std::size_t idx, const Shader& shader, std::size_t& unitId) = 0;

        virtual void prepareShadowMap(const Shader &shader, const BoundingInfo& frustumBoundingInfo) = 0;
    };
} // UltEngine

#endif //ULTENGINE_ILIGHT_H
