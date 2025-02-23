#include "ResourceManager.h"

std::map<std::string, Shader*> ResourceManager::Shaders;
std::map<std::string, Texture*> ResourceManager::Textures;

Shader* ResourceManager::GetShader(std::string name) {
	return Shaders[name];
};

Shader* ResourceManager::LoadShader(const char* filePath, std::string name) {
	Shaders[name] = new Shader(filePath);
	return Shaders[name];
};

Texture* ResourceManager::GetTexture(std::string name) {
	return Textures[name];
};

Texture* ResourceManager::LoadTexture(const char* filePath, std::string name, bool flip) {
	Textures[name] = new Texture(filePath, flip);	
	return Textures[name];
};

Texture* ResourceManager::LoadTexture(const unsigned int width, const unsigned int height, std::string name) {
	Textures[name] = new Texture(width, height);
	return Textures[name];
}

void ResourceManager::Clear() {
	for (const auto& pair : Shaders) {
		delete pair.second;
	}

	for (const auto& pair : Textures) {
		delete pair.second;
	}

	Shaders.clear();
	Textures.clear();
};