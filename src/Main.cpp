#include <filesystem>
#include <iostream>
#include "Engine.h"
#include "Camera.h"
#include "Scene.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "CubicEnvironment.h"

int main()
{
    auto pEngine = std::make_shared<UltEngine::Engine>();

    auto pCamera = std::make_shared<UltEngine::Camera>();
    pCamera->translation = glm::vec3(0.0f, 0.0f, 2.0f);

    auto pPointLight = std::make_shared<UltEngine::PointLight>(
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{0.1f, 0.1f, 0.1f});
    pPointLight->translation = {3.0f, 3.0f, 3.0f};

    auto pDirectionalLight = std::make_shared<UltEngine::DirectionalLight>(
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{0.1f, 0.1f, 0.1f});
    pDirectionalLight->setDirection(glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)));

    auto pSpotLight = std::make_shared<UltEngine::SpotLight>(
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{1.0f, 1.0f, 1.0f},
            glm::vec3{0.1f, 0.1f, 0.1f});

    const std::filesystem::path envDir = std::filesystem::path(RESOURCES_DIRECTORY) / std::string("skybox");
    auto pEnvironment = std::make_shared<UltEngine::CubicEnvironment>(std::array<std::string, 6>{
        envDir / "right.jpg",
        envDir / "left.jpg",
        envDir / "top.jpg",
        envDir / "bottom.jpg",
        envDir / "front.jpg",
        envDir / "back.jpg"
    });

    pEngine->onBeforeRenderObservable.add(pCamera);

    UltEngine::Scene scene;
    scene.pDefaultShader = pEngine->pDefaultShader;
    scene.setCamera(pCamera);
    scene.setEnvironment(pEnvironment);
//    scene.addLight(pPointLight);
    scene.addLight(pDirectionalLight);
//    scene.addLight(pSpotLight);
    scene.load(std::filesystem::path(RESOURCES_DIRECTORY) / std::string("backpack/backpack.obj"));

    pEngine->render(scene);

    return 0;
}