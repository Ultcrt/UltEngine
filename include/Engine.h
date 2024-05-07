//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_ENGINE_H
#define ULTENGINE_ENGINE_H

#include "Options.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Observable.h"

namespace UltEngine {
    class Engine {
    private:
        std::string title;
        int width;
        int height;

    public:
        double deltaTime = 0.0;

        explicit Engine(const Options::EngineOptions& options = {});
        virtual ~Engine();
        void render(const Scene& scene);

        Observable<void (const Engine&)> inputObservable;

        GLFWwindow* pWindow = nullptr;
    };
} // UltEngine

#endif //ULTENGINE_ENGINE_H
