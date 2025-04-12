#include <GL/glew.h>

#include "FrameBuffer.h"
#include "Renderer.h"

FrameBuffer::FrameBuffer() {
	GLCall(glGenFramebuffers(1, &m_RendererID));
	this->Bind();
}

FrameBuffer::~FrameBuffer() {
	this->Unbind();
	GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::Read() const {
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Write() const {
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Bind() const {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Unbind() const {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::AttachTexture(unsigned int id) const {
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0));
}

void FrameBuffer::AttachTexture(Texture* texture) const {
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0));
}

void FrameBuffer::AttachRenderBuffer(unsigned int id, unsigned int attachment) const {
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id));
}

void FrameBuffer::AttachRenderBuffer(RenderBuffer* renderBuffer, unsigned int attachment) const {
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer->GetID()));
}

bool FrameBuffer::IsComplete() {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
