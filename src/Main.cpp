#include <filesystem>
#include <iostream>
#include "Engine.h"
#include "Camera.h"
#include "Scene.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/SpotLight.h"
#include "environment/CubicEnvironment.h"
#include "postprocessor/BlurPostProcessor.h"
#include "postprocessor/BloomPostProcessor.h"
#include "postprocessor/SSAOPostProcessor.h"

int main()
{
    auto pEngine = std::make_shared<UltEngine::Engine>();

    auto pCamera = std::make_shared<UltEngine::Camera>();
    pCamera->translation = glm::vec3(0.0f, 0.0f, 2.0f);
    pEngine->onBeforeRenderObservable.add(pCamera);

    auto pPointLight = std::make_shared<UltEngine::PointLight>(
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{0.1f, 0.1f, 0.1f});
    pPointLight->translation = {3.0f, 3.0f, 3.0f};
    pPointLight->castShadows = true;

    auto pDirectionalLight = std::make_shared<UltEngine::DirectionalLight>(
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{0.1f, 0.1f, 0.1f});
    pDirectionalLight->setDirection(glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)));
    pDirectionalLight->castShadows = true;

    auto pSpotLight = std::make_shared<UltEngine::SpotLight>(
            glm::vec3{10.0f, 10.0f, 10.0f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{0.1f, 0.1f, 0.1f});
    pSpotLight->translation = glm::vec3(0.0f, 0.0f, 1.0f);
    pSpotLight->castShadows = true;

    const std::filesystem::path envDir = std::filesystem::path(RESOURCES_DIRECTORY) / "skybox";
    auto pEnvironment = std::make_shared<UltEngine::CubicEnvironment>(std::array<std::filesystem::path, 6>{
        envDir / "right.jpg",
        envDir / "left.jpg",
        envDir / "top.jpg",
        envDir / "bottom.jpg",
        envDir / "front.jpg",
        envDir / "back.jpg"
    });

    const std::shared_ptr<UltEngine::SSAOPostProcessor> pSSAO = std::make_shared<UltEngine::SSAOPostProcessor>();
    pEngine->addPostProcessor(pSSAO);

    const std::shared_ptr<UltEngine::BloomPostProcessor> pBloom = std::make_shared<UltEngine::BloomPostProcessor>();
    pEngine->addPostProcessor(pBloom);

    // TODO: Should require engine to create scene
    UltEngine::Scene scene;
    scene.pDefaultShader = pEngine->pDefaultShader;
    scene.setCamera(pCamera);
    scene.setEnvironment(pEnvironment);
    scene.addLight(pPointLight);
    scene.addLight(pDirectionalLight);
    //scene.addLight(pSpotLight);
    scene.load(std::filesystem::path(RESOURCES_DIRECTORY) / "backpack/backpack.obj");

    pEngine->render(scene);

    return 0;
}