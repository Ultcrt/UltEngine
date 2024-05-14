//
// Created by ultcrt on 24-5-6.
//

#include <stdexcept>
#include <array>
#include <filesystem>
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

        // Initialize first pass frame buffer
        glGenFramebuffers(1, &multiFBO_);
        glGenTextures(1, &multiCTO_);
        glGenRenderbuffers(1, &multiRBO_);

        glBindFramebuffer(GL_FRAMEBUFFER, multiFBO_);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiCTO_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // TODO: Fixed sample size
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width_, height_, GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiCTO_, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, multiRBO_);
            // TODO: Fixed sample size
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width_, height_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multiRBO_);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Initialize two pass blit frame buffer
        glGenFramebuffers(1, &screenFBO_);
            glGenTextures(1, &screenCTO_);
            glGenRenderbuffers(1, &screenRBO_);
            glBindFramebuffer(GL_FRAMEBUFFER, screenFBO_);
            glBindTexture(GL_TEXTURE_2D, screenCTO_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenCTO_, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, screenRBO_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenRBO_);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Initialize screen quad
        glGenVertexArrays(1, &screenVAO_);
        glGenBuffers(1, &screenVBO_);
        std::array<float, 30> vertices = {
            -1.0f,  1.0f,     0.0f, 1.0f,
            -1.0f, -1.0f,     0.0f, 0.0f,
             1.0f,  1.0f,     1.0f, 1.0f,

             1.0f, -1.0f,     1.0f, 0.0f,
             1.0f,  1.0f,     1.0f, 1.0f,
            -1.0f, -1.0f,     0.0f, 0.0f,
        };

        glBindVertexArray(screenVAO_);
            glBindBuffer(GL_ARRAY_BUFFER, screenVBO_);
            glBufferData(GL_ARRAY_BUFFER, static_cast<long>(sizeof(vertices)), &vertices[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
        glBindVertexArray(0);

        // Initialize second pass filler shader
        pScreenShader_ = std::make_shared<Shader>(std::filesystem::path(SHADER_DIRECTORY) / std::string("ScreenFiller.vert"), std::filesystem::path(SHADER_DIRECTORY) / std::string("ScreenFiller.frag"));

        // Set up viewport callback
        glfwSetFramebufferSizeCallback(pWindow, [](GLFWwindow* pWin, int w, int h){
            auto* pEngine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(pWin));

            pEngine->setSize(w, h);
        });

        // Set up scroll callback
        glfwSetScrollCallback(pWindow, [](GLFWwindow* pWin, double offsetX, double offsetY){
            auto* pEngine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(pWin));

            pEngine->scrollOffset.x = static_cast<float>(offsetX);
            pEngine->scrollOffset.y = static_cast<float>(offsetY);
        });

        // Initialize gl context related member
        pDefaultShader = std::make_shared<Shader>(std::filesystem::path(SHADER_DIRECTORY) / std::string("BlinnPhong.vert"), std::filesystem::path(SHADER_DIRECTORY) / std::string("BlinnPhong.frag"));

        // Set up cursor
        //glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Enabling features
        glfwWindowHint(GLFW_SAMPLES, 4);
        glEnable(GL_MULTISAMPLE);

        glEnable(GL_FRAMEBUFFER_SRGB);

        glEnable(GL_CULL_FACE);
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
            onBeforeRenderObservable.notifyAll(pWindow);

            // First pass
            glBindFramebuffer(GL_FRAMEBUFFER, multiFBO_);
            glEnable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            scene.draw();

            // Blit frame buffer
            glBindFramebuffer(GL_READ_FRAMEBUFFER, multiFBO_);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFBO_);
            glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);

            // Second pass
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            pScreenShader_->use();
            glBindVertexArray(screenVAO_);
            glBindTexture(GL_TEXTURE_2D, screenCTO_);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindTexture(GL_TEXTURE_2D, 0);

            // Update glfw window frame buffer
            glfwSwapBuffers(pWindow);

            // Update attributes
            scrollOffset = {0.0f, 0.0f};
            lastFrameTime = currentFrameTime;

            // GLFW event
            glfwPollEvents();
        }
    }

    glm::vec2 Engine::getSize() {
        return { width_, height_ };
    }

    void Engine::setSize(int w, int h) {
        // Update engine info
        width_ = w;
        height_ = h;

        // Update first pass frame buffer
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiCTO_);
        // TODO: Fixed sample size
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width_, height_, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, multiRBO_);
        // TODO: Fixed sample size
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width_, height_);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Update blit frame buffer
        glBindTexture(GL_TEXTURE_2D, screenCTO_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, screenRBO_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Update view port (second pass)
        glViewport(0, 0, w, h);
    }

    Engine::~Engine() {
        // Destroy GLFW
        glfwTerminate();
    }
} // UltEngine