#include <GL/glew.h>

#include "RenderBuffer.h"
#include "Renderer.h"


RenderBuffer::RenderBuffer(const unsigned int width, const unsigned int height, const unsigned int format, const unsigned int samples) {
	GLCall(glGenRenderbuffers(1, &m_RendererID));
	this->Bind();
	if (samples > 0) {
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height));
	} else {
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height));
	}
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

unsigned int RenderBuffer::GetID() const {
	return this->m_RendererID;
}