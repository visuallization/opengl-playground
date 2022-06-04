#pragma once

#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

// Assert: set breakpoint programmatically
#define ASSERT(x) if(!(x)) __debugbreak();
// OpenGL Debug Macro
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:

public:
    void Clear() const;
    void SetClearColor(float r, float g, float b, float a) const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawArrays(const VertexArray& va, const Shader& shader, unsigned int size = 0) const;
    void DrawPoints(const VertexArray& va, const Shader& shaer, unsigned int size = 0) const;
};