#include "Model.h"

std::vector<Texture> loadedTexture;

void Model::Draw(Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Failed to import the model with " << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	// process the vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

		}
		if (mesh->mTextureCoords[0]) {
			glm::vec2 TexCoords;
			TexCoords.x = mesh->mTextureCoords[0][i].x;
			TexCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = TexCoords;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// process the indices

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	// process the textures if they exist
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, textures, indices);

}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial* material, aiTextureType textureType, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(textureType); i++) {
		aiString str;
		material->GetTexture(textureType, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < loadedTexture.size(); j++) {
			if (std::strcmp(loadedTexture[j].Path.data(), str.C_Str()) == 0) {
				textures.push_back(loadedTexture[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			std::string fulPath = directory + '/' + str.C_Str();
			const char* actualPath = (fulPath).c_str();
			texture.ID = loadTexture(actualPath, "simple");
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			loadedTexture.push_back(texture);

		}
	}
	return textures;
}