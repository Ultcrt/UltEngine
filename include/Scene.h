//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_SCENE_H
#define ULTENGINE_SCENE_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <assimp/scene.h>
#include "Mesh.h"
#include "Data.h"
#include "Camera.h"

namespace UltEngine {
    class Scene {
    private:
        std::vector<Mesh> meshes_;
        std::unordered_map<std::string, unsigned> textureIDs_;
        std::shared_ptr<Camera> pCamera_;

        void loadNode_(const aiNode* pNode, const aiScene* pScene, const std::string& dir);
        Mesh loadMesh_(const aiMesh* pMesh, const aiScene* pScene, const std::string& dir);
        std::vector<Texture> loadMaterial_(const aiMaterial* pMaterial, aiTextureType type, const std::string& dir);
        unsigned loadTexture_(const std::string& path);

    public:
        void load(const std::string& path);

        void draw(const Shader& shader) const;

        void setCamera(const std::shared_ptr<Camera>& pCamera);
    };
} // UltEngine

#endif //ULTENGINE_SCENE_H
