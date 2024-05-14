//
// Created by ultcrt on 24-5-13.
//

#include <filesystem>
#include "IPostProcessor.h"

namespace UltEngine {
    IPostProcessor::IPostProcessor(const std::filesystem::path &path):
    shader_(std::filesystem::path(SHADER_DIRECTORY) / "IPostProcessor.vert",path) {}
} // UltEngine