//
// Created by ultcrt on 24-5-13.
//

#include "Options.h"

namespace UltEngine {
    namespace Options {
        TextureOptions TextureOptions::ColorTextureOptions = {
                true,
                true,
                {
                        {GL_TEXTURE_WRAP_S, GL_REPEAT},
                        {GL_TEXTURE_WRAP_T, GL_REPEAT},
                        {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
                        {GL_TEXTURE_MAG_FILTER, GL_LINEAR}}
        };

        TextureOptions TextureOptions::NonColorTextureOptions = {
                false,
                true,
                {
                        {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
                        {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
                        {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
                        {GL_TEXTURE_MAG_FILTER, GL_LINEAR}}
        };
    }
} // UltEngine