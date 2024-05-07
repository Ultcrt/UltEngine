#include "Engine.h"
#include "Camera.h"
#include "Scene.h"

int main()
{
    auto pEngine = std::make_shared<UltEngine::Engine>();
    auto pCamera = std::make_shared<UltEngine::Camera>();

    UltEngine::Scene scene;

    scene.setCamera(pCamera);

    scene.load("../resources/backpack/backpack.obj");

    pEngine->render(scene);

    return 0;
}