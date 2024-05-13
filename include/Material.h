//
// Created by ultcrt on 24-5-8.
//

#ifndef ULTENGINE_MATERIAL_H
#define ULTENGINE_MATERIAL_H

#include <memory>
#include <vector>
#include "Data.h"
#include "Shader.h"
#include "Texture.h"

namespace UltEngine {
	class Material {
	public:
        float shininess = 32.0f;

		std::vector<Texture> textures;

		Material(const std::vector<Texture>& textures, const std::shared_ptr<Shader>& pShader);

		std::shared_ptr<Shader> pShader;

		void prepare();
	};
} // UltEngine

#endif //ULTENGINE_MATERIAL_H