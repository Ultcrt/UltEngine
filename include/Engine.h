//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_ENGINE_H
#define ULTENGINE_ENGINE_H

#include "Options.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace UltEngine {
    class Engine {
    public:
        explicit Engine(Options::EngineOptions options = {});
        virtual ~Engine();
        void start();

    private:
        GLFWwindow* pWindow_ = nullptr;
        Options::EngineOptions options_;
    };
} // UltEngine

#endif //ULTENGINE_ENGINE_H
