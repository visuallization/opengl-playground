#pragma once

#include "RenderBuffer.h"
#include "Texture.h"

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;
	void Read() const;
	void Write() const;

	void AttachTexture(unsigned int id) const;
	void AttachTexture(Texture* texture) const;
	void AttachRenderBuffer(unsigned int id, unsigned int attachment = GL_DEPTH_STENCIL_ATTACHMENT) const;
	void AttachRenderBuffer(RenderBuffer* renderBuffer, unsigned int attachment = GL_DEPTH_STENCIL_ATTACHMENT) const;

	bool IsComplete();

private:
	unsigned int m_RendererID;	
};