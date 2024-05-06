//
// Created by ultcrt on 24-5-6.
//

#ifndef ULTENGINE_MODEL_H
#define ULTENGINE_MODEL_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Mesh.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace UltEngine {
    class Model {
    public:
        Model(const std::string& path);

    private:
        std::vector<Mesh> meshes_;
        std::string directory_;

        void loadModel_(const std::string& path);
        void loadNode_(const aiNode* pNode, const aiScene* pScene);
        Mesh loadMesh_(const aiMesh* pMesh, const aiScene* pScene);
        std::vector<Texture> loadMaterial_(const aiMaterial* pMaterial, aiTextureType type);
        unsigned loadTexture_(const std::string& path);
    };
} // UltEngine

#endif //ULTENGINE_MODEL_H
