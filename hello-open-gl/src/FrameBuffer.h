#pragma once

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	void AttachTexture(unsigned int id) const;
	void AttachRenderBuffer(unsigned int id) const;

	bool IsComplete();

private:
	unsigned int m_RendererID;	
};