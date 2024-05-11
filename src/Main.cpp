#include "Engine.h"
#include "Camera.h"
#include "Scene.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

int main()
{
    auto pEngine = std::make_shared<UltEngine::Engine>();
    auto pCamera = std::make_shared<UltEngine::Camera>();
    auto pPointLight = std::make_shared<UltEngine::PointLight>(
            glm::vec3{1.0f, 0.0f, 0.0f},
            glm::vec3{1.0f, 0.0f, 0.0f},
            glm::vec3{0.1f, 0.0f, 0.0f});

    pPointLight->translation = {3.0f, 3.0f, 3.0f};

    auto pDirectionalLight = std::make_shared<UltEngine::DirectionalLight>(
            glm::vec3{1.0f, 0.0f, 0.0f},
            glm::vec3{1.0f, 0.0f, 0.0f},
            glm::vec3{0.1f, 0.0f, 0.0f});

    auto pSpotLight = std::make_shared<UltEngine::SpotLight>(
            glm::vec3{1.0f, 0.0f, 0.0f},
            glm::vec3{1.0f, 0.0f, 0.0f},
            glm::vec3{0.1f, 0.0f, 0.0f});

    pEngine->onBeforeRenderObservable.add(pCamera);

    UltEngine::Scene scene;
    scene.pDefaultShader = pEngine->pDefaultShader;
    scene.setCamera(pCamera);
    scene.addLight(pPointLight);
    scene.addLight(pDirectionalLight);
    scene.addLight(pSpotLight);

    scene.load(PROJECT_SOURCE_DIR + std::string("/resources/backpack/backpack.obj"));

    pEngine->render(scene);

    return 0;
}