//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_OPTIONS_H
#define ULTENGINE_OPTIONS_H

#include <string>

namespace UltEngine {
    namespace Options {
        struct EngineOptions {
            int width = 800;
            int height = 600;
            std::string title = "UltEngine";
        };
    } // Options
} // UltEngine

#endif //ULTENGINE_OPTIONS_H
