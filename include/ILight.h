//
// Created by ultcrt on 24-5-11.
//

#ifndef ULTENGINE_ILIGHT_H
#define ULTENGINE_ILIGHT_H

#include "TransformationNode.h"
#include "Shader.h"
#include "Data.h"

namespace UltEngine {
    class ILight: public TransformationNode {
    public:
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 ambient;

        explicit ILight(const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& ambient);

        virtual std::string signature();

        virtual LightType type() = 0;

        virtual void prepare(std::size_t idx, const Shader& shader) = 0;
    };
} // UltEngine

#endif //ULTENGINE_ILIGHT_H
