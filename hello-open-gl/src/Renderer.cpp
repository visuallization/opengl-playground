#include <iostream>

#include "Renderer.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::SetClearColor(float r, float g, float b, float a) const {
    GLCall(glClearColor(r, g, b, a));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr))
}


void Renderer::DrawArrays(const VertexArray & va, const Shader & shader, unsigned int size) const {
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, size));
}

void Renderer::DrawPoints(const VertexArray& va, const Shader& shader, unsigned int size) const {
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_POINTS, 0, size));
}