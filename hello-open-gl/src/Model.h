#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "Mesh.h"

class Model {
public:
	std::vector<Mesh> Meshes;

	Model(const char* path);

private:
	std::string m_Directory;
	std::vector<MeshTexture> m_LoadedTextures;

	void loadModel(const std::string& path);
	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(const aiMaterial* material, const aiTextureType& type, const std::string& typeName);
};