//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_SCENE_H
#define ULTENGINE_SCENE_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <assimp/scene.h>
#include <filesystem>
#include "Mesh.h"
#include "Data.h"
#include "Camera.h"
#include "ILight.h"
#include "Texture.h"
#include "IEnvironment.h"

namespace UltEngine {
    class Scene {
    private:
        std::vector<Mesh> meshes_;
        std::unordered_map<std::filesystem::path, unsigned> textureIDs_;

        std::shared_ptr<Camera> pCamera_;
        std::shared_ptr<IEnvironment> pEnvironment_;
        std::vector<std::shared_ptr<ILight>> pLights_;

        void loadNode_(const aiNode* pNode, const aiScene* pScene, const std::filesystem::path& dir);
        Mesh loadMesh_(const aiMesh* pMesh, const aiScene* pScene, const std::filesystem::path& dir);
        std::vector<Texture> loadTextureGroup_(const aiMaterial* pMaterial, aiTextureType type, const std::filesystem::path& dir);

    public:
        std::shared_ptr<Shader> pDefaultShader;

        void load(const std::filesystem::path& path);

        void draw() const;

        void drawMesh() const;

        void drawSkybox() const;

        void prepareCamera(const Shader& shader) const;

        void prepareLights(const Shader& shader) const;

        void setCamera(const std::shared_ptr<Camera>& pCamera);

        void setEnvironment(const std::shared_ptr<IEnvironment>& pEnvironment);

        void addLight(const std::shared_ptr<ILight>& pLight);

        std::shared_ptr<Camera> getCamera() const;
    };
} // UltEngine

#endif //ULTENGINE_SCENE_H
