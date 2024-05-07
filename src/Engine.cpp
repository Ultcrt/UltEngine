//
// Created by ultcrt on 24-5-6.
//

#include <stdexcept>
#include <utility>
#include "Engine.h"

namespace UltEngine {
    Engine::Engine(const Options::EngineOptions& options): title(options.title), width(options.width), height(options.height) {
        // Init GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Cannot initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create GLFW window
        pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!pWindow) {
            glfwTerminate();
            throw std::runtime_error("Cannot create GLFW window");
        }
        glfwMakeContextCurrent(pWindow);

        // Init GLAD
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Cannot initialize GLAD");
        }

        // Set up viewport
        glfwSetFramebufferSizeCallback(pWindow, [](GLFWwindow* win, int w, int h){
            glViewport(0, 0, w, h);
        });
    }

    void Engine::render(const Scene& scene) {
        // Render loop
        // TODO: Test code
        Shader shader("../src/shaders/BlinnPhong.vert", "../src/shaders/BlinnPhong.frag");

        double lastFrameTime = glfwGetTime();
        while (!glfwWindowShouldClose(pWindow)) {
            deltaTime = glfwGetTime() - lastFrameTime;

            if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(pWindow, true);

            glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            scene.draw(shader);

            glfwSwapBuffers(pWindow);
            glfwPollEvents();
        }
    }

    Engine::~Engine() {
        // Destroy GLFW
        glfwTerminate();
    }
} // UltEngine