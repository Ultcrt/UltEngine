//
// Created by ultcrt on 24-5-13.
//

#ifndef ULTENGINE_CUBICENVIRONMENT_H
#define ULTENGINE_CUBICENVIRONMENT_H

#include <array>
#include <string>
#include <memory>
#include "IEnvironment.h"
#include "Texture.h"

namespace UltEngine {
    class CubicEnvironment: public IEnvironment {
    private:
        // TODO: Multiple environment using multiple shader object right now
        Shader shader_;

        unsigned multiCTO_;
        unsigned screenVAO_;
        unsigned screenVBO_;

        std::array<Texture, 6> textures_;

    public:
        explicit CubicEnvironment(const std::array<std::string, 6>& paths);

        void draw() override;

        Shader * getShader() override;
    };
} // UltEngine

#endif //ULTENGINE_CUBICENVIRONMENT_H
