//
// Created by ultcrt on 24-5-6.
//

#include <stdexcept>
#include <array>
#include <filesystem>
#include <iostream>
#include <format>
#include "Engine.h"
#include "Shader.h"

// glDebugOutput function is from: https://learnopengl.com/In-Practice/Debugging
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cerr << "---------------" << std::endl;
    std::cerr << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cerr << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cerr << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cerr << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cerr << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cerr << "Source: Other"; break;
    } std::cerr << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cerr << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cerr << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cerr << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cerr << "Type: Other"; break;
    } std::cerr << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Severity: notification"; break;
    } std::cerr << std::endl;
    std::cerr << std::endl;
}

namespace UltEngine {
    Engine::Engine(const Options::EngineOptions& options): title_(options.title), width_(options.width), height_(options.height), deferRendering_(options.deferRendering) {
        // Init GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Cannot initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // TODO: Enable debug mode, can slow down rendering
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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
        std::cout << "Running at " << glGetString(GL_VERSION) << std::endl;

        // Bind Engine to GLFWwindow object
        glfwSetWindowUserPointer(pWindow, this);

        // Initialize geometry pass
        if (deferRendering_) {
            drawAttachments_ = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            pDefaultShader = std::make_shared<Shader>(std::filesystem::path(SHADER_DIRECTORY) / "DeferBlinnPhong.vert", std::filesystem::path(SHADER_DIRECTORY) / "DeferBlinnPhong.frag");

            glGenFramebuffers(1, &geometryFBO_);
            glGenTextures(geometryGBO_.size(), geometryGBO_.data());
            glGenRenderbuffers(1, &geometryRBO_);
            glBindFramebuffer(GL_FRAMEBUFFER, geometryFBO_);
            glBindRenderbuffer(GL_RENDERBUFFER, geometryRBO_);
            // TODO: Fixed sample size
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERER, geometryRBO_);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            for (std::size_t idx = 0; idx < geometryGBO_.size(); idx++) {
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, geometryGBO_[idx]);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                // TODO: Fixed sample size
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB16F, width_, height_, GL_TRUE);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D_MULTISAMPLE, geometryGBO_[idx], 0);
            }
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else {
            drawAttachments_ = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            pDefaultShader = std::make_shared<Shader>(std::filesystem::path(SHADER_DIRECTORY) / "BlinnPhong.vert", std::filesystem::path(SHADER_DIRECTORY) / "BlinnPhong.frag");
        }
        glDrawBuffers(static_cast<GLint>(drawAttachments_.size()), drawAttachments_.data());

        // Initialize lighting pass frame buffer
        glGenFramebuffers(1, &lightingFBO_);
        glGenTextures(lightingCTOs_.size(), lightingCTOs_.data());
        glGenRenderbuffers(1, &lightingRBO_);
        glBindFramebuffer(GL_FRAMEBUFFER, lightingFBO_);
        glBindRenderbuffer(GL_RENDERBUFFER, lightingRBO_);
        // TODO: Fixed sample size
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width_, height_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, lightingRBO_);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        for (std::size_t idx = 0; idx < lightingCTOs_.size(); idx++) {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, lightingCTOs_[idx]);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // TODO: Fixed sample size
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB16F, width_, height_, GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D_MULTISAMPLE, lightingCTOs_[idx], 0);
        }
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Initialize downscaled texture
        glGenFramebuffers(1, &resolvedFBO_);
        glGenTextures(resolvedCTOs_.size(), resolvedCTOs_.data());
        glGenRenderbuffers(1, &resolvedRBO_);
        glBindFramebuffer(GL_FRAMEBUFFER, resolvedFBO_);
        glBindRenderbuffer(GL_RENDERBUFFER, resolvedRBO_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
        // TODO: Is RBO necessary?
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, resolvedRBO_);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        for (std::size_t idx = 0; idx < resolvedCTOs_.size(); idx++) {
            glBindTexture(GL_TEXTURE_2D, resolvedCTOs_[idx]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, resolvedCTOs_[idx], 0);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Initialize ping pong buffer
        glGenFramebuffers(pingPongFBOs_.size(), pingPongFBOs_.data());
        glGenTextures(pingPongCTOs_.size(), pingPongCTOs_.data());
        glGenRenderbuffers(pingPongCTOs_.size(), pingPongRBOs_.data());
        for (std::size_t idx = 0; idx < 2; idx++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs_[idx]);

            glBindTexture(GL_TEXTURE_2D, pingPongCTOs_[idx]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongCTOs_[idx], 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, pingPongRBOs_[idx]);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pingPongRBOs_[idx]);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

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
            glBufferData(GL_ARRAY_BUFFER, static_cast<long>(sizeof(vertices)), vertices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
        glBindVertexArray(0);

        // Initialize second pass filler shader
        pScreenShader_ = std::make_shared<Shader>(std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.vert", std::filesystem::path(SHADER_DIRECTORY) / "ScreenFiller.frag");

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

        // Set up cursor
        //glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Set up debug mode
        int contextFlags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);
        if (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            std::cerr << "Running at OpenGL debug mode" << std::endl;
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

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

            // Clean up last frame
            glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (deferRendering_) {
                glBindFramebuffer(GL_FRAMEBUFFER, geometryFBO_);
                glEnable(GL_DEPTH_TEST);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                scene.draw();
            }

            // Lighting pass
            glBindFramebuffer(GL_FRAMEBUFFER, lightingFBO_);
            glEnable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            scene.draw();

            // Blit frame buffer
            glBindFramebuffer(GL_READ_FRAMEBUFFER, lightingFBO_);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolvedFBO_);
            glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);

            // Blit from multi-sampled frame buffer to resolved texture
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            glDrawBuffer(GL_COLOR_ATTACHMENT1);
            glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glDrawBuffers(static_cast<GLint>(drawAttachments_.size()), drawAttachments_.data());

            // Invoke post processor
            std::size_t outputIdx = 0;
            for (const auto& pPostProcessor : pPostProcessors_) {
                pPostProcessor->process(screenVAO_, pingPongFBOs_, pingPongCTOs_, pingPongRBOs_, resolvedFBO_, resolvedCTOs_);
            }

            // Second pass
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            pScreenShader_->use();
            glBindVertexArray(screenVAO_);
            glBindTexture(GL_TEXTURE_2D, resolvedCTOs_[0]);
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

        // Update lighting pass frame buffer
        for (std::size_t idx = 0; idx < lightingCTOs_.size(); idx++) {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, lightingCTOs_[idx]);
            // TODO: Fixed sample size
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB16F, width_, height_, GL_TRUE);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, lightingRBO_);
        // TODO: Fixed sample size
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width_, height_);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Update downscaled texture
        for (std::size_t idx = 0; idx < resolvedCTOs_.size(); idx++) {
            glBindTexture(GL_TEXTURE_2D, resolvedCTOs_[idx]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, resolvedRBO_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Update ping pong buffer
        for (std::size_t idx = 0; idx < 2; idx++) {
            glBindTexture(GL_TEXTURE_2D, pingPongCTOs_[idx]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindRenderbuffer(GL_RENDERBUFFER, pingPongRBOs_[idx]);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        // Update view port (second pass)
        glViewport(0, 0, w, h);
    }

    Engine::~Engine() {
        // Destroy GLFW
        glfwTerminate();

        // TODO: Delete frame buffers
    }

    void Engine::addPostProcessor(const std::shared_ptr<IPostProcessor> &pPostProcessor) {
        pPostProcessors_.emplace_back(pPostProcessor);
    }
} // UltEngine