//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_ENGINE_H
#define ULTENGINE_ENGINE_H

#include <memory>
#include <array>
#include "Options.h"
#include "Scene.h"
#include "Observable.h"
#include "Shader.h"
#include "IPostProcessor.h"

namespace UltEngine {
    class Engine {
    private:
        std::vector<GLenum> drawAttachments_;

        std::string title_;
        int width_;
        int height_;

        bool deferRendering_;

        // Geometry pass
        GLuint geometryFBO_;
        std::array<GLuint, 4> geometryGBO_;
        GLuint geometryRBO_;
        
        // Lighting pass
        GLuint lightingFBO_;
        std::array<GLuint, 2> lightingCTOs_;
        GLuint lightingRBO_;

        // Screen pass
        GLuint screenVAO_;
        GLuint screenVBO_;

        // Downscaled multi-sampled texture
        GLuint resolvedFBO_;
        std::array<GLuint, 2> resolvedCTOs_;
        GLuint resolvedRBO_;

        std::array<GLuint, 2> pingPongFBOs_;
        std::array<GLuint, 2> pingPongCTOs_;
        std::array<GLuint, 2> pingPongRBOs_;

        std::shared_ptr<Shader> pScreenShader_;
        std::vector<std::shared_ptr<IPostProcessor>> pPostProcessors_;

    public:
        Observable<GLFWwindow*> onBeforeRenderObservable;

        std::shared_ptr<Shader> pDefaultShader;

        double deltaTime = 0.0;
        glm::vec2 scrollOffset = {0.0, 0.0};

        explicit Engine(const Options::EngineOptions& options = {});
        virtual ~Engine();
        void render(const Scene& scene);

        GLFWwindow* pWindow = nullptr;

        glm::vec2 getSize();
        void setSize(int w, int h);

        void addPostProcessor(const std::shared_ptr<IPostProcessor>& pPostProcessor);
    };
} // UltEngine

#endif //ULTENGINE_ENGINE_H
