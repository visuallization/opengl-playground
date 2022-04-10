#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
    // generate vertex array object
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        // Enable vertex attribute (in this case position). If enabled, the values in the generic vertex attribute array will be accessed & used for rendering when calling OpenGL render functions.
        GLCall(glEnableVertexAttribArray(i));
        // Define the layout of the data (you only have to call it once if you have only one atttribute e.g. position in your vertex)
        // This line also links the buffer with the vao.
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}


void VertexArray::Bind() const {
    // Select the generated vertex array object to work on it
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}
