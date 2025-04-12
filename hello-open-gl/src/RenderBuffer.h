#pragma once

#include <GL/glew.h>

class RenderBuffer {
public:
	RenderBuffer(const unsigned int width, const unsigned int height, const unsigned int format = GL_DEPTH24_STENCIL8, const unsigned int samples = 0);
	~RenderBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetID() const;

private:
	unsigned int m_RendererID;
};