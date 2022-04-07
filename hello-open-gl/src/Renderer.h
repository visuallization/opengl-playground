#pragma once

#include <GL/glew.h>

// Assert: set breakpoint programmatically
#define ASSERT(x) if(!(x)) __debugbreak();
// OpenGL Debug Macro
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);