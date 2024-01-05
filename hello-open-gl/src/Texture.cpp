#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const unsigned int width, const unsigned int height, unsigned int unit, bool flip)
	: m_RendererID(0), m_FilePath(""), m_LocalBuffer(nullptr), m_Width(width), m_Height(height), m_BitsPerPixel(0)
{
	stbi_set_flip_vertically_on_load(flip);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// The texture minifying function is used whenever the level-of-detail function determines that the texture should be minified
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	// The texture magnification function is used whenever the level-of-detail function determines that the texture should be magnified
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// Clamp texture horizontally
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	// Clamp texture vertically
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL));

	GLCall(glBindImageTexture(unit, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));

	Unbind();
}

Texture::Texture(const std::string& path, bool flip)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BitsPerPixel(0)
{
	stbi_set_flip_vertically_on_load(flip);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// The texture minifying function is used whenever the level-of-detail function determines that the texture should be minified
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	// The texture magnification function is used whenever the level-of-detail function determines that the texture should be magnified
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// Clamp texture horizontally
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	// Clamp texture vertically
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Send image buffer data to OPENGL/the GPU
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	Unbind();

	// Free buffer
	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const {
	// Set the slot
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	// Bind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}