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
        src/Model.cpp
        include/Model.h
        include/Data.h
        src/Mesh.cpp
        include/Mesh.h
)

target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/GLAD/include)
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/STB/include)

target_link_libraries(engine PRIVATE glfw)
target_link_libraries(engine PRIVATE glm::glm)
target_link_libraries(engine PRIVATE assimp::assimp)

