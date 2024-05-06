//
// Created by ultcrt on 24-5-6.
//

#include <stdexcept>
#include <utility>
#include "Engine.h"

namespace UltEngine {
    Engine::Engine(Options::EngineOptions options): options_(std::move(options)) {
        // Init GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Cannot initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void Engine::start() {
        // Create GLFW window
        pWindow_ = glfwCreateWindow(options_.width, options_.height, options_.title.c_str(), nullptr, nullptr);
        if (!pWindow_) {
            glfwTerminate();
            throw std::runtime_error("Cannot create GLFW window");
        }
        glfwMakeContextCurrent(pWindow_);

        // Init GLAD
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Cannot initialize GLAD");
        }

        // Set up viewport
        glfwSetFramebufferSizeCallback(pWindow_, [](GLFWwindow* win, int w, int h){
            glViewport(0, 0, w, h);
        });

        // Render loop
        while (!glfwWindowShouldClose(pWindow_)) {
            if (glfwGetKey(pWindow_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(pWindow_, true);

            glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(pWindow_);
            glfwPollEvents();
        }
    }

    Engine::~Engine() {
        // Destroy GLFW
        glfwTerminate();
    }
} // UltEngine