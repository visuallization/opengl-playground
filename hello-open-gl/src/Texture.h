#pragma once

#include <GL/glew.h>
#include <string>

class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BitsPerPixel;

public:
	Texture(const unsigned int width, const unsigned int height, unsigned int unit = 0, unsigned int wrap = GL_CLAMP_TO_EDGE);
	Texture(const std::string& path, bool flip = false);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetID() const { return m_RendererID; }
};