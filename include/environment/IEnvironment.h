//
// Created by ultcrt on 24-5-13.
//

#ifndef ULTENGINE_IENVIRONMENT_H
#define ULTENGINE_IENVIRONMENT_H

#include "Shader.h"

namespace UltEngine {
    class IEnvironment {
    public:
        virtual void draw() = 0;
        virtual Shader* getShader() = 0;
    };
} // UltEngine

#endif //ULTENGINE_IENVIRONMENT_H
