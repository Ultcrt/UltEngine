//
// Created by ultcrt on 24-5-7.
//

#include <iostream>
#include <format>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Scene.h"
#include "glad/glad.h"

namespace UltEngine {
    void Scene::load(const std::filesystem::path &path) {
        std::filesystem::path directory = path.parent_path();

        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
            throw std::runtime_error(std::format("Cannot load model file {}", path.string()));
        }

        loadNode_(pScene->mRootNode, pScene, directory);
    }

    void Scene::loadNode_(const aiNode* pNode, const aiScene* pScene, const std::filesystem::path& dir) {
        for (unsigned i = 0; i < pNode->mNumMeshes; i++) {
            const aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
            meshes_.emplace_back(loadMesh_(pMesh, pScene, dir));
        }

        for (unsigned i = 0; i < pNode->mNumChildren; i++) {
            loadNode_(pNode->mChildren[i], pScene, dir);
        }
    }

    Mesh Scene::loadMesh_(const aiMesh* pMesh, const aiScene* pScene, const std::filesystem::path& dir) {
        std::vector<Vertex>   vertices;
        std::vector<vec3u>    triangles;
        std::vector<vec2u>    lines;
        std::vector<unsigned> points;
        std::vector<Texture>  textures;

        // Load vertices
        for (unsigned i = 0; i < pMesh->mNumVertices; i++) {
            Vertex vertex;

            vertex.position.x = pMesh->mVertices[i].x;
            vertex.position.y = pMesh->mVertices[i].y;
            vertex.position.z = pMesh->mVertices[i].z;

            vertex.normal.x = pMesh->mNormals[i].x;
            vertex.normal.y = pMesh->mNormals[i].y;
            vertex.normal.z = pMesh->mNormals[i].z;

            vertex.tangent.x = pMesh->mTangents[i].x;
            vertex.tangent.y = pMesh->mTangents[i].y;
            vertex.tangent.z = pMesh->mTangents[i].z;

            // Texture coordinate can be empty
            if (pMesh->mTextureCoords[0]) {
                vertex.texCoord.x = pMesh->mTextureCoords[0][i].x;
                vertex.texCoord.y = pMesh->mTextureCoords[0][i].y;
            }

            vertices.emplace_back(vertex);
        }

        // Load primitives
        for (unsigned i = 0; i < pMesh->mNumFaces; i++) {
            switch (pMesh->mFaces[i].mNumIndices) {
                case 1:
                    points.emplace_back(pMesh->mFaces[i].mIndices[0]);
                    break;
                case 2:
                    lines.emplace_back(pMesh->mFaces[i].mIndices[0], pMesh->mFaces[i].mIndices[1]);
                    break;
                case 3:
                    triangles.emplace_back(pMesh->mFaces[i].mIndices[0], pMesh->mFaces[i].mIndices[1], pMesh->mFaces[i].mIndices[2]);
                    break;
                default:
                    throw std::runtime_error(std::format("Unexpected num indices {}", pMesh->mFaces[i].mNumIndices));
            }
        }

        // Load materials
        const aiMaterial* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];

        for (int i = 0; i < pMaterial->mNumProperties; i++) {
            auto pProp = pMaterial->mProperties[i];
        }

        std::vector<Texture> diffuseTextures  = loadTextureGroup_(pMaterial, aiTextureType_DIFFUSE, dir);
        std::vector<Texture> specularTextures = loadTextureGroup_(pMaterial, aiTextureType_SPECULAR, dir);
        std::vector<Texture> normalTextures   = loadTextureGroup_(pMaterial, aiTextureType_HEIGHT, dir);
        textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
        textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
        auto pMat = std::make_shared<Material>(textures, pDefaultShader);

        return { vertices, triangles, lines, points, pMat };
    }

    std::vector<Texture> Scene::loadTextureGroup_(const aiMaterial* pMaterial, aiTextureType type, const std::filesystem::path& dir) {
        // Generate options
        Options::TextureOptions options;
        switch (type) {
            case aiTextureType_DIFFUSE:
                options = Options::TextureOptions::ColorTextureOptions;
                break;
            // TODO: Specular can be in different color space, see: https://www.reddit.com/r/blenderhelp/comments/hda14n/quick_question_which_color_space_for_specular_maps/
            case aiTextureType_SPECULAR:
            case aiTextureType_HEIGHT:
                options = Options::TextureOptions::NonColorTextureOptions;
                break;
            default:
                options = Options::TextureOptions::NonColorTextureOptions;
                std::cerr << std::format("Unknown image dataType {}, load as non-color texture", static_cast<int>(type));
                break;
        }
        
        std::vector<Texture> textures;

        for (unsigned i = 0; i < pMaterial->GetTextureCount(type); i++) {
            aiString filename;
            pMaterial->GetTexture(type, i, &filename);

            // Avoid redundant texture loading
            const auto absolute = std::filesystem::absolute(filename.C_Str());
            if (textureIDs_.find(absolute) == textureIDs_.end()) {
                textures.emplace_back(dir / filename.C_Str(), type, options);
                textureIDs_.insert({absolute, textures.back().id});
            }
            else {
                textures.emplace_back(textureIDs_[absolute], type);
            }
        }

        return textures;
    }

    void Scene::draw() const {
        drawShadow();
        drawMesh();
        drawSkybox();
    }

    void Scene::setCamera(const std::shared_ptr<Camera> &pCamera) {
        pCamera_ = pCamera;
    }

    void Scene::addLight(const std::shared_ptr<ILight> &pLight) {
        pLights_.emplace_back(pLight);
    }

    std::shared_ptr<Camera> Scene::getCamera() const {
        return pCamera_;
    }

    void Scene::setEnvironment(const std::shared_ptr<IEnvironment> &pEnvironment) {
        pEnvironment_ = pEnvironment;
    }

    void Scene::drawMesh() const {
        // Rendering pass
        for (const Mesh& mesh: meshes_) {
            const auto& pShader = mesh.pMaterial->pShader;

            // Use shader program
            pShader->use();

            prepareCamera(*pShader);
            prepareLights(*pShader);

            // Set up mesh & material
            mesh.draw();
        }
    }

    void Scene::drawSkybox() const {
        // Draw environment
        const auto& pEnvironmentShader = pEnvironment_->getShader();
        pEnvironmentShader->use();
        // Set camera params
        const auto& view = pCamera_->getView();
        const auto& projection = pCamera_->getProjection();
        pEnvironmentShader->set("view", glm::mat3(view));
        pEnvironmentShader->set("projection", projection);
        // Set environment
        pEnvironment_->draw();
    }

    void Scene::prepareCamera(const Shader& shader) const {
        // Set camera params
        const auto& view = pCamera_->getView();
        const auto& projection = pCamera_->getProjection();
        const auto t = glm::inverse(view);
        shader.set("view", view);
        shader.set("projection", projection);
        shader.set("viewPosition", pCamera_->translation);
    }

    void Scene::prepareLights(const Shader& shader) const {
        // Set up lights
        std::size_t unitId = 7;
        std::size_t pointLightNum = 0;
        std::size_t directionalLightNum = 0;
        std::size_t spotLightNum = 0;
        for (const auto& pLight: pLights_) {
            std::size_t idx = 0;
            switch (pLight->type()) {
                case LightType::PointLight:
                    idx = pointLightNum++;
                    break;
                case LightType::DirectionalLight:
                    idx = directionalLightNum++;
                    break;
                case LightType::SpotLight:
                    idx = spotLightNum++;
                    break;
            }
            pLight->prepare(idx, shader, unitId);
        }
        shader.set("pointLightNum", static_cast<int>(pointLightNum));
        shader.set("directionalLightNum", static_cast<int>(directionalLightNum));
        shader.set("spotLightNum", static_cast<int>(spotLightNum));
    }

    void Scene::drawShadow() const {
        const auto boundingInfo = pCamera_->getBoundingInfo();
        for (const auto& pLight: pLights_) {
            if (pLight->castShadows) {
                const Shader* pShader = nullptr;
                switch (pLight->type()) {
                case LightType::PointLight:
                    pShader = &pointShadowMapShader;
                    break;
                case LightType::DirectionalLight:
                case LightType::SpotLight:
                    pShader = &shadowMapShader;
                    break;
                }
                pShader->use();
                pLight->prepareShadowMap(*pShader, boundingInfo);
                // Clean up
                glClear(GL_DEPTH_BUFFER_BIT);
                for (const Mesh& mesh: meshes_) {
                    mesh.draw(*pShader);
                }
            }
        }
    }
} // UltEngine