//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_ENGINE_H
#define ULTENGINE_ENGINE_H

#include "Options.h"
#include "Scene.h"
#include "Observable.h"
#include <Shader.h>

namespace UltEngine {
    class Engine {
    private:
        std::string title_;
        int width_;
        int height_;

    public:
        Observable<GLFWwindow*> inputObservable;

        std::shared_ptr<Shader> pDefaultShader;

        double deltaTime = 0.0;
        glm::vec2 scrollOffset = {0.0, 0.0};

        explicit Engine(const Options::EngineOptions& options = {});
        virtual ~Engine();
        void render(const Scene& scene);

        GLFWwindow* pWindow = nullptr;
    };
} // UltEngine

#endif //ULTENGINE_ENGINE_H
