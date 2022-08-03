#include "Model.h"

#include <iostream>

Model::Model(const char* path) {
	loadModel(path);
}

void Model::loadModel(const std::string& path) {
	std::cout << "Loading Model..." << std::endl;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}

	m_Directory = path.substr(0, path.find_last_of('/')) + "/";

	processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* node, const aiScene* scene) {
	// process all the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(processMesh(mesh, scene));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene) {
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;

	// process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		MeshVertex vertex;

		vertex.Position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);
		vertex.Normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);
		vertex.TexCoords = mesh->mTextureCoords[0]
			? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
			: glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// process materials
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<MeshTexture> Model::loadMaterialTextures(const aiMaterial* material, const aiTextureType& type, const std::string& typeName) {
	std::vector<MeshTexture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString path;
		material->GetTexture(type, i, &path);
		bool createNewTexture = true;

		for (unsigned int j = 0; j < m_LoadedTextures.size(); j++) {
			if (std::strcmp(m_LoadedTextures[j].Path.data(), path.C_Str()) == 0) {
				textures.push_back(m_LoadedTextures[j]);
				
				createNewTexture = false;
				break;
			}
		}

		if (createNewTexture) {
			MeshTexture texture;
			texture.Texture = std::make_shared<Texture>(m_Directory + path.C_Str());
			texture.Path = path.C_Str();

			textures.push_back(texture);
			m_LoadedTextures.push_back(texture);
		}
	}

	return textures;
}
