#pragma once

#include <map>
#include <string>

#include "Shader.h"
#include "Texture.h"

// Singleton
class ResourceManager {
public:
	// resource storage
	static std::map<std::string, Shader*> Shaders;
	static std::map<std::string, Texture*> Textures;

	// shaders
	static Shader* GetShader(std::string name);
	static Shader* LoadShader(const char* filePath, std::string name);

	// textures
	static Texture* GetTexture(std::string name);
	static Texture* LoadTexture(const char* filePath, std::string name, bool flip = false);
	static Texture* LoadTexture(const unsigned int width, const unsigned int height, std::string name, unsigned int unit = 0, unsigned int wrap = GL_CLAMP_TO_EDGE);

	// deallocate all loaded resources
	static void Clear();
private:
	// private constructor, that is we do not want any actual resource manager objects.
	// Its members and functions should be publicly available (static) as the resource manager will be a singleton.
	ResourceManager() {}
};

