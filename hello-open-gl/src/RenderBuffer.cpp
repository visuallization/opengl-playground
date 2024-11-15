#include <GL/glew.h>

#include "RenderBuffer.h"
#include "Renderer.h"


RenderBuffer::RenderBuffer(const unsigned int width, const unsigned int height) {
	GLCall(glGenRenderbuffers(1, &m_RendererID));
	this->Bind();
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
}

RenderBuffer::~RenderBuffer() {
	GLCall(glDeleteRenderbuffers(1, &m_RendererID));
}

void RenderBuffer::Bind() const{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::Unbind() const {
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}