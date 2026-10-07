#pragma once
#include <iostream>
#include "Mesh.h"
#include "loadTextures.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{
public:
	Model(const char* path) {
		loadModel(path);
	}
	void Draw(Shader& shader);
	std::vector<Mesh> meshes;

private:
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType textureType, std::string typeName);
};

