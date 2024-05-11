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
        src/ILight.cpp
        include/ILight.h
        src/PointLight.cpp
        include/PointLight.h
        src/DirectionalLight.cpp
        include/DirectionalLight.h
        src/SpotLight.cpp
        include/SpotLight.h)

target_compile_definitions(engine PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/GLAD/include)
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/STB/include)

target_link_libraries(engine PRIVATE glfw)
target_link_libraries(engine PRIVATE glm::glm)
target_link_libraries(engine PRIVATE assimp::assimp)

