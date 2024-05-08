#include "Engine.h"
#include "Camera.h"
#include "Scene.h"

int main()
{
    auto pEngine = std::make_shared<UltEngine::Engine>();
    auto pCamera = std::make_shared<UltEngine::Camera>();

    pEngine->inputObservable.add(pCamera);

    UltEngine::Scene scene;
    scene.pDefaultShader = pEngine->pDefaultShader;
    scene.setCamera(pCamera);

    scene.load(PROJECT_SOURCE_DIR + std::string("/resources/backpack/backpack.obj"));

    pEngine->render(scene);

    return 0;
}