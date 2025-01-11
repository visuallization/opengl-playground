#pragma once

#include "RenderBuffer.h"
#include "Texture.h"

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	void AttachTexture(unsigned int id) const;
	void AttachTexture(Texture* texture) const;
	void AttachRenderBuffer(unsigned int id) const;
	void AttachRenderBuffer(RenderBuffer* renderBuffer) const;

	bool IsComplete();

private:
	unsigned int m_RendererID;	
};