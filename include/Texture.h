//
// Created by ultcrt on 24-5-13.
//

#ifndef ULTENGINE_TEXTURE_H
#define ULTENGINE_TEXTURE_H

#include "Options.h"

namespace UltEngine {
    class Texture {
    public:
        unsigned id;
        int type;

        Texture(const std::string& path, int type, const Options::TextureOptions& options);

        Texture(unsigned id, int type);

        Texture();
    };
} // UltEngine

#endif //ULTENGINE_TEXTURE_H
