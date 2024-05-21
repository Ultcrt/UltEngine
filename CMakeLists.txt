cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)

project(UltEngine)

find_package(glfw3 3.4 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(engine
        dependencies/GLAD/include/glad/glad.h
        dependencies/GLAD/include/KHR/khrplatform.h
        dependencies/GLAD/src/glad.c
        dependencies/STB/include/stb_image.h
        src/Main.cpp
        src/Engine.cpp
        include/Engine.h
        include/Options.h
        src/Shader.cpp
        include/Shader.h
        include/Data.h
        src/Mesh.cpp
        include/Mesh.h
        src/Scene.cpp
        include/Scene.h
        src/Camera.cpp
        include/Camera.h
        src/TransformationNode.cpp
        include/TransformationNode.h
        src/Observable.cpp
        include/Observable.h
        include/Observer.h
        include/Material.h
        src/Material.cpp
        src/light/ILight.cpp
        include/light/ILight.h
        src/light/PointLight.cpp
        include/light/PointLight.h
        src/light/DirectionalLight.cpp
        include/light/DirectionalLight.h
        src/light/SpotLight.cpp
        include/light/SpotLight.h
        src/Options.cpp
        src/environment/IEnvironment.cpp
        include/environment/IEnvironment.h
        src/environment/CubicEnvironment.cpp
        include/environment/CubicEnvironment.h
        src/Texture.cpp
        include/Texture.h
        include/postprocessor/IPostProcessor.h
        src/postprocessor/BlurPostProcessor.cpp
        include/postprocessor/BlurPostProcessor.h
        src/postprocessor/BloomPostProcessor.cpp
        include/postprocessor/BloomPostProcessor.h
        src/postprocessor/SSAOPostProcessor.cpp
        include/postprocessor/SSAOPostProcessor.h)

target_compile_definitions(engine PRIVATE SHADER_DIRECTORY="${PROJECT_SOURCE_DIR}/src/shaders")
target_compile_definitions(engine PRIVATE RESOURCES_DIRECTORY="${PROJECT_SOURCE_DIR}/resources")

target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/GLAD/include)
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/STB/include)

target_link_libraries(engine PRIVATE glfw)
target_link_libraries(engine PRIVATE glm::glm)
target_link_libraries(engine PRIVATE assimp::assimp)

