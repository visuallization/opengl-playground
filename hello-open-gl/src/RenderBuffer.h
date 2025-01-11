#pragma once

class RenderBuffer {
public:
	RenderBuffer(const unsigned int width, const unsigned int height);
	~RenderBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetID() const;

private:
	unsigned int m_RendererID;
};