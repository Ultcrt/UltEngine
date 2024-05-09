//
// Created by ultcrt on 24-5-6.
//

#include <stdexcept>
#include "Engine.h"
#include "Shader.h"

namespace UltEngine {
    Engine::Engine(const Options::EngineOptions& options): title_(options.title), width_(options.width), height_(options.height) {
        // Init GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Cannot initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create GLFW window
        pWindow = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
        if (!pWindow) {
            glfwTerminate();
            throw std::runtime_error("Cannot create GLFW window");
        }
        glfwMakeContextCurrent(pWindow);

        // Init GLAD
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Cannot initialize GLAD");
        }

        // Bind Engine to GLFWwindow object
        glfwSetWindowUserPointer(pWindow, this);

        // Set up viewport callback
        glfwSetFramebufferSizeCallback(pWindow, [](GLFWwindow* win, int w, int h){
            glViewport(0, 0, w, h);
        });

        // Set up scroll callback
        glfwSetScrollCallback(pWindow, [](GLFWwindow* pWin, double offsetX, double offsetY){
            auto* pEngine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(pWin));

            pEngine->scrollOffset.x = static_cast<float>(offsetX);
            pEngine->scrollOffset.y = static_cast<float>(offsetY);
        });

        // Initialize gl context related member
        pDefaultShader = std::make_shared<Shader>(PROJECT_SOURCE_DIR + std::string("/src/shaders/BlinnPhong.vert"), PROJECT_SOURCE_DIR + std::string("/src/shaders/BlinnPhong.frag"));

        // Set up cursor
        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Enabling features
        glEnable(GL_DEPTH_TEST);

        glfwWindowHint(GLFW_SAMPLES, 4);
        glEnable(GL_MULTISAMPLE);

        glEnable(GL_FRAMEBUFFER_SRGB);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Engine::render(const Scene& scene) {
        // Render loop
        double lastFrameTime = glfwGetTime();
        double currentFrameTime;
        while (!glfwWindowShouldClose(pWindow)) {
            currentFrameTime = glfwGetTime();
            deltaTime = currentFrameTime - lastFrameTime;

            // Escape listener
            if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(pWindow, true);

            // Notify other input observers
            inputObservable.notifyAll(pWindow);

            // Clear up color
            glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.draw();
            glfwSwapBuffers(pWindow);

            // Update attributes
            scrollOffset = {0.0f, 0.0f};
            lastFrameTime = currentFrameTime;

            // GLFW event
            glfwPollEvents();
        }
    }

    Engine::~Engine() {
        // Destroy GLFW
        glfwTerminate();
    }
} // UltEngine