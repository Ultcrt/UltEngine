//
// Created by ultcrt on 24-5-6.
//

#include <stdexcept>
#include <format>
#include "Model.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace UltEngine {
	Model::Model(const std::string& path): directory_(path.begin(), path.begin() + path.find_last_of('/')) {
		loadModel_(path);
	}
	void Model::loadModel_(const std::string& path) {
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
			throw std::runtime_error("Cannot load model file");
		}

		loadNode_(pScene->mRootNode, pScene);
	}
	void Model::loadNode_(const aiNode* pNode, const aiScene* pScene) {
		for (unsigned i = 0; i < pNode->mNumMeshes; i++) {
			const aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			meshes_.emplace_back(loadMesh_(pMesh, pScene));
		}

		for (unsigned i = 0; i < pNode->mNumChildren; i++) {
			loadNode_(pNode->mChildren[i], pScene);
		}
	}
	Mesh Model::loadMesh_(const aiMesh* pMesh, const aiScene* pScene) {
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

			// Texture coordinate can be empty
			if (pMesh->mTextureCoords[0]) {
				vertex.texCoords.x = pMesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = pMesh->mTextureCoords[0][i].y;
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
		std::vector<Texture> diffuseTextures = loadMaterial_(pMaterial, aiTextureType_DIFFUSE);
		std::vector<Texture> specularTextures = loadMaterial_(pMaterial, aiTextureType_SPECULAR);
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
		textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

		return { vertices, triangles, lines, points, textures };
	}
	std::vector<Texture> Model::loadMaterial_(const aiMaterial* pMaterial, aiTextureType type) {
		std::vector<Texture> textures;

		for (unsigned i = 0; i < pMaterial->GetTextureCount(type); i++) {
			aiString path;
			pMaterial->GetTexture(type, i, &path);
			textures.emplace_back(loadTexture_(directory_ + '/' + path.C_Str()), type);
		}

		return textures;
	}

	unsigned Model::loadTexture_(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		unsigned textureID;

		if (data) {
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			GLenum format;
			switch (channels) {
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				throw std::runtime_error(std::format("Unexpected texture channels {}", channels));
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			throw std::runtime_error(std::format("Cannot load texture '{}'", path));
		}

		// Free up memory
		stbi_image_free(data);

		return textureID;
	}
} // UltEngine